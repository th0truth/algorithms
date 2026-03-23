#pragma once
#include <chrono>
#include "base.h"

namespace sort 
{
  void SelectionSort(i32* array, int size, bool visualize = false, std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now()); 
  void InsertionSort(i32* array, int size, bool visualize = false, std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now());
  void BubbleSort(i32* array, int size, bool visualize = false, std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now());
  void CocktailSort(i32* array, int size, bool visualize = false, std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now());
  void HeapSort(i32* array, int size, bool visualize = false, std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now());
  void MergeSort(i32* array, int size, bool visualize = false, std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now());
  void QuickSort(i32* array, int size, bool visualize = false, std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now());
  void ShellSort(i32* array, int size, bool visualize = false, std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now());
}
