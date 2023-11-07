#include "../../../version.h"
#if (defined(BACKPROP_TOOLS_DISABLE_INCLUDE_GUARDS) || !defined(BACKPROP_TOOLS_NN_LAYERS_DENSE_PERSIST_CODE_H)) && (BACKPROP_TOOLS_USE_THIS_VERSION == 1)
#pragma once
#define BACKPROP_TOOLS_NN_LAYERS_DENSE_PERSIST_CODE_H
#include "layer.h"
#include "../../../containers/persist_code.h"
#include <sstream>
#include "../../../persist/code.h"
#include "../../../containers/persist_code.h"

BACKPROP_TOOLS_NAMESPACE_WRAPPER_START
namespace backprop_tools {
    namespace nn::layers::dense::persist{
        template<nn::activation_functions::ActivationFunction ACTIVATION_FUNCTION>
        auto get_activation_function_string(){
            static_assert(ACTIVATION_FUNCTION == nn::activation_functions::ActivationFunction::IDENTITY ||
                          ACTIVATION_FUNCTION == nn::activation_functions::ActivationFunction::RELU ||
                          ACTIVATION_FUNCTION == nn::activation_functions::ActivationFunction::GELU ||
                          ACTIVATION_FUNCTION == nn::activation_functions::ActivationFunction::TANH ||
                          ACTIVATION_FUNCTION == nn::activation_functions::ActivationFunction::FAST_TANH ||
                          ACTIVATION_FUNCTION == nn::activation_functions::ActivationFunction::SIGMOID);

            if constexpr (ACTIVATION_FUNCTION == nn::activation_functions::ActivationFunction::IDENTITY){
                return "BACKPROP_TOOLS""_NAMESPACE_WRAPPER ::backprop_tools::nn::activation_functions::ActivationFunction::IDENTITY";
            } else if constexpr (ACTIVATION_FUNCTION == nn::activation_functions::ActivationFunction::RELU){
                return "BACKPROP_TOOLS""_NAMESPACE_WRAPPER ::backprop_tools::nn::activation_functions::ActivationFunction::RELU";
            } else if constexpr (ACTIVATION_FUNCTION == nn::activation_functions::ActivationFunction::TANH){
                return "BACKPROP_TOOLS""_NAMESPACE_WRAPPER ::backprop_tools::nn::activation_functions::ActivationFunction::TANH";
            } else if constexpr (ACTIVATION_FUNCTION == nn::activation_functions::ActivationFunction::FAST_TANH){
                return "BACKPROP_TOOLS""_NAMESPACE_WRAPPER ::backprop_tools::nn::activation_functions::ActivationFunction::FAST_TANH";
            } else if constexpr (ACTIVATION_FUNCTION == nn::activation_functions::ActivationFunction::SIGMOID){
                return "BACKPROP_TOOLS""_NAMESPACE_WRAPPER ::backprop_tools::nn::activation_functions::ActivationFunction::SIGMOID";
            }
        }
    }

    template<typename DEVICE, typename SPEC>
    persist::Code save_split(DEVICE& device, nn::layers::dense::Layer <SPEC> &layer, std::string name, bool const_declaration=false, typename DEVICE::index_t indent=0){
        using TI = typename DEVICE::index_t;
        std::stringstream indent_ss;
        for(TI i=0; i < indent; i++){
            indent_ss << "    ";
        }
        std::string ind = indent_ss.str();
        using TI = typename DEVICE::index_t;
        std::stringstream ss_header;
        ss_header << "#include <backprop_tools/nn/layers/dense/layer.h>\n";
        std::stringstream ss;
        ss << ind << "namespace " << name << " {\n";
        auto weights = save_split(device, layer.weights, "weights", const_declaration, indent+1);
        ss_header << weights.header;
        ss << weights.body;
        auto biases = save_split(device, layer.biases, "biases", const_declaration, indent+1);
        ss_header << biases.header;
        ss << biases.body;
        ss << ind << "    using SPEC = " << "BACKPROP_TOOLS""_NAMESPACE_WRAPPER ::backprop_tools::nn::layers::dense::Specification<"
            << containers::persist::get_type_string<typename SPEC::T>() << ", "
            << containers::persist::get_type_string<typename SPEC::TI>() << ", "
            << SPEC::INPUT_DIM << ", "
            << SPEC::OUTPUT_DIM << ", "
            << nn::layers::dense::persist::get_activation_function_string<SPEC::ACTIVATION_FUNCTION>() << ", "
            << get_type_string(typename SPEC::PARAMETER_TYPE{}) << ", "
            << 1 << ", "
            << get_type_string_tag(device, typename SPEC::PARAMETER_GROUP{}) << ", "
            << "BACKPROP_TOOLS""_NAMESPACE_WRAPPER ::backprop_tools::MatrixDynamicTag" << ", "
            << "true, "
            << "BACKPROP_TOOLS""_NAMESPACE_WRAPPER ::backprop_tools::matrix::layouts::RowMajorAlignment<" << containers::persist::get_type_string<TI>() << ", 1>"
            << ">; \n";
        ss << ind << "    " << "using TYPE = BACKPROP_TOOLS""_NAMESPACE_WRAPPER ::backprop_tools::nn::layers::dense::Layer<SPEC>;";
        ss << ind << "    " << (const_declaration ? "const " : "") << "TYPE layer = {weights::parameters, biases::parameters};\n";
        ss << ind << "}\n";

        return {ss_header.str(), ss.str()};
    }
    template<typename DEVICE, typename SPEC>
    std::string save_code(DEVICE& device, nn::layers::dense::Layer <SPEC> &layer, std::string name, bool const_declaration=false, typename DEVICE::index_t indent=0){
        auto code = save_split(device, layer, name, const_declaration, indent);
        return code.header + code.body;
    }
}
BACKPROP_TOOLS_NAMESPACE_WRAPPER_END

#endif
