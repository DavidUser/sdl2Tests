#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_pixels.h>
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
#include <thread>

// Fix SDL old C memory management style
#define SAFE_UNIQUE_PTR(TYPE, DELETER)                          \
  struct TYPE##__Destroyer {                                    \
    void operator()(TYPE* w) const { DELETER(w); }              \
  };                                                            \
  template <>                                                   \
  class std::unique_ptr<TYPE>                                   \
      : public std::unique_ptr<TYPE, TYPE##__Destroyer> {       \
    using std::unique_ptr<TYPE, TYPE##__Destroyer>::unique_ptr; \
                                                                \
   public:                                                      \
    operator TYPE*() { return get(); }                          \
  };
SAFE_UNIQUE_PTR(SDL_Window, SDL_DestroyWindow)
SAFE_UNIQUE_PTR(SDL_Renderer, SDL_DestroyRenderer)
SAFE_UNIQUE_PTR(SDL_Texture, SDL_DestroyTexture)

int main(void) {
  std::cout << "Drawing animation" << '\n';

  SDL_Init(SDL_INIT_VIDEO);
  std::unique_ptr<SDL_Window> window(
      SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       600, 400, SDL_WINDOW_SHOWN));
  std::unique_ptr<SDL_Renderer> renderer(
      SDL_CreateRenderer(window.get(), -1, 0));

  std::unique_ptr<SDL_Texture> texture(SDL_CreateTexture(
      renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 600, 400));

  // Save the static redering on texture
  SDL_SetRenderTarget(renderer, texture);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderTarget(renderer, NULL);

  struct {
    int width = 40;
    int heigth = 40;
  } tile;
  int spacer = 10;

  // Apply linear movement
  const struct { int x, y; } BEGIN{100, 100}, END{100, 500};
  const float PIXEL_PER_SECOND = 100;

  int frames = 0;
  auto last_time_clock = std::chrono::system_clock().now();
  auto last_second = last_time_clock;

  auto position = [initial_position = BEGIN.x,
                   velocity = PIXEL_PER_SECOND](float elapsed_time) {
    return initial_position + velocity * elapsed_time;
  };

  auto start_time = std::chrono::system_clock().now();

  SDL_Rect rectangle{0, 0, -1, -1};

  float y = position(0);
  do {
    auto current_time_clock = std::chrono::system_clock().now();
    auto elapsed_time = current_time_clock - start_time;
    y = position(std::chrono::duration<float>(elapsed_time).count());

    // Just clear the previous object rendered
    // using previous rendered texture
    SDL_RenderCopy(renderer, texture, &rectangle, &rectangle);

    rectangle = SDL_Rect{.x = (int)y,
                         .y = 100 + (spacer + tile.heigth),
                         .w = tile.width,
                         .h = tile.heigth};

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rectangle);
    SDL_RenderPresent(renderer);

    SDL_Event event;
    if (SDL_PollEvent(&event) && event.type == SDL_QUIT) exit(0);

    auto time_diference = current_time_clock - last_time_clock;

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

    current_time_clock = std::chrono::system_clock().now();
    time_diference = current_time_clock - last_time_clock;
    if (time_diference >= 2ms)
      std::cerr << "Lag "
                << duration_cast<milliseconds>(time_diference - 1ms).count()
                << "ms \n";
    last_time_clock = std::chrono::system_clock().now();
  } while (y < END.y);

  SDL_Event event;
  while (SDL_WaitEvent(&event) && event.type != SDL_QUIT)
    ;

  return 0;
}
