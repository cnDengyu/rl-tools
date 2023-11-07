#include <backprop_tools/operations/cpu_mux.h>
#include <backprop_tools/nn/operations_cpu_mux.h>

#include <backprop_tools/rl/environments/car/operations_cpu.h>
#if BACKPROP_TOOLS_ENABLE_GTK
#include <backprop_tools/rl/environments/car/ui.h>
#endif

#include <backprop_tools/nn_models/operations_generic.h>

#include <backprop_tools/rl/algorithms/td3/loop.h>
namespace bpt = BACKPROP_TOOLS_NAMESPACE_WRAPPER ::backprop_tools;

namespace training_config {
    using namespace bpt::nn_models::sequential::interface; // to simplify the model definition we import the sequential interface but we don't want to pollute the global namespace hence we do it in a model definition namespace
    struct Config{
        using DEV_SPEC = bpt::devices::DefaultCPUSpecification;
//    using DEVICE = bpt::devices::CPU<DEV_SPEC>;
        using DEVICE = bpt::DEVICE_FACTORY<DEV_SPEC>;
        using T = float;
        using TI = typename DEVICE::index_t;

        using ENV_SPEC = bpt::rl::environments::car::SpecificationTrack<T, TI, 100, 100, 20>;
        using ENVIRONMENT = bpt::rl::environments::CarTrack<ENV_SPEC>;
        using ENVIRONMENT_EVALUATION = ENVIRONMENT;
#if BACKPROP_TOOLS_ENABLE_GTK
        using UI = bpt::rl::environments::car::UI<bpt::rl::environments::car::ui::Specification<T, TI, ENVIRONMENT, 1000, 60>>;
#else
        using UI = bool;
#endif

        struct DEVICE_SPEC: bpt::devices::DefaultCPUSpecification {
            using LOGGING = bpt::devices::logging::CPU;
        };
        static constexpr bool CONSTRUCT_LOGGER = false;
        struct TD3PendulumParameters: bpt::rl::algorithms::td3::DefaultParameters<T, TI>{
            constexpr static TI CRITIC_BATCH_SIZE = 100;
            constexpr static TI ACTOR_BATCH_SIZE = 100;
            constexpr static T GAMMA = 0.997;
        };

        using TD3_PARAMETERS = TD3PendulumParameters;

        template <typename PARAMETER_TYPE>
        struct ACTOR{
            static constexpr TI HIDDEN_DIM = 64;
            static constexpr TI BATCH_SIZE = TD3_PARAMETERS::ACTOR_BATCH_SIZE;
            using LAYER_1_SPEC = bpt::nn::layers::dense::Specification<T, TI, ENVIRONMENT::OBSERVATION_DIM, HIDDEN_DIM, bpt::nn::activation_functions::ActivationFunction::RELU, PARAMETER_TYPE, BATCH_SIZE>;
            using LAYER_1 = bpt::nn::layers::dense::LayerBackwardGradient<LAYER_1_SPEC>;
            using LAYER_2_SPEC = bpt::nn::layers::dense::Specification<T, TI, HIDDEN_DIM, HIDDEN_DIM, bpt::nn::activation_functions::ActivationFunction::RELU, PARAMETER_TYPE, BATCH_SIZE>;
            using LAYER_2 = bpt::nn::layers::dense::LayerBackwardGradient<LAYER_2_SPEC>;
            using LAYER_3_SPEC = bpt::nn::layers::dense::Specification<T, TI, HIDDEN_DIM, ENVIRONMENT::ACTION_DIM, bpt::nn::activation_functions::ActivationFunction::TANH, PARAMETER_TYPE, BATCH_SIZE>;
            using LAYER_3 = bpt::nn::layers::dense::LayerBackwardGradient<LAYER_3_SPEC>;

            using MODEL = Module<LAYER_1, Module<LAYER_2, Module<LAYER_3>>>;
        };

        template <typename PARAMETER_TYPE>
        struct CRITIC{
            static constexpr TI HIDDEN_DIM = 64;
            static constexpr TI BATCH_SIZE = TD3_PARAMETERS::CRITIC_BATCH_SIZE;

            using LAYER_1_SPEC = bpt::nn::layers::dense::Specification<T, TI, ENVIRONMENT::OBSERVATION_DIM + ENVIRONMENT::ACTION_DIM, HIDDEN_DIM, bpt::nn::activation_functions::ActivationFunction::RELU, PARAMETER_TYPE, BATCH_SIZE>;
            using LAYER_1 = bpt::nn::layers::dense::LayerBackwardGradient<LAYER_1_SPEC>;
            using LAYER_2_SPEC = bpt::nn::layers::dense::Specification<T, TI, HIDDEN_DIM, HIDDEN_DIM, bpt::nn::activation_functions::ActivationFunction::RELU, PARAMETER_TYPE, BATCH_SIZE>;
            using LAYER_2 = bpt::nn::layers::dense::LayerBackwardGradient<LAYER_2_SPEC>;
            using LAYER_3_SPEC = bpt::nn::layers::dense::Specification<T, TI, HIDDEN_DIM, 1, bpt::nn::activation_functions::ActivationFunction::IDENTITY, PARAMETER_TYPE, BATCH_SIZE>;
            using LAYER_3 = bpt::nn::layers::dense::LayerBackwardGradient<LAYER_3_SPEC>;

            using MODEL = Module<LAYER_1, Module<LAYER_2, Module<LAYER_3>>>;
        };



        //using ACTOR_STRUCTURE_SPEC = bpt::nn_models::mlp::StructureSpecification<T, TI, ENVIRONMENT::OBSERVATION_DIM, ENVIRONMENT::ACTION_DIM, 3, 64, bpt::nn::activation_functions::RELU, bpt::nn::activation_functions::TANH, TD3_PARAMETERS::ACTOR_BATCH_SIZE>;
        //using CRITIC_STRUCTURE_SPEC = bpt::nn_models::mlp::StructureSpecification<T, TI, ENVIRONMENT::OBSERVATION_DIM + ENVIRONMENT::ACTION_DIM, 1, 3, 64, bpt::nn::activation_functions::RELU, bpt::nn::activation_functions::IDENTITY, TD3_PARAMETERS::CRITIC_BATCH_SIZE>;

        using OPTIMIZER_PARAMETERS = typename bpt::nn::optimizers::adam::DefaultParametersTorch<T, TI>;
        using OPTIMIZER = bpt::nn::optimizers::Adam<OPTIMIZER_PARAMETERS>;
        using ACTOR_TYPE = typename ACTOR<bpt::nn::parameters::Adam>::MODEL;
        using ACTOR_TARGET_TYPE = typename ACTOR<bpt::nn::parameters::Plain>::MODEL;
        using CRITIC_TYPE = typename CRITIC<bpt::nn::parameters::Adam>::MODEL;
        using CRITIC_TARGET_TYPE = typename CRITIC<bpt::nn::parameters::Plain>::MODEL;

        using ACTOR_CRITIC_SPEC = bpt::rl::algorithms::td3::Specification<T, TI, ENVIRONMENT, ACTOR_TYPE, ACTOR_TARGET_TYPE, CRITIC_TYPE, CRITIC_TARGET_TYPE, OPTIMIZER, TD3_PARAMETERS>;
        using ACTOR_CRITIC_TYPE = bpt::rl::algorithms::td3::ActorCritic<ACTOR_CRITIC_SPEC>;


        static constexpr int N_WARMUP_STEPS_ACTOR = ACTOR_CRITIC_TYPE::SPEC::PARAMETERS::ACTOR_BATCH_SIZE;
        static constexpr int N_WARMUP_STEPS_CRITIC = ACTOR_CRITIC_TYPE::SPEC::PARAMETERS::ACTOR_BATCH_SIZE;
#ifndef BACKPROP_TOOLS_STEP_LIMIT
        static constexpr TI STEP_LIMIT = 500000000; //2 * N_WARMUP_STEPS;
#else
        static constexpr TI STEP_LIMIT = BACKPROP_TOOLS_STEP_LIMIT;
#endif
        static constexpr bool DETERMINISTIC_EVALUATION = true;
        static constexpr TI NUM_EVALUATION_EPISODES = 10;
        static constexpr TI EVALUATION_INTERVAL = 20000;
        static constexpr TI REPLAY_BUFFER_CAP = 1000000;
        static constexpr TI ENVIRONMENT_STEP_LIMIT = 1000;
        static constexpr bool COLLECT_EPISODE_STATS = true;
        static constexpr TI EPISODE_STATS_BUFFER_SIZE = 1000;
        using OFF_POLICY_RUNNER_SPEC = bpt::rl::components::off_policy_runner::Specification<
                T,
                TI,
                ENVIRONMENT,
                1,
                false,
                REPLAY_BUFFER_CAP,
                ENVIRONMENT_STEP_LIMIT,
                bpt::rl::components::off_policy_runner::DefaultParameters<T>,
                false,
                COLLECT_EPISODE_STATS,
                EPISODE_STATS_BUFFER_SIZE
        >;
        const T STATE_TOLERANCE = 0.00001;
        static_assert(ACTOR_CRITIC_TYPE::SPEC::PARAMETERS::ACTOR_BATCH_SIZE == ACTOR_CRITIC_TYPE::SPEC::PARAMETERS::CRITIC_BATCH_SIZE);
    };
}

int main(){
    using CONFIG = typename training_config::Config;
    using TI = typename CONFIG::TI ;
    bpt::rl::algorithms::td3::loop::TrainingState<CONFIG> ts;
    bpt::init(ts.device, ts.envs[0]);
    bpt::rl::algorithms::td3::loop::init(ts, 3);
//    ts.envs[0].parameters.dt = 0.01;
    for(TI step_i=0; step_i < CONFIG::STEP_LIMIT; step_i++){
        bpt::rl::algorithms::td3::loop::step(ts);
    }
    return 0;
}