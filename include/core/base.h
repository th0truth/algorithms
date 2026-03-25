#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <random>

typedef int32_t i32;
typedef int16_t i16;

// Global trace macro
#define TRACE(visualize, x) do { if (!visualize) { std::cout << x; } } while(0)
