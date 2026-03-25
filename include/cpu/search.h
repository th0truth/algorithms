#pragma once
#include "base.h"
using namespace std;

namespace cpu {
  namespace search {
    int LinearSearch(i32* array, int size, i32 target, bool visualize = false, chrono::steady_clock::time_point start_time = chrono::steady_clock::now());
    int BinarySearch(i32* array, int size, i32 target, bool visualize = false, chrono::steady_clock::time_point start_time = chrono::steady_clock::now());
  }
}