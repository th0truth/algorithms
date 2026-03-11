# Sorting Algorithms Visualizer

A real-time sorting algorithm visualizer with audio feedback, built in C++.

![Build Status](https://img.shields.io/badge/build-passing-green)
![C++](https://img.shields.io/badge/C++-17-blue)
![SDL2](https://img.shields.io/badge/SDL2-2.0+-orange)

## Features

### Supported Algorithms
| Algorithm | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| BubbleSort | O(n²) | O(1) |
| CocktailSort | O(n²) | O(1) |
| HeapSort | O(n log n) | O(1) |
| InsertionSort | O(n²) | O(1) |
| MergeSort | O(n log n) | O(n) |
| QuickSort | O(n log n) avg | O(log n) |
| SelectionSort | O(n²) | O(1) |
| ShellSort | O(n log n) | O(1) |

### Visualization Mode
Watch sorting algorithms in action with ASCII bar charts that animate in real-time. Each comparison and swap is visually highlighted, making it easy to understand how each algorithm works.

### Audio Feedback
Optional sound effects that play tones based on the value being sorted. Higher values produce higher pitches, creating a musical representation of the sorting process.

- `--visualize` - Show visual representation
- `--sound` - Enable audio (requires `--visualize`)

### Flexible Input
- Provide custom arrays: `BubbleSort 64 34 25 12 22 11 90`
- Generate random arrays: `BubbleSort random 15 1 100`
- Combine flags: `BubbleSort --visualize --sound random 20 1 50`

### Detailed Logging
Step-by-step TRACE logging shows:
- Each pass number
- Comparisons made
- Swaps performed
- Array state after each operation

### Performance Metrics
Automatically measures and displays execution time in nanoseconds.

## Installation

### Prerequisites
- C++17 compiler
- SDL2 library

### Build
```bash
make
```

### Clean
```bash
make clean
```

## Usage

```bash
./build/main.o <algorithm> [--visualize] [--sound] <array_elements...>
./build/main.o <algorithm> [--visualize] [--sound] random <count> <min> <max>
```

### Examples

```bash
# Basic sorting
./build/main.o BubbleSort 64 34 25 12 22 11 90

# Visualize sorting
./build/main.o QuickSort --visualize random 20 1 100

# Visualize with sound
./build/main.o MergeSort --visualize --sound random 15 1 50

# Compare algorithms
./build/main.o SelectionSort 50 25 75 10 30
./build/main.o InsertionSort 50 25 75 10 30
```

## Project Structure

```
algorithms/
├── include/
│   ├── algorithms.h    # Algorithm declarations
│   ├── base.h          # Base definitions and types
│   └── sound.h         # Audio system
├── src/
│   ├── main.cpp        # Entry point & CLI
│   ├── algorithms.cpp  # Sorting implementations
│   └── sound.cpp       # SDL2 audio
├── Makefile
└── README.md
```

## How It Works

### TRACE System
The `TRACE()` macro outputs detailed logging when visualization is disabled:
```cpp
TRACE("Comparing " << a << " and " << b << endl);
```

### Visualization
Uses ANSI escape codes for terminal control:
- `\033[2J\033[H` - Clear screen and home cursor
- `\033[1;31m` - Red highlight for active elements
- `\033[0m` - Reset formatting

### Audio
SDL2-based tone generator:
- Frequency mapped to element value
- 30ms duration per tone
- Configurable max frequency

## License

MIT License
