#pragma once
#include "base.h"
#include <vector>
#include <string>

struct Config {
  std::string algorithm;
  std::string sort_algorithm;
  std::vector<std::string> raw_elements;
  i32 target = 0;
  bool target_set = false;
  bool visualize = false;
  bool is_random = false;
  bool is_sound = false;
  int random_count = 0;
  i32 random_min = 0;
  i32 random_max = 0;

  static Config parse(int argc, char** argv);
  static void print_usage(const char* program_name);
};
