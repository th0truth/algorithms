#include "sort.cuh"
#include "utils.h"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <algorithm>

/**
 * CUDA kernel to merge two sorted portions of an array.
 * width is the current size of the sorted segments to be merged.
 */
__global__ void MergeSortKernel(i32* vram_array, i32* vram_temp, int size, int width) {
  int idx = blockIdx.x * blockDim.x + threadIdx.x;
  int start = idx * 2 * width;
  
  if (start < size) {
    int mid = min(start + width, size);
    int end = min(start + 2 * width, size);
    
    int i = start;
    int j = mid;
    
    for (int k = start; k < end; k++) {
      if (i < mid && (j >= end || vram_array[i] <= vram_array[j])) {
        vram_temp[k] = vram_array[i];
        i++;
      } else {
        vram_temp[k] = vram_array[j];
        j++;
      }
    }
  }
}

namespace gpu {
  namespace sort {
    /**
     * Iterative CUDA merge sort.
     */
    void MergeSort(i32* array, int size, bool visualize, std::chrono::steady_clock::time_point start_time) {
      if (visualize) {
        Viz::draw_state("Gpu Merge Sort", array, size, start_time);
      }

      i32 *vram_array, *vram_temp;
      cudaMalloc(&vram_array, size * sizeof(i32));
      cudaMalloc(&vram_temp, size * sizeof(i32));
      
      cudaMemcpy(vram_array, array, size * sizeof(i32), cudaMemcpyHostToDevice);

      // Iteratively merge subarrays of increasing width
      for (int width = 1; width < size; width *= 2) {
        int threadsPerBlock = 256;
        int numSegments = (size + (2 * width) - 1) / (2 * width);
        int blocksPerGrid = (numSegments + threadsPerBlock - 1) / threadsPerBlock;
        
        MergeSortKernel<<<blocksPerGrid, threadsPerBlock>>>(vram_array, vram_temp, size, width);
        
        // Copy the merged temporary array back to the main array for the next width iteration
        cudaMemcpy(vram_array, vram_temp, size * sizeof(i32), cudaMemcpyDeviceToDevice);

        if (visualize) {
          // Copy to host for visualization
          cudaMemcpy(array, vram_array, size * sizeof(i32), cudaMemcpyDeviceToHost);
          Viz::draw_state("Gpu Merge Sort", array, size, start_time);
        }
      }

      cudaMemcpy(array, vram_array, size * sizeof(i32), cudaMemcpyDeviceToHost);

      cudaFree(vram_array);
      cudaFree(vram_temp);

      if (visualize) {
        Viz::draw_state("Gpu Merge Sort", array, size, start_time);
      }
    }
  }
}
