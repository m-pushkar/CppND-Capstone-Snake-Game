#ifndef COMPUTER_SNAKE_H
#define COMPUTER_SNAKE_H

#include "snake.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>

using std::vector;

class Computer_snake : public Snake {
public:
  Computer_snake(const int &grid_width, const int &grid_height, int &&num)
      : Snake(grid_width, grid_height, std::move(num)) {
    speed = 0.1f;
  }
  void new_path() { update_path = true; } // require_new_path = new_path
  void Update(const Snake &other) override;
  void record_food(const SDL_Point &position);

private:
  bool update_path{false};
  SDL_Point _food{0, 0};
  Direction _last_dir{unknown}; // _last_direction_state = _last_dir
  struct Change                 // Delta = Change
  {
    int x;
    int y;
    Direction action;
  };

  struct Find // Search_pt = Find
  {
  public:
    Find() : x(-1), y(-1){};
    Find(int current_x, int current_y) : x(current_x), y(current_y){};

    // Public variables
    int x;
    int y;
    int cost{0};
    bool visited{false};
    Direction action{unknown};
    SDL_Point parent{-1, -1};

    // Public variable functions
    bool operator<(Find const &a) const {
      return (cost > a.cost) ? true : false;
    }
  };

  bool path_search(vector<vector<Direction>> &direction_arr,
                   const SDL_Point &food, const SDL_Point &head,
                   int &&grid_width, int &&grid_height);

  vector<Change> change_list = {// delta_list = change_list
                                {0, -1, kUp},
                                {0, 1, kDown},
                                {-1, 0, kLeft},
                                {1, 0, kRight}};
};

#endif
