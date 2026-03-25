#include "base.h"
#include "utils.h"
#include "sound.h"
#include "sort.h"
using namespace std;

namespace cpu::sort {
  static string current_alg_name;
  static int merge_count = 0;

  void BubbleSort(i32* array, int size, bool visualize, chrono::steady_clock::time_point start_time)
  {
    current_alg_name = "Bubble Sort";
    TRACE(visualize, "Start bubble sorting ...\n" << endl);
    int pass = 0;
    int total_swaps = 0;

    for (int i = 0; i < size; i++) {
      pass++;
      TRACE(visualize, "\tPass " << pass << " (comparing elements from index 0 to " << (size - i - 2) << "):" << endl);
      bool swapped = false;
      int pass_swaps = 0;

      for (int j = 0; j < size - i - 1; j++) {
        TRACE(visualize, "\t\tComparing " << array[j] << " and " << array[j+1]);
        if (visualize)
          Viz::draw_state(current_alg_name, array, size, start_time, j, j+1);

        if (array[j] > array[j+1]) {
          TRACE(visualize, " -> Swapping");
          swap(array[j], array[j+1]);
          swapped = true;
          pass_swaps++;
          total_swaps++;
          if (visualize)
            Viz::draw_state(current_alg_name, array, size, start_time, j, j+1);
        }
        TRACE(visualize, endl);
      }

      TRACE(visualize, "\tPass " << pass << " swaps: " << pass_swaps << endl);
      TRACE(visualize, "\tArray now: ");
      for (int k = 0; k < size; k++) {
        TRACE(visualize, array[k] << " ");
      }
      TRACE(visualize, endl);

      if (!swapped) {
        TRACE(visualize, "\tNo swaps made in this pass. Array is sorted!" << endl);
        break;
      }
      TRACE(visualize, endl);
    }
    TRACE(visualize, "\nTotal passes: " << pass << ", Total swaps: " << total_swaps << endl);
    if (visualize) 
      Viz::draw_state(current_alg_name, array, size, start_time);
  }

  void InsertionSort(i32* array, int size, bool visualize, chrono::steady_clock::time_point start_time)
  {
    current_alg_name = "Insertion Sort";
    TRACE(visualize, "Start INSERTION sorting ...\n" << endl);
    int pass = 0;
    int total_shifts = 0;

    for (int i = 1; i < size; i++) {
      pass++;
      TRACE(visualize, "\tPass " << pass << " (processing element at index " << i << "):" << endl);
        
      i32 temp = array[i];
      TRACE(visualize, "\tCurrent element to insert: " << temp << endl);

      int j = i - 1;
      int pass_shifts = 0;
      bool shifted = false;
      
      if (visualize)
        Viz::draw_state(current_alg_name, array, size, start_time, i);

      while (j >= 0 && temp < array[j]) {
        TRACE(visualize, "\t\tShifting " << array[j] << " from index " << j << " to index " << (j+1) << endl);
        
        array[j+1] = array[j];
        j--;

        if (visualize)
          Viz::draw_state(current_alg_name, array, size, start_time, j, j+1);
        
        pass_shifts++;
        total_shifts++;
        shifted = true;
      }
      
      array[j+1] = temp;
      
      if (visualize)
        Viz::draw_state(current_alg_name, array, size, start_time, j+1);
    
      if (shifted) {
        TRACE(visualize, "\tInserted " << temp << " at index " << (j+1) << endl);
      } else {
        TRACE(visualize, "\tElement " << temp << " already in correct position" << endl);
      }

      TRACE(visualize, "\tPass " << pass << " shifts: " << pass_shifts << endl);
      TRACE(visualize, "\tArray now: ");
      for (int k = 0; k < size; k++) {
        TRACE(visualize, array[k] << " ");
      }
      TRACE(visualize, endl << endl);
    }

    TRACE(visualize, "\nTotal passes: " << pass << ", Total shifts: " << total_shifts << endl);
    if (visualize)
      Viz::draw_state(current_alg_name, array, size, start_time);
  }

  void SelectionSort(i32* array, int size, bool visualize, chrono::steady_clock::time_point start_time)
  {
    current_alg_name = "Selection Sort";
    TRACE(visualize, "Start SELECTION sorting ...\n" << endl);
    int pass = 0;
    int total_swaps = 0;

    for (int j = size - 1; j >= 0; j--) {
      TRACE(visualize, "\tPass " << pass++ << " (finding max in range 0 to " << j << "):" << endl);
      
      i32 _max = array[j];
      int idx = j;

      if (visualize)
        Viz::draw_state(current_alg_name, array, size, start_time, j);
      
      for (int i = 0; i < j; i++) {
        TRACE(visualize, "\t\tComparing " << array[i] << " with current max " << _max);

        if (visualize)
          Viz::draw_state(current_alg_name, array, size, start_time, i, idx, j);

        if (array[i] > _max) {
          _max = array[i];
          idx = i;
          
          TRACE(visualize, " -> New max found!");
          if (visualize)
            Viz::draw_state(current_alg_name, array, size, start_time, i, idx, j);
        }
        TRACE(visualize, endl);
      }    

      TRACE(visualize, "\tMax found: " << _max << " at index " << idx << endl);
      
      if (idx != j) {
        TRACE(visualize, "\tSwapping " << array[j] << " at index " << j << " with " << array[idx] << " at index " << idx << endl);
        total_swaps++;
      } else {
        TRACE(visualize, "\tElement already in correct position, no swap needed" << endl);
      }

      swap(array[j], array[idx]);

      if (visualize)
        Viz::draw_state(current_alg_name, array, size, start_time, j, idx);
      
      TRACE(visualize, "\tArray now: ");
      for (int k = 0; k < size; k++) {
        TRACE(visualize, array[k] << " ");
      }
      TRACE(visualize, endl << endl);
    }
    
    TRACE(visualize, "\nTotal passes: " << pass << ", Total swaps: " << total_swaps << endl);

    if (visualize)
      Viz::draw_state(current_alg_name, array, size, start_time);
  }

  static void merge(i32* array, int left, int mid, int right, int total_size, bool visualize, chrono::steady_clock::time_point start_time)
  {
    merge_count++;
    TRACE(visualize, "\n\tMerge #" << merge_count << " (merging indices " << left << "-" << mid << " and " << (mid + 1) << "-" << right << "):" << endl);
    
    int n1 = mid - left + 1;
    int n2 = right - mid;

    i32* L = new i32[n1];
    i32* R = new i32[n2];

    TRACE(visualize, "\t\tLeft subarray: ");
    for (int i = 0; i < n1; i++) {
      L[i] = array[left + i];
      TRACE(visualize, L[i] << " ");
    }
    TRACE(visualize, endl);

    TRACE(visualize, "\t\tRight subarray: ");
    for (int j = 0; j < n2; j++) {
      R[j] = array[mid + 1 + j];
      TRACE(visualize, R[j] << " ");
    }
    TRACE(visualize, endl);

    int i = 0;
    int j = 0;
    int k = left;
    int comparisons = 0;

    TRACE(visualize, "\t\tMerging process:" << endl);
    while (i < n1 && j < n2) {
      comparisons++;
      TRACE(visualize, "\t\t\tComparing " << L[i] << " (left) and " << R[j] << " (right) -> ");

      if (visualize)
        Viz::draw_state(current_alg_name, array, total_size, start_time, left + i, mid + 1 + j, k);

      if (L[i] <= R[j]) {
        TRACE(visualize, L[i] << " is smaller, placing at index " << k);
        array[k] = L[i];
        i++;
      } else {
        TRACE(visualize, R[j] << " is smaller, placing at index " << k);
        array[k] = R[j];
        j++;
      }
      TRACE(visualize, endl);
      if (visualize)
        Viz::draw_state(current_alg_name, array, total_size, start_time, k);
      k++;
    }

    while (i < n1) {
      TRACE(visualize, "\t\t\tCopying remaining " << L[i] << " from left to index " << k << endl);
      array[k] = L[i];
      if (visualize)
        Viz::draw_state(current_alg_name, array, total_size, start_time, k);
      i++;
      k++;
    }

    while (j < n2) {
      TRACE(visualize, "\t\t\tCopying remaining " << R[j] << " from right to index " << k << endl);
      array[k] = R[j];
      if (visualize)
        Viz::draw_state(current_alg_name, array, total_size, start_time, k);
      j++;
      k++;
    }

    delete[] L;
    delete[] R;

    TRACE(visualize, "\t\tComparisons in this merge: " << comparisons << endl);
    TRACE(visualize, "\t\tArray after merge: ");
    for (int idx = left; idx <= right; idx++) {
      TRACE(visualize, array[idx] << " ");
    }
    TRACE(visualize, endl);
  }

  static void merge_sort_helper(i32* array, int left, int right, int total_size, bool visualize, chrono::steady_clock::time_point start_time)
  {
    if (left < right) {
      int mid = left + (right - left) / 2;
      
      TRACE(visualize, "\tSplitting range [" << left << "-" << right << "] at midpoint " << mid << endl);
      
      merge_sort_helper(array, left, mid, total_size, visualize, start_time);
      merge_sort_helper(array, mid + 1, right, total_size, visualize, start_time);
      merge(array, left, mid, right, total_size, visualize, start_time);
    }
  }

  void MergeSort(i32* array, int size, bool visualize, chrono::steady_clock::time_point start_time)
  {
    current_alg_name = "Merge Sort";
    TRACE(visualize, "Start merge sorting ...\n" << endl);
    merge_count = 0;
    
    TRACE(visualize, "Recursive splitting phase:" << endl);
    merge_sort_helper(array, 0, size - 1, size, visualize, start_time);
    
    TRACE(visualize, "\n\nTotal merges performed: " << merge_count << endl);
    if (visualize)
      Viz::draw_state(current_alg_name, array, size, start_time);
  }

  static int quick_sort_partition(i32* array, int low, int high, int total_size, bool visualize, chrono::steady_clock::time_point start_time)
  {
    TRACE(visualize, "\tPartitioning range [" << low << "-" << high << "]" << endl);
    
    int pivot = array[high];
    TRACE(visualize, "\t\tPivot selected: " << pivot << " at index " << high << endl);

    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
      TRACE(visualize, "\t\tComparing " << array[j] << " with pivot " << pivot);
      if (visualize)
        Viz::draw_state(current_alg_name, array, total_size, start_time, j, high, i + 1);
      

      if (array[j] < pivot) {
        i++;
        TRACE(visualize, " -> " << array[j] << " < pivot, swapping with index " << i);
        swap(array[i], array[j]);
        if (visualize)
          Viz::draw_state(current_alg_name, array, total_size, start_time, i, j, high);
      }
      TRACE(visualize, endl);
    }

    TRACE(visualize, "\t\tPlacing pivot at correct position " << (i + 1) << endl);
    swap(array[i + 1], array[high]);
    if (visualize)
      Viz::draw_state(current_alg_name, array, total_size, start_time, i + 1, high);
    
    TRACE(visualize, "\t\tPivot placed at index " << (i + 1) << endl);
    return i + 1;
  }

  static void quick_sort_helper(i32* array, int low, int high, int total_size, bool visualize, chrono::steady_clock::time_point start_time, int &pass_count)
  {
    if (low < high) {
      pass_count++;
      TRACE(visualize, "\nQuickSort Pass #" << pass_count << ": Sorting range [" << low << "-" << high << "]" << endl);
      
      int pi = quick_sort_partition(array, low, high, total_size, visualize, start_time);
      
      TRACE(visualize, "\tPartition complete. Pivot now at index " << pi << endl);
      
      quick_sort_helper(array, low, pi - 1, total_size, visualize, start_time, pass_count);
      quick_sort_helper(array, pi + 1, high, total_size, visualize, start_time, pass_count);
    }
  }

  void QuickSort(i32* array, int size, bool visualize, chrono::steady_clock::time_point start_time)
  {
    current_alg_name = "Quick Sort";
    TRACE(visualize, "Start quick sorting ...\n" << endl);
    int pass_count = 0;
    
    quick_sort_helper(array, 0, size - 1, size, visualize, start_time, pass_count);
    
    TRACE(visualize, "\nTotal partitions: " << pass_count << endl);
    if (visualize)
      Viz::draw_state(current_alg_name, array, size, start_time);
  }

  void ShellSort(i32* array, int size, bool visualize, chrono::steady_clock::time_point start_time)
  {
    current_alg_name = "Shell Sort";
    TRACE(visualize, "Start SHELL sorting ...\n" << endl);
    int pass = 0;
    int total_shifts = 0;

    for (int gap = size / 2; gap > 0; gap /= 2) {
      for (int i = gap; i < size; i++) {
        pass++;
        TRACE(visualize, "\tPass " << pass << " (processing element at index " << i << " with gap " << gap << "):" << endl);

        i32 temp = array[i];
        TRACE(visualize, "\tCurrent element to insert: " << temp << endl);
        
        int j = i;
        int pass_shifts = 0;

        if (visualize)
          Viz::draw_state(current_alg_name, array, size, start_time, i);

        while (j >= gap && array[j - gap] > temp) {
          TRACE(visualize, "\t\tShifting " << array[j - gap] << " from index " << (j - gap) << " to index " << j << endl);
          
          array[j] = array[j - gap];
          j -= gap;
          pass_shifts++;
          total_shifts++;

          if (visualize)
            Viz::draw_state(current_alg_name, array, size, start_time, j, j + gap);
        }
        array[j] = temp;
      }
    }

    TRACE(visualize, "\nTotal passes: " << pass << ", Total shifts: " << total_shifts << endl);
    if (visualize)
      Viz::draw_state(current_alg_name, array, size, start_time);
  }

  void CocktailSort(i32* array, int size, bool visualize, chrono::steady_clock::time_point start_time)
  {
    current_alg_name = "Cocktail Sort";
    TRACE(visualize, "Start cocktail sorting ...\n" << endl);
    int start = 0;
    int end = size - 1;
    bool swapped = true;
    int pass = 0;
    int total_swaps = 0;

    while (swapped) {
      swapped = false;
      pass++;
      TRACE(visualize, "\tPass " << pass << " (forward, comparing elements from index " << start << " to " << end - 1 << "):" << endl);
      int pass_swaps = 0;

      for (int i = start; i < end; i++) {
        TRACE(visualize, "\t\tComparing " << array[i] << " and " << array[i+1]);
        if (visualize) {
          Viz::draw_state(current_alg_name, array, size, start_time, i, i+1);
        }

        if (array[i] > array[i + 1]) {
          TRACE(visualize, " -> Swapping");
          swap(array[i], array[i + 1]);
          swapped = true;
          pass_swaps++;
          total_swaps++;
          if (visualize) {
            Viz::draw_state(current_alg_name, array, size, start_time, i, i+1);
          }
        }
        TRACE(visualize, endl);
      }

      TRACE(visualize, "\tForward pass swaps: " << pass_swaps << endl);

      if (!swapped) {
        TRACE(visualize, "\tNo swaps made in forward pass. Array is sorted!" << endl);
        break;
      }
      
      swapped = false;
      --end;
      pass_swaps = 0;

      TRACE(visualize, "\tPass " << pass << " (backward, comparing elements from index " << end - 1 << " down to " << start << "):" << endl);

      for (int i = end - 1; i >= start; i--) {
        TRACE(visualize, "\t\tComparing " << array[i] << " and " << array[i+1]);
        if (visualize) {
          Viz::draw_state(current_alg_name, array, size, start_time, i, i+1);
        }

        if (array[i] > array[i + 1]) {
          TRACE(visualize, " -> Swapping");
          swap(array[i], array[i + 1]);
          swapped = true;
          pass_swaps++;
          total_swaps++;
          if (visualize) {
            Viz::draw_state(current_alg_name, array, size, start_time, i, i+1);
          }
        }
        TRACE(visualize, endl);
      }

      TRACE(visualize, "\tBackward pass swaps: " << pass_swaps << endl);
      ++start;

      TRACE(visualize, "\tArray now: ");
      for (int k = 0; k < size; k++) {
        TRACE(visualize, array[k] << " ");
      }
      TRACE(visualize, endl << endl);
    }

    TRACE(visualize, "\nTotal passes: " << pass << ", Total swaps: " << total_swaps << endl);
    if (visualize)
      Viz::draw_state(current_alg_name, array, size, start_time);
  }

  static void heapify(i32* array, int size, int index, bool visualize, chrono::steady_clock::time_point start_time, int& total_swaps)
  {
    int largest = index;
    int l = 2 * index + 1;
    int r = 2 * index + 2;

    if (l < size) {
      TRACE(visualize, "\t\tComparing " << array[l] << " (left child) and " << array[largest] << " (largest so far)");
      if (visualize) Viz::draw_heap_state(current_alg_name, array, size, start_time, l, largest);
      if (array[l] > array[largest]) {
        largest = l;
      }
      TRACE(visualize, endl);
    }

    if (r < size) {
      TRACE(visualize, "\t\tComparing " << array[r] << " (right child) and " << array[largest] << " (largest so far)");
      if (visualize) Viz::draw_heap_state(current_alg_name, array, size, start_time, r, largest);
      if (array[r] > array[largest]) {
        largest = r;
      }
      TRACE(visualize, endl);
    }

    if (largest != index) {
      TRACE(visualize, "\t\tSwapping " << array[index] << " and " << array[largest] << endl);
      swap(array[index], array[largest]);
      total_swaps++;
      if (visualize) Viz::draw_heap_state(current_alg_name, array, size, start_time, index, largest);
      heapify(array, size, largest, visualize, start_time, total_swaps);
    }
  }

  void HeapSort(i32* array, int size, bool visualize, chrono::steady_clock::time_point start_time)
  {
    current_alg_name = "Heap Sort";
    TRACE(visualize, "Start heap sorting ...\n" << endl);
    int total_swaps = 0;

    TRACE(visualize, "\tBuilding heap..." << endl);
    for (int i = size / 2 - 1; i >= 0; i--) {
      TRACE(visualize, "\tHeapifying index " << i << endl);
      heapify(array, size, i, visualize, start_time, total_swaps);
    }

    TRACE(visualize, "\tHeap built. Array now: ");
    for (int k = 0; k < size; k++) {
      TRACE(visualize, array[k] << " ");
    }
    TRACE(visualize, endl << endl);

    for (int i = size - 1; i > 0; i--) {
      TRACE(visualize, "\tExtracting element " << array[0] << " to position " << i << " (swapping with " << array[i] << ")" << endl);
      swap(array[0], array[i]);
      total_swaps++;
      if (visualize) Viz::draw_heap_state(current_alg_name, array, size, start_time, 0, i);

      TRACE(visualize, "\tHeapifying reduced heap (size " << i << ") at index 0" << endl);
      heapify(array, i, 0, visualize, start_time, total_swaps);

      TRACE(visualize, "\tArray now: ");
      for (int k = 0; k < size; k++) {
        TRACE(visualize, array[k] << " ");
      }
      TRACE(visualize, endl << endl);
    }

    TRACE(visualize, "\nTotal swaps: " << total_swaps << endl);
    if (visualize)
      Viz::draw_heap_state(current_alg_name, array, size, start_time);
  }
}
