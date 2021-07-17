#include <SDL2/SDL.h>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

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

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  struct {
    int width = 40;
    int heigth = 40;
  } tile;
  int spacer = 10;
  for (int column = 0; column < 4; ++column)
    for (int row = 0; row < 4; ++row) {
      SDL_Rect rectangle{.x = 100 + (spacer + tile.width) * column,
                         .y = 100 + (spacer + tile.heigth) * row,
                         .w = tile.width,
                         .h = tile.heigth};
      SDL_RenderFillRect(renderer, &rectangle);
    }

  SDL_RenderPresent(renderer);
  SDL_Delay(3000);

  return 0;
}
