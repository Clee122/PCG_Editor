#include "PCG.h"
#include <cstdio>

// Required to call raygui buttons
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

// =============================================
// void PCG_CreateMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS])
// =============================================
void PCG::PCG_CreateMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS])
{
    for (int y = 0; y < MAP_ROWS; y++)
    {
        for (int x = 0; x < MAP_COLUMNS; x++)
        {
            _tileArray[y][x] = (TileType)GetRandomValue(0, TILE_COUNT - 1);
        }
    }
}

// =============================================
// Color PCG_GetTileColor(TileType tileType)
// Return a colour based on the type input
// =============================================
Color PCG::PCG_GetTileColor(TileType tileType)
{
    switch (tileType)
    {
    case TILE_TYPE_GRASS: return GRASS_COLOR;
    case TILE_TYPE_ROCK: return ROCK_COLOR;
    default: return UNKNOWN_COLOR;
    }
}

// =============================================
// void PCG_DrawMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS])
// =============================================
void PCG::PCG_DrawMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS])
{
    for (int y = 0; y < MAP_ROWS; y++)
    {
        for (int x = 0; x < MAP_COLUMNS; x++)
        {
            DrawRectangle(
                x * TILE_SIZE,
                y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE,
                PCG_GetTileColor(_tileArray[y][x])
            );
        }
    }
}

// =============================================
// void PCG_PrintMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS])
// =============================================
void PCG::PCG_PrintMap(TileType _tileArray[MAP_ROWS][MAP_COLUMNS])
{
    std::printf("\n-------Map Layout:--------\n");
    std::printf("--------------------------\n");
}

// =============================================
// char GetTileChar(TileType tileType)
// Return a char value based on the type of tile passed in
// =============================================
char PCG::GetTileChar(TileType tileType)
{
    switch (tileType)
    {
    case TILE_TYPE_GRASS: return GRASS_CHAR;
    case TILE_TYPE_ROCK: return ROCK_CHAR;
    default: return '?';
    }
}

// =============================================
// void PCG_SaveMapData(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* _filename)
// =============================================
void PCG::PCG_SaveMapData(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* _filename)
{
    FILE* file = std::fopen(_filename, "w");
    if (file == nullptr)
    {
        return;
    }

    for (int y = 0; y < MAP_ROWS; y++)
    {
        for (int x = 0; x < MAP_COLUMNS; x++)
        {
            std::fputc(GetTileChar(_tileArray[y][x]), file);
        }
        std::fputc('\n', file);
    }

    std::fclose(file);
    std::printf("Map saved to %s\n", _filename);
}

// =============================================
// void PCG_LoadMapData(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* _filename)
// =============================================
void PCG::PCG_LoadMapData(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* _filename)
{
    FILE* file = std::fopen(_filename, "r");
    if (file == nullptr)
    {
        return;
    }

    for (int y = 0; y < MAP_ROWS; y++)
    {
        for (int x = 0; x < MAP_COLUMNS; x++)
        {
            int ch = std::fgetc(file);

            while (ch == '\n' || ch == '\r')
            {
                ch = std::fgetc(file);
            }

            if (ch == GRASS_CHAR)
            {
                _tileArray[y][x] = TILE_TYPE_GRASS;
            }
            else if (ch == ROCK_CHAR)
            {
                _tileArray[y][x] = TILE_TYPE_ROCK;
            }
        }
    }

    std::fclose(file);
    std::printf("Map loaded from %s\n", _filename);
}

// =============================================
// void PCG_SaveMapImage(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* filename)
// =============================================
void PCG::PCG_SaveMapImage(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], const char* filename)
{
    Image mapImage = GenImageColor(MAP_COLUMNS, MAP_ROWS, BLACK);

    for (int y = 0; y < MAP_ROWS; y++)
    {
        for (int x = 0; x < MAP_COLUMNS; x++)
        {
            Color c = PCG_GetTileColor(_tileArray[y][x]);
            ImageDrawPixel(&mapImage, x, y, c);
        }
    }

    if (ExportImage(mapImage, filename))
    {
        std::printf("Image saved: %s\n", filename);
    }

    UnloadImage(mapImage);
}

// =============================================
// void PCG_DrawGUI(TileType tileArray[MAP_ROWS][MAP_COLUMNS])
// =============================================
void PCG::PCG_DrawGUI(TileType tileArray[MAP_ROWS][MAP_COLUMNS])
{
    if (GuiButton(RESET_BUTTON_BOUNDS, "Reset Map"))
    {
        PCG_CreateMap(tileArray);
    }

    Rectangle saveRect = { BUTTON_X, BUTTON_Y - 70, BUTTON_WIDTH, BUTTON_HEIGHT };
    if (GuiButton(saveRect, "Save Map Data"))
    {
        PCG_SaveMapData(tileArray, MAP_TEXT_FILENAME);
    }

    Rectangle loadRect = { BUTTON_X, BUTTON_Y - 140, BUTTON_WIDTH, BUTTON_HEIGHT };
    if (GuiButton(loadRect, "Load Map Data"))
    {
        PCG_LoadMapData(tileArray, MAP_TEXT_FILENAME);
    }

    Rectangle imgRect = { BUTTON_X, BUTTON_Y - 210, BUTTON_WIDTH, BUTTON_HEIGHT };
    if (GuiButton(imgRect, "Save Map PNG"))
    {
        PCG_SaveMapImage(tileArray, MAP_IMAGE_FILENAME);
    }
}