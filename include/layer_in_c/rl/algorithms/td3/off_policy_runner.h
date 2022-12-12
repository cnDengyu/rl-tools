#ifndef LAYER_IN_C_RL_ALGORITHMS_TD3_OFF_POLICY_RUNNER
#define LAYER_IN_C_RL_ALGORITHMS_TD3_OFF_POLICY_RUNNER
#include "replay_buffer.h"
#include <layer_in_c/rl/environments/environments.h>

namespace lic = layer_in_c;

namespace layer_in_c::rl::algorithms::td3 {
    template<typename T, int T_CAPACITY, int T_STEP_LIMIT>
    struct DefaultOffPolicyRunnerParameters {
        static constexpr uint32_t CAPACITY = T_CAPACITY;
        static constexpr uint32_t STEP_LIMIT = T_STEP_LIMIT;
        static constexpr T EXPLORATION_NOISE = 0.1;
    };

    template<typename T, typename ENVIRONMENT, typename PARAMETERS>
    struct OffPolicyRunner {
        ReplayBuffer<T, ENVIRONMENT::OBSERVATION_DIM, ENVIRONMENT::ACTION_DIM, PARAMETERS::CAPACITY> replay_buffer;
        T state[ENVIRONMENT::STATE_DIM];
        uint32_t episode_step = 0;
        T episode_return = 0;
    };
}
namespace layer_in_c{
    template<typename T, typename ENVIRONMENT, typename POLICY, typename PARAMETERS, typename RNG>
    void step(rl::algorithms::td3::OffPolicyRunner<T, ENVIRONMENT, PARAMETERS> &runner, POLICY &policy, RNG &rng) {
        // if the episode is done (step limit activated for STEP_LIMIT > 0) or if the step is the first step for this runner, reset the environment
        if ((PARAMETERS::STEP_LIMIT > 0 && runner.episode_step == PARAMETERS::STEP_LIMIT) ||
            (runner.replay_buffer.position == 0 && !runner.replay_buffer.full)) {
            // first step
            lic::sample_initial_state(ENVIRONMENT(), runner.state, rng);
            runner.episode_step = 0;
            runner.episode_return = 0;
        }
        // todo: increase efficiency by removing the double observation of each state
        T observation[ENVIRONMENT::OBSERVATION_DIM];
        observe(ENVIRONMENT(), runner.state, observation);
        T next_state[ENVIRONMENT::STATE_DIM];
        T action[ENVIRONMENT::ACTION_DIM];
        T action_clipped[ENVIRONMENT::ACTION_DIM];
        lic::evaluate(policy, observation, action);
        std::normal_distribution<T> exploration_noise_distribution(0, PARAMETERS::EXPLORATION_NOISE);
        for (int i = 0; i < ENVIRONMENT::ACTION_DIM; i++) {
            action[i] += exploration_noise_distribution(rng);
            action_clipped[i] = std::clamp<T>(action[i], -1, 1);
        }
        T reward = lic::step(ENVIRONMENT(), runner.state, action_clipped, next_state);
        memcpy(runner.state, next_state, sizeof(T) * ENVIRONMENT::STATE_DIM);
        T next_observation[ENVIRONMENT::OBSERVATION_DIM];
        lic::observe(ENVIRONMENT(), next_state, next_observation);
        bool terminated = false;
        runner.episode_step += 1;
        runner.episode_return += reward;
        bool truncated = runner.episode_step == PARAMETERS::STEP_LIMIT;
        if (truncated || terminated) {
            std::cout << "Episode return: " << runner.episode_return << std::endl;
        }
        // todo: add truncation / termination handling (stemming from the environment)
        add(runner.replay_buffer, observation, action, reward, next_observation, terminated, truncated);
    }
    template<typename ENVIRONMENT, typename POLICY, typename RNG, int STEP_LIMIT>
    typename POLICY::T evaluate(POLICY &policy, const typename POLICY::T initial_state[ENVIRONMENT::STATE_DIM], RNG &rng) {
        typedef typename POLICY::T T;
        T state[ENVIRONMENT::STATE_DIM];
        memcpy(state, initial_state, sizeof(T) * ENVIRONMENT::STATE_DIM);
        T episode_return = 0;
        for (int i = 0; i < STEP_LIMIT; i++) {
            T observation[ENVIRONMENT::OBSERVATION_DIM];
            lic::observe(ENVIRONMENT(), state, observation);
            T action[ENVIRONMENT::ACTION_DIM];
            lic::evaluate(policy, observation, action);
            T action_clipped[ENVIRONMENT::ACTION_DIM];
            for(int action_i=0; action_i<ENVIRONMENT::ACTION_DIM; action_i++){
                action_clipped[action_i] = std::clamp<T>(action[action_i], -1, 1);
            }
            T next_state[ENVIRONMENT::STATE_DIM];
            T reward = lic::step(ENVIRONMENT(), state, action_clipped, next_state);
            memcpy(state, next_state, sizeof(T) * ENVIRONMENT::STATE_DIM);
            episode_return += reward;
            bool terminated = lic::terminated(ENVIRONMENT(), state);
            if (terminated) {
                break;
            }
        }
        return episode_return;
    }
    template<typename ENVIRONMENT, typename POLICY, typename RNG, int STEP_LIMIT>
    typename POLICY::T evaluate(POLICY &policy, RNG &rng, uint32_t N) {
        typedef typename POLICY::T T;
        T episode_returns[N];
        for (int i = 0; i < N; i++) {
            T initial_state[ENVIRONMENT::STATE_DIM];
            lic::sample_initial_state(ENVIRONMENT(), initial_state, rng);
            episode_returns[i] = evaluate<ENVIRONMENT, POLICY, RNG, STEP_LIMIT>(policy, initial_state, rng);
        }
        T mean = 0;
        for (int i = 0; i < N; i++) {
            mean += episode_returns[i];
        }
        mean /= N;
        T variance = 0;
        for (int i = 0; i < N; i++) {
            variance += (episode_returns[i] - mean) * (episode_returns[i] - mean);
        }
        variance /= N;
        T standard_deviation = std::sqrt(variance);
        std::cout << "Mean: " << mean << ", standard deviation: " << standard_deviation << std::endl;
        return mean;
    }
}
#endif