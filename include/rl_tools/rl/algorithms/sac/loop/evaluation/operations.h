#include "../../../../../version.h"
#if (defined(RL_TOOLS_DISABLE_INCLUDE_GUARDS) || !defined(RL_TOOLS_RL_ALGORITHMS_SAC_LOOP_EVALUATION_OPERATIONS_H)) && (RL_TOOLS_USE_THIS_VERSION == 1)
#pragma once
#define RL_TOOLS_RL_ALGORITHMS_SAC_LOOP_EVALUATION_OPERATIONS_H

#include "../../../../../rl/algorithms/sac/operations_generic.h"
#include "../../../../../rl/components/off_policy_runner/operations_generic.h"


#include "../../../../../rl/utils/evaluation.h"

#include "config.h"

RL_TOOLS_NAMESPACE_WRAPPER_START
namespace rl_tools{
    template <typename T_CONFIG>
    void init(rl::algorithms::sac::loop::evaluation::TrainingState<T_CONFIG>& ts, typename T_CONFIG::TI seed = 0){
        using STATE = rl::algorithms::sac::loop::evaluation::TrainingState<T_CONFIG>;
        init(static_cast<typename STATE::NEXT&>(ts), seed);
    }

    template <typename T_CONFIG>
    void destroy(rl::algorithms::sac::loop::evaluation::TrainingState<T_CONFIG>& ts){
        using STATE = rl::algorithms::sac::loop::evaluation::TrainingState<T_CONFIG>;
        destroy(static_cast<typename STATE::NEXT&>(ts));
    }

    template <typename CONFIG>
    bool step(rl::algorithms::sac::loop::evaluation::TrainingState<CONFIG>& ts){
        using TI = typename CONFIG::TI;
        using PARAMETERS = typename CONFIG::PARAMETERS;
        using STATE = rl::algorithms::sac::loop::evaluation::TrainingState<CONFIG>;
        if constexpr(PARAMETERS::DETERMINISTIC_EVALUATION == true){

            TI evaluation_index = ts.step / PARAMETERS::EVALUATION_INTERVAL;
            if(ts.step % PARAMETERS::EVALUATION_INTERVAL == 0 && evaluation_index < PARAMETERS::N_EVALUATIONS){
                auto result = evaluate(ts.device, ts.envs[0], ts.ui, ts.actor_critic.actor, rl::utils::evaluation::Specification<PARAMETERS::NUM_EVALUATION_EPISODES, CONFIG::NEXT::PARAMETERS::ENVIRONMENT_STEP_LIMIT>(), ts.observations_mean, ts.observations_std, ts.actor_deterministic_evaluation_buffers, ts.rng, false);
                std::cout << "Step: " << ts.step << " Mean return: " << result.returns_mean << std::endl;
                ts.evaluation_results[evaluation_index] = result;
            }
        }
        bool finished = step(static_cast<typename STATE::NEXT&>(ts));
        return finished;
    }
}
RL_TOOLS_NAMESPACE_WRAPPER_END


#endif
