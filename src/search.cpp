#include "base.h"

#include <chrono>
#include <thread>

#include "sound.h"
#include "utils.h"

namespace search
{
  int LinearSearch(i32* array, int size, i32 target, bool visualize, std::chrono::steady_clock::time_point start_time) {
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
}
