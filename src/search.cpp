#include "base.h"

#include <chrono>
#include <thread>

#include "sound.h"
#include "utils.h"
using namespace std;

namespace search
{
  int LinearSearch(i32* array, int size, i32 target, bool visualize, chrono::steady_clock::time_point start_time)
  {
    for (int i = 0; i < size; i++) {
      TRACE(visualize, "Checking index " << i << ": " << array[i] << " == " << target << std::endl);
      
      if (visualize) {
        Viz::draw_state("Linear Search", array, size, start_time, i);
      }

      if (array[i] == target) {
        TRACE(visualize, " -> Found!" << std::endl);
        return i;
      }
    }
    return -1;
  }

  int BinarySearch(i32* array, int size, i32 target, bool visualize, chrono::steady_clock::time_point start_time)
  {
    int left = 0;
    int right = size - 1;

    while (left <= right)
    {
      int middle = left + (right - left) / 2;

      if (visualize) {
        Viz::draw_state("Binary Search", array, size, start_time, left, middle, right);
      } else {
        TRACE(visualize, "Searching range [" << left << ", " << right << "] - Middle: " << middle << " (" << array[middle] << ")" << std::endl);
      }

      if (array[middle] == target) {
        TRACE(visualize, " -> Found at index " << middle << std::endl);
        return middle;
      }

      if (array[middle] < target) {
        left = middle + 1;
      } else {
        right = middle - 1;
      }
    }
    return -1;
  }
}
