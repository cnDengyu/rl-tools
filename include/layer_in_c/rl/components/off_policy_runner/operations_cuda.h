#ifndef LAYER_IN_C_RL_COMPONENTS_OFF_POLICY_RUNNER_OPERATIONS_CUDA_H
#define LAYER_IN_C_RL_COMPONENTS_OFF_POLICY_RUNNER_OPERATIONS_CUDA_H

#include "operations_generic.h"
#include <layer_in_c/devices/dummy.h>

namespace layer_in_c{
    template <typename DEV_SPEC, typename RUNNER_SPEC, typename BATCH_SPEC, typename RNG>
    __global__
    void gather_batch_kernel(devices::CUDA<DEV_SPEC>& device, const rl::components::OffPolicyRunner<RUNNER_SPEC>* runner, rl::components::off_policy_runner::Batch<BATCH_SPEC>* batch, RNG rng, bool DETERMINISTIC=false) {
        using BATCH = rl::components::off_policy_runner::Batch<BATCH_SPEC>;
        using T = typename RUNNER_SPEC::T;
        using TI = typename RUNNER_SPEC::TI;
        using DEVICE = devices::CUDA<DEV_SPEC>;
        static_assert(decltype(batch->observations)::COL_PITCH == 1);
        static_assert(decltype(batch->actions)::COL_PITCH == 1);
        static_assert(decltype(batch->next_observations)::COL_PITCH == 1);

        typename DEVICE::index_t env_i = DETERMINISTIC ? 0 : random::uniform_int_distribution(typename DEVICE::SPEC::RANDOM(), (typename DEVICE::index_t) 0, (typename DEVICE::index_t)(RUNNER_SPEC::N_ENVIRONMENTS - 1), rng);
        auto& replay_buffer = runner->replay_buffers[env_i];
        static_assert(decltype(replay_buffer.observations)::COL_PITCH == 1);
        static_assert(decltype(replay_buffer.actions)::COL_PITCH == 1);
        static_assert(decltype(replay_buffer.next_observations)::COL_PITCH == 1);

        TI batch_step_i = threadIdx.x + blockIdx.x * blockDim.x;
        curand_init(1234, batch_step_i, 0, &rng);
        if(batch_step_i < BATCH_SPEC::BATCH_SIZE){
            set(batch->observations, batch_step_i, 0, get(replay_buffer.observations, batch_step_i, 0));
            typename DEVICE::index_t sample_index_max = (replay_buffer.full ? RUNNER_SPEC::REPLAY_BUFFER_CAPACITY : replay_buffer.position) - 1;
            typename DEVICE::index_t sample_index = DETERMINISTIC ? batch_step_i : random::uniform_int_distribution( typename DEVICE::SPEC::RANDOM(), (typename DEVICE::index_t) 0, sample_index_max, rng);

            // todo: replace with smarter, coalesced copy
            for(typename DEVICE::index_t i = 0; i < BATCH::OBSERVATION_DIM; i++){
                set(batch->observations, batch_step_i, i, get(replay_buffer.observations, sample_index, i));
                set(batch->next_observations, batch_step_i, i, get(replay_buffer.next_observations, sample_index, i));
            }
            for(typename DEVICE::index_t i = 0; i < BATCH::ACTION_DIM; i++){
                set(batch->actions, batch_step_i, i, get(replay_buffer.actions, sample_index, i));
            }

            set(batch->rewards, 0, batch_step_i, get(replay_buffer.rewards, sample_index, 0));
            set(batch->terminated, 0, batch_step_i, get(replay_buffer.terminated, sample_index, 0));
            set(batch->truncated, 0, batch_step_i, get(replay_buffer.truncated,  sample_index, 0));
        }
    }
    template <typename DEV_SPEC, typename SPEC, typename BATCH_SPEC, typename RNG>
    void gather_batch(devices::CUDA<DEV_SPEC>& device, const rl::components::OffPolicyRunner<SPEC>* runner, rl::components::off_policy_runner::Batch<BATCH_SPEC>* batch, RNG& rng, bool DETERMINISTIC=false){
        using DEVICE = devices::CUDA<DEV_SPEC>;
        static_assert(utils::typing::is_same_v<SPEC, typename BATCH_SPEC::SPEC>);
        using T = typename SPEC::T;
        using TI = typename SPEC::TI;
        constexpr typename DEVICE::index_t BATCH_SIZE = BATCH_SPEC::BATCH_SIZE;
        constexpr TI BLOCKSIZE_COLS = 32;
        constexpr TI N_BLOCKS_COLS = LAYER_IN_C_DEVICES_CUDA_CEIL(BATCH_SIZE, BLOCKSIZE_COLS);
        dim3 bias_grid(N_BLOCKS_COLS);
        dim3 bias_block(BLOCKSIZE_COLS);
        gather_batch_kernel<<<bias_grid, bias_block>>>(device, runner, batch, rng, DETERMINISTIC);

        // get cuda status
        cudaError_t cudaStatus = cudaGetLastError();
        if (cudaStatus != cudaSuccess) {
            fprintf(stderr, "gather_batch_kernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
        }
    }
}


#endif
