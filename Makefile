TARGET = main
CC = g++

SRC_DIRS = ./src/core ./src/cpu
BUILD_DIR= ./build
INC_DIRS = ./include/core ./include/cpu

SRCS = $(shell find $(SRC_DIRS) -name "*.cpp") 
INCS = $(shell find $(INC_DIRS) -name "*.h")

INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS = $(INC_FLAGS)

LDFLAGS = -lSDL2

$(BUILD_DIR)/$(TARGET).o: $(SRCS) $(INCS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(SRCS) -o $@ $(CFLAGS) $(LDFLAGS)

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)