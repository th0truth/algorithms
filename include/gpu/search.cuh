#pragma once
#include "base.h"
#ifdef __CUDACC__
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#endif

namespace gpu {
  namespace search {
    /**
     * @brief CUDA-accelerated search.
     * Handles data transfer and kernel execution.
    */
    int LinearSearch(i32* array, int size, i32 target, bool visualize = false, std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now());

    /**
     * @brief CUDA-accelerated binary search.
    */
    int BinarySearch(i32* array, int size, i32 target, bool visualize = false, std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now());

  }
}

#ifdef __CUDACC__
/**
 * @brief CUDA kernel for performing a grid-stride linear search.
 * This kernel uses atomic operations to find the *first* occurrence
 * of a target value in an array. It is designed to handle arrays
 * larger than the maximum grid size.
 * @param vram_array Pointer to the array in GPU memory.
 * @param size Number of elements in the array.
 * @param target The value to search for.
 * @param vram_result Pointer to a single integer in GPU memory to store the result.
*/
__global__ void LinearSearchKernel(const i32* vram_array, int size, i32 target, int* vram_result);
/**
 * @brief CUDA kernel for performing a parallel multi-way binary search.
*/
__global__ void BinarySearchKernel(const i32* vram_array, int size, i32 target, int* vram_result);
#endif