# Algorithms & CUDA Parallel Processing

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
*Image Source: NVIDIA*

---

## 🏗️ Architecture & Concepts

### The CUDA Hierarchy: Grids, Blocks, and Threads
To manage thousands of threads, CUDA organizes them into a hierarchy:
1.  **Thread:** The smallest unit of execution.
2.  **Block:** A group of threads that can cooperate and share memory.
3.  **Grid:** A group of blocks that execute the same kernel.

In this project's `GPULinearSearch`, each thread is responsible for checking exactly one element of the array. For an array of 1 million elements, 1 million threads are launched nearly simultaneously!

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

### ⚡ GPU Accelerated Searching
*   **GPULinearSearch:** Massive parallel search where every element is checked in parallel. Uses `atomicMin` to find the first occurrence.
*   **GPUBinarySearch:** A hybrid approach where the array is partitioned into segments, and each GPU thread performs a binary search on its assigned segment.

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

# GPU Parallel Search (Benchmark)
./build/main GPULinearSearch random 1000000 1 5000 --target 42

# CPU Search with Visualization
./build/main LinearSearch --visualize random 20 1 50 --target 10
```

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

## 🔬 How the GPU Search Works

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

By launching `(size / 256)` blocks of 256 threads each, we achieve near-instantaneous search across massive datasets that would take a CPU significantly longer to scan linearly.

---

## 📜 License
This project is open-source and available under the MIT License.
