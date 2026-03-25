#include "base.h"
#include <SDL2/SDL.h>
#include "sound.h"
#include <cmath>
#include <chrono>
#include <thread>
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
  auto freq = 200.0 + (value * 1000.0 / max_value);

  int sample_rate = 44100;
  int samples = (duration_ms * sample_rate) / 1000;
  i16* buffer = new i16[samples];

  // Generate the sound wave
  for (int i = 0; i < samples; i++) {
    double t = i * 1.0 / sample_rate;
    
    // Generate base sine wave
    double wave = sin(2.0 * M_PI * freq * t);
    
    // Add harmonics for a richer, more pleasant "organ-like" tone
    wave += 0.5 * sin(2.0 * M_PI * (freq * 2.0) * t); // 1st harmonic (octave)
    wave += 0.25 * sin(2.0 * M_PI * (freq * 3.0) * t); // 2nd harmonic
    
    // Normalize to prevent audio clipping (1.0 + 0.5 + 0.25 = 1.75)
    wave /= 1.75;
    
    // Apply ADSR Envelope (Attack and Release) to eliminate clicking/popping
    double envelope = 1.0;
    
    // Attack phase (smooth fade in over the first 10% of the sample)
    int attack_samples = samples * 0.10;
    if (i < attack_samples) {
      envelope = i * 1.0 / attack_samples;
    }
    
    // Release phase (smooth fade out over the last 20% of the sample)
    int release_samples = samples * 0.20;
    if (i > samples - release_samples) {
      envelope = (samples - i) * 1.0 / release_samples;
    }

    // Apply the wave, envelope, and volume scaling (8000 is our chosen volume out of 32767 max)
    buffer[i] = wave * 8000.0 * envelope;
  }

  // Push audio to the sound card
  SDL_QueueAudio(audioDevice, buffer, samples * sizeof(i16));
  delete[] buffer;

  // Sleep to sync the visualizer with the audio duration
  this_thread::sleep_for(chrono::milliseconds(duration_ms));
}
