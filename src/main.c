#include "raylib.h"
#include "resource_dir.h"
#include "PCG.h" // Import our new module

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Construct Map Editor");

    TileType tileArray[MAP_ROWS][MAP_COLUMNS] = { 0 };
    PCG_CreateMap(tileArray);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        PCG_DrawMap(tileArray); // Function from PCG.c
        DrawText("Construct Map Editor", 20, 20, 20, WHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}