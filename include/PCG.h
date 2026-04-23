#pragma once
#include "raylib.h"

namespace PCG
{
    // Screen & Map Dimensions
    constexpr int SCREEN_WIDTH = 1024;
    constexpr int SCREEN_HEIGHT = 1024;
    constexpr int TILE_SIZE = 64;
    constexpr int MAP_COLUMNS = (SCREEN_WIDTH / TILE_SIZE);
    constexpr int MAP_ROWS = (SCREEN_HEIGHT / TILE_SIZE);

    // Tile Types
    typedef enum
    {
        TILE_TYPE_GRASS = 0,
        TILE_TYPE_ROCK = 1,
        TILE_COUNT
    } TileType;

    // Visual & Character settings
    constexpr char GRASS_CHAR = '.';
    constexpr char ROCK_CHAR = '#';
    constexpr Color GRASS_COLOR = { 69, 182, 156, 255 };
    constexpr Color ROCK_COLOR = { 114, 147, 160, 255 };
    constexpr Color UNKNOWN_COLOR = WHITE;

    // File Names
    constexpr const char* MAP_TEXT_FILENAME = "pcg_map_data.txt";
    constexpr const char* MAP_IMAGE_FILENAME = "pcg_map.png";

    // UI variable defines used to position buttons on screen
    constexpr int BUTTON_WIDTH = 200;
    constexpr int BUTTON_HEIGHT = 50;
    constexpr int BUTTON_X = (SCREEN_WIDTH - BUTTON_WIDTH - 20);
    constexpr int BUTTON_Y = (SCREEN_HEIGHT - BUTTON_HEIGHT - 20);
    constexpr Rectangle RESET_BUTTON_BOUNDS = { BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT };

    // Function Declarations
    void PCG_CreateMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]);
    void PCG_DrawMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]);
    void PCG_PrintMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]);
    Color PCG_GetTileColor(TileType tileType);

    // Helpers
    char GetTileChar(TileType tileType);

    // I/O Functions
    void PCG_SaveMapData(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* filename);
    void PCG_LoadMapData(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* filename);
    void PCG_SaveMapImage(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* filename);

    // UI
    void PCG_DrawGUI(TileType tileArray[MAP_ROWS][MAP_COLUMNS]);
}