#pragma once

#include <cstdint>
#include "base.h"
#include <chrono>

#define TRACE(x) do { if (!visualize) { cout << x; } } while(0)
static int merge_count = 1;

namespace sort 
{
  void SelectionSort(i32* array, int size, bool visualize = false, std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now()); 
  void InsertionSort(i32* array, int size, bool visualize = false, std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now());
  void BubbleSort(i32* array, int size, bool visualize = false, std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now());
  void MergeSort(i32* array, int size, bool visualize = false, std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now());
  void QuickSort(i32* array, int size, bool visualize = false, std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now());
}
