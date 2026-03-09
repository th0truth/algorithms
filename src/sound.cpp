#include "base.h"

#include <SDL2/SDL.h>

#include "algorithms.h"
#include "sound.h"
using namespace std;

// Initialize SortAudio for algorithms
void SortAudio::init()
{
  SDL_Init(SDL_INIT_AUDIO);
  SDL_AudioSpec want, have;
  SDL_zero(want);
  want.freq = 44100; // Standard CD-quality sample rate
  want.format = AUDIO_S16SYS; // 16-bit audio
  want.channels = 1; // Mono sound
  want.samples = 2048; // Buffer size

  audioDevice = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
  SDL_PauseAudioDevice(audioDevice, 0); // Unpause to start listening for audio
}

void SortAudio::cleanup()
{
  SDL_CloseAudioDevice(audioDevice);
  SDL_Quit();
}

void SortAudio::play_tone(int value, int max_value, int duration_ms)
{
  if (max_value <= 0)
    max_value = 1;

  // Map array value to a frequency (e.g., 200Hz to 1200Hz)
  auto freq = 200.0 + ((double)value / max_value) * 1000.0;

  int sample_rate = 44100;
  int samples = (duration_ms * sample_rate) / 1000;
  i16* buffer = new i16[samples];

  // Generate the sine wave
  for (int i = 0; i < samples; i++) {
    double t = (double)i / sample_rate;
    
    // We calculate the sine wave, but force it to be strictly ON (+8000) or OFF (-8000)
    double sine_val = sin(2.0 * M_PI * freq * t);
    
    if (sine_val > 0) {
      buffer[i] = 8000;
    } else {
      buffer[i] = -8000;
    }
  }

  // Push audio to the sound card
  SDL_QueueAudio(audioDevice, buffer, samples * sizeof(i16));
  delete[] buffer;

  // Sleep to sync the visualizer with the audio duration
  this_thread::sleep_for(chrono::milliseconds(duration_ms));
}
