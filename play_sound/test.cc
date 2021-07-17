#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>

#include <iostream>

int main(void) {
  std::cout << "Hello" << std::endl;

  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window *window =
      SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       600, 400, SDL_WINDOW_SHOWN);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  SDL_RenderClear(renderer);

  SDL_RenderPresent(renderer);

  Uint32 wav_length;
  Uint8 *wav_buffer;
  SDL_AudioSpec wav_spec {
    .callback = [](void *userdata, Uint8 *stream, int len) {
      SDL_MixAudio(stream, wav_buffer, len, SDL_MIX_MAXVOLUME);
      wav_buffer += len;
      wav_length -= len;
    }
  };
  if (SDL_LoadWAV("./resources/hard_kick.wav", &wav_spec, &wav_buffer,
                  &wav_length) == NULL)
    return 1;

  if (SDL_OpenAudio(&wav_spec, NULL) < 0) {
    std::cout << SDL_GetError() << std::endl;
    exit(-1);
  }

  SDL_PauseAudio(0);

  SDL_Event event;
  do {
    std::cout << "Playing audio: " << wav_length << std::endl;
  } while (wav_length > 0
      && SDL_PollEvent(&event)
      && event.type != SDL_QUIT);

  SDL_FreeWAV(wav_buffer);

  return 0;
}
