#pragma once
#include <SDL2/SDL.h>

static SDL_AudioDeviceID audioDevice;

class SortAudio
{
  public:
    static void init();
    static void cleanup();
    static void play_tone(int value, int max_value, int duration_ms);
};