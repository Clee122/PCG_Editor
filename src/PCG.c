#include "PCG.h"
#include <stdio.h>

// ============================================= 
// void PCG_CreateMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS])
// ============================================= 
void PCG_CreateMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]) {
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            _tileArray[y][x] = (TileType)GetRandomValue(0, TILE_COUNT - 1);
        }
    }
}


// ============================================= 
// Color PCG_GetTileColor(TileType tileType)
// Return a colour based on the type type input
// ============================================= 
Color PCG_GetTileColor(TileType tileType) {
    switch (tileType) {
    case TILE_TYPE_GRASS: return GRASS_COLOR;
    case TILE_TYPE_ROCK: return ROCK_COLOR;
    default: return UNKNOWN_COLOR;
    }
}


// ============================================= 
// void PCG_DrawMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS])
// ============================================= 
void PCG_DrawMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]) {
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, PCG_GetTileColor(_tileArray[y][x]));
        }
    }
}

// ============================================= 
// void PCG_PrintMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS])
// ============================================= 
void PCG_PrintMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]) {
    printf("\n-------Map Layout:--------\n");
    // (Existing Print Logic here...)
    printf("--------------------------\n");
}