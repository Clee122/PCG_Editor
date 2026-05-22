#include "PCG.h"
#include <stdio.h>
#include <iostream>
#include <fstream>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


// Constructor for our TileMap class.
PCG::TileMap::TileMap()
{
    // I initialise the map as grass so the editor starts from a clean readable state
    // before any generator or manual painting changes the tiles.
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            tileArray[y][x] = TILE_TYPE_GRASS;
        }
    }

    // The generator pointer starts as nullptr because the user chooses which
    // generation method will control the map later.
    mapGenerator = nullptr;
}

// Destructor for our TileMap class.
PCG::TileMap::~TileMap()
{
    // Since generators are created on the heap, they need to be deleted when
    // the TileMap is destroyed to avoid leaving memory behind.
    if (mapGenerator != nullptr) {
        delete mapGenerator;
        mapGenerator = nullptr;
    }
}


// void CreateMap()
void PCG::TileMap::CreateMap() {
    // This gives the editor a quick random map option, which is useful as a
    // simple baseline before comparing it with more structured generators.
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            tileArray[y][x] = (TileType)GetRandomValue(0, TILE_COUNT - 1);
        }
    }
}

// void SetTile(int x, int y, TileType tileType)
void PCG::TileMap::SetTile(int x, int y, TileType tileType)
{
    // This check stops invalid tile positions from changing the array,
    // which prevents out-of-bounds errors when editing near the map edges.
    if (x >= 0 && x < MAP_COLUMNS && y >= 0 && y < MAP_ROWS) {
        tileArray[y][x] = tileType;
    }
}

// Color PCG_GetTileColor(TileType tileType)
Color PCG::TileMap::GetTileColor(PCG::TileType _tileType) const {
    // I keep tile colours in one function so grass and rock always look consistent
    // across drawing, saving, and exporting.
    switch (_tileType) {
    case PCG::TileType::TILE_TYPE_GRASS:
        return GRASS_COLOR;
    case TILE_TYPE_ROCK:
        return ROCK_COLOR;
    default:
        return UNKNOWN_COLOR;
    }
}


// void PCG_DrawMap()
void PCG::TileMap::DrawMap() const {
    // The map is drawn directly from the tile data, so any generator or mouse edit
    // immediately changes what the user sees on screen.
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            DrawRectangle(
                x * PCG::TILE_SIZE,
                y * PCG::TILE_SIZE,
                PCG::TILE_SIZE,
                PCG::TILE_SIZE,
                PCG::TileMap::GetTileColor(tileArray[y][x])
            );
        }
    }
}


// void PCG_PrintMap()
void PCG::TileMap::PrintMap() const {
    // This console output was kept as a basic debugging tool while testing map data.
    std::cout << "\n-------Map Layout:--------\n";
    std::cout << "--------------------------\n";
}



// char GetTileChar(TileType tileType)

char PCG::TileMap::GetTileChar(PCG::TileType _tileType) const {
    // These characters are used when saving the map so the text file is readable
    // and can be loaded back into tile data later.
    switch (_tileType) {
    case PCG::TileType::TILE_TYPE_GRASS:
        return PCG::GRASS_CHAR;
    case PCG::TileType::TILE_TYPE_ROCK:
        return PCG::ROCK_CHAR;
    default:
        return '?';
    }
}


// void PCG_SaveMapData(const char* _filename)
void PCG::TileMap::SaveMapData(const char* _filename) const {
    std::fstream file;
    file.open(_filename, std::ios::out);

    // If the file cannot open, the function exits safely rather than trying to
    // write to an invalid stream.
    if (!file.is_open()) {
        return;
    }

    // Saving the map as text makes the generated level reusable later instead of
    // only existing while the program is running.
    for (int y = 0; y < PCG::MAP_ROWS; y++) {
        for (int x = 0; x < PCG::MAP_COLUMNS; x++) {
            file.put(PCG::TileMap::GetTileChar(tileArray[y][x]));
        }

        // Each row is separated so the saved file still resembles the grid layout.
        file.put('\n');
    }

    file.close();
    printf("Map saved to %s\n", _filename);
}


// void PCG_LoadMapData(const char* _filename)
void PCG::TileMap::LoadMapData(const char* _filename) {
    std::fstream file;
    file.open(_filename, std::ios::in);

    // The load function exits if there is no valid file, which avoids overwriting
    // the current map with incomplete or invalid data.
    if (!file.is_open()) {
        return;
    }

    // The saved file is read back into the tile array so the editor can restore
    // maps created in earlier sessions.
    for (int y = 0; y < PCG::MAP_ROWS; y++) {
        for (int x = 0; x < PCG::MAP_COLUMNS; x++) {
            int ch = file.get();

            // Newline characters are skipped because the saved file stores the map row by row.
            // Without this, loading would treat line breaks as tile data.
            while (ch == '\n' || ch == '\r') {
                ch = file.get();
            }

            if (ch == PCG::GRASS_CHAR) {
                tileArray[y][x] = PCG::TileType::TILE_TYPE_GRASS;
            }
            else if (ch == PCG::ROCK_CHAR) {
                tileArray[y][x] = PCG::TileType::TILE_TYPE_ROCK;
            }
        }
    }

    file.close();
    std::cout << "Map loaded from " << _filename << std::endl;
}

// void PCG_SaveMapImage(const char* filename
void PCG::TileMap::SaveMapImage(const char* filename) const {
    // Exporting an image gives a quick visual record of the generated map,
    // which is useful for checking results without reopening the editor.
    Image mapImage = GenImageColor(PCG::MAP_COLUMNS, PCG::MAP_ROWS, BLACK);

    for (int y = 0; y < PCG::MAP_ROWS; y++) {
        for (int x = 0; x < PCG::MAP_COLUMNS; x++) {
            Color c = PCG::TileMap::GetTileColor(tileArray[y][x]);
            ImageDrawPixel(&mapImage, x, y, c);
        }
    }

    if (ExportImage(mapImage, filename)) {
        std::cout << "Image saved: " << filename << std::endl;
    }

    UnloadImage(mapImage);
}


// void HandleMouseEditing()
void PCG::TileMap::HandleMouseEditing()
{
    Vector2 mousePos = GetMousePosition();

    // I block mouse painting over the UI area so clicking buttons does not accidentally
    // edit the map underneath.
    if (mousePos.x >= BUTTON_X - 10) {
        return;
    }

    int tileX = (int)(mousePos.x / TILE_SIZE);
    int tileY = (int)(mousePos.y / TILE_SIZE);

    // Converting mouse position to tile position lets the user edit the same
    // array data that the generators use.
    if (tileX >= 0 && tileX < MAP_COLUMNS && tileY >= 0 && tileY < MAP_ROWS) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            SetTile(tileX, tileY, TILE_TYPE_ROCK);
        }

        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
            SetTile(tileX, tileY, TILE_TYPE_GRASS);
        }
    }
}



// void PCG_DrawGUI()
void PCG::TileMap::DrawGUI() {
    // Reset uses the current generator instead of always using random generation,
    // so the button behaves consistently with the selected algorithm.
    if (GuiButton(RESET_BUTTON_BOUNDS, "Reset Map")) {
        if (GetMapGenerator() != nullptr) {
            GetMapGenerator()->Generate(tileArray);
        }
    }

    Rectangle saveRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 70, PCG::BUTTON_WIDTH, PCG::BUTTON_HEIGHT };
    if (GuiButton(saveRect, "Save Map Data")) {
        SaveMapData(MAP_TEXT_FILENAME);
    }

    Rectangle loadRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 140, PCG::BUTTON_WIDTH, PCG::BUTTON_HEIGHT };
    if (GuiButton(loadRect, "Load Map Data")) {
        LoadMapData(MAP_TEXT_FILENAME);
    }

    Rectangle imgRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 210, PCG::BUTTON_WIDTH, PCG::BUTTON_HEIGHT };
    if (GuiButton(imgRect, "Save Map PNG")) {
        SaveMapImage(MAP_IMAGE_FILENAME);
    }

    // Generator buttons make the difference between algorithms visible to the user,
    // which helps compare random, noise, and Cellular Automata output.
    Rectangle randomRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 280, PCG::BUTTON_WIDTH, PCG::BUTTON_HEIGHT };
    if (GuiButton(randomRect, "Random Generator")) {
        SetMapGenerator(new PCG::RandomMapGenerator());
        GetMapGenerator()->Generate(tileArray);
    }

    Rectangle noiseRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 350, PCG::BUTTON_WIDTH, PCG::BUTTON_HEIGHT };
    if (GuiButton(noiseRect, "Noise Generator")) {
        SetMapGenerator(new PCG::NoiseMapGenerator());
        GetMapGenerator()->Generate(tileArray);
    }

    Rectangle cellularRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 420, PCG::BUTTON_WIDTH, PCG::BUTTON_HEIGHT };
    if (GuiButton(cellularRect, "Cellular Automata")) {
        SetMapGenerator(new PCG::CellularAutomataGenerator());
        GetMapGenerator()->Generate(tileArray);
    }

    DrawText("Left click: Rock", 20, SCREEN_HEIGHT - 50, 20, WHITE);
    DrawText("Right click: Grass", 20, SCREEN_HEIGHT - 25, 20, WHITE);
}


// SetMapGenerator and GetMapGenerator functions.
void PCG::TileMap::SetMapGenerator(PCG::MapGenerator* generator) {
    // When switching generators, the old generator is deleted first so the pointer
    // does not lose access to heap memory and create a memory leak.
    if (mapGenerator != nullptr) {
        delete mapGenerator;
        mapGenerator = nullptr;
    }

    mapGenerator = generator;
}


// GetMapGenerator
PCG::MapGenerator* PCG::TileMap::GetMapGenerator() const {
    // Returning the base pointer lets the editor call Generate() without needing
    // to know which specific generator class is currently active.
    return mapGenerator;
}


// RandomMapGenerator
PCG::RandomMapGenerator::RandomMapGenerator() {
    // No setup is needed because this generator only uses Raylib's random value function.
}

PCG::RandomMapGenerator::~RandomMapGenerator() {
    // No heap memory is created inside this class, so there is nothing extra to clean up.
}

void PCG::RandomMapGenerator::Generate(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]) {
    // Random generation is kept as a baseline so I can compare it against more
    // structured methods like noise and Cellular Automata.
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            _tileArray[y][x] = (TileType)GetRandomValue(0, TILE_COUNT - 1);
        }
    }
}

// NoiseGenerator
PCG::NoiseMapGenerator::NoiseMapGenerator() {
    // No setup is needed here because the noise image is generated inside Generate().
}

PCG::NoiseMapGenerator::~NoiseMapGenerator() {
    // The generated noise image is unloaded in Generate(), so this destructor stays empty.
}

void PCG::NoiseMapGenerator::Generate(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]) {
    // Random offsets stop the noise map from looking identical every time it is generated.
    int offsetX = GetRandomValue(0, 1000);
    int offsetY = GetRandomValue(0, 1000);
    float scale = 2.5f;

    // Noise generation creates smoother patterns than pure random generation,
    // which makes the map feel less chaotic.
    Image noiseImg = GenImagePerlinNoise(MAP_COLUMNS, MAP_ROWS, offsetX, offsetY, scale);

    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            Color col = GetImageColor(noiseImg, x, y);
            float brightness = (col.r + col.g + col.b) / (3.0f * 255.0f);

            // A simple threshold converts brightness into tile types.
            // Darker areas become rock and lighter areas become grass.
            if (brightness < 0.5f) {
                _tileArray[y][x] = TILE_TYPE_ROCK;
            }
            else {
                _tileArray[y][x] = TILE_TYPE_GRASS;
            }
        }
    }

    UnloadImage(noiseImg);
}

// CellularAutomataGenerator
PCG::CellularAutomataGenerator::CellularAutomataGenerator() {
    // Values such as fill percentage and smoothing passes could be exposed later
    // if I wanted more user control over cave generation.
}

PCG::CellularAutomataGenerator::~CellularAutomataGenerator() {
    // This generator does not allocate extra memory, so no cleanup is needed here.
}


// CountRockNeighbours
int PCG::CellularAutomataGenerator::CountRockNeighbours(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], int x, int y)
{
    int rockCount = 0;

    // The algorithm checks nearby tiles because each tile's final state depends
    // on the shape of the surrounding area, not only on its own random value.
    for (int neighbourY = y - 1; neighbourY <= y + 1; neighbourY++) {
        for (int neighbourX = x - 1; neighbourX <= x + 1; neighbourX++) {
            if (neighbourX == x && neighbourY == y) {
                continue;
            }

            // Out-of-bounds neighbours are counted as rock so the map forms solid outer edges
            // instead of open gaps around the border.
            if (neighbourX < 0 || neighbourX >= MAP_COLUMNS || neighbourY < 0 || neighbourY >= MAP_ROWS) {
                rockCount++;
            }
            else if (_tileArray[neighbourY][neighbourX] == TILE_TYPE_ROCK) {
                rockCount++;
            }
        }
    }

    return rockCount;
}


// Generate
void PCG::CellularAutomataGenerator::Generate(TileType _tileArray[MAP_ROWS][MAP_COLUMNS])
{
    // Cellular Automata starts with random tiles, then smooths them using neighbour rules.
    // This helps turn noisy data into more cave-like spaces.
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            int randomValue = GetRandomValue(0, 100);
            _tileArray[y][x] = (randomValue < 45) ? TILE_TYPE_ROCK : TILE_TYPE_GRASS;
        }
    }

    // Several smoothing passes are used because one pass usually still looks too noisy.
    for (int i = 0; i < 5; i++) {
        TileType tempArray[MAP_ROWS][MAP_COLUMNS];

        for (int y = 0; y < MAP_ROWS; y++) {
            for (int x = 0; x < MAP_COLUMNS; x++) {
                int neighbours = CountRockNeighbours(_tileArray, x, y);

                if (neighbours > 4) {
                    tempArray[y][x] = TILE_TYPE_ROCK;
                }
                else {
                    tempArray[y][x] = TILE_TYPE_GRASS;
                }
            }
        }

        // The temporary array prevents early changes in the loop from affecting
        // neighbour counts for tiles that have not been processed yet.
        for (int y = 0; y < MAP_ROWS; y++) {
            for (int x = 0; x < MAP_COLUMNS; x++) {
                _tileArray[y][x] = tempArray[y][x];
            }
        }
    }
}