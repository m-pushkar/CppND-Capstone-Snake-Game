#ifndef SNAKE_H
#define SNAKE_H

#include <SDL2/SDL.h>

#include <vector>
#include <deque>
#include <iostream>

class Snake
{
public:
  Snake(const int &grid_width, const int &grid_height, int &&num) : grid_width(grid_width), grid_height(grid_height), head_y(grid_height * 0.75f)
  {
    if (num == 0)
      head_x = grid_width * 0.25;
    else
      head_x = grid_width * 0.75;

    speed = 0.15f;
  }

  enum Direction
  {
    kUp,
    kDown,
    kLeft,
    kRight,
    unknown
  };
  Direction direction = kUp;
  std::deque<SDL_Point> body;

  float speed{0.0f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  bool moved{false};

  static std::vector<std::vector<bool>> grid;

  virtual void Update(const Snake &other);

  void GrowBody();
  bool GetFood(SDL_Point food);
  int get_score() const { return score; }
  bool SnakeCell(const int &x, const int &y);

protected:
  int score{0};
  bool growing{false};
  int grid_width;
  int grid_height;

  void UpdateHead();
  void UpdateBody(const SDL_Point *current_head_cell, SDL_Point &prev_head_cell, const Snake &other);
};

#endif
