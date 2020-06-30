#ifndef RENDERER_H
#define RENDERER_H

#include "snake.h"
#include <vector>
#include <SDL2/SDL.h>

class Renderer
{
public:
  Renderer(const std::size_t &screen_width, const std::size_t &screen_height,
           const std::size_t &grid_width, const std::size_t &grid_height);
  ~Renderer();

  void Render(Snake const &snake, Snake const &comp_snake, SDL_Point const &food);
  void UpdateWindowTitle(int &&score_1, int &&score_2, int &fps);

private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif
