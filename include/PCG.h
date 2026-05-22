#pragma once
#include "raylib.h"

namespace PCG {
    // These values define the editor window and tile scale.
    // TILE_SIZE was set to 8 because it gives a clearer balance between map detail and mouse editing visibility.
    constexpr int SCREEN_WIDTH = 1024;
    constexpr int SCREEN_HEIGHT = 1024;
    constexpr int TILE_SIZE = 8;
    constexpr int MAP_COLUMNS = (SCREEN_WIDTH / TILE_SIZE);
    constexpr int MAP_ROWS = (SCREEN_HEIGHT / TILE_SIZE);

    // Enum is used instead of raw numbers so tile values are easier to understand and less error-prone.
    typedef enum {
        TILE_TYPE_GRASS = 0,
        TILE_TYPE_ROCK = 1,
        TILE_COUNT
    } TileType;

    // These values keep the visual and saved text representation consistent.
    // This matters because the same tile data is used for drawing, saving, loading, and exporting.
    constexpr char GRASS_CHAR = '.';
    constexpr char ROCK_CHAR = '#';
    constexpr Color GRASS_COLOR = { 69, 182, 156, 255 };
    constexpr Color ROCK_COLOR = { 114, 147, 160, 255 };
    constexpr Color UNKNOWN_COLOR = WHITE;

    // Button positions are calculated from the screen size so the UI stays aligned to the right side.
    constexpr int BUTTON_WIDTH = 200;
    constexpr int BUTTON_HEIGHT = 50;
    constexpr int BUTTON_X = (SCREEN_WIDTH - BUTTON_WIDTH - 20);
    constexpr int BUTTON_Y = (SCREEN_HEIGHT - BUTTON_HEIGHT - 20);
    constexpr Rectangle RESET_BUTTON_BOUNDS = { BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT };

    // These file names are centralised so save/load/export paths can be changed from one place.
    constexpr const char* MAP_TEXT_FILENAME = "pcg_map_data.txt";
    constexpr const char* MAP_IMAGE_FILENAME = "pcg_map.png";

    // Base class for all map generators.
    // This allows the TileMap to use different algorithms without needing to know their internal logic.
    class MapGenerator {
    public:
        virtual ~MapGenerator() = default;

        // Pure virtual function forces every generator to provide its own generation behaviour.
        virtual void Generate(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]) = 0;
    };

    // Random generator is kept as a simple baseline for comparison against more structured algorithms.
    class RandomMapGenerator : public MapGenerator {
    public:
        RandomMapGenerator();
        ~RandomMapGenerator();
        void Generate(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]) override;
    };

    // Noise generator creates smoother patterns than pure random generation.
    class NoiseMapGenerator : public MapGenerator {
    public:
        NoiseMapGenerator();
        ~NoiseMapGenerator();
        void Generate(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]) override;
    };

    // Cellular Automata generator was added to create cave-like maps by refining random tile data over time.
    class CellularAutomataGenerator : public MapGenerator {
    public:
        CellularAutomataGenerator();
        ~CellularAutomataGenerator();
        void Generate(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]) override;

    private:
        // Kept private because neighbour counting is only needed internally by this algorithm.
        int CountRockNeighbours(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], int x, int y);
    };

    class TileMap {
    public:
        TileMap();
        ~TileMap();

        // Core map behaviours are grouped inside TileMap so map data and map actions stay together.
        void CreateMap();
        void DrawMap() const;
        void PrintMap() const;
        void DrawGUI();

        // Manual editing is separated from drawing so user input can be handled clearly each frame.
        void HandleMouseEditing();

        // Save/load/export functions support the assignment pipeline by allowing generated maps to persist.
        void SaveMapData(const char* filename) const;
        void SaveMapImage(const char* filename) const;
        void LoadMapData(const char* filename);

        // SetTile protects the map from invalid coordinates before changing data.
        void SetTile(int x, int y, PCG::TileType tileType);
        Color GetTileColor(TileType tileType) const;
        char GetTileChar(TileType tileType) const;

        // Generator setter/getter allows the editor to switch algorithms at runtime.
        void SetMapGenerator(MapGenerator* generator);
        MapGenerator* GetMapGenerator() const;

        // This is public for convenience in the tutorial structure, but ideally it would be private later.
        TileType tileArray[MAP_ROWS][MAP_COLUMNS] = { PCG::TileType::TILE_TYPE_ROCK };

    private:
        // Pointer is used so the TileMap can hold any generator derived from MapGenerator.
        MapGenerator* mapGenerator;
    };
}