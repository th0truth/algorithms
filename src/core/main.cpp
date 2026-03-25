#include "base.h"
#include "sound.h"
#include "utils.h"
#include "sort.h"
#include "search.h"
using namespace std;

struct Config {
  string algorithm;
  string sort_algorithm;
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
  cout << "\033[1;36mAlgorithm Visualizer CLI v2.0\033[0m\n";
  cout << "--------------------------------------------------\n";
  cout << "Usage:\n";
  cout << "  " << program_name << " <algorithm> [flags] <elements...>\n";
  cout << "  " << program_name << " BinarySearch <target> --sort <sort_alg> [flags] <elements...>\n";
  cout << "  " << program_name << " <algorithm> --random <count> [min] [max] [flags]\n\n";
  cout << "Algorithms:\n";
  cout << "  - Sorts:   BubbleSort, InsertionSort, SelectionSort, MergeSort, \n";
  cout << "             QuickSort, ShellSort, CocktailSort, HeapSort\n";
  cout << "  - Search:  LinearSearch, BinarySearch\n\n";
  cout << "Flags:\n";
  cout << "  --visualize    Enable step-by-step visualization\n";
  cout << "  --sound        Enable audio feedback\n";
  cout << "  --sort <alg>   Sort data before searching (required for BinarySearch)\n";
  cout << "  --random <n>   Generate 'n' random elements\n";
  cout << "--------------------------------------------------\n";
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
    } else if (arg == "--sort") {
      if (i + 1 < argc) {
        config.sort_algorithm = argv[++i];
      } else {
        cerr << "Error: --sort requires an algorithm name" << endl;
        return 1;
      }
    } else if (arg == "random" || arg == "--random") {
      config.is_random = true;
      if (i + 1 < argc && isdigit(argv[i+1][0])) {
        config.random_count = stoi(argv[++i]);
        if (i + 1 < argc && isdigit(argv[i+1][0])) {
          config.random_min = stoi(argv[++i]);
          if (i + 1 < argc && isdigit(argv[i+1][0])) {
            config.random_max = stoi(argv[++i]);
          } else {
            config.random_max = 100;
          }
        } else {
          config.random_min = 1;
          config.random_max = 100;
        }
      } else {
        config.random_count = 10;
        config.random_min = 1;
        config.random_max = 100;
      }
    } else {
      // If it's LinearSearch or BinarySearch and we haven't set a target yet, the first non-flag is the target
      if ((config.algorithm == "LinearSearch" || config.algorithm == "BinarySearch") && !config.target_set) {
        try {
          config.target = stoi(arg);
          config.target_set = true;
        } catch (...) {
          cerr << "Error: Invalid target '" << arg << "' for " << config.algorithm << endl;
          return 1;
        }
      } else {
        config.raw_elements.push_back(arg);
      }
    }
  }

  if ((config.algorithm == "LinearSearch" || config.algorithm == "BinarySearch") && !config.target_set) {
    cerr << "Error: " << config.algorithm << " requires a target value" << endl;
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
  }

  if (!is_search && config.algorithm.find("Sort") == string::npos) {
    cerr << "Unknown algorithm: " << config.algorithm << endl;
    if (config.is_sound) SortAudio::cleanup();
    return 1;
  }

  auto end_time = chrono::steady_clock::now();
  auto duration = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();

  if (!config.visualize) {
    if (is_search) {
      if (search_result != -1)
        cout << "\nFound target " << config.target << " at index " << search_result << endl;
      else
        cout << "\nTarget " << config.target << " not found" << endl;
    } else {
      cout << "\nSorted array: ";
      
      for (int i = 0; i < size; i++)
      {
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
