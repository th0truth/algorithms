TARGET = main

# Compilers
CC = g++
NVCC = nvcc

INC_DIRS = ./include/core ./include/cpu ./include/gpu
SRC_DIRS = ./src/core ./src/cpu ./src/gpu
BUILD_DIR = ./build

# Find source files
CPP_SRCS = $(shell find $(SRC_DIRS) -name "*.cpp")
CU_SRCS = $(shell find $(SRC_DIRS) -name "*.cu")

# Create object file paths
CPP_OBJS = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(CPP_SRCS))
CU_OBJS = $(patsubst %.cu, $(BUILD_DIR)/%.o, $(CU_SRCS))

# Flags
INC_FLAGS = $(addprefix -I,$(INC_DIRS))
CXXFLAGS = $(INC_FLAGS) -O3 -Wall -std=c++17 -fopenmp
NVCCFLAGS = $(INC_FLAGS) -O3 -std=c++17 -Xcompiler -fopenmp
LDFLAGS = -lSDL2 -lcudart -L/opt/cuda/lib64 -fopenmp

# Default target
all: $(BUILD_DIR)/$(TARGET)

# Compile C++ files
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CXXFLAGS) -c $< -o $@

# Compile CUDA files
$(BUILD_DIR)/%.o: %.cu
	@mkdir -p $(dir $@)
	$(NVCC) $(NVCCFLAGS) -c $< -o $@

# Link all objects
$(BUILD_DIR)/$(TARGET): $(CPP_OBJS) $(CU_OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPP_OBJS) $(CU_OBJS) $(LDFLAGS) -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
