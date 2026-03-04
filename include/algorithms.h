#pragma once

#include <cstdint>
#include <chrono>

#define TRACE(x) do { if (!visualize) { cout << x; } } while(0)
static int merge_count = 1;

namespace sort 
{
  void SelectionSort(int32_t* array, int size, bool visualize = false, std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now()); 
  void InsertionSort(int32_t* array, int size, bool visualize = false, std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now());
  void BubbleSort(int32_t* array, int size, bool visualize = false, std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now());
  void MergeSort(int32_t* array, int size, bool visualize = false, std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now());
}
