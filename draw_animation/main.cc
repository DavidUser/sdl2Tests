#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

#include <chrono>
using namespace std::chrono_literals;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

#include <iostream>
#include <memory>

// Fix SDL old C memory management style
#define SAFE_UNIQUE_PTR(TYPE, DELETER)                          \
  struct TYPE##__Destroyer {                                    \
    void operator()(TYPE* w) const { DELETER(w); }              \
  };                                                            \
  template <>                                                   \
  class std::unique_ptr<TYPE>                                   \
      : public std::unique_ptr<TYPE, TYPE##__Destroyer> {       \
    using std::unique_ptr<TYPE, TYPE##__Destroyer>::unique_ptr; \
  };
SAFE_UNIQUE_PTR(SDL_Window, SDL_DestroyWindow)
SAFE_UNIQUE_PTR(SDL_Renderer, SDL_DestroyRenderer)

int main(void) {
  std::cout << "Drawing animation" << '\n';

  SDL_Init(SDL_INIT_VIDEO);
  std::unique_ptr<SDL_Window> window(
      SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       600, 400, SDL_WINDOW_SHOWN));
  std::unique_ptr<SDL_Renderer> renderer(
      SDL_CreateRenderer(window.get(), -1, 0));

  auto screen_clear = [&renderer]() {
    SDL_SetRenderDrawColor(renderer.get(), 0, 255, 0, 255);
    SDL_RenderClear(renderer.get());
  };

  struct {
    int width = 40;
    int heigth = 40;
  } tile;
  int spacer = 10;

  // Apply linear movement
  const struct { int x, y; } BEGIN{100, 100}, END{100, 400};
  const float PIXEL_PER_MS = 0.01;

  int frames = 0;
  auto last_time_clock = std::chrono::system_clock().now();
  auto last_second = last_time_clock;
  for (float y_momentum = BEGIN.y; y_momentum < END.y;
       y_momentum += PIXEL_PER_MS) {
    screen_clear();

    SDL_Rect rectangle{.x = (int)y_momentum,
                       .y = 100 + (spacer + tile.heigth),
                       .w = tile.width,
                       .h = tile.heigth};
    SDL_SetRenderDrawColor(renderer.get(), 255, 0, 0, 255);
    SDL_RenderFillRect(renderer.get(), &rectangle);
    SDL_RenderPresent(renderer.get());

    SDL_Event event;
    if (SDL_PollEvent(&event) && event.type == SDL_QUIT) exit(0);

    auto current_time_clock = std::chrono::system_clock().now();
    auto time_diference = current_time_clock - last_time_clock;
    if (time_diference < 1ms)
      SDL_Delay(duration_cast<milliseconds>(1ms - time_diference).count());
    else if (time_diference >= 2ms)
      std::cerr << "Lag "
                << duration_cast<milliseconds>(time_diference - 1ms).count()
                << "ms \n";
    last_time_clock = std::chrono::system_clock().now();

    ++frames;
    time_diference = current_time_clock - last_second;
    if (time_diference >= 1s) {
      double second_rate = time_diference / 1000ms;
      auto fps = frames / second_rate;
      std::cout << fps << "fps"
                << " (measured during " << second_rate << "s)" << '\n';
      frames = 0;
      last_second = current_time_clock;
    }
  }

  SDL_Event event;
  while (SDL_WaitEvent(&event) && event.type != SDL_QUIT)
    ;

  return 0;
}
