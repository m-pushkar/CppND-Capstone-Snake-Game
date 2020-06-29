#include "snake.h"
#include "calibration.h"
#include "game.h"
#include <cmath>
#include <iostream>

using std::vector;

vector<vector<bool>> Snake::grid(height, std::vector<bool>(width, false));
extern std::mutex mutlock;

bool Snake::GetFood(SDL_Point food) {
  int x_new = static_cast<int>(head_x);
  int y_new = static_cast<int>(head_y);

  bool get_food = false;

  if (food.x == x_new && food.y == y_new) {
    get_food = true;

    score++;    // Score incresed
    GrowBody(); // Grow snake
  }
  return get_food;
}

void Snake::Update(const Snake &other) {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)}; // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)}; // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.

  moved = false; // Initialization of variable

  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    moved = true;
    UpdateBody(&current_cell, prev_cell, other);
  }
}

void Snake::UpdateHead() {
  switch (direction) {
  case Direction::unknown:
    break;

  case Direction::kUp:
    head_y -= speed;
    break;

  case Direction::kDown:
    head_y += speed;
    break;

  case Direction::kLeft:
    head_x -= speed;
    break;

  case Direction::kRight:
    head_x += speed;
    break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  if (head_x < 0.0f || head_y < 0.0f || head_x >= 32.0f || head_y >= 32.0f) {
    alive = false;
    if (head_x >= 32.0f)
      head_x = 31.50;
    else if (head_y >= 32.0f)
      head_y = 31.50;
  }
}

void Snake::UpdateBody(const SDL_Point *current_head_cell,
                       SDL_Point &prev_head_cell, const Snake &other) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  // unique_lock prevents data

  std::unique_lock<std::mutex> lock_obj(mutlock);
  Snake::grid[prev_head_cell.x][prev_head_cell.y] = 1;
  lock_obj.unlock();

  if (!growing) {
    // Remove the tail from the vector.
    lock_obj.lock();
    Snake::grid[body[0].x][body[0].y]] = 0;
    lock_obj.unlock();
    body.pop_front();
  } else {
    growing = false;
    size++;
  }

  // Check if the snake has died.

  // Additional condition is added preventing computer snake coming here as
  // update_path is True in computer_snake.cpp: Computer_snake::Update

  if (current_head_cell->x != prev_head_cell.x ||
      current_head_cell->y != prev_head_cell.y) {
    for (auto const &item : body) {
      if (current_head_cell->x == item.x && current_head_cell->y == item.y) {
        alive = false;
      }
    }
    if (SnakeCell(other.head_x, other.head_y) == true) {
      alive = false;
    }
  }
}

void Snake::GrowBody() { growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(const int &x, const int &y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}
