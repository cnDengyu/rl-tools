#include "../../../../../version.h"
#if (defined(RL_TOOLS_DISABLE_INCLUDE_GUARDS) || !defined(RL_TOOLS_RL_ALGORITHMS_SAC_LOOP_EVALUATION_STATE_H)) && (RL_TOOLS_USE_THIS_VERSION == 1)
#pragma once
#define RL_TOOLS_RL_ALGORITHMS_SAC_LOOP_EVALUATION_STATE_H

#include "../../../../../rl/algorithms/sac/operations_generic.h"
#include "../../../../../rl/components/off_policy_runner/operations_generic.h"

#include "../../../../../rl/utils/evaluation.h"

RL_TOOLS_NAMESPACE_WRAPPER_START
namespace rl_tools::rl::algorithms::sac::loop::evaluation{
    template<typename T_CONFIG, typename T_NEXT = typename T_CONFIG::NEXT::template State<typename T_CONFIG::NEXT>>
    struct TrainingState: T_NEXT {
        using CONFIG = T_CONFIG;
        using NEXT = T_NEXT;
        using T = typename CONFIG::T;
        using TI = typename CONFIG::TI;
        rl::utils::evaluation::Result<T, TI, CONFIG::PARAMETERS::NUM_EVALUATION_EPISODES> evaluation_results[CONFIG::PARAMETERS::N_EVALUATIONS];
    };
}
#endif




