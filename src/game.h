#ifndef GAME_H
#define GAME_H

#include "computer_snake.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include <SDL2/SDL.h>
#include <chrono>
#include <future>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

class Game {
public:
  Game(const std::size_t &grid_width, const std::size_t &grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           const std::size_t &target_frame_duration);
  int GetScore() const;
  int GetSize() const;

private:
  Snake snake;
  Computer_snake computer_snake;
  SDL_Point food;
  std::mutex game_mutex;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  void PlaceFood();
  void Update();
};

#endif
