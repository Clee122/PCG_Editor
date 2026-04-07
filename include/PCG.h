#pragma once
#ifndef PCG_H
#define PCG_H
#include "raylib.h"

// Screen & Map Dimensions
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 1024
#define TILE_SIZE 64
#define MAP_COLUMNS (SCREEN_WIDTH / TILE_SIZE)
#define MAP_ROWS (SCREEN_HEIGHT / TILE_SIZE)

// Tile Types (Using Enum for readability)
typedef enum {
    TILE_TYPE_GRASS = 0,
    TILE_TYPE_ROCK = 1,
    TILE_COUNT  // Automatically counts total types
} TileType;

// Visual & Character settings
#define GRASS_CHAR '.'
#define ROCK_CHAR '#'
#define GRASS_COLOR (Color){69, 182, 156, 255}
#define ROCK_COLOR (Color){114, 147, 160, 255}
#define UNKNOWN_COLOR WHITE

// Function Declarations
void PCG_CreateMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]);
void PCG_DrawMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]);
void PCG_PrintMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]);
Color PCG_GetTileColor(TileType tileType);

#endif // PCG_H