# 🚀 Algorithms & CUDA Parallel Processing

A comprehensive, real-time visualization and benchmarking suite for CPU and GPU algorithms, written in C++ and CUDA. This project demonstrates the power of parallel computing by comparing traditional serial CPU algorithms with high-performance GPU implementations.

![Build Status](https://img.shields.io/badge/build-passing-green)
![C++](https://img.shields.io/badge/C++-17-blue)
![CUDA](https://img.shields.io/badge/CUDA-11.0+-green)
![SDL2](https://img.shields.io/badge/SDL2-2.0+-orange)

---

## 🧠 What is CUDA & Parallel Computing?

### Parallel Computing
Traditional CPU programs are **serial**: they execute instructions one after another. **Parallel computing** is the simultaneous use of multiple compute resources to solve a computational problem. It breaks a large task into smaller ones, which are then solved at the same time.

### CUDA (Compute Unified Device Architecture)
CUDA is a parallel computing platform and programming model created by NVIDIA. It allows developers to use a CUDA-enabled graphics processing unit (GPU) for general-purpose processing (GPGPU).

#### CPU vs. GPU Architecture
*   **CPU (Central Processing Unit):** Designed for complex logic and serial tasks. It has a few powerful cores optimized for sequential serial processing.
*   **GPU (Graphics Processing Unit):** Designed for data-parallel tasks. It has thousands of smaller, more efficient cores designed for handling multiple tasks simultaneously.

![CPU vs GPU](https://media.licdn.com/dms/image/v2/D4D12AQEpbXYIppRJmw/article-cover_image-shrink_720_1280/B4DZZgRxPuH4AI-/0/1745371993405?e=2147483647&v=beta&t=WGF2Pysnp0CYNmY7nisWqxwW478ROK0gi2zeAO8IkIM)

---

## 🏗️ Architecture & Concepts

### The CUDA Hierarchy: Grids, Blocks, and Threads
To manage thousands of threads, CUDA organizes them into a hierarchy:
1.  **Thread:** The smallest unit of execution.
2.  **Block:** A group of threads that can cooperate and share memory.
3.  **Grid:** A group of blocks that execute the same kernel.

In this project, GPU algorithms leverage this hierarchy to process massive datasets by launching thousands of threads simultaneously.

![CUDA Thread Hierarchy](https://docs.nvidia.com/cuda/cuda-c-programming-guide/_images/grid-of-thread-blocks.png)
*Image Source: NVIDIA CUDA C Programming Guide*

---

## ✨ Features

### 🖥️ CPU Sorting Algorithms
| Algorithm | Best Case | Average Case | Worst Case | Space |
|-----------|-----------|--------------|------------|-------|
| **QuickSort** | O(n log n) | O(n log n) | O(n²) | O(log n) |
| **MergeSort** | O(n log n) | O(n log n) | O(n log n) | O(n) |
| **HeapSort** | O(n log n) | O(n log n) | O(n log n) | O(1) |
| **ShellSort** | O(n log n) | O(n log^2 n)| O(n log^2 n)| O(1) |
| **InsertionSort**| O(n) | O(n²) | O(n²) | O(1) |
| **SelectionSort**| O(n²) | O(n²) | O(n²) | O(1) |
| **BubbleSort** | O(n) | O(n²) | O(n²) | O(1) |
| **CocktailSort** | O(n) | O(n²) | O(n²) | O(1) |

### 🔍 Search Algorithms
| Algorithm | CPU Time | GPU Strategy |
|-----------|----------|--------------|
| **Linear Search** | O(n) | **Massive Parallel:** Every thread checks one element. |
| **Binary Search** | O(log n) | **Hybrid Parallel:** Array is split into segments; each thread binary searches its segment. |

### ⚡ GPU Accelerated Algorithms
*   **GpuMergeSort:** A high-performance parallel implementation of the divide-and-conquer merge sort.
*   **GpuLinearSearch:** Parallel search where every element is checked simultaneously. Uses `atomicMin` to find the first occurrence.
*   **GpuBinarySearch:** Parallelized search that divides the search space among thousands of threads for extremely fast lookups in sorted data.

### 🎨 Visualizer & Audio
*   **Real-time Visualization:** ASCII-based bar charts that animate in your terminal.
*   **Sonification:** Using SDL2 to generate tones mapped to element values. Hear the "sound of sorting"!

---

## 🛠️ Getting Started

### Prerequisites
*   **Compiler:** `g++` (C++17 support)
*   **GPU Toolkit:** `nvcc` (NVIDIA CUDA Toolkit)
*   **Libraries:** `SDL2` (for audio)
*   **Hardware:** NVIDIA GPU (required for GPU algorithms)

### Build
```bash
make
```

### Usage
```bash
# CPU Sorting with Visualization and Sound
./build/main QuickSort --visualize --sound random 50 1 100

# GPU Merge Sort (Large Scale)
./build/main GpuMergeSort random 1000000 1 10000

# GPU Parallel Search
./build/main GpuLinearSearch random 1000000 1 5000 --target 42

# CPU Binary Search with Visualization
./build/main BinarySearch --visualize random 20 1 50 --target 10
```

---

## 🔬 Deep Dive: GPU Algorithms

### GPU Linear Search
In `src/gpu/search.cu`, we implement a **Parallel Linear Search**:
```cpp
__global__ void LinearSearchKernel(const i32* vram_array, int size, i32 target, int* vram_result) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < size) {
        if (vram_array[idx] == target) {
            atomicMin(vram_result, idx); // Thread-safe update of result
        }
    }
}
```

### GPU Binary Search
Instead of a single binary search, the GPU version divides the array into thousands of segments. Each thread is assigned a segment and performs a standard binary search within it. For an array of 1 million elements, using 65,000 threads means each thread only has to check ~15 elements!

### GPU Merge Sort
Merge Sort on the GPU uses a **bottom-up iterative approach**:
1.  **Phase 1 (Block Sort):** Thread blocks sort small chunks in **Shared Memory**.
2.  **Phase 2 (Global Merge):** Sorted chunks are merged in parallel across the entire GPU.

---

## 📂 Project Structure
```text
algorithms/
├── include/
│   ├── core/      # Base classes, Config, Sound, Utils
│   ├── cpu/       # CPU Algorithm headers (Sort, Search)
│   └── gpu/       # CUDA Kernel headers
├── src/
│   ├── core/      # Main entry, CLI logic, Audio implementation
│   ├── cpu/       # C++ implementations of sorting/searching
│   └── gpu/       # CUDA (.cu) implementations
├── Makefile       # Hybrid C++/CUDA Build System
└── README.md
```

---

## 📜 License
This project is open-source and available under the MIT License.
