#pragma once
#include <SDL2/SDL.h>

class SortAudio {
  public:
    static void init();
    static void cleanup();
    static void play_tone(int value, int max_value, int duration_ms);
};
