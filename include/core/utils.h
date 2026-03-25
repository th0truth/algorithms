#pragma once
#include "base.h"
using namespace std;

class Viz {
  public:
    static void draw_state(const string& current_alg_name, i32* array, int size, chrono::steady_clock::time_point start_time, int h1 = -1, int h2 = -1, int h3 = -1);
    static void draw_heap_state(const string& current_alg_name, i32* array, int size, chrono::steady_clock::time_point start_time, int h1 = -1, int h2 = -1, int h3 = -1);

  private:
    static void print_heap_node(i32* array, int size, int index, const string& prefix, bool is_last, int h1, int h2, int h3);
};
