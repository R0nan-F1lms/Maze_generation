#include "splashkit.h"
#include <vector>
#include <cstdlib> // Include for random number generation

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TILE_SIZE 40

using namespace std;

enum class TileType
{
    BLACK,
    WHITE,
    RED // Added red color for walls
};

struct Tile
{
    TileType type;
};

vector<vector<Tile>> grid;

void create_checkered_board_with_maze()
{
    int rows = SCREEN_HEIGHT / TILE_SIZE;
    int cols = SCREEN_WIDTH / TILE_SIZE;

    grid.clear();
    grid.resize(rows, vector<Tile>(cols, {TileType::WHITE})); // Initialize all tiles as white

    // Start from the middle of the top edge
    int row = 0;
    int col = cols / 2;

    // Maze generation algorithm
    while (row >= 0 && row < rows)
    {
        grid[row][col].type = TileType::BLACK; // Mark the current cell as visited

        // Move towards the center while making random turns
        int direction = rnd(3); // 0: left, 1: straight, 2: right

        if (direction == 0 && col > 0 && grid[row][col - 1].type != TileType::BLACK)
        {
            col--;
        }
        else if (direction == 2 && col < cols - 1 && grid[row][col + 1].type != TileType::BLACK)
        {
            col++;
        }

        row++;

        // Ensure that the path cannot cross to the opposite edge
        if (row < rows)
        {
            grid[row][col].type = TileType::BLACK;
        }
    }
}

void draw_checkered_board_with_maze()
{
    for (int i = 0; i < grid.size(); ++i)
    {
        for (int j = 0; j < grid[i].size(); ++j)
        {
            float x = j * TILE_SIZE;
            float y = i * TILE_SIZE;

            // Draw tiles based on TileType
            switch (grid[i][j].type)
            {
            case TileType::BLACK:
                fill_rectangle(COLOR_BLACK, x, y, x + TILE_SIZE, y + TILE_SIZE);
                break;
            case TileType::WHITE:
                fill_rectangle(COLOR_WHITE, x, y, x + TILE_SIZE, y + TILE_SIZE);
                break;
            case TileType::RED: // Draw walls as red
                fill_rectangle(COLOR_RED, x, y, x + TILE_SIZE, y + TILE_SIZE);
                break;
            }
        }
    }
}

int main()
{
    open_window("Checkered Board with Maze", SCREEN_WIDTH, SCREEN_HEIGHT);
    create_checkered_board_with_maze();

    while (!window_close_requested("Checkered Board with Maze"))
    {
        process_events();
        clear_screen(COLOR_WHITE);
        draw_checkered_board_with_maze();
        refresh_screen(60);
    }

    close_window("Checkered Board with Maze");
    return 0;
}
