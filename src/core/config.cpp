#include "config.h"
#include <iostream>
using namespace std;

void Config::print_usage(const char* program_name)
{
  cout << "\033[1;36mAlgorithm Visualizer CLI v2.0\033[0m\n";
  cout << "--------------------------------------------------\n";
  cout << "Usage:\n";
  cout << "  " << program_name << " <algorithm> [flags] <elements...>\n";
  cout << "  " << program_name << " BinarySearch <target> --sort <sort_alg> [flags] <elements...>\n";
  cout << "  " << program_name << " <algorithm> --random <count> [min] [max] [flags]\n\n";
  cout << "Algorithms:\n";
  cout << "  - Sorts:   BubbleSort, InsertionSort, SelectionSort, MergeSort, \n";
  cout << "             QuickSort, ShellSort, CocktailSort, HeapSort, GpuMergeSort\n";
  cout << "  - Search:  LinearSearch, BinarySearch, GpuLinearSearch, GpuBinarySearch\n\n";
  cout << "Flags:\n";
  cout << "  --visualize    Enable step-by-step visualization\n";
  cout << "  --sound        Enable audio feedback\n";
  cout << "  --sort <alg>   Sort data before searching (required for BinarySearch)\n";
  cout << "  --random <n>   Generate 'n' random elements\n";
  cout << "--------------------------------------------------\n";
}

Config Config::parse(int argc, char** argv)
{
  Config config;
  if (argc < 2) return config;

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
      if ((config.algorithm == "LinearSearch" || config.algorithm == "BinarySearch" || config.algorithm == "GpuLinearSearch" || config.algorithm == "GpuBinarySearch") && !config.target_set) {
        try {
          config.target = stoi(arg);
          config.target_set = true;
        } catch (...) {
          // Fallback or handle later
        }
      } else {
        config.raw_elements.push_back(arg);
      }
    }
  }

  return config;
}
