#include <iostream>
#include <cstdint>
#include "base.h"
#include <thread>
#include <chrono>

#include "algorithms.h"
#include "sound.h"

using namespace std;

namespace sort
{
  static void draw_state(i32* array, int size, chrono::steady_clock::time_point start_time, int h1 = -1, int h2 = -1, int h3 = -1)
  {
    cout << "\033[2J\033[H";
    auto now = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
    
    cout << "Visualizing Sorting Algorithm... [Elapsed Time: " << elapsed << " ms]\n\n";
    
    int max_val = 1;
    for (int i = 0; i < size; i++) {
      if (array[i] > max_val) {
        max_val = array[i];
      }
    }
    for (int i = 0; i < size; i++) {
      printf("%4d | ", array[i]);
      if (i == h1 || i == h2 || i == h3) {
        cout << "\033[1;31m";
      }
      int bars = (max(0, array[i]) * 50) / max_val;
      for (int j = 0; j < bars; j++) {
        cout << "\xe2\x96\x88";
      }
      if (i == h1 || i == h2 || i == h3) {
        cout << "\033[0m";
      }
      cout << "\n";
    }
    cout << flush;
    
    // If we have a highlighted element (like a swap or comparison), play its tone
    if (h1 != -1) {
      SortAudio::play_tone(array[h1], max_val, 30);
    } else {
      // Sleep to let the audio play and the frame render at a viewable speed
      this_thread::sleep_for(chrono::milliseconds(30));
    }
  }

  void BubbleSort(i32* array, int size, bool visualize, chrono::steady_clock::time_point start_time)
  {
    TRACE("Start bubble sorting ...\n" << endl);
    int pass = 0;
    int total_swaps = 0;

    for (int i = 0; i < size; i++) {
      pass++;
      TRACE("\tPass " << pass << " (comparing elements from index 0 to " << (size - i - 2) << "):" << endl);
      bool swapped = false;
      int pass_swaps = 0;

      for (int j = 0; j < size - i - 1; j++) {
        TRACE("\t\tComparing " << array[j] << " and " << array[j+1]);
        if (visualize) {
          draw_state(array, size, start_time, j, j+1);
        }

        if (array[j] > array[j+1]) {
          TRACE(" -> Swapping");
          swap(array[j], array[j+1]);
          swapped = true;
          pass_swaps++;
          total_swaps++;
          if (visualize) {
            draw_state(array, size, start_time, j, j+1);
          }
        }
        TRACE(endl);
      }

      TRACE("\tPass " << pass << " swaps: " << pass_swaps << endl);
      TRACE("\tArray now: ");
      for (int k = 0; k < size; k++) {
        TRACE(array[k] << " ");
      }
      TRACE(endl);

      if (!swapped) {
        TRACE("\tNo swaps made in this pass. Array is sorted!" << endl);
        break;
      }
      TRACE(endl);
    }
    TRACE("\nTotal passes: " << pass << ", Total swaps: " << total_swaps << endl);
    if (visualize) 
      draw_state(array, size, start_time);
  }

  void InsertionSort(i32* array, int size, bool visualize, chrono::steady_clock::time_point start_time)
  {
    TRACE("Start INSERTION sorting ...\n" << endl);
    int pass = 0;
    int total_shifts = 0;

    for (int i = 1; i < size; i++) {
      pass++;
      TRACE("\tPass " << pass << " (processing element at index " << i << "):" << endl);
        
      // Store temporary element of the array
      i32 temp = array[i];
      TRACE("\tCurrent element to insert: " << temp << endl);

      int j = i - 1;
      int pass_shifts = 0;
      bool shifted = false;
      
      if (visualize)
        draw_state(array, size, start_time, i);

      // compare with the previous element (temp < j) 
      while (j >= 0 && temp < array[j]) {
        TRACE("\t\tShifting " << array[j] << " from index " << j << " to index " << (j+1) << endl);
        
        // shift elem
        array[j+1] = array[j];
        j--;

        if (visualize)
          draw_state(array, size, start_time, j, j+1);
        
        pass_shifts++;
        total_shifts++;
        shifted = true;
      }
      
      array[j+1] = temp;
      
      if (visualize)
        draw_state(array, size, start_time, j+1);
    
      if (shifted) {
        TRACE("\tInserted " << temp << " at index " << (j+1) << endl);
      } else {
        TRACE("\tElement " << temp << " already in correct position" << endl);
      }

      TRACE("\tPass " << pass << " shifts: " << pass_shifts << endl);
      TRACE("\tArray now: ");
      for (int k = 0; k < size; k++) {
        TRACE(array[k] << " ");
      }
      TRACE(endl << endl);
    }

    TRACE("\nTotal passes: " << pass << ", Total shifts: " << total_shifts << endl);
    if (visualize)
      draw_state(array, size, start_time);
  }

  void SelectionSort(i32* array, int size, bool visualize, chrono::steady_clock::time_point start_time)
  {
    TRACE("Start SELECTION sorting ...\n" << endl);
    int pass = 0;
    int total_swaps = 0;

    for (int j = size - 1; j >= 0; j--) {
      TRACE("\tPass " << pass++ << " (finding max in range 0 to " << j << "):" << endl);
      
      i32 _max = array[j];
      int idx = j;

      if (visualize)
        draw_state(array, size, start_time, j);
      
      // Iterate to the j index (size - 1)
      for (int i = 0; i < j; i++) {
        TRACE("\t\tComparing " << array[i] << " with current max " << _max);

        if (visualize)
          draw_state(array, size, start_time, i, idx, j);

        // Compare array[i] with _max, if it greater than _max,
        // rewrite _max variable, and index (idx) of the _max
        if (array[i] > _max) {
          _max = array[i];
          idx = i;
          
          TRACE(" -> New max found!");
          if (visualize)
            draw_state(array, size, start_time, i, idx, j);
        }
        TRACE(endl);
      }    

      TRACE("\tMax found: " << _max << " at index " << idx << endl);
      
      if (idx != j) {
        TRACE("\tSwapping " << array[j] << " at index " << j << " with " << array[idx] << " at index " << idx << endl);
        total_swaps++;
      } else {
        TRACE("\tElement already in correct position, no swap needed" << endl);
      }

      swap(array[j], array[idx]);

      if (visualize)
        draw_state(array, size, start_time, j, idx);
      
      TRACE("\tArray now: ");
      for (int k = 0; k < size; k++) {
        TRACE(array[k] << " ");
      }
      TRACE(endl << endl);
    }
    
    TRACE("\nTotal passes: " << pass << ", Total swaps: " << total_swaps << endl);

    if (visualize)
      draw_state(array, size, start_time);
  }

  static void merge(i32* array, int left, int mid, int right, int total_size, bool visualize, chrono::steady_clock::time_point start_time)
  {
    merge_count++;
    TRACE("\n\tMerge #" << merge_count << " (merging indices " << left << "-" << mid << " and " << (mid + 1) << "-" << right << "):" << endl);
    
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Dynamically allocate memory for new arrays
    i32* L = new i32[n1];
    i32* R = new i32[n2];

    // Devide an array into two parts
    // - Left subarray 
    // - Right subarray

    TRACE("\t\tLeft subarray: ");
    for (int i = 0; i < n1; i++) {
      L[i] = array[left + i];
      TRACE(L[i] << " ");
    }
    TRACE(endl);

    TRACE("\t\tRight subarray: ");
    for (int j = 0; j < n2; j++) {
      R[j] = array[mid + 1 + j];
      TRACE(R[j] << " ");
    }
    TRACE(endl);

    int i = 0;
    int j = 0;
    int k = left;
    int comparisons = 0;

    TRACE("\t\tMerging process:" << endl);
    while (i < n1 && j < n2) {
      comparisons++;
      TRACE("\t\t\tComparing " << L[i] << " (left) and " << R[j] << " (right) -> ");

      if (visualize)
        draw_state(array, total_size, start_time, left + i, mid + 1 + j, k);

      if (L[i] <= R[j]) {
        TRACE(L[i] << " is smaller, placing at index " << k);
        array[k] = L[i];
        i++;
      } else {
        TRACE(R[j] << " is smaller, placing at index " << k);
        array[k] = R[j];
        j++;
      }
      TRACE(endl);
      if (visualize)
        draw_state(array, total_size, start_time, k);
      k++;
    }

    // Copy the remaining elements of L[]
    while (i < n1) {
      TRACE("\t\t\tCopying remaining " << L[i] << " from left to index " << k << endl);
      array[k] = L[i];
      if (visualize)
        draw_state(array, total_size, start_time, k);
      i++;
      k++;
    }

    // Copy the remaining elements of R[]
    while (j < n2) {
      TRACE("\t\t\tCopying remaining " << R[j] << " from right to index " << k << endl);
      array[k] = R[j];
      if (visualize)
        draw_state(array, total_size, start_time, k);
      j++;
      k++;
    }

    // Deallocate memory
    delete[] L;
    delete[] R;

    TRACE("\t\tComparisons in this merge: " << comparisons << endl);
    TRACE("\t\tArray after merge: ");
    for (int idx = left; idx <= right; idx++) {
      TRACE(array[idx] << " ");
    }
    TRACE(endl);
  }

  static void merge_sort_helper(i32* array, int left, int right, int total_size, bool visualize, chrono::steady_clock::time_point start_time)
  {
    if (left < right) {
      int mid = left + (right - left) / 2;
      
      TRACE("\tSplitting range [" << left << "-" << right << "] at midpoint " << mid << endl);
      
      merge_sort_helper(array, left, mid, total_size, visualize, start_time);
      merge_sort_helper(array, mid + 1, right, total_size, visualize, start_time);
      merge(array, left, mid, right, total_size, visualize, start_time);
    }
  }

  void MergeSort(i32* array, int size, bool visualize, chrono::steady_clock::time_point start_time)
  {
    TRACE("Start merge sorting ...\n" << endl);
    merge_count = 0;
    
    TRACE("Recursive splitting phase:" << endl);
    merge_sort_helper(array, 0, size - 1, size, visualize, start_time);
    
    TRACE("\n\nTotal merges performed: " << merge_count << endl);
    if (visualize)
      draw_state(array, size, start_time);
  }

  static int quick_sort_partition(i32* array, int low, int high, int total_size, bool visualize, chrono::steady_clock::time_point start_time)
  {
    TRACE("\tPartitioning range [" << low << "-" << high << "]" << endl);
    
    int pivot = array[high];
    TRACE("\t\tPivot selected: " << pivot << " at index " << high << endl);

    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
      TRACE("\t\tComparing " << array[j] << " with pivot " << pivot);
      if (visualize)
        draw_state(array, total_size, start_time, j, high, i + 1);
      

      if (array[j] < pivot) {
        i++;
        TRACE(" -> " << array[j] << " < pivot, swapping with index " << i);
        swap(array[i], array[j]);
        if (visualize)
          draw_state(array, total_size, start_time, i, j, high);
      }
      TRACE(endl);
    }

    TRACE("\t\tPlacing pivot at correct position " << (i + 1) << endl);
    swap(array[i + 1], array[high]);
    if (visualize)
      draw_state(array, total_size, start_time, i + 1, high);
    
    TRACE("\t\tPivot placed at index " << (i + 1) << endl);
    return i + 1;
  }

  static void quick_sort_helper(i32* array, int low, int high, int total_size, bool visualize, chrono::steady_clock::time_point start_time, int &pass_count)
  {
    if (low < high) {
      pass_count++;
      TRACE("\nQuickSort Pass #" << pass_count << ": Sorting range [" << low << "-" << high << "]" << endl);
      
      int pi = quick_sort_partition(array, low, high, total_size, visualize, start_time);
      
      TRACE("\tPartition complete. Pivot now at index " << pi << endl);
      
      quick_sort_helper(array, low, pi - 1, total_size, visualize, start_time, pass_count);
      quick_sort_helper(array, pi + 1, high, total_size, visualize, start_time, pass_count);
    }
  }

  void QuickSort(i32* array, int size, bool visualize, chrono::steady_clock::time_point start_time)
  {
    TRACE("Start quick sorting ...\n" << endl);
    int pass_count = 0;
    
    quick_sort_helper(array, 0, size - 1, size, visualize, start_time, pass_count);
    
    TRACE("\nTotal partitions: " << pass_count << endl);
    if (visualize)
      draw_state(array, size, start_time);
  }
}
