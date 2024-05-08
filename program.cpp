#include "splashkit.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TILE_SIZE = 40;
const int BORDER_THICKNESS = 1;
const int MIN_DISTANCE = 6;      // Minimum distance between start and end tiles
const int MAX_PATH_LENGTH = 500; // Maximum path length
enum class TileType
{
    EMPTY,
    ORANGE,
    START,
    END,
    WALKED // New tile type for walked path
};

struct Tile
{
    TileType type;

    bool is_traversable() const
    {
        return type != TileType::ORANGE && type != TileType::WALKED && type != TileType::END;
    }
};

struct App
{
    Tile tiles[SCREEN_WIDTH / TILE_SIZE][SCREEN_HEIGHT / TILE_SIZE];
    int start_row, start_col;
    int end_row, end_col;
    int path_length;
};

App app;

void setup(App &app);
void draw_tiles(const App &app);
void move_player(App &app);
bool is_valid_move(Tile tiles[][SCREEN_HEIGHT / TILE_SIZE], int x, int y);
void move(int x, int y);

int main()
{
    open_window("Maze App", SCREEN_WIDTH, SCREEN_HEIGHT);
    setup(app);
    do
    {
        process_events();
        clear_screen(COLOR_BLACK);
        
        move_player(app);
        draw_tiles(app);

        refresh_screen(60);
    } while (!window_close_requested("Maze App"));

    return 0;
}

void setup(App &app)
{
    // Initialize tiles
    for (int i = 0; i < SCREEN_WIDTH / TILE_SIZE; ++i)
    {
        for (int j = 0; j < SCREEN_HEIGHT / TILE_SIZE; ++j)
        {
            if (i < BORDER_THICKNESS || i >= SCREEN_WIDTH / TILE_SIZE - BORDER_THICKNESS ||
                j < BORDER_THICKNESS || j >= SCREEN_HEIGHT / TILE_SIZE - BORDER_THICKNESS)
            {
                app.tiles[i][j].type = TileType::ORANGE;
            }
            else
            {
                app.tiles[i][j].type = TileType::EMPTY;
            }
        }
    }

    // Set start tile on the top border
    app.start_row = 0;
    app.start_col = rnd(BORDER_THICKNESS, (SCREEN_WIDTH / TILE_SIZE) - BORDER_THICKNESS - 1);
    app.tiles[app.start_col][app.start_row].type = TileType::START;

    // Set end tile on the bottom border, at least 6 tiles away from start tile
    app.end_row = SCREEN_HEIGHT / TILE_SIZE - 1;
    do
    {
        app.end_col = rnd(BORDER_THICKNESS, (SCREEN_WIDTH / TILE_SIZE) - BORDER_THICKNESS - 1);
    } while (abs(app.end_col - app.start_col) < MIN_DISTANCE);
    app.tiles[app.end_col][app.end_row].type = TileType::END;

    // Initialize path length
    app.path_length = 0;
}

void draw_tiles(const App &app)
{
    for (int i = 0; i < SCREEN_WIDTH / TILE_SIZE; ++i)
    {
        for (int j = 0; j < SCREEN_HEIGHT / TILE_SIZE; ++j)
        {
            float x = i * TILE_SIZE;
            float y = j * TILE_SIZE;

            switch (app.tiles[i][j].type)
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
            case TileType::WALKED:
                fill_rectangle(COLOR_GRAY, x, y, TILE_SIZE, TILE_SIZE);
                break;
            }
        }
    }
}


// Moving the player
void move_player(App &app)
{
    // If the path is already too long or the player has reached the end, stop moving
    if (app.path_length >= MAX_PATH_LENGTH || (app.start_col == app.end_col && app.start_row == app.end_row))
    {
        printf("Path is too long or player has reached the end. Stopping movement.\n");
        return;
    }

    int dx = 0, dy = 0;

    // Calculate the direction towards the end tile
    if (app.start_col < app.end_col)
    {
        printf("Moving right towards end tile.\n");
        dx = 1;
    }
    else if (app.start_col > app.end_col)
    {
        printf("Moving left towards end tile.\n");
        dx = -1;
    }
    else if (app.start_row < app.end_row)
    {
        printf("Moving down towards end tile.\n");
        dy = 1;
    }
    else if (app.start_row > app.end_row)
    {
        printf("Moving up towards end tile.\n");
        dy = -1;
    }

    // Attempt to move to the nearest valid tile in each direction
    for (int i = 0; i < 4; ++i)
    {
        // Check if the move in the calculated direction is valid
        if (is_valid_move(app.tiles, app.start_col + dx, app.start_row + dy))
        {
            // Move to the nearest valid tile
            move(dx, dy);
            return;
        }

        // Rotate the direction to the next cardinal direction
        int temp = dx;
        dx = -dy;
        dy = temp;
    }

    // If all directions are invalid, print an error message
    printf("Cannot move to a valid tile. Choosing the next valid tile.\n");

    // Choose the next valid tile as the spot for the player to move to
    if (is_valid_move(app.tiles, app.start_col + dx, app.start_row + dy))
    {
        move(dx, dy);
    }
    else
    {
        printf("No valid tile available. Staying in place.\n");
    }
}


void move(int x, int y)
{
// Check if the new position is within the screen bounds and the tile is traversable
            if (app.tiles[app.start_col + x][app.start_row + y].type != TileType::ORANGE &&
                app.tiles[app.start_col + x][app.start_row + y].type != TileType::WALKED)
            {
                // Update the path
                app.tiles[app.start_col + x][app.start_row + y].type = TileType::WALKED;
                app.path_length++;

                // Update the start position
                app.start_col += x;
                app.start_row += y;

                printf("Moved to (%d, %d).\n", app.start_col, app.start_row);
            }
            else
            {
                printf("Cannot move to (%d, %d) because it is an orange tile or already walked. Staying in place.\n", app.start_col + x, app.start_row + y);
            }
}

bool is_valid_move(Tile tiles[][SCREEN_HEIGHT / TILE_SIZE], int x, int y)
{
    // Check if any adjacent tile is traversable and not a border tile
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            int adj_x = x + i;
            int adj_y = y + j;

            // Skip the current tile
            if (i == 0 && j == 0)
            {
                continue;
            }
            else if (i * j == 1 || i * j == -1)
            {
                printf("I: %d and J: %d are -1 or positive one: trying to move diagonal\n", i, j);
                continue;
            }
            else
            {
                // Check if the adjacent tile is within bounds and traversable
                if (x > 0 && y > 0 && x <= SCREEN_WIDTH && y <= SCREEN_HEIGHT &&
                    tiles[adj_x][adj_y].type != TileType::ORANGE)
                {
                    printf("Everything is true Returning true for is valid move.\n");
                    return true;
                }
            }
        }
    }

    // If no adjacent tile is traversable, or if all adjacent tiles are borders, the move is invalid
    return false;
}