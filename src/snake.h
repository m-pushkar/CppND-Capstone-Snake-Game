#ifndef SNAKE_H
#define SNAKE_H

#include <SDL2/SDL.h>
#include <deque>
#include <iostream>
#include <vector>

using std::vector;

class Snake {
public:
  enum Direction { unknown, kUp, kDown, kLeft, kRight };
  Direction direction = kUp;
  std::deque<SDL_Point> body;

  Snake(const int &grid_width, const int &grid_height, int &&num)
      : grid_width(grid_width), grid_height(grid_height),
        head_y(grid_height * 0.75f) {
    if (num == 0)
      head_x = grid_width * 0.25;
    else
      head_x = grid_width * 0.75;

    speed = 0.15f;
  }
  float speed{0.0f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  bool moved{false};

  virtual void Update(const Snake &other);

  static vector<vector<bool>> grid;

  void GrowBody();
  bool SnakeCell(const int &x, const int &y);
  bool GetFood(SDL_Point food);
  int get_score() const { return score; }

protected:
  void UpdateHead();
  void UpdateBody(const SDL_Point *current_head_cell, SDL_Point &prev_head_cell,
                  const Snake &other);

  bool growing{false};
  int grid_width;
  int grid_height;
  int score{0};
};

#endif
