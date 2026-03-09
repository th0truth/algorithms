#include "base.h"
#include "algorithms.h"
#include "sound.h"
using namespace std;

struct Config {
  string algorithm;
  vector<string> raw_elements;
  bool visualize = false;
  bool is_random = false;
  bool is_sound = false;
  int random_count = 0;
  i32 random_min = 0;
  i32 random_max = 0;
};

void print_usage(const char* program_name)
{
  cout << "Usage: " << program_name << " <algorithm> [--visualize] [--sound] <array_elements...>" << endl;
  cout << "       " << program_name << " <algorithm> [--visualize] [--sound] random <count> <min> <max>" << endl;
  cout << "\nAvailable algorithms:" << endl;
  cout << "  BubbleSort      - Bubble sort algorithm" << endl;
  cout << "  InsertionSort   - Insertion sort algorithm" << endl;
  cout << "  SelectionSort   - Selection sort algorithm" << endl;
  cout << "  MergeSort       - Merge sort algorithm" << endl;
  cout << "  QuickSort       - Quick sort algorithm" << endl;
  cout << "  ShellSort       - Shell sort algorithm" << endl;
  cout << "\nOptions:" << endl;
  cout << "  --visualize     Show visual representation of sorting" << endl;
  cout << "  --sound         Enable sound during visualization" << endl;
  cout << "\nExample:" << endl;
  cout << "  " << program_name << " BubbleSort 64 34 25 12 22 11 90" << endl;
  cout << "  " << program_name << " BubbleSort --visualize random 15 1 100" << endl;
  cout << "  " << program_name << " BubbleSort --visualize --sound random 15 1 100" << endl;
}

bool parse_arguments(int argc, char **argv, Config &config)
{
  if (argc < 3)
    return false;

  vector<string> args;
  for (int i = 1; i < argc; i++) {
    string arg = argv[i];
    if (arg == "--visualize") {
      config.visualize = true;
    } else if (arg == "--sound") {
      config.is_sound = true;
    } else {
      args.push_back(arg);
    }
  }

  if (args.empty())
    return false;

  config.algorithm = args[0];

  if (args.size() > 1 && args[1] == "random") {
    if (args.size() != 5) {
      cerr << "Error: 'random' option requires 3 arguments: <count> <min> <max>" << endl;
      return false;
    }
    config.is_random = true;
    config.random_count = stoi(args[2]);
    config.random_min = static_cast<i32>(stoi(args[3]));
    config.random_max = static_cast<i32>(stoi(args[4]));
  } else {
    for (size_t i = 1; i < args.size(); i++) {
      config.raw_elements.push_back(args[i]);
    }
  }

  return true;
}

vector<i32> prepare_array(const Config &config)
{
  vector<i32> array;

  if (config.is_random) {
    if (config.random_count <= 0 || config.random_min > config.random_max)
      return array;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<i32> dis(config.random_min, config.random_max);
    for (int i = 0; i < config.random_count; i++) {
      array.push_back(dis(gen));
    }
  } else {
    for (const string &s : config.raw_elements) {
      array.push_back(static_cast<i32>(stoi(s)));
    }
  }

  return array;
}

void execute_sort(const string &algorithm, i32 *data, int size, bool visualize, chrono::steady_clock::time_point begin)
{
  if (algorithm == "BubbleSort") {
    sort::BubbleSort(data, size, visualize, begin);
  } else if (algorithm == "InsertionSort") {
    sort::InsertionSort(data, size, visualize, begin);
  } else if (algorithm == "SelectionSort") {
    sort::SelectionSort(data, size, visualize, begin);
  } else if (algorithm == "MergeSort") {
    sort::MergeSort(data, size, visualize, begin);
  } else if (algorithm == "QuickSort") {
    sort::QuickSort(data, size, visualize, begin);
  } else if (algorithm == "ShellSort") {
    sort::ShellSort(data, size, visualize, begin);
  } else {
    cerr << "Error: Unknown algorithm '" << algorithm << "'" << endl;
  }
}

int main(int argc, char** argv)
{
  Config config;
  if (!parse_arguments(argc, argv, config)) {
    print_usage(argv[0]);
    return 1;
  }

  vector<i32> array = prepare_array(config);
  if (array.empty()) {
    cerr << "Error: Invalid array parameters or empty array" << endl;
    return 1;
  }

  if (!config.visualize) {
    cout << "Original array: ";
    for (i32 val : array) {
      cout << val << " ";
    }
    cout << "\n\nRunning " << config.algorithm << "..." << endl;
  }

  if (config.visualize && config.is_sound)
    SortAudio::init();

  auto begin = chrono::steady_clock::now();
  execute_sort(config.algorithm, array.data(), static_cast<int>(array.size()), config.visualize, begin);
  auto end = chrono::steady_clock::now();

  if (!config.visualize) {
    cout << "\nSorted: ";
    for (i32 val : array) {
      cout << val << " ";
    }
  } else {
    cout << "Sorted completely.\n";
    if (config.is_sound)
      SortAudio::cleanup();
  }

  cout << "\nDuration time: " << chrono::duration_cast<chrono::nanoseconds>(end - begin).count() << " ns" << endl;

  return 0;
}
