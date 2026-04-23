#include "raylib.h"
#include "resource_dir.h"
#include "PCG.h"

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(PCG::SCREEN_WIDTH, PCG::SCREEN_HEIGHT, "Construct Map Editor");
    SearchAndSetResourceDir("resources");

    PCG::TileType tileArray[PCG::MAP_ROWS][PCG::MAP_COLUMNS] = { PCG::TileType::TILE_TYPE_ROCK };
    PCG::PCG_CreateMap(tileArray);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        PCG::PCG_DrawMap(tileArray);
        DrawText("Construct Map Editor", 20, 20, 20, WHITE);
        PCG::PCG_DrawGUI(tileArray);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}