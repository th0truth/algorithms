#include "base.h"
#include "sort.h"
#include "search.h"
#include "sound.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <random>

using namespace std;

struct Config {
  string algorithm;
  vector<string> raw_elements;
  i32 target = 0;
  bool target_set = false;
  bool visualize = false;
  bool is_random = false;
  bool is_sound = false;
  int random_count = 0;
  i32 random_min = 0;
  i32 random_max = 0;
};

void print_usage(const char* program_name)
{
  cout << "Usage: " << program_name << " <algorithm> [flags] <elements...>" << endl;
  cout << "       " << program_name << " <algorithm> [flags] random <count> <min> <max>" << endl;
  cout << "       " << program_name << " LinearSearch <target> [flags] <elements...>" << endl;
  cout << "\nFlags: --visualize, --sound" << endl;
  cout << "Algorithms: BubbleSort, InsertionSort, SelectionSort, MergeSort, QuickSort, ShellSort, CocktailSort, HeapSort, LinearSearch" << endl;
}

int main(int argc, char* argv[])
{
  if (argc < 2) {
    print_usage(argv[0]);
    return 1;
  }

  Config config;
  config.algorithm = argv[1];

  for (int i = 2; i < argc; i++) {
    string arg = argv[i];
    
    if (arg == "--visualize") {
      config.visualize = true;
    } else if (arg == "--sound") {
      config.is_sound = true;
    } else if (arg == "random") {
      config.is_random = true;
      if (i + 3 < argc) {
        config.random_count = stoi(argv[++i]);
        config.random_min = stoi(argv[++i]);
        config.random_max = stoi(argv[++i]);
      } else {
        cerr << "Error: random requires count, min, and max" << endl;
        return 1;
      }
    } else {
      // If it's LinearSearch and we haven't set a target yet, the first non-flag is the target
      if (config.algorithm == "LinearSearch" && !config.target_set) {
        try {
          config.target = stoi(arg);
          config.target_set = true;
        } catch (...) {
          cerr << "Error: Invalid target '" << arg << "' for LinearSearch" << endl;
          return 1;
        }
      } else {
        config.raw_elements.push_back(arg);
      }
    }
  }

  if (config.algorithm == "LinearSearch" && !config.target_set) {
    cerr << "Error: LinearSearch requires a target value" << endl;
    print_usage(argv[0]);
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

  int search_result = -1;
  if (config.algorithm == "BubbleSort") {
    sort::BubbleSort(array, size, config.visualize, start_time);
  } else if (config.algorithm == "InsertionSort") {
    sort::InsertionSort(array, size, config.visualize, start_time);
  } else if (config.algorithm == "SelectionSort") {
    sort::SelectionSort(array, size, config.visualize, start_time);
  } else if (config.algorithm == "MergeSort") {
    sort::MergeSort(array, size, config.visualize, start_time);
  } else if (config.algorithm == "QuickSort") {
    sort::QuickSort(array, size, config.visualize, start_time);
  } else if (config.algorithm == "ShellSort") {
    sort::ShellSort(array, size, config.visualize, start_time);
  } else if (config.algorithm == "CocktailSort") {
    sort::CocktailSort(array, size, config.visualize, start_time);
  } else if (config.algorithm == "HeapSort") {
    sort::HeapSort(array, size, config.visualize, start_time);
  } else if (config.algorithm == "LinearSearch") {
    search_result = search::LinearSearch(array, size, config.target, config.visualize, start_time);
  } else {
    cerr << "Unknown algorithm: " << config.algorithm << endl;
    if (config.is_sound) SortAudio::cleanup();
    return 1;
  }

  auto end_time = chrono::steady_clock::now();
  auto duration = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();

  if (!config.visualize) {
    if (config.algorithm == "LinearSearch") {
      if (search_result != -1) cout << "\nFound target " << config.target << " at index " << search_result << endl;
      else cout << "\nTarget " << config.target << " not found" << endl;
    } else {
      cout << "\nSorted array: ";
      for (int i = 0; i < size; i++) {
        cout << array[i] << " ";
      }
      cout << endl;
    }
  }
  
  cout << "\nExecution time: " << duration << " ns" << endl;

  if (config.is_sound) {
    SortAudio::cleanup();
  }

  return 0;
}
