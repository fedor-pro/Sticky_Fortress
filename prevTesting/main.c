// SYSTEM HEAD FILES:
#include <stdio.h>  // for printf
#include <raylib.h> // for graphics
#include <stdlib.h> // for rand() func
#include <time.h>   // for initialise MAX_RAND for rand() func

// MODULES:
#include "./modules/all.h"

#define VERSION "0.0.1"

#define WINDOW_WIDTH 1900
#define WINDOW_HEIGHT 1000
#define CELL_WIDTH 15
#define CELL_HEIGHT 16
#define TARGET_FPS 60
#define TIMER_RESET 50
#define TEXT_BUFFER_SIZE 500

#define DEFAULT_FONT_SIZE 23
#define DEFAULT_FOOD_CHAR "o"
#define DEFAULT_HUMAN_CHAR "&"

#define ENTITIES_LIST_SIZE 25
#define FOOD_ON_MAP 5

void drawGuiPannel(GuiPannel guip) // draws pannel
{
    DrawRectangle(guip.startCoords.x, guip.startCoords.y, guip.canvSizeCoords.x, guip.canvSizeCoords.y, guip.backgroundColor);
}

void drawGuiText(GuiText guitx) // draws text
{
    DrawText(guitx.text, guitx.startCoords.x, guitx.startCoords.y, guitx.fontSize, guitx.fontColor);
}

void drawEntity(Entity ent, int rectSizeX, int rectSizeY) // draw entity(ASCII)
{
    DrawText(ent.charValue, ent.coords.x * rectSizeX + 1.8, ent.coords.y * rectSizeY - 2.5, rectSizeX * 1.5, ent.drawingColor);
}

void drawResource(Resource res, int rectSizeX, int rectSizeY)
{
    DrawText(res.charValue, res.coords.x * rectSizeX + 1.5, res.coords.y * rectSizeY - 7, rectSizeX * 1.8, YELLOW);
}

int main()
{
    //------------------------------------------------------------------------------------------
    int timer = 0;
    int fps = 0;

    char *stringFPS = malloc(TEXT_BUFFER_SIZE);

    srand(time(NULL));

    // Initializng time
    time_t rawTime = time(NULL);
    struct tm *tm = localtime(&rawTime);

    // Initializing log file
    char *sourceLogFilePath = malloc(1024);
    FILE *sourceLogFile;

    initializeLogFile(sourceLogFilePath, &sourceLogFile, tm);

    if (sourceLogFile == NULL)
    {
        printf("Not found %s\n", sourceLogFilePath);
        free(sourceLogFilePath);
        return 1;
    }

    rawLogToFile(sourceLogFile, "------------------------------------------------\n");
    logToFile(sourceLogFile, tm, "PROGRAM STARTED\n");
    rawLogToFile(sourceLogFile, "------------------------------------------------\n");

    int windowSizeX = WINDOW_WIDTH;
    int windowSizeY = WINDOW_HEIGHT;

    int rectSizeX = CELL_WIDTH;
    int rectSizeY = CELL_HEIGHT;

    Coord mapSize = {WINDOW_WIDTH / CELL_WIDTH, WINDOW_HEIGHT / CELL_HEIGHT};

    printf("%s %d,%d\n\n", "Defined window size", windowSizeX, windowSizeY);
    printf("%s %d,%d\n\n", "Defined cell size", rectSizeX, rectSizeY);

    //------------------------------------------------------------------------------------------
    // Define landscape types
    LandscapeType basicLandscape = {LAND_BASIC, "Basic landscape", true, (Color){26, 119, 67, 225}};

    LandscapeType waterLandscape = {LAND_WATER, "Water surface", false, (Color){15, 20, 65, 255}};

    LandscapeType deepWaterLandscape = {LAND_DEEP_WATER, "Deep water", false, (Color){5, 200, 45, 255}};

    LandscapeType mountainsLandscape = {LAND_MOUNTAINS, "Mountains and hills", true, (Color){128, 124, 123, 255}};

    LandscapeType rockLandscape = {LAND_ROCK, "Hard rock landscape", false, (Color){78, 74, 73, 255}};

    World *world = initializeWorld(TEXT_BUFFER_SIZE, mapSize, FOOD_ON_MAP, ENTITIES_LIST_SIZE, basicLandscape);

    logToFile(sourceLogFile, tm, "INITIALIZED WORLD MAP\n");

    // Generating world map
    generateWorldStructures(10, mapSize, world->map, basicLandscape, waterLandscape, deepWaterLandscape, mountainsLandscape, rockLandscape);

    rawLogToFile(sourceLogFile, "\n");

    // Initializing window
    char *windowName = malloc(124);
    sprintf(windowName, "Sticky Fortress %s", VERSION);

    InitWindow(windowSizeX, windowSizeY, windowName);
    SetTargetFPS(TARGET_FPS);

    Image windowIcon = LoadImage("../images/windowIcon.png");

    if (windowIcon.format != PIXELFORMAT_UNCOMPRESSED_R8G8B8A8) // formatting image
    {
        ImageFormat(&windowIcon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    }

    SetWindowIcon(windowIcon);

    logToFile(sourceLogFile, tm, "INITIALIZED WINDOW\n");

    // Initializing GUI

    GuiPannel mouseInfo = {.startCoords.x = 0, .startCoords.y = 0, .canvSizeCoords.x = 280, .canvSizeCoords.y = 150, .backgroundColor = BLACK};

    GuiText mouseCoordsString = {.text = (char *)malloc(TEXT_BUFFER_SIZE), .startCoords.x = mouseInfo.startCoords.x + 15, .startCoords.y = mouseInfo.startCoords.y + 25, .fontSize = DEFAULT_FONT_SIZE, .fontColor = RED};

    GuiPannel selectedCellsInfo = {.startCoords.y = 0, .canvSizeCoords.x = 300, .canvSizeCoords.y = 200, .backgroundColor = BLACK};
    selectedCellsInfo.startCoords.x = windowSizeX - selectedCellsInfo.canvSizeCoords.x;

    GuiText selectedCellsString = {.text = (char *)malloc(TEXT_BUFFER_SIZE), .startCoords.x = selectedCellsInfo.startCoords.x + 15, .startCoords.y = selectedCellsInfo.startCoords.y + 25, .fontSize = DEFAULT_FONT_SIZE, .fontColor = GOLD};

    int selectedCells[5];

    GuiPannel entitiesInfo = {.startCoords.x = 0, .canvSizeCoords.x = 300, .canvSizeCoords.y = 200, .backgroundColor = BLACK};
    entitiesInfo.startCoords.y = windowSizeY - entitiesInfo.canvSizeCoords.y;

    GuiText entitiesNumberString = {.text = (char *)malloc(TEXT_BUFFER_SIZE), .startCoords.x = entitiesInfo.startCoords.x + 20, .startCoords.y = entitiesInfo.startCoords.y + 20, .fontSize = DEFAULT_FONT_SIZE, .fontColor = GREEN};

    // GuiText fpsString = {.text = (char *)malloc(TEXT_BUFFER_SIZE), .startCoords.x = 180, .startCoords.y = 25, .fontSize = DEFAULT_FONT_SIZE, .fontColor = GREEN};

    rawLogToFile(sourceLogFile, "------------------------------------------------\n");
    logToFile(sourceLogFile, tm, "STARTED APP\n");
    rawLogToFile(sourceLogFile, "------------------------------------------------\n");

    while (!WindowShouldClose()) // main
    {
        time(&rawTime);
        tm = localtime(&rawTime); // updating time

        BeginDrawing();

        fps = GetFPS();

        if (!IsKeyDown(KEY_SPACE))
        {
            timer++; // updating timer
            if (timer >= TIMER_RESET)
            {
                timer = 0;

                sprintf(stringFPS, "%d", fps);
                logToFile(sourceLogFile, tm, "Current FPS: ");
                rawLogToFile(sourceLogFile, stringFPS);
                rawLogToFile(sourceLogFile, "\n");
            }
        }

        ClearBackground(BLACK); // clear background

        for (int u = 0; u < 5; u++)
        {
            selectedCells[u] = 0;
        }

        for (int x = 0; x < mapSize.x; x++) // drawing map and update info about selected cells
        {
            for (int y = 0; y < mapSize.y; y++)
            {
                if (world->map[x+mapSize.x*y].isSelected == 1)
                {
                    DrawRectangle(x * rectSizeX, y * rectSizeY, rectSizeX + 1, rectSizeY + 1, GOLD);

                    if (world->map[x+mapSize.x*y].landType.gameId == LAND_BASIC)
                    {
                        selectedCells[0]++;
                    }
                    else if (world->map[x+mapSize.x*y].landType.gameId == LAND_WATER)
                    {
                        selectedCells[1]++;
                    }
                    else if (world->map[x+mapSize.x*y].landType.gameId == LAND_MOUNTAINS)
                    {
                        selectedCells[2]++;
                    }
                    else if (world->map[x+mapSize.x*y].landType.gameId == LAND_ROCK)
                    {
                        selectedCells[3]++;
                    }
                    else if (world->map[x+mapSize.x*y].landType.gameId == LAND_DEEP_WATER)
                    {
                        selectedCells[4]++;
                    }
                }

                DrawRectangle(x * rectSizeX + 1, y * rectSizeY + 1, rectSizeX - 1, rectSizeY - 1, world->map[x+mapSize.x*y].landType.drawColor);
            }
        }

        for (int x = 0; x < FOOD_ON_MAP; x++) // update(uhhh...) and draw resources
        {
            if (world->resources[x].number > 0)
            {
                drawResource(world->resources[x], rectSizeX, rectSizeY);
            }
        }

        for (int x = 0; x < ENTITIES_LIST_SIZE; x++) // update and draw entities
        {
            if (world->entities[x].isAlive == true)
            {
                updateEntity(world, mapSize, &world->entities[x], timer, FOOD_ON_MAP, sourceLogFile, tm);
            }

            drawEntity(world->entities[x], rectSizeX, rectSizeY);
        }

        Vector2 mousePosition = GetMousePosition(); // updating info about mouse position
        int mouseCoordX = (int)mousePosition.x;
        int mouseCoordY = (int)mousePosition.y;

        drawGuiPannel(mouseInfo);

        sprintf(mouseCoordsString.text, "X: %d Y: %d", mouseCoordX, mouseCoordY);
        drawGuiText(mouseCoordsString);
        DrawText("Lmb to select, \nrmb to deselect area.", mouseCoordsString.startCoords.x, mouseCoordsString.startCoords.y + 30, 23, GREEN);

        drawGuiPannel(selectedCellsInfo);

        sprintf(selectedCellsString.text, "Selected: \nbasic landscape: %d; \nwater: %d; \nmountains: %d; \nro\ncks: %d; deep water: %d", selectedCells[0], selectedCells[1], selectedCells[2], selectedCells[3], selectedCells[4]);
        drawGuiText(selectedCellsString);

        drawGuiPannel(entitiesInfo);
        sprintf(entitiesNumberString.text, "Entities on map: %d", ENTITIES_LIST_SIZE);
        drawGuiText(entitiesNumberString);

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) // selecting cells
        {
            int cellX = mouseCoordX / rectSizeX;
            int cellY = mouseCoordY / rectSizeY;
            if (cellX >= 0 && cellX < mapSize.x && cellY >= 0 && cellY < mapSize.y)
            {
                world->map[cellX+mapSize.x*cellY].isSelected = 1;
            }
        }

        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) // deselecting area of cells
        {
            int cellX = mouseCoordX / rectSizeX;
            int cellY = mouseCoordY / rectSizeY;

            for (int x = cellX - 1; x <= cellX + 1; x++)
            {
                for (int y = cellY - 1; y <= cellY + 1; y++)
                {
                    world->map[x+mapSize.x*y].isSelected = 0;
                }
            }
        }

        EndDrawing();
    }

    CloseWindow();

    for (int i = 0; i < ENTITIES_LIST_SIZE; i++)
    {
        if (world->entities[i].gameName != NULL)
        {
            free(world->entities[i].gameName);
        }
    }

    rawLogToFile(sourceLogFile, "------------------------------------------------\n");
    logToFile(sourceLogFile, tm, "APP CORRECTLY CLOSED\n");
    rawLogToFile(sourceLogFile, "------------------------------------------------\n");

    deleteWorld(world);

    free(entitiesNumberString.text);
    free(stringFPS);
    free(mouseCoordsString.text);
    free(selectedCellsString.text);
    free(windowName);

    free(sourceLogFilePath);
    fclose(sourceLogFile);

    return 0;
}