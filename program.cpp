#include "splashkit.h"

#define TILE_SIZE 40
#define BORDER_THICKNESS 1
#define MIN_DISTANCE 5

enum class TileType
{
    EMPTY,
    ORANGE,
    START,
    END,
    BLOCK
};

std::vector<std::vector<TileType>> tiles;

int calculate_distance(int row1, int col1, int row2, int col2)
{
    return abs(row1 - row2) + abs(col1 - col2);
}

bool is_adjacent_to_start_or_end(int row, int col, int start_row, int start_col, int end_row, int end_col)
{
    return (abs(row - start_row) <= 1 && abs(col - start_col) <= 1) ||
           (abs(row - end_row) <= 1 && abs(col - end_col) <= 1);
}

void initialize_tiles(int rows, int cols)
{
    tiles.clear();
    tiles.resize(rows, std::vector<TileType>(cols, TileType::EMPTY));

    // Fill border with orange tiles
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            if (row < BORDER_THICKNESS || row >= rows - BORDER_THICKNESS ||
                col < BORDER_THICKNESS || col >= cols - BORDER_THICKNESS)
            {
                tiles[row][col] = TileType::ORANGE;
            }
        }
    }

    // Spawn start tile randomly on one of the orange border tiles
    int start_row, start_col;
    do
    {
        start_row = rnd(0, rows - 1);
        start_col = rnd(0, cols - 1);
    } while (tiles[start_row][start_col] != TileType::ORANGE);
    tiles[start_row][start_col] = TileType::START;

    // Spawn end tile randomly on one of the orange border tiles, at least 5 tiles away from the start tile
    int end_row, end_col;
    do
    {
        end_row = rnd(0, rows - 1);
        end_col = rnd(0, cols - 1);
    } while (tiles[end_row][end_col] != TileType::ORANGE ||
             calculate_distance(start_row, start_col, end_row, end_col) < MIN_DISTANCE);
    tiles[end_row][end_col] = TileType::END;

    // Spawn blocks randomly within the inner area of the screen, not touching the start or end tiles
    for (int row = BORDER_THICKNESS; row < rows - BORDER_THICKNESS; ++row)
    {
        for (int col = BORDER_THICKNESS; col < cols - BORDER_THICKNESS; ++col)
        {
            if (tiles[row][col] == TileType::EMPTY &&
                !is_adjacent_to_start_or_end(row, col, start_row, start_col, end_row, end_col) &&
                rnd() < 0.1) // Adjust the probability to control the density of blocks
            {
                tiles[row][col] = TileType::BLOCK;
            }
        }
    }
}

void draw_tiles(int rows, int cols)
{
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            float x = col * TILE_SIZE;
            float y = row * TILE_SIZE;

            switch (tiles[row][col])
            {
            case TileType::EMPTY:
                fill_rectangle(COLOR_BLACK, x, y, TILE_SIZE, TILE_SIZE);
                break;
            case TileType::ORANGE:
                fill_rectangle(COLOR_ORANGE, x, y, TILE_SIZE, TILE_SIZE);
                break;
            case TileType::START:
                fill_rectangle(COLOR_GREEN, x, y, TILE_SIZE, TILE_SIZE);
                break;
            case TileType::END:
                fill_rectangle(COLOR_RED, x, y, TILE_SIZE, TILE_SIZE);
                break;
            case TileType::BLOCK:
                fill_rectangle(COLOR_ORANGE, x, y, TILE_SIZE, TILE_SIZE);
                break;
            }
        }
    }
}

int main()
{
    open_window("Tiles", 800, 600);

    int rows = screen_height() / TILE_SIZE;
    int cols = screen_width() / TILE_SIZE;

    initialize_tiles(rows, cols);

    while (!window_close_requested("Tiles"))
    {
        process_events();
        clear_screen(COLOR_BLACK);
        draw_tiles(rows, cols);
        refresh_screen(60);
    }

    close_window("Tiles");
    return 0;
}
