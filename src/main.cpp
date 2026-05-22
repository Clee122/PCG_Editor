#include "raylib.h"
#include "resource_dir.h"
#include "PCG.h"

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(PCG::SCREEN_WIDTH, PCG::SCREEN_HEIGHT, "Construct Map Editor");

    // TileMap owns the grid data and handles drawing, editing, saving, loading, and UI.
    PCG::TileMap tileMap;

    // Cellular Automata is used as the default generator because it produces more structured cave-like maps.
    tileMap.SetMapGenerator(new PCG::CellularAutomataGenerator());

    // Generate the first map before the main loop so the user starts with visible content.
    tileMap.GetMapGenerator()->Generate(tileMap.tileArray);

    while (!WindowShouldClose()) {
        // Input is checked every frame so manual painting feels responsive.
        tileMap.HandleMouseEditing();

        BeginDrawing();
        ClearBackground(BLACK);

        // Draw the current map state first, then draw text and UI on top.
        tileMap.DrawMap();
        DrawText("Construct Map Editor", 20, 20, 20, WHITE);
        tileMap.DrawGUI();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}