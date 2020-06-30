#ifndef GAME_H
#define GAME_H

#include "renderer.h"
#include "snake.h"
#include "comp_snake.h"
#include "controller.h"
#include <random>
#include <SDL2/SDL.h>
#include <thread>
#include <future>
#include <chrono>
#include <mutex>
#include <vector>

class Game
{
public:
  Game(const std::size_t &grid_width, const std::size_t &grid_height);
  void Run(Controller const &controller, Renderer &renderer, const std::size_t &target_frame_duration);
  int GetScore() const;
  int GetSize() const;

private:
  Snake snake;
  Comp_snake comp_snake;
  SDL_Point food;

  std::mutex game_mutex;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<unsigned char> random_w;
  std::uniform_int_distribution<unsigned char> random_h;

  void PlaceFood();
  void Update();
};

#endif
