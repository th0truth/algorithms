#include "search.cuh"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
using namespace std;

#include <chrono>

/**
 * CUDA kernel for parallel linear search.
 * Each thread checks one element. If found, it uses atomicMin to ensure
 * the smallest index (first occurrence) is stored in vram_result.
 */
__global__ void LinearSearchKernel(const i32* vram_array, int size, i32 target, int* vram_result)
{
  int idx = blockIdx.x * blockDim.x + threadIdx.x;
  
  if (idx < size) {
    if (vram_array[idx] == target) {
      // Use atomicMin to find the FIRST occurrence across all threads
      atomicMin(vram_result, idx);
    }
  }
}

/**
 * CUDA kernel for simplified parallel binary search.
 * Divides the array into equal segments. Each thread performs a standard
 * CPU-style binary search on its own independent segment.
 */
__global__ void BinarySearchKernel(const i32* vram_array, int size, i32 target, int* vram_result)
{
  int idx = blockIdx.x * blockDim.x + threadIdx.x;
  int total_threads = gridDim.x * blockDim.x;

  // Calculate the bounds of the segment assigned to this thread
  int elements_per_thread = (size + total_threads - 1) / total_threads;
  int left = idx * elements_per_thread;
  int right = left + elements_per_thread - 1;

  if (left < size) {
    // Prevent reading out of bounds for the last thread
    if (right >= size) right = size - 1;

    // Standard binary search within this thread's specific segment
    while (left <= right) {
      int mid = left + (right - left) / 2;
      
      if (vram_array[mid] == target) {
        atomicMin(vram_result, mid);
        return; // Target found, no need to search further in this thread
      }
      
      if (vram_array[mid] < target) {
        left = mid + 1;
      } else {
        right = mid - 1;
      }
    }
  }
}

namespace gpu {
  namespace search {
    /**
     * Host wrapper for GPU Linear Search.
     * Manages device memory and kernel execution configuration.
     */
    int LinearSearch(const i32* array, int size, i32 target) {
      i32 *vram_array;
      int *vram_result;
      int h_result = size + 1;

      // Allocate GPU memory
      cudaMalloc(&vram_array, size * sizeof(i32));
      cudaMalloc(&vram_result, sizeof(int));

      // Copy data to GPU
      cudaMemcpy(vram_array, array, size * sizeof(i32), cudaMemcpyHostToDevice);
      cudaMemcpy(vram_result, &h_result, sizeof(int), cudaMemcpyHostToDevice);

      // Launch kernel with grid-stride pattern
      int threadsPerBlock = 256;
      int blocksPerGrid = (size + threadsPerBlock - 1) / threadsPerBlock;
      LinearSearchKernel<<<blocksPerGrid, threadsPerBlock>>>(vram_array, size, target, vram_result);

      // Retrieve result
      cudaMemcpy(&h_result, vram_result, sizeof(int), cudaMemcpyDeviceToHost);

      // Clean up
      cudaFree(vram_array);
      cudaFree(vram_result);

      return (h_result > size) ? -1 : h_result;
    }

    /**
     * Host wrapper for GPU Binary Search.
     */
    int BinarySearch(const i32* array, int size, i32 target) {
      i32 *vram_array;
      int *vram_result;
      int h_result = size + 1;

      cudaMalloc(&vram_array, size * sizeof(i32));
      cudaMalloc(&vram_result, sizeof(int));

      cudaMemcpy(vram_array, array, size * sizeof(i32), cudaMemcpyHostToDevice);
      cudaMemcpy(vram_result, &h_result, sizeof(int), cudaMemcpyHostToDevice);

      // Launch enough threads to divide the array into small, fast segments
      // 256 blocks of 256 threads = 65,536 threads total.
      // For a 1 million element array, each thread only binary searches ~15 elements.
      int threadsPerBlock = 256;
      int blocksPerGrid = 256; 
      BinarySearchKernel<<<blocksPerGrid, threadsPerBlock>>>(vram_array, size, target, vram_result);

      cudaMemcpy(&h_result, vram_result, sizeof(int), cudaMemcpyDeviceToHost);

      cudaFree(vram_array);
      cudaFree(vram_result);

      return (h_result > size) ? -1 : h_result;
    }
  }
}
