#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>

#include <chrono>
#include <functional>
#include <ios>
#include <sstream>
using std::chrono::duration_cast;
;
using std::chrono::hours;
using std::chrono::milliseconds;
using std::chrono::minutes;
using std::chrono::seconds;
using namespace std::chrono_literals;
#include <iomanip>
#include <iostream>
#include <sstream>

std::string ClockFormat(milliseconds ms) {
  auto ss = duration_cast<seconds>(ms);
  auto mm = duration_cast<minutes>(ss);
  auto hh = duration_cast<hours>(mm);
  mm -= hh;
  ss -= mm;
  ms -= ss;
  return (std::stringstream{}
          << std::setfill('0') << std::setw(2) << hh.count() << ":"
          << std::setfill('0') << std::setw(2) << mm.count() << ":"
          << std::setfill('0') << std::setw(2) << ss.count() << "."
          << std::setfill('0') << std::setw(3) << ms.count())
      .str();
}

int main(void) {
  std::cout << "Playing audio file" << std::endl;

  if (SDL_Init(SDL_INIT_AUDIO) < 0) return 1;

  SDL_AudioSpec wav_spec;
  Uint32 wav_length;
  Uint8 *wav_buffer;

  SDL_LoadWAV("./resources/hard_kick.wav", &wav_spec, &wav_buffer, &wav_length);
  SDL_AudioDeviceID device = SDL_OpenAudioDevice(NULL, 0, &wav_spec, NULL, 0);
  SDL_QueueAudio(device, wav_buffer, wav_length);
  SDL_PauseAudioDevice(device, 0);

  const int AUDIO_BITS_LENGTH = wav_length * 8;
  const int BITS_PER_SAMPLE = (wav_spec.format & SDL_AUDIO_MASK_BITSIZE);
  const int BITRATE = wav_spec.freq * BITS_PER_SAMPLE * wav_spec.channels;
  const milliseconds DURATION =
      (AUDIO_BITS_LENGTH * milliseconds(1s)) / BITRATE;

  std::cout << wav_length << "B"
            << ", " << wav_spec.freq << "Hz"
            << ", " << wav_spec.samples << " samples"
            << ", " << BITS_PER_SAMPLE << " b/sample"
            << ", " << (int)wav_spec.channels << " channels"
            << ", " << BITRATE << "bps"
            << ", " << ClockFormat(DURATION) << std::endl;

  SDL_Delay(DURATION.count());

  SDL_CloseAudioDevice(device);
  SDL_FreeWAV(wav_buffer);
  SDL_Quit();

  return 0;
}
