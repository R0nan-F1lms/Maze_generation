#include "splashkit.h"
#include <vector>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TILE_SIZE 40

using namespace std;

enum class TileType
{
    BLACK,
    WHITE
};

struct Tile
{
    TileType type;
};

vector<vector<Tile>> grid;

void create_checkered_board()
{
    int rows = SCREEN_HEIGHT / TILE_SIZE;
    int cols = SCREEN_WIDTH / TILE_SIZE;

    grid.clear();
    grid.resize(rows, vector<Tile>(cols));

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            // Alternate between black and white tiles based on row and column indices
            if ((i + j) % 2 == 0)
                grid[i][j].type = TileType::BLACK;
            else
                grid[i][j].type = TileType::WHITE;
        }
    }
}

void draw_checkered_board()
{
    for (int i = 0; i < grid.size(); ++i)
    {
        for (int j = 0; j < grid[i].size(); ++j)
        {
            float x = j * TILE_SIZE;
            float y = i * TILE_SIZE;

            // Draw black or white tiles based on TileType
            if (grid[i][j].type == TileType::BLACK)
                fill_rectangle(COLOR_BLACK, x, y, x + TILE_SIZE, y + TILE_SIZE);
            else
                fill_rectangle(COLOR_WHITE, x, y, x + TILE_SIZE, y + TILE_SIZE);
        }
    }
}

int main()
{
    open_window("Checkered Board", SCREEN_WIDTH, SCREEN_HEIGHT);
    create_checkered_board();

    while (!window_close_requested("Checkered Board"))
    {
        process_events();
        clear_screen(COLOR_WHITE);
        draw_checkered_board();
        refresh_screen(60);
    }

    close_window("Checkered Board");
    return 0;
}
