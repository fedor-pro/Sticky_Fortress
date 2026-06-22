#include <stdio.h>  // for printf
#include <raylib.h> // for graphics
#include <stdlib.h> // for rand() func
#include <time.h>   // for timestamp and rand() initialize

#include "entities.h"
#include "logging.h"
#include "uilord.h"
#include "world.h"
#include "datalord.h"
#include "draw.h"

#define VERSION "0.0.2"

#define TARGET_FPS 60
#define TIMER_RESET 60

#define LOGS_BARRIERS "---------------------------------------------------------------\n"

int main()
{
    SetTraceLogLevel(LOG_NONE); // for delete all raylib's sys logs

    //------------------------------------------------------------------------------------------
    // Creating dataLords
    progParamsDataLord* progParamsData = defineProgParamsDataLord();
    worldParamsDataLord* worldParamsData = defineWorldParamsDataLord();
    drawDataLord* drawData = defineDrawDataLord();
    logDataLord* logData = defineLogDataLord();

    bool isPaused = false;
    int timer = 0; // it's only for update
    int fps = 0;

    bool ifSquareSelectingActive = false;
    Coord squareSelectingStartCellCoords;
    int squareSelectingFreeze = 0;

    char *stringFPS = malloc(progParamsData->textBufferSize);

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

    //----------------------------------------------------------------------------------------------------------------------------------
    // Creating world
    World *world = initializeWorld(worldParamsData, progParamsData, LOGS_BARRIERS, ms, logData, drawData);

    // Initializing window
    char *windowName = malloc(progParamsData->textBufferSize);
    sprintf(windowName, "Sticky Fortress %s", VERSION);

    InitWindow(progParamsData->windowSize.x, progParamsData->windowSize.y, windowName);
    SetTargetFPS(TARGET_FPS);

    // SetExitKey(KEY_NULL); // Window willn't close on ESC button

    Image windowIcon = LoadImage("./images/windowIcon.png"); // loading icon

    if (windowIcon.format != PIXELFORMAT_UNCOMPRESSED_R8G8B8A8) // formatting icon
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

    while (!WindowShouldClose()) // main
    {
        time(&logData->rawTime);
        logData->tm = localtime(&logData->rawTime); // updating time

        BeginDrawing();

        fps = GetFPS();

        worldParamsData->entitiesAlive = 0;
        worldParamsData->entitiesSelected = 0;

        if (squareSelectingFreeze > 0)
        {
           squareSelectingFreeze --;
        }

        // pause
        if (IsKeyPressed(KEY_SPACE)) 
        {
            isPaused = !isPaused;
        }

        if (!isPaused)
        {
            timer++; // updating timer
        }

        for (int x = 0; x < worldParamsData->startEntitiesNumber; x++) // update entities
            {
                if (world->entities[x].isAlive == true)
                {
                    worldParamsData->entitiesAlive ++;

                    if (!isPaused) {
                        updateEntity(world, world->mapSize, &world->entities[x], timer, worldParamsData, logData);
                    }
                }

                if (world->map[world->entities[x].coords.x + ms.x * world->entities[x].coords.y].isSelected) {
                    worldParamsData->entitiesSelected ++;
                } 
        }

        if (!isPaused && timer >= TIMER_RESET)
        {
            timer = 0;
            
            for (int u = 0; u < worldParamsData->startFoodOnMap; u ++) {
                if ((rand() % 100) > 95) {
                    world->items[u].number ++;

                    char* si = malloc(sizeof(char)*12);
                    sprintf(si, "%d", u);

                    logToFile(logData, "Food with id |");
                    rawLogToFile(logData, si);
                    rawLogToFile(logData, "| restored\n");
                }
            }

            sprintf(stringFPS, "%d", fps);
            logToFile(logData, "Current FPS: ");
            rawLogToFile(logData, stringFPS);
            rawLogToFile(logData, "\n");
        }

        ClearBackground(BLACK); // clear background

        for (int u = 0; u < 5; u++) // reset selected landscape cells stats
        {
            selectedCells[u] = 0;
        }

        for (int x = 0; x < world->mapSize.x; x++) // drawing map and update info about selected cells
        {
            for (int y = 0; y < world->mapSize.y; y++)
            {
                if (world->map[x+world->mapSize.x*y].isSelected == 1) // if cell is selected
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

        for (int x = 0; x < worldParamsData->startFoodOnMap; x++) // draw items
        {
            if (world->items[x].number > 0)
            {
                drawItem(world->items[x], progParamsData);
            }
        }

        for (int x = 0; x < worldParamsData->startEntitiesNumber; x++) // draw entities
        {
            drawEntity(world->entities[x], progParamsData);
        }

        Vector2 mp = GetMousePosition(); // updating info about mouse position
        Coord mousePosition = {(int) mp.x, (int) mp.y};

        updateUILord(UICentral, mousePosition, selectedCells, worldParamsData, timer, isPaused); // update main UI 
        drawUILord(UICentral); // draw main UI

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) // selecting cells
        {
            if (squareSelectingFreeze == 0) {
                ifSquareSelectingActive = !ifSquareSelectingActive;

                if (ifSquareSelectingActive == true) 
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
                        }
                    }
                }

                squareSelectingFreeze = 30;
            }
        } 
        else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))  // Stub for deselecting
        {
            world->map[(mousePosition.x/progParamsData->rectSize.x) + world->mapSize.x * (mousePosition.y/progParamsData->rectSize.y)].isSelected = false;

            world->map[(mousePosition.x/progParamsData->rectSize.x) + 1 + world->mapSize.x * (mousePosition.y/progParamsData->rectSize.y)].isSelected = false;
            world->map[(mousePosition.x/progParamsData->rectSize.x) - 1 + world->mapSize.x * (mousePosition.y/progParamsData->rectSize.y)].isSelected = false;

            world->map[(mousePosition.x/progParamsData->rectSize.x) + world->mapSize.x * (mousePosition.y/progParamsData->rectSize.y + 1)].isSelected = false;
            world->map[(mousePosition.x/progParamsData->rectSize.x) + world->mapSize.x * (mousePosition.y/progParamsData->rectSize.y - 1)].isSelected = false;
        } 
        else if (IsKeyDown(KEY_ESCAPE))
        {
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

    free(stringFPS);
    free(windowName);

    free(sourceLogFilePath);
    fclose(logData->sourceLogFile);
}