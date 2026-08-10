#include <stdio.h>
#include <raylib.h>
#include <stdlib.h> // For rand() func
#include <time.h>   // For timestamp and rand() initialize

#include "entities.h"
#include "logging.h"
#include "uilord.h"
#include "world.h"
#include "datalord.h"
#include "draw.h"
#include "updatelord.h"

#define VERSION "0.0.2"

#define TARGET_FPS 60

#define LOGS_BARRIERS "---------------------------------------------------------------\n"

int main()
{
    SetTraceLogLevel(LOG_NONE); // For delete all raylib's sys logs

    // Creating dataLords
    progParamsDataLord *progParamsData = defineProgParamsDataLord();
    worldParamsDataLord *worldParamsData = defineWorldParamsDataLord();
    drawDataLord *drawData = defineDrawDataLord();
    logDataLord *logData = defineLogDataLord();

    progParamsData->timer = 0;
    progParamsData->currentFPS = 0;
    progParamsData->isPaused = false;

    bool ifSquareSelectingActive = false;
    Coord squareSelectingStartCellCoords;
    int squareSelectingFreeze = 0;

    srand(time(NULL));

    // Initializing log file
    char *sourceLogFilePath = malloc(progParamsData->textBufferSize);

    initializeLogFile(sourceLogFilePath, logData);

    if (logData->sourceLogFile == NULL)
    {
        printf("Not found %s\n", sourceLogFilePath);
        free(sourceLogFilePath);
        return 1;
    }

    rawLogToFile(logData, LOGS_BARRIERS);
    logToFile(logData, "PROGRAM STARTED\n");
    rawLogToFile(logData, LOGS_BARRIERS);

    Coord ms = {progParamsData->windowSize.x / progParamsData->rectSize.x, progParamsData->windowSize.y / progParamsData->rectSize.y};

    // Log start info
    char *initLogInfo = malloc(progParamsData->textBufferSize);
    sprintf(initLogInfo, "%s %d,%d\n\n", "Defined window size", progParamsData->windowSize.x, progParamsData->windowSize.y);
    logToFile(logData, initLogInfo);

    sprintf(initLogInfo, "%s %d,%d\n\n", "Defined cell size", progParamsData->rectSize.x, progParamsData->rectSize.y);
    logToFile(logData, initLogInfo);
    
    sprintf(initLogInfo, "Defined map size %d, %d\n", ms.x, ms.y);
    logToFile(logData, initLogInfo);

    rawLogToFile(logData, LOGS_BARRIERS);

    // Creating world
    World *world = initializeWorld(worldParamsData, progParamsData, LOGS_BARRIERS, ms, logData, drawData);

    // Initializing window
    char *windowName = malloc(progParamsData->textBufferSize);
    sprintf(windowName, "Sticky Fortress %s", VERSION);

    InitWindow(progParamsData->windowSize.x, progParamsData->windowSize.y, windowName);
    SetTargetFPS(TARGET_FPS);

    SetExitKey(KEY_NULL); // If active, window willn't close on ESC button

    Image windowIcon = LoadImage("./images/windowIcon.png"); // Loading icon
    if (windowIcon.format != PIXELFORMAT_UNCOMPRESSED_R8G8B8A8) // Formatting icon
    {
        ImageFormat(&windowIcon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    }
    ImageColorReplace(&windowIcon, WHITE, BLANK);
    SetWindowIcon(windowIcon);

    logToFile(logData, "INITIALIZED WINDOW\n");

    int *selectedCells = malloc(sizeof(int)*5);

    // Initialize main UI
    UILord *UICentral = initializeUILord(progParamsData, drawData->defaultFontSize);

    rawLogToFile(logData,  LOGS_BARRIERS);
    logToFile(logData, "STARTED APP\n");
    rawLogToFile(logData,  LOGS_BARRIERS);

    while (!WindowShouldClose())
    {
        updateGameRunningParams(progParamsData, logData);

        BeginDrawing();

        worldParamsData->entitiesAlive = 0;
        worldParamsData->entitiesSelected = 0;

        if (squareSelectingFreeze > 0) {
            squareSelectingFreeze --;
        }

        ClearBackground(BLACK); // Clear background

        updateGameObjects(world, progParamsData, worldParamsData, logData);

        for (int u = 0; u < 5; u++) // Reset selected landscape cells stats
        {
            selectedCells[u] = 0;
        }

        for (int x = 0; x < world->mapSize.x; x++) // Drawing map and update info about selected cells
        {
            for (int y = 0; y < world->mapSize.y; y++)
            {
                if (world->map[x+world->mapSize.x*y].isSelected == 1) // If cell is selected
                {
                    DrawRectangle(x * progParamsData->rectSize.x, y * progParamsData->rectSize.y, progParamsData->rectSize.x + 1, progParamsData->rectSize.y + 1, GOLD); 

                    // Update selected landscape cells stats
                    if (world->map[x+world->mapSize.x*y].landType.gameId == LAND_BASIC)
                    {
                        selectedCells[0]++;
                    }
                    else if (world->map[x+world->mapSize.x*y].landType.gameId == LAND_WATER)
                    {
                        selectedCells[1]++;
                    }
                    else if (world->map[x+world->mapSize.x*y].landType.gameId == LAND_MOUNTAINS)
                    {
                        selectedCells[2]++;
                    }
                    else if (world->map[x+world->mapSize.x*y].landType.gameId == LAND_ROCK)
                    {
                        selectedCells[3]++;
                    }
                    else if (world->map[x+world->mapSize.x*y].landType.gameId == LAND_DEEP_WATER)
                    {
                        selectedCells[4]++;
                    }
                }

                DrawRectangle(x * progParamsData->rectSize.x + 1, y * progParamsData->rectSize.y + 1, progParamsData->rectSize.x - 1, progParamsData->rectSize.y - 1, world->map[x+world->mapSize.x*y].landType.drawColor);
            }
        }

        for (int x = 0; x < worldParamsData->startFoodOnMap; x++) // Draw items
        {
            if (world->items[x].number > 0)
            {
                drawItem(world->items[x], progParamsData);
            }
        }

        for (int x = 0; x < worldParamsData->startEntitiesNumber; x++) // Draw entities
        {
            drawEntity(world->entities[x], progParamsData);
        }

        Vector2 mp = GetMousePosition(); // Updating info about mouse position
        Coord mousePosition = {(int) mp.x, (int) mp.y};

        updateUILord(UICentral, mousePosition, selectedCells, worldParamsData, progParamsData->timer, progParamsData->isPaused); // Update main UI 
        drawUILord(UICentral); // Draw main UI

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) // Selecting cells
        {
            if (squareSelectingFreeze == 0) {
                ifSquareSelectingActive = !ifSquareSelectingActive;

                if (ifSquareSelectingActive) 
                {
                    deselectAllWorldMap(world);

                    squareSelectingStartCellCoords = mousePosition;
                    world->map[(squareSelectingStartCellCoords.x/progParamsData->rectSize.x) + world->mapSize.x * (squareSelectingStartCellCoords.y/progParamsData->rectSize.y)].isSelected = true;
                } 
                else 
                {
                    // Select square from (c1.x; c1.y) to (c2.x; c2.y)
                    // Go from selecting start to selecting end (mouse position right now)

                    for (int ab = squareSelectingStartCellCoords.x; ab < mousePosition.x; ab ++) 
                    {
                        for (int ord = squareSelectingStartCellCoords.y; ord < mousePosition.y; ord ++) 
                        {
                            world->map[(ab/progParamsData->rectSize.x) + world->mapSize.x * (ord/progParamsData->rectSize.y)].isSelected = true; 

                            if (world->map[(ab/progParamsData->rectSize.x)+world->mapSize.x*(ord/progParamsData->rectSize.y)].landType.gameId == LAND_BASIC)
                            {
                                selectedCells[0]++;
                            }
                            else if (world->map[(ab/progParamsData->rectSize.x)+world->mapSize.x*(ord/progParamsData->rectSize.y)].landType.gameId == LAND_WATER)
                            {
                                selectedCells[1]++;
                            }
                            else if (world->map[(ab/progParamsData->rectSize.x)+world->mapSize.x*(ord/progParamsData->rectSize.y)].landType.gameId == LAND_MOUNTAINS)
                            {
                                selectedCells[2]++;
                            }
                            else if (world->map[(ab/progParamsData->rectSize.x)+world->mapSize.x*(ord/progParamsData->rectSize.y)].landType.gameId == LAND_ROCK)
                            {
                                selectedCells[3]++;
                            }
                            else if (world->map[(ab/progParamsData->rectSize.x)+world->mapSize.x*(ord/progParamsData->rectSize.y)].landType.gameId == LAND_DEEP_WATER)
                            {
                                selectedCells[4]++;
                            }
                        }
                    }
                }

                squareSelectingFreeze = 30;
            }
        } 
        else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) // STUB
        {
            world->map[(mousePosition.x/progParamsData->rectSize.x) + world->mapSize.x * (mousePosition.y/progParamsData->rectSize.y)].isSelected = false;

            world->map[(mousePosition.x/progParamsData->rectSize.x) + 1 + world->mapSize.x * (mousePosition.y/progParamsData->rectSize.y)].isSelected = false;
            world->map[(mousePosition.x/progParamsData->rectSize.x) - 1 + world->mapSize.x * (mousePosition.y/progParamsData->rectSize.y)].isSelected = false;

            world->map[(mousePosition.x/progParamsData->rectSize.x) + world->mapSize.x * (mousePosition.y/progParamsData->rectSize.y + 1)].isSelected = false;
            world->map[(mousePosition.x/progParamsData->rectSize.x) + world->mapSize.x * (mousePosition.y/progParamsData->rectSize.y - 1)].isSelected = false;
        } 
        else if (IsKeyDown(KEY_ESCAPE))
        {
            ifSquareSelectingActive = !ifSquareSelectingActive;
            deselectAllWorldMap(world);
        }

        EndDrawing();
    }

    CloseWindow();

    rawLogToFile(logData, LOGS_BARRIERS);
    logToFile(logData, "APP CORRECTLY CLOSED\n");
    rawLogToFile(logData, LOGS_BARRIERS);

    deleteWorld(world, worldParamsData, logData);
    deleteUILord(UICentral);

    free(windowName);

    free(sourceLogFilePath);
    fclose(logData->sourceLogFile);

    undefineAllDatalords(progParamsData, worldParamsData, drawData, logData);
}