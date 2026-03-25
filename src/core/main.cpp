#include "base.h"
#include "sound.h"
#include "utils.h"
#include "sort.h"
#include "sort.cuh"
#include "search.h"
#include "search.cuh"
#include "config.h"

using namespace std;

int main(int argc, char** argv)
{
  Config config = Config::parse(argc, argv);

  if (config.algorithm.empty()) {
    Config::print_usage(argv[0]);
    return 1;
  }

  if ((config.algorithm == "LinearSearch" || config.algorithm == "BinarySearch" || config.algorithm == "GpuLinearSearch" || config.algorithm == "GpuBinarySearch") && !config.target_set) {
    cerr << "Error: " << config.algorithm << " requires a target value" << endl;
    Config::print_usage(argv[0]);
    return 1;
  }

  vector<i32> data;
  if (config.is_random) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<i32> dis(config.random_min, config.random_max);
    for (int i = 0; i < config.random_count; i++) {
      data.push_back(dis(gen));
    }
  } else {
    for (const string& s : config.raw_elements) {
      try {
        data.push_back(stoi(s));
      } catch (...) {
        cerr << "Error: Invalid array element '" << s << "'" << endl;
        return 1;
      }
    }
  }

  if (data.empty()) {
    cerr << "Error: No data to process" << endl;
    return 1;
  }

  if (config.is_sound) {
    SortAudio::init();
  }

  i32* array = data.data();
  int size = data.size();
  auto start_time = chrono::steady_clock::now();

  // 1. Run Sorting if requested via --sort or if it's the main algorithm
  string active_sort = "";
  if (!config.sort_algorithm.empty()) {
    active_sort = config.sort_algorithm;
  } else if (config.algorithm.find("Sort") != string::npos) {
    active_sort = config.algorithm;
  }

  if (!active_sort.empty()) {
    if (active_sort == "BubbleSort") {
      cpu::sort::BubbleSort(array, size, config.visualize, start_time);
    } else if (active_sort == "InsertionSort") {
      cpu::sort::InsertionSort(array, size, config.visualize, start_time);
    } else if (active_sort == "SelectionSort") {
      cpu::sort::SelectionSort(array, size, config.visualize, start_time);
    } else if (active_sort == "MergeSort") {
      cpu::sort::MergeSort(array, size, config.visualize, start_time);
    } else if (active_sort == "QuickSort") {
      cpu::sort::QuickSort(array, size, config.visualize, start_time);
    } else if (active_sort == "ShellSort") {
      cpu::sort::ShellSort(array, size, config.visualize, start_time);
    } else if (active_sort == "CocktailSort") {
      cpu::sort::CocktailSort(array, size, config.visualize, start_time);
    } else if (active_sort == "HeapSort") {
      cpu::sort::HeapSort(array, size, config.visualize, start_time);
    } else if (active_sort == "GpuMergeSort") {
      gpu::sort::MergeSort(array, size, config.visualize, start_time);
    } else if (config.algorithm.find("Sort") != string::npos) {
       cerr << "Unknown sorting algorithm: " << active_sort << endl;
       if (config.is_sound) SortAudio::cleanup();
       return 1;
    }
  }

  // 2. Run Search if requested
  int search_result = -1;
  bool is_search = false;
  if (config.algorithm == "LinearSearch") {
    search_result = cpu::search::LinearSearch(array, size, config.target, config.visualize, start_time);
    is_search = true;
  } else if (config.algorithm == "BinarySearch") {
    search_result = cpu::search::BinarySearch(array, size, config.target, config.visualize, start_time);
    is_search = true;
  } else if (config.algorithm == "GpuLinearSearch") {
    search_result = gpu::search::LinearSearch(array, size, config.target, config.visualize, start_time);
    is_search = true;
  } else if (config.algorithm == "GpuBinarySearch") {
    search_result = gpu::search::BinarySearch(array, size, config.target, config.visualize, start_time);
    is_search = true;
  }

  if (!is_search && config.algorithm.find("Sort") == string::npos) {
    cerr << "Unknown algorithm: " << config.algorithm << endl;
    if (config.is_sound) SortAudio::cleanup();
    return 1;
  }

  auto end_time = chrono::steady_clock::now();
  auto duration_ns = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
  double duration_s = duration_ns / 1e9;

  if (!config.visualize) {
    if (is_search) {
      if (search_result != -1)
        cout << "\nFound target " << config.target << " at index " << search_result << endl;
      else
        cout << "\nTarget " << config.target << " not found" << endl;
    } else {
      cout << "\nSorted array: ";
      for (int i = 0; i < size; i++) cout << array[i] << " ";
      cout << endl;
    }
  }
  
  cout << "\nExecution time: " << duration_ns << " ns (" << fixed << duration_s << " s)" << endl;

  if (config.is_sound) {
    SortAudio::cleanup();
  }

  return 0;
}
