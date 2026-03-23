#include <iostream>
#include <chrono>
#include <thread>
#include "utils.h"
#include "sound.h"

using namespace std;

void Viz::draw_state(const string& current_alg_name, i32* array, int size, chrono::steady_clock::time_point start_time, int h1, int h2, int h3)
{
  cout << "\033[2J\033[H";
  auto now = chrono::steady_clock::now();
  auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
  
  cout << "\"" << current_alg_name << "\" Visualizing Sorting Algorithm... [Elapsed Time: " << elapsed << " ms]\n\n";
  
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
  
  if (h1 != -1) {
    SortAudio::play_tone(array[h1], max_val, 30);
  } else {
    this_thread::sleep_for(chrono::milliseconds(30));
  }
}

void Viz::print_heap_node(i32* array, int size, int index, const string& prefix, bool is_last, int h1, int h2, int h3) {
  if (index >= size) return;

  cout << prefix;
  if (index > 0)
    cout << (is_last ? "\xe2\x94\x94\xe2\x94\x80\xe2\x94\x80 " : "\xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 ");

  if (index == h1 || index == h2 || index == h3) cout << "\033[1;31m";
  cout << array[index];

  if (index == h1 || index == h2 || index == h3) cout << "\033[0m";
  cout << "\n";

  int left = 2 * index + 1;
  int right = 2 * index + 2;

  string new_prefix = prefix;
  if (index > 0)
    new_prefix += (is_last ? "    " : "\xe2\x94\x82   ");

  if (left < size) {
      bool left_is_last = (right >= size);
      print_heap_node(array, size, left, new_prefix, left_is_last, h1, h2, h3);
  }
  if (right < size)
    print_heap_node(array, size, right, new_prefix, true, h1, h2, h3);
}

void Viz::draw_heap_state(const string& current_alg_name, i32* array, int size, chrono::steady_clock::time_point start_time, int h1, int h2, int h3)
{
  cout << "\033[2J\033[H";
  auto now = chrono::steady_clock::now();
  auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
  
  cout << "\"" << current_alg_name << "\" Visualizing Sorting Algorithm... [Elapsed Time: " << elapsed << " ms]\n\n";

  int max_val = 1;
  for (int i = 0; i < size; i++) {
    if (array[i] > max_val)
      max_val = array[i];
  }

  cout << "Heap Tree Representation:\n";
  print_heap_node(array, size, 0, "", true, h1, h2, h3);
  cout << "\nArray State:\n";
  for (int i = 0; i < size; i++) {
    printf("%4d | ", array[i]);
    if (i == h1 || i == h2 || i == h3) {
      cout << "\033[1;31m";
    }
    int bars = (max(0, array[i]) * 50) / max_val;
    for (int j = 0; j < bars; j++) {
      cout << "\xe2\x96\x88";
    }
    if (i == h1 || i == h2 || i == h3) 
      cout << "\033[0m";
    cout << "\n";
  }
  cout << flush;
  
  if (h1 != -1) {
    SortAudio::play_tone(array[h1], max_val, 30);
  } else {
    this_thread::sleep_for(chrono::milliseconds(30));
  }
}
