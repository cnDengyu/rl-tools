#ifndef LAYER_IN_C_CONTAINERS_PERSIST_CODE_H
#define LAYER_IN_C_CONTAINERS_PERSIST_CODE_H

#include <layer_in_c/utils/generic/typing.h>
#include <sstream>


namespace layer_in_c{
    namespace containers::persist{
        using STORAGE_TYPE = unsigned char;
        static_assert(sizeof(unsigned char) == 1);
        constexpr auto INDEX_TYPE = "unsigned int";
        template <typename T>
        auto get_type_string(){
            static_assert(utils::typing::is_same_v<T, float> || utils::typing::is_same_v<T, double> || utils::typing::is_same_v<T, int> || utils::typing::is_same_v<T, unsigned int> || utils::typing::is_same_v<T, long> || utils::typing::is_same_v<T, unsigned long> || utils::typing::is_same_v<T, long long> || utils::typing::is_same_v<T, unsigned long long> || utils::typing::is_same_v<T, char> || utils::typing::is_same_v<T, unsigned char> || utils::typing::is_same_v<T, short> || utils::typing::is_same_v<T, unsigned short>);
            if constexpr(std::is_same_v<T, float>){
                return "float";
            } else if constexpr(std::is_same_v<T, double>){
                return "double";
            } else if constexpr(std::is_same_v<T, int>){
                return "int";
            } else if constexpr(std::is_same_v<T, unsigned int>){
                return "unsigned int";
            } else if constexpr(std::is_same_v<T, long>){
                return "long";
            } else if constexpr(std::is_same_v<T, unsigned long>){
                return "unsigned long";
            } else if constexpr(std::is_same_v<T, long long>){
                return "long long";
            } else if constexpr(std::is_same_v<T, unsigned long long>){
                return "unsigned long long";
            } else if constexpr(std::is_same_v<T, char>){
                return "char";
            } else if constexpr(std::is_same_v<T, unsigned char>){
                return "unsigned char";
            } else if constexpr(std::is_same_v<T, short>){
                return "short";
            } else if constexpr(std::is_same_v<T, unsigned short>){
                return "unsigned short";
            } else {
                return "Unsupported type";
            }
        }
    }
    template<typename DEVICE, typename SPEC>
    std::string save(DEVICE &device, Matrix<SPEC>& m, std::string name){
        using T = typename SPEC::T;
        using TI = typename DEVICE::index_t;
        static_assert(utils::typing::is_same_v<containers::persist::STORAGE_TYPE, unsigned char>);
        static_assert(sizeof(T) % sizeof(containers::persist::STORAGE_TYPE) == 0);
        std::stringstream ss;
        ss << "#include <layer_in_c/containers.h>\n";
        ss << "namespace " << name << " {\n";
        ss << "    unsigned char memory[] = {";
        bool first = true;
        for(TI i=0; i < SPEC::ROWS; i++){
            for(TI j=0; j < SPEC::COLS; j++){
                auto value = get(m, i, j);
                auto* ptr = reinterpret_cast<containers::persist::STORAGE_TYPE*>(&value);
                for(int k=0; k < sizeof(T); k++){
                    if(!first){
                        ss << ", ";
                    }
                    first = false;
                    ss << (int)ptr[k];
                }
            }
        }
        ss << "};\n";
        ss << "    layer_in_c::Matrix<layer_in_c::matrix::Specification<" << containers::persist::get_type_string<T>() << ", " << containers::persist::get_type_string<TI>() << ", " << SPEC::ROWS << ", " << SPEC::COLS << ", " << "layer_in_c::matrix::layouts::RowMajorAlignment<" << containers::persist::get_type_string<TI>() << ", " << 1 << ">>>matrix = {(" << containers::persist::get_type_string<T>() << "*)" << "memory}; \n";
        ss << "}\n";
        return ss.str();
    }
}
#endif
