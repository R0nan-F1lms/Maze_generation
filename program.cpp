#include "splashkit.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TILE_SIZE = 40;
const int BORDER_THICKNESS = 1;
const int MIN_DISTANCE = 6; // Minimum distance between start and end tiles

enum class TileType
{
    EMPTY,
    ORANGE,
    START,
    END,
    BLOCK,
    CORNER_BLOCK // New tile type for corner blocks
};

struct Tile
{
    TileType type;

    bool is_traversable() const
    {
        return type != TileType::ORANGE && type != TileType::BLOCK && type != TileType::CORNER_BLOCK;
    }
};

struct Player
{
    int x, y;
};

struct App
{
    Player player;
    Tile tiles[SCREEN_WIDTH / TILE_SIZE][SCREEN_HEIGHT / TILE_SIZE];
    int start_row, start_col;
    int end_row, end_col;
};

App app;

void setup(App &app);
void draw_tiles(const App &app);
void handle_input(App &app);
void move_player(App &app, int dx, int dy);
void spawn_corner_blocks(App &app); // Function to spawn corner blocks

int main()
{
    open_window("Maze App", SCREEN_WIDTH, SCREEN_HEIGHT);
    setup(app);

    do
    {
        process_events();
        clear_screen(COLOR_BLACK);

        handle_input(app);
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

    // Set player initial position to start tile
    app.player.x = app.start_col * TILE_SIZE;
    app.player.y = app.start_row * TILE_SIZE;

    // Spawn corner blocks
    spawn_corner_blocks(app);
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
            case TileType::BLOCK:
                fill_rectangle(COLOR_ORANGE, x, y, TILE_SIZE, TILE_SIZE);
                break;
            case TileType::CORNER_BLOCK: // New case for corner blocks
                fill_rectangle(COLOR_PINK, x, y, TILE_SIZE, TILE_SIZE);
                break;
            }
        }
    }

    // Draw player
    fill_circle(COLOR_YELLOW, app.player.x + TILE_SIZE / 2, app.player.y + TILE_SIZE / 2, TILE_SIZE / 4);
}

void handle_input(App &app)
{
    if (key_typed(D_KEY))
    {
        move_player(app, TILE_SIZE, 0);
    }
    else if (key_typed(A_KEY))
    {
        move_player(app, -TILE_SIZE, 0);
    }
    else if (key_typed(W_KEY))
    {
        move_player(app, 0, -TILE_SIZE);
    }
    else if (key_typed(S_KEY))
    {
        move_player(app, 0, TILE_SIZE);
    }
}

void move_player(App &app, int dx, int dy)
{
    int new_x = app.player.x + dx;
    int new_y = app.player.y + dy;

    int tile_x = new_x / TILE_SIZE;
    int tile_y = new_y / TILE_SIZE;

    if (new_x >= 0 && new_x < SCREEN_WIDTH && new_y >= 0 && new_y < SCREEN_HEIGHT &&
        app.tiles[tile_x][tile_y].is_traversable())
    {
        app.player.x = new_x;
        app.player.y = new_y;
    }
}

void spawn_corner_blocks(App &app)
{
    // Define the coordinates of the corners
    const int corners[4][2] = {
        {0, 0},                                          // Top-left corner
        {SCREEN_WIDTH / TILE_SIZE - 1, 0},               // Top-right corner
        {0, SCREEN_HEIGHT / TILE_SIZE - 1},              // Bottom-left corner
        {SCREEN_WIDTH / TILE_SIZE - 1, SCREEN_HEIGHT / TILE_SIZE - 1} // Bottom-right corner
    };

    // Spawn corner blocks
    for (int i = 0; i < 4; ++i)
    {
        int corner_x = corners[i][0];
        int corner_y = corners[i][1];

        // Check if the corner block overlaps with the start or end tiles
        if ((corner_x != app.start_col || corner_y != app.start_row) &&
            (corner_x != app.end_col || corner_y != app.end_row))
        {
            app.tiles[corner_x][corner_y].type = TileType::CORNER_BLOCK;
        }
    }
}
