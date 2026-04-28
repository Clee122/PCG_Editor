#include "PCG.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
// Required to call Raylib gui buttons. Add this near the top of PCG.c
#define RAYGUI_IMPLEMENTATION
#include "raygui.h" 

// =============================================
// Constructor for our TileMap class.
// =============================================
PCG::TileMap::TileMap()
{
    // Initialise our tileMap array to all grass tiles by default when we create a new TileMap object. 
    // We can change this later using the CreateMap() function, or by setting individual tiles with SetTile().
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            tileArray[y][x] = TILE_TYPE_GRASS;
        }
    }

    // initialise the mapGenerator to null.
    mapGenerator = nullptr;
}


// =============================================
// Destructor for our TileMap class.
// =============================================
PCG::TileMap::~TileMap()
{
    if (mapGenerator != nullptr) {
        delete mapGenerator; // Clean up the map generator if it exists
        mapGenerator = nullptr;
    }
}


// ============================================= 
// void CreateMap()
// ============================================= 
void PCG::TileMap::CreateMap() {
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            tileArray[y][x] = (TileType)GetRandomValue(0, TILE_COUNT - 1);
        }
    }
}


// ============================================= 
// void SetTile(int x, int y, TileType tileType)
// set a tile in our tileMap array, using the input x and y coordinates, and the type of tile we want to set it to (tileType)
// ============================================= 
void PCG::TileMap::SetTile(int x, int y, TileType tileType)
{
    if (x >= 0 && x < MAP_COLUMNS && y >= 0 && y < MAP_ROWS) {
        tileArray[y][x] = tileType;
    }
}


// ============================================= 
// Color PCG_GetTileColor(TileType tileType)
// Return a colour based on the type type input
// ============================================= 
Color PCG::TileMap::GetTileColor(PCG::TileType _tileType) const {
    switch (_tileType) {
    case PCG::TileType::TILE_TYPE_GRASS:
        return GRASS_COLOR;
    case TILE_TYPE_ROCK:
        return ROCK_COLOR;
    default:
        return UNKNOWN_COLOR;
    }
}


// ============================================= 
// void PCG_DrawMap()
// ============================================= 
void PCG::TileMap::DrawMap() const {
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            DrawRectangle(x * PCG::TILE_SIZE, y * PCG::TILE_SIZE, PCG::TILE_SIZE, PCG::TILE_SIZE, PCG::TileMap::GetTileColor(tileArray[y][x]));
        }
    }
}


// ============================================= 
// void PCG_PrintMap()
// ============================================= 
void PCG::TileMap::PrintMap() const {
    std::cout << "\n-------Map Layout:--------\n";
    // (Existing Print Logic here...)
    std::cout << "--------------------------\n";
}


// ============================================= 
// char GetTileChar(TileType tileType)
// Return a char value based on the type of tile passed in
// ============================================= 
char PCG::TileMap::GetTileChar(PCG::TileType _tileType) const {
    switch (_tileType) {
    case PCG::TileType::TILE_TYPE_GRASS:
        return PCG::GRASS_CHAR;
    case PCG::TileType::TILE_TYPE_ROCK:
        return PCG::ROCK_CHAR;
    default:
        return '?';
    }
}


// ============================================= 
// void PCG_SaveMapData(const char* _filename)
// Store our tilemap data to a text file using the input _filename
// ============================================= 
void PCG::TileMap::SaveMapData(const char* _filename) const {
    std::fstream file;
    file.open(_filename, std::ios::out); // Open C++ file stream for writing
    if (!file.is_open()) { // Check if file opened successfully
        return;
    }

    // Write each single tileArray character into our file stream
    for (int y = 0; y < PCG::MAP_ROWS; y++) {
        for (int x = 0; x < PCG::MAP_COLUMNS; x++) {
            file.put(PCG::TileMap::GetTileChar(tileArray[y][x])); // Write char to C++ file stream
        }

        file.put('\n'); // New line at end of row for C++ file stream
    }

    file.close(); // Close C++ file stream
    printf("Map saved to %s\n", _filename);
}


// ============================================= 
// void PCG_LoadMapData(const char* _filename)
// Load our tilemap data from a text file, using input _filename
// ============================================= 
void PCG::TileMap::LoadMapData(const char* _filename) {
    std::fstream file;  // C++ file stream object for reading
    file.open(_filename, std::ios::in); // Open C++ file stream for reading

    if (!file.is_open()) { // Check if file opened successfully for C++ stream
        return;
    }

    // Get each character from our file stream, and load it into our tileMap array
    for (int y = 0; y < PCG::MAP_ROWS; y++) {
        for (int x = 0; x < PCG::MAP_COLUMNS; x++) {
            int ch = file.get(); // Get char from C++ file stream

            // Skip invisible newline characters
            while (ch == '\n' || ch == '\r') {
                ch = file.get(); // Get char from C++ file stream for skipping newlines
            }

            if (ch == PCG::GRASS_CHAR) {
                tileArray[y][x] = PCG::TileType::TILE_TYPE_GRASS;
            }
            else if (ch == PCG::ROCK_CHAR) {
                tileArray[y][x] = PCG::TileType::TILE_TYPE_ROCK;
            }
        }
    }

    file.close(); // Close C++ file stream
    std::cout << "Map loaded from " << _filename << std::endl; // C++ style print statement
}


// ============================================= 
// void PCG_SaveMapImage(const char* filename)
// Store our tileMap data as a .png image, using the input filename.
// ============================================= 
void PCG::TileMap::SaveMapImage(const char* filename) const {
    Image mapImage = GenImageColor(PCG::MAP_COLUMNS, PCG::MAP_ROWS, BLACK);

    for (int y = 0; y < PCG::MAP_ROWS; y++) {
        for (int x = 0; x < PCG::MAP_COLUMNS; x++) {
            Color c = PCG::TileMap::GetTileColor(tileArray[y][x]);
            ImageDrawPixel(&mapImage, x, y, c);
        }
    }

    if (ExportImage(mapImage, filename)) {
        // printf("Image saved: %s\n", filename); // old C-style print statement
        std::cout << "Image saved: " << filename << std::endl; // C++ style print statements
    }

    UnloadImage(mapImage);
}


// ============================================= 
// void HandleMouseEditing()
// Allows the user to manually paint the map.
// Left mouse button paints rock.
// Right mouse button paints grass.
// ============================================= 
void PCG::TileMap::HandleMouseEditing()
{
    Vector2 mousePos = GetMousePosition();

    // Prevent mouse painting when interacting with the UI panel on the right side.
    if (mousePos.x >= BUTTON_X - 10) {
        return;
    }

    int tileX = (int)(mousePos.x / TILE_SIZE);
    int tileY = (int)(mousePos.y / TILE_SIZE);

    if (tileX >= 0 && tileX < MAP_COLUMNS && tileY >= 0 && tileY < MAP_ROWS) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            SetTile(tileX, tileY, TILE_TYPE_ROCK);
        }

        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
            SetTile(tileX, tileY, TILE_TYPE_GRASS);
        }
    }
}


// ============================================= 
// void PCG_DrawGUI()
// ============================================= 
void PCG::TileMap::DrawGUI() {
    // Reset Button
    if (GuiButton(RESET_BUTTON_BOUNDS, "Reset Map")) {
        //CreateMap();
        // pass in this instances tileArray to our map generator, and call the generate function to fill it with new data.
        if (GetMapGenerator() != nullptr) {
            GetMapGenerator()->Generate(tileArray);
        }
    }

    // Save Data Button
    Rectangle saveRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 70, PCG::BUTTON_WIDTH, PCG::BUTTON_HEIGHT };
    if (GuiButton(saveRect, "Save Map Data")) {
        SaveMapData(MAP_TEXT_FILENAME);
    }

    // Load Data Button
    Rectangle loadRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 140, PCG::BUTTON_WIDTH, PCG::BUTTON_HEIGHT };
    if (GuiButton(loadRect, "Load Map Data")) {
        LoadMapData(MAP_TEXT_FILENAME);
    }

    // Save Image Button
    Rectangle imgRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 210, PCG::BUTTON_WIDTH, PCG::BUTTON_HEIGHT };
    if (GuiButton(imgRect, "Save Map PNG")) {
        SaveMapImage(MAP_IMAGE_FILENAME);
    }

    // Random Generator Button
    Rectangle randomRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 280, PCG::BUTTON_WIDTH, PCG::BUTTON_HEIGHT };
    if (GuiButton(randomRect, "Random Generator")) {
        SetMapGenerator(new PCG::RandomMapGenerator());
        GetMapGenerator()->Generate(tileArray);
    }

    // Noise Generator Button
    Rectangle noiseRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 350, PCG::BUTTON_WIDTH, PCG::BUTTON_HEIGHT };
    if (GuiButton(noiseRect, "Noise Generator")) {
        SetMapGenerator(new PCG::NoiseMapGenerator());
        GetMapGenerator()->Generate(tileArray);
    }

    // Cellular Automata Generator Button
    Rectangle cellularRect = { PCG::BUTTON_X, PCG::BUTTON_Y - 420, PCG::BUTTON_WIDTH, PCG::BUTTON_HEIGHT };
    if (GuiButton(cellularRect, "Cellular Automata")) {
        SetMapGenerator(new PCG::CellularAutomataGenerator());
        GetMapGenerator()->Generate(tileArray);
    }

    // Small instruction text for manual editing.
    DrawText("Left click: Rock", 20, SCREEN_HEIGHT - 50, 20, WHITE);
    DrawText("Right click: Grass", 20, SCREEN_HEIGHT - 25, 20, WHITE);
}


// =============================================
// SetMapGenerator and GetMapGenerator functions for our TileMap class, to allow us to assign a map generator to our tilemap, and retrieve it when we want to generate new maps.
// =============================================
void PCG::TileMap::SetMapGenerator(PCG::MapGenerator* generator) {
    // Delete the previous generator before assigning a new one to avoid leaking memory.
    if (mapGenerator != nullptr) {
        delete mapGenerator;
        mapGenerator = nullptr;
    }

    mapGenerator = generator;
}


// =============================================
// GetMapGenerator returns a pointer to the current map generator assigned to this tilemap, so we can call its Generate function when we want to create new maps.
// =============================================
PCG::MapGenerator* PCG::TileMap::GetMapGenerator() const {
    return mapGenerator;
}


// =============================================
// MapGenerator
// =============================================
// As it is a pure virtual class, we don't need to implement anything here. The derived classes will provide the actual generation logic.


// Derived classes will implement the Generate function to create different types of maps.
// =============================================
// RandomMapGenerator
// =============================================
// Constructor
PCG::RandomMapGenerator::RandomMapGenerator() {
    // nothing to initialize for now, but you could seed a random generator here if you want reproducible maps
}

// Destructor
PCG::RandomMapGenerator::~RandomMapGenerator() {
    // nothing to clean up for now, but if you had allocated resources (like noise generators) you would release them here
}

void PCG::RandomMapGenerator::Generate(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]) {
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            _tileArray[y][x] = (TileType)GetRandomValue(0, TILE_COUNT - 1);
        }
    }
}


// =============================================
// NoiseGenerator
// =============================================
// Constructor
PCG::NoiseMapGenerator::NoiseMapGenerator() {
    // nothing to initialize for now, but you could seed a random noise here if you want reproducible maps
}

// Destructor
PCG::NoiseMapGenerator::~NoiseMapGenerator() {
    // nothing to clean up for now, but if you had allocated resources (like noise generators) you would release them here
}

void PCG::NoiseMapGenerator::Generate(TileType _tileArray[MAP_ROWS][MAP_COLUMNS]) {
    // Random offsets make the map different every time
    int offsetX = GetRandomValue(0, 1000);
    int offsetY = GetRandomValue(0, 1000);
    float scale = 2.5f;

    // Raylib's Perlin Noise function
    Image noiseImg = GenImagePerlinNoise(MAP_COLUMNS, MAP_ROWS, offsetX, offsetY, scale);

    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            // Read the brightness of the noise pixel
            Color col = GetImageColor(noiseImg, x, y);
            float brightness = (col.r + col.g + col.b) / (3.0f * 255.0f);

            // Threshold: Dark spots are Rock, Light spots are Grass
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


// =============================================
// CellularAutomataGenerator
// Creates cave-like maps by first randomising tiles, then smoothing them based on nearby rock tiles.
// =============================================
// Constructor
PCG::CellularAutomataGenerator::CellularAutomataGenerator() {
    // nothing to initialize for now, but this class could later expose parameters such as fill percentage or smoothing passes
}

// Destructor
PCG::CellularAutomataGenerator::~CellularAutomataGenerator() {
    // nothing to clean up for now
}


// =============================================
// CountRockNeighbours
// Counts how many surrounding tiles are rocks.
// Out-of-bounds tiles are treated as rocks to create solid map borders.
// =============================================
int PCG::CellularAutomataGenerator::CountRockNeighbours(TileType _tileArray[MAP_ROWS][MAP_COLUMNS], int x, int y)
{
    int rockCount = 0;

    for (int neighbourY = y - 1; neighbourY <= y + 1; neighbourY++) {
        for (int neighbourX = x - 1; neighbourX <= x + 1; neighbourX++) {
            if (neighbourX == x && neighbourY == y) {
                continue;
            }

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


// =============================================
// Generate
// First creates a noisy random rock/grass map, then repeatedly smooths it.
// More neighbouring rocks means the current tile becomes rock.
// Fewer neighbouring rocks means the current tile becomes grass.
// =============================================
void PCG::CellularAutomataGenerator::Generate(TileType _tileArray[MAP_ROWS][MAP_COLUMNS])
{
    // Initial random fill. A higher number creates more rock-heavy caves.
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLUMNS; x++) {
            int randomValue = GetRandomValue(0, 100);
            _tileArray[y][x] = (randomValue < 45) ? TILE_TYPE_ROCK : TILE_TYPE_GRASS;
        }
    }

    // Smooth the map multiple times to create more natural cave-like shapes.
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

        for (int y = 0; y < MAP_ROWS; y++) {
            for (int x = 0; x < MAP_COLUMNS; x++) {
                _tileArray[y][x] = tempArray[y][x];
            }
        }
    }
}