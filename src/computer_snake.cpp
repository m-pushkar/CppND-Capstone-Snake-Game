#include "computer_snake.h"
#include "calibration.h"

extern std::mutex mutlock;

void Computer_snake::record_food(const SDL_Point &position) {
  _food.x = position.x;
  _food.y = position.y;
}

void Computer_snake::Update(const Snake &other) {
  // Captur head's position before and after updating
  SDL_Point prev_cell{static_cast<int>(head_x), static_cast<int>(head_y)};

  UpdateHead();

  SDL_Point current_cell{static_cast<int>(head_x), static_cast<int>(head_y)};

  // Update body vector if head has moved to new position
  bool self_moved = false;

  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    self_moved = true;
    UpdateBody(&current_cell, prev_cell, other);
  }
  if (self_moved == true || update_path == true || other.moved == true) {
    update_path = false;

    vector<vector<Direction>> dir_ar( // direction_arr = dir_ar
        height, std::vector<Direction>(width, unknown));
    bool path_set = false;
    path_set = path_search(dir_ar, _food, current_cell, width, height);
    direction = dir_ar[current_cell.x][current_cell.y];

    if (direction != unknown) {
      _last_dir = direction;
    }
    if (path_set != true) {
      bool crash = true; // suicide = crash
      for (auto &move : change_list) {
        int next_x = head_x + move.x;
        int next_y = head_y + move.y;
        if (next_x >= 0 && next_x < grid_height && next_y >= 0 &&
            next_y < grid_width && Snake::grid[next_x][next_y] != true) {
          direction = move.action;
          crash = false;
          break;
        }
      }
      if (crash = true) {
        direction = _last_dir;
      }
    }
  }
}

bool Computer_snake::path_search(vector<vector<Direction>> &dir_ar,
                                 const SDL_Point &food, const SDL_Point &head,
                                 const int grid_width, const int grid_height) {

  // Parameter initilaization
  bool find_path = false;
  std::unique_lock<std::mutex> lock_obj(mutlock);
  vector<vector<Find>> close_mtx(grid_height, vector<Find>(grid_width));
  vector<Find> open_list;

  // Start point
  Find start(head.x, head.y);
  open_list.emplace_back(start);

  close_mtx[head.x][head.y].visited = true;
  close_mtx[head.x][head.y].x = head.x;
  close_mtx[head.x][head.y].y = head.y;

  // Search
  while (!open_list.empty()) {
    sort(open_list.begin(), open_list.end());

    Find expand = open_list.back();
    open_list.pop_back();

    if (expand.x == food.x && expand.y == food.y) {
      find_path = true;
      break;
    } else {
      for (auto &move : change_list) {
        int next_x = expand.x + move.x;
        int next_y = expand.y + move.y;

        // Check avialability of next point
        if (next_x >= 0 && next_x < grid_height && next_y >= 0 &&
            next_y < grid_width && Snake::grid[next_x][next_y] != true &&
            close_mtx[next_x][next_y].visited != true) {
          close_mtx[next_x][next_y].cost = expand.cost + 1U;
          close_mtx[next_x][next_y].parent.x = expand.x;
          close_mtx[next_x][next_y].parent.y = expand.y;
          close_mtx[next_x][next_y].action = move.action;
          close_mtx[next_x][next_y].x = next_x;
          close_mtx[next_x][next_y].y = next_y;

          open_list.emplace_back(close_mtx[next_x][next_y]);
          close_mtx[next_x][next_y].visited = true;
        }
      }
    }
  }

  if (find_path == true) {
    Find *current = &close_mtx[food.x][food.y];

    while (current->x != start.x || current->y != start.y) {
      dir_ar[current->parent.x][current->parent.y] = current->action;
      current = &close_mtx[current->parent.x][current->parent.y];
    }
  }
  return find_path;
}
