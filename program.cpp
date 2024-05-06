#include "splashkit.h"

#define TILE_SIZE 40

enum class TileType
{
    WALL,
    PATH,
    START,
    END
};

struct Tile
{
    TileType type;
};

std::vector<std::vector<Tile>> maze;

void initialize_maze(int rows, int cols)
{
    maze.clear();
    maze.resize(rows, std::vector<Tile>(cols, {TileType::WALL}));

    // Set start and end points
    maze[rnd(rows)][0].type = TileType::START;
    maze[rnd(rows)][cols - 1].type = TileType::END;
}

void generate_path(int row, int col, int rows, int cols)
{
    if (row < 0 || row >= rows || col < 0 || col >= cols || maze[row][col].type != TileType::WALL)
        return;

    maze[row][col].type = TileType::PATH;

    if (col < cols - 1)
        generate_path(row, col + 1, rows, cols); // Move right
}

void draw_maze(int rows, int cols)
{
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            float x = col * TILE_SIZE;
            float y = row * TILE_SIZE;

            switch (maze[row][col].type)
            {
            case TileType::WALL:
                fill_rectangle(COLOR_BLACK, x, y, TILE_SIZE, TILE_SIZE);
                break;
            case TileType::PATH:
                fill_rectangle(COLOR_WHITE, x, y, TILE_SIZE, TILE_SIZE);
                break;
            case TileType::START:
                fill_rectangle(COLOR_GREEN, x, y, TILE_SIZE, TILE_SIZE);
                break;
            case TileType::END:
                fill_rectangle(COLOR_RED, x, y, TILE_SIZE, TILE_SIZE);
                break;
            }
        }
    }
}

int main()
{
    open_window("Maze Generator", 800, 600);

    int rows = screen_height() / TILE_SIZE;
    int cols = screen_width() / TILE_SIZE;

    initialize_maze(rows, cols);
    generate_path(rnd(rows), 0, rows, cols);

    while (!window_close_requested("Maze Generator"))
    {
        process_events();
        clear_screen(COLOR_BLACK);
        draw_maze(rows, cols);
        refresh_screen(60);
    }

    close_window("Maze Generator");
    return 0;
}
