#include <rl_tools/rl/environments/pendulum/operations_cpu.h>

#include <rl_tools/rl/algorithms/sac/loop/core/config.h>

RL_TOOLS_NAMESPACE_WRAPPER_START
namespace rl_tools::rl::zoo::sac::pendulum_v1{
    namespace rlt = rl_tools;
    template <typename DEVICE, typename T, typename TI, typename RNG>
    struct PendulumV1{
        using ENVIRONMENT_SPEC = rlt::rl::environments::pendulum::Specification<T, TI, rlt::rl::environments::pendulum::DefaultParameters<T>>;
        using ENVIRONMENT = rlt::rl::environments::Pendulum<ENVIRONMENT_SPEC>;
        struct LOOP_CORE_PARAMETERS: rlt::rl::algorithms::sac::loop::core::DefaultParameters<T, TI, ENVIRONMENT>{
            struct SAC_PARAMETERS: rl::algorithms::sac::DefaultParameters<T, TI, ENVIRONMENT::ACTION_DIM>{
                static constexpr TI ACTOR_BATCH_SIZE = 100;
                static constexpr TI CRITIC_BATCH_SIZE = 100;
            };
            static constexpr TI STEP_LIMIT = 20000;
            static constexpr TI REPLAY_BUFFER_CAP = STEP_LIMIT;
            static constexpr TI ACTOR_NUM_LAYERS = 3;
            static constexpr TI ACTOR_HIDDEN_DIM = 64;
            static constexpr TI CRITIC_NUM_LAYERS = 3;
            static constexpr TI CRITIC_HIDDEN_DIM = 64;
            static constexpr T ALPHA = 1.0;
        };
        using LOOP_CORE_CONFIG = rlt::rl::algorithms::sac::loop::core::Config<T, TI, RNG, ENVIRONMENT, LOOP_CORE_PARAMETERS, rlt::rl::algorithms::sac::loop::core::ConfigApproximatorsSequential>;
    };
}
RL_TOOLS_NAMESPACE_WRAPPER_END
