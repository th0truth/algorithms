#pragma once
#include "base.h"
#ifdef __CUDACC__
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#endif

namespace gpu {
  namespace sort {
    /**
     * @brief CUDA-accelerated merge sort.
    */
    void MergeSort(i32* array, int size, bool visualize = false, std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now());
  }
}

#ifdef __CUDACC__
/**
 * @brief CUDA kernel for merging two sorted subarrays.
*/
__global__ void MergeSortKernel(i32* vram_array, i32* vram_temp, int size, int width);
#endif
