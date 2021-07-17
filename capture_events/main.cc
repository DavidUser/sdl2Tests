#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>

#include <iostream>

#define ECHO_MOUSE_CLICK_BUTTON(STATE, BUTTON) \
  if ((STATE)&SDL_BUTTON(BUTTON)) std::cout << #BUTTON;

#define ECHO_ALL_MOUSE_CLICK_BUTTON(STATE)                   \
  ECHO_MOUSE_CLICK_BUTTON(STATE, SDL_BUTTON_LEFT);           \
  ECHO_MOUSE_CLICK_BUTTON(STATE, SDL_BUTTON_RIGHT);          \
  ECHO_MOUSE_CLICK_BUTTON(STATE, SDL_BUTTON_MIDDLE);         \
  ECHO_MOUSE_CLICK_BUTTON(mouse_click_state, SDL_BUTTON_X1); \
  ECHO_MOUSE_CLICK_BUTTON(mouse_click_state, SDL_BUTTON_X2);

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

  // track mouse position
  SDL_Event event;
  while (SDL_WaitEvent(&event) && event.type != SDL_QUIT) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
      int x, y;
      int mouse_click_state = SDL_GetMouseState(&x, &y);
      ECHO_ALL_MOUSE_CLICK_BUTTON(mouse_click_state);

      std::cout << " Mouse position (" << x << ", " << y << ")" << std::endl;
    }
  }

  return 0;
}
