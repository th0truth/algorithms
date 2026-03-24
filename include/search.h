#pragma once

#include "base.h"

namespace search
{
  int LinearSearch(i32* array, int size, int target, bool visualize = false, std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now());
  int BinarySearch(i32* array, int size, i32 target, bool visualize = false, std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now());
}