// SYSTEM HEAD FILES:
#include <stdio.h>  // for printf
#include <raylib.h> // for graphics
#include <stdlib.h> // for rand() func
#include <time.h>   // for timestamp and rand() initialize

// MODULES:
#include "./modules/all.h"

#define VERSION "0.0.1"

#define WINDOW_WIDTH 1900
#define WINDOW_HEIGHT 1000
#define CELL_WIDTH 11
#define CELL_HEIGHT 12
#define TARGET_FPS 60
#define TIMER_RESET 60
#define TEXT_BUFFER_SIZE 500

#define LOGS_BARRIERS "---------------------------------------------------------------\n"

#define DEFAULT_FONT_SIZE 23
#define DEFAULT_FOOD_CHAR "*"
#define DEFAULT_HUMAN_CHAR "&"

#define ENTITIES_LIST_SIZE 40
#define FOOD_ON_MAP 10


int main()
{
    SetTraceLogLevel(LOG_NONE); // for delete all raylib's sys logs

    //------------------------------------------------------------------------------------------
    bool isPaused = false;
    int timer = 0;
    int fps = 0;

    int entitiesAlive = 0;
    int entitiesSelected = 0;

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

    rawLogToFile(sourceLogFile, LOGS_BARRIERS);
    logToFile(sourceLogFile, tm, "PROGRAM STARTED\n");
    rawLogToFile(sourceLogFile, LOGS_BARRIERS);

    // Window settings
    int windowSizeX = WINDOW_WIDTH;
    int windowSizeY = WINDOW_HEIGHT;

    int rectSizeX = CELL_WIDTH;
    int rectSizeY = CELL_HEIGHT;

    Coord ms = {WINDOW_WIDTH / CELL_WIDTH, WINDOW_HEIGHT / CELL_HEIGHT};

    // Log start info
    char *initLogInfo = malloc(sizeof(char) * 1024);
    sprintf(initLogInfo, "%s %d,%d\n\n", "Defined window size", windowSizeX, windowSizeY);
    logToFile(sourceLogFile, tm, initLogInfo);

    sprintf(initLogInfo, "%s %d,%d\n\n", "Defined cell size", rectSizeX, rectSizeY);
    logToFile(sourceLogFile, tm, initLogInfo);
    
    sprintf(initLogInfo, "Defined map size %d, %d\n", ms.x, ms.y);
    logToFile(sourceLogFile, tm, initLogInfo);

    rawLogToFile(sourceLogFile, LOGS_BARRIERS);

    //----------------------------------------------------------------------------------------------------------------------------------
    // Creating world
    World *world = initializeWorld(30, TEXT_BUFFER_SIZE, LOGS_BARRIERS, ms, FOOD_ON_MAP, ENTITIES_LIST_SIZE, tm, sourceLogFile, rawTime);

    // Initializing window
    char *windowName = malloc(124);
    sprintf(windowName, "Sticky Fortress %s", VERSION);

    InitWindow(windowSizeX, windowSizeY, windowName);
    SetTargetFPS(TARGET_FPS);

    Image windowIcon = LoadImage("./images/windowIcon.png");

    if (windowIcon.format != PIXELFORMAT_UNCOMPRESSED_R8G8B8A8) // formatting image
    {
        ImageFormat(&windowIcon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    }

    SetWindowIcon(windowIcon);

    logToFile(sourceLogFile, tm, "INITIALIZED WINDOW\n");

    int *selectedCells = malloc(sizeof(int)*5);
    // Initialize main UI
    UILord *UICentral = initializeUILord(windowSizeX, windowSizeY, TEXT_BUFFER_SIZE, DEFAULT_FONT_SIZE);

    rawLogToFile(sourceLogFile,  LOGS_BARRIERS);
    logToFile(sourceLogFile, tm, "STARTED APP\n");
    rawLogToFile(sourceLogFile,  LOGS_BARRIERS);

    while (!WindowShouldClose()) // main
    {
        time(&rawTime);
        tm = localtime(&rawTime); // updating time

        BeginDrawing();

        fps = GetFPS();

        if (IsKeyPressed(KEY_SPACE)) isPaused = !isPaused;

        if (!IsKeyDown(KEY_SPACE))
        {
            timer++; // updating timer

            entitiesAlive = 0;
            entitiesSelected = 0;

            for (int x = 0; x < ENTITIES_LIST_SIZE; x++) // update entities
            {
                if (world->entities[x].isAlive == true)
                {
                    entitiesAlive ++;
                    updateEntity(world, world->mapSize, &world->entities[x], timer, FOOD_ON_MAP, sourceLogFile, tm);
                }

                if (world->map[world->entities[x].coords.x + ms.x * world->entities[x].coords.y].isSelected) {
                    entitiesSelected ++;
                }
            }

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

        for (int x = 0; x < world->mapSize.x; x++) // drawing map and update info about selected cells
        {
            for (int y = 0; y < world->mapSize.y; y++)
            {
                if (world->map[x+world->mapSize.x*y].isSelected == 1)
                {
                    DrawRectangle(x * rectSizeX, y * rectSizeY, rectSizeX + 1, rectSizeY + 1, GOLD);

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

                DrawRectangle(x * rectSizeX + 1, y * rectSizeY + 1, rectSizeX - 1, rectSizeY - 1, world->map[x+world->mapSize.x*y].landType.drawColor);
            }
        }

        for (int x = 0; x < FOOD_ON_MAP; x++) // update and draw items
        {
            if (world->items[x].number > 0)
            {
                drawItem(world->items[x], rectSizeX, rectSizeY);
            }
        }

        for (int x = 0; x < ENTITIES_LIST_SIZE; x++) // draw entities
        {
            drawEntity(world->entities[x], rectSizeX, rectSizeY);
        }

        Vector2 mp = GetMousePosition(); // updating info about mouse position
        Coord mousePosition = {(int) mp.x, (int) mp.y};

        updateUILord(UICentral, mousePosition, selectedCells, entitiesAlive); // update main UI 
        drawUILord(UICentral); // draw main UI

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) // selecting cells
        {
            int cellX = mousePosition.x / rectSizeX;
            int cellY = mousePosition.y / rectSizeY;
            if (cellX >= 0 && cellX < world->mapSize.x && cellY >= 0 && cellY < world->mapSize.y)
            {
                world->map[cellX+world->mapSize.x*cellY].isSelected = 1;
            }
        }

        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) // deselecting area of cells
        {
            int cellX = mousePosition.x / rectSizeX;
            int cellY = mousePosition.y / rectSizeY;

            for (int x = cellX - 1; x <= cellX + 1; x++)
            {
                for (int y = cellY - 1; y <= cellY + 1; y++)
                {
                    if (cellX - 1 >= 0 && cellX + 1 < world->mapSize.x && cellY - 1 >= 0 && cellY + 1 < world->mapSize.y) {
                        world->map[x+world->mapSize.x*y].isSelected = 0;
                    }
                }
            }
        }

        EndDrawing();
    }

    CloseWindow();

    rawLogToFile(sourceLogFile, LOGS_BARRIERS);
    logToFile(sourceLogFile, tm, "APP CORRECTLY CLOSED\n");
    rawLogToFile(sourceLogFile, LOGS_BARRIERS);

    deleteWorld(world, ENTITIES_LIST_SIZE);
    deleteUILord(UICentral);

    free(stringFPS);
    free(windowName);

    free(sourceLogFilePath);
    fclose(sourceLogFile);

    return 0;
}