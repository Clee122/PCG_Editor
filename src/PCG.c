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

// ============================================= 
// char GetTileChar(TileType tileType)
// Return a char value based on the type of tile passed in
// ============================================= 
char GetTileChar(TileType tileType) {
    switch (tileType) {
    case TILE_TYPE_GRASS: return GRASS_CHAR;
    case TILE_TYPE_ROCK: return ROCK_CHAR;
    default: return '?';
    }
}

// ============================================= 
// void PCG_SaveMapData(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* _filename)
// Store our tilemap data to a text file using the input _filename
// ============================================= 
void PCG_SaveMapData(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* _filename) {
    FILE* file = fopen(_filename, "w"); // "w" = Write
    if (file == NULL) {
        return;
    }

    // Write each single tileArray charater into our file stream
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            fputc(GetTileChar(_tileArray[y][x]), file);
        }
        fputc('\n', file); // New line at end of row
    }
    fclose(file);
    printf("Map saved to %s\n", _filename);
}


// ============================================= 
// void PCG_LoadMapData(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* _filename)
// Load our tilemap data from a text file, using input _filename
// ============================================= 
void PCG_LoadMapData(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* _filename) {
    FILE* file = fopen(_filename, "r"); // "r" = Read
    if (file == NULL) {
        return;
    }

    // Get each character from our file stream, and load it into our tileMap array
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            int ch = fgetc(file);
            // Skip invisible newline characters
            while (ch == '\n' || ch == '\r') {
                ch = fgetc(file);
            }

            if (ch == GRASS_CHAR) {
                _tileArray[y][x] = TILE_TYPE_GRASS;
            }
            else if (ch == ROCK_CHAR) {
                _tileArray[y][x] = TILE_TYPE_ROCK;
            }
        }
    }
    fclose(file);
    printf("Map loaded from %s\n", _filename);
}

// ============================================= 
// void PCG_SaveMapImage(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* filename)
// Store our tileMap data as a .png image, using the input filename.
// ============================================= 
void PCG_SaveMapImage(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* filename) {
    Image mapImage = GenImageColor(MAP_COLUMNS, MAP_ROWS, BLACK);

    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            Color c = PCG_GetTileColor(_tileArray[y][x]);
            ImageDrawPixel(&mapImage, x, y, c);
        }
    }
    if (ExportImage(mapImage, filename)) {
        printf("Image saved: %s\n", filename);
    }
    UnloadImage(mapImage);
}

// Required to call Raylib gui buttons. Add this near the top of PCG.c
#define RAYGUI_IMPLEMENTATION
#include "raygui.h" 

// ============================================= 
// void PCG_DrawGUI(TileType tileArray[MAP_ROWS][MAP_COLUMNS])
// ============================================= 
void PCG_DrawGUI(TileType tileArray[MAP_ROWS][MAP_COLUMNS]) {
    // Reset Button
    if (GuiButton(RESET_BUTTON_BOUNDS, "Reset Map")) {
        PCG_CreateMap(tileArray);
    }

    // Save Data Button
    Rectangle saveRect = { BUTTON_X, BUTTON_Y - 70, BUTTON_WIDTH, BUTTON_HEIGHT };
    if (GuiButton(saveRect, "Save Map Data")) {
        PCG_SaveMapData(tileArray, MAP_TEXT_FILENAME);
    }

    // Load Data Button
    Rectangle loadRect = { BUTTON_X, BUTTON_Y - 140, BUTTON_WIDTH, BUTTON_HEIGHT };
    if (GuiButton(loadRect, "Load Map Data")) {
        PCG_LoadMapData(tileArray, MAP_TEXT_FILENAME);
    }

    // Save Image Button
    Rectangle imgRect = { BUTTON_X, BUTTON_Y - 210, BUTTON_WIDTH, BUTTON_HEIGHT };
    if (GuiButton(imgRect, "Save Map PNG")) {
        PCG_SaveMapImage(tileArray, MAP_IMAGE_FILENAME);
    }
}