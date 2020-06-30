#include "comp_snake.h"
#include "calibration.h"

extern std::mutex mutlock;

void Comp_snake::record_food(const SDL_Point &position)
{
  _food.x = position.x;
  _food.y = position.y;
}

void Comp_snake::Update(const Snake &other)
{
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)}; // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)}; // Capture the head's cell after updating.

  /*Update the body vector items if the snake head has moved to a new cell
  * condition update_path == true : when comp_snake get food, immediately need to build new path
  * condition other.moved == true : two snakes have different speed, need to re-build path in case auto doesn't move but player does
  * without this condition, in some cases, auto will bump into player, but this will cause lagged.
  */
  bool self_moved = false;
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y)
  {
    self_moved = true;
    UpdateBody(&current_cell, prev_cell, other);
  }
  if (self_moved == true || update_path == true || other.moved == true)
  {
    update_path = false;
    /*snake and comp_snake body has record in Snake::grid by function UpdateBody */
    std::vector<std::vector<Direction>> direction_arr(height, std::vector<Direction>(width, unknown));
    bool path_set = false;
    path_set = path_search(direction_arr, _food, current_cell, width, height);
    direction = direction_arr[current_cell.x][current_cell.y];

    if (direction != unknown)
    {
      _last_dir = direction;
    }
    /*
     * in case food is surround by opponent, path can not be built
     * find next possible direction
     */
    if (path_set != true)
    {
      bool suicide = true;
      for (auto &move : change_list)
      {
        int next_x = head_x + move.x;
        int next_y = head_y + move.y;
        if (next_x >= 0 && next_x < grid_height && next_y >= 0 && next_y < grid_width && Snake::grid[next_x][next_y] != true)
        {
          direction = move.action;
          suicide = false;
          break;
        }
      }
      if (suicide = true)
      {
        std::cout << "suicide mission" << std::endl;
        direction = _last_dir;
      }
    }
  }
}

bool Comp_snake::path_search(std::vector<std::vector<Direction>> &direction_arr, const SDL_Point &food, const SDL_Point &head, int &&grid_width, int &&grid_height)
{
  /* initialize parameters */
  bool find_path = false;
  std::unique_lock<std::mutex> lock_obj(mutlock);
  std::vector<std::vector<Find>> close_mtx(grid_height, std::vector<Find>(grid_width));
  std::vector<Find> open_list;

  /* set first point*/
  Find start(head.x, head.y);
  open_list.emplace_back(start);
  /* initialize close_mtx*/
  close_mtx[head.x][head.y].visited = true;
  close_mtx[head.x][head.y].x = head.x;
  close_mtx[head.x][head.y].y = head.y;

  /* start searching*/
  while (!open_list.empty())
  {
    sort(open_list.begin(), open_list.end());

    Find expand = open_list.back();
    open_list.pop_back();

    if (expand.x == food.x && expand.y == food.y)
    {
      find_path = true;
      break;
    }
    else
    {
      for (auto &move : change_list)
      {
        int next_x = expand.x + move.x;
        int next_y = expand.y + move.y;
        /* check if next possible point is inside windown and not occupied by player snake body and by itself, also not been calculated */
        if (next_x >= 0 && next_x < grid_height && next_y >= 0 && next_y < grid_width && Snake::grid[next_x][next_y] != true && close_mtx[next_x][next_y].visited != true)
        {
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
  /* build direction_arr if find_path is true */
  if (find_path == true)
  {
    Find *current = &close_mtx[food.x][food.y];

    while (current->x != start.x || current->y != start.y)
    {
      direction_arr[current->parent.x][current->parent.y] = current->action;
      current = &close_mtx[current->parent.x][current->parent.y];
    }
  }

  return find_path;
}
