#include "game.h"
#include <SDL2/SDL.h>
#include <iostream>

std::mutex mutlock;
Game::Game(const std::size_t &grid_width, const std::size_t &grid_height)
    : snake(grid_width, grid_height, 1U),
      computer_snake(grid_width, grid_height, 0U), engine(dev()),
      // Place food withing -5 of frame
      random_w(0, static_cast<int>(grid_width) - 2.5),
      random_h(0, static_cast<int>(grid_height) - 2.5) {
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               const std::size_t &target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, computer_snake, food);
    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(snake.get_score(), computer_snake.get_score(),
                                 frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y) && !computer_snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  if (snake.alive == false || computer_snake.alive == false) {
    return;
  }

  // Initialize player on another thread
  std::future<void> update_snake =
      std::async(&Snake::Update, &snake, computer_snake);
  update_snake.wait();

  computer_snake.record_food(food);
  computer_snake.Update(snake);

  // Check if there's food over here
  if (computer_snake.GetFood(food) == true) {
    computer_snake.new_path();
    PlaceFood();
  } else if (snake.GetFood(food) == true) {
    PlaceFood();
  }
}

int Game::GetScore() const { return snake.get_score(); }
int Game::GetSize() const { return snake.size; }
