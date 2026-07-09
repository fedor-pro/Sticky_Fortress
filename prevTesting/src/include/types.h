#if !defined(TYPES_H)
#define TYPES_H

// ✗✗✗ RESTRICTED ✗✗✗ #define LL() printf("Check at %s : %d passed succesfully\n", __FILE__, __LINE__) ✗✗✗ RESTRICTED ✗✗✗

#include <stdio.h>
#include <stdbool.h>
#include <raylib.h>
#include <time.h>

typedef struct
{
    int x;
    int y;
} Coord;

typedef enum
{
    LAND_BASIC,
    LAND_WATER,
    LAND_DEEP_WATER,
    LAND_MOUNTAINS,
    LAND_ROCK
} LandscapeTypeID;

typedef enum
{
    RIGHT,
    LEFT,
    UP,
    DOWN
} Direction;

typedef enum
{
    UNTARGET_MOVING,
    TARGETING,
    EATING
} EntityMovingState;

typedef struct
{
    Coord startCoords;

    Coord canvSizeCoords;

    Color backgroundColor;
} GuiPannel;

typedef struct
{
    char *text;

    Coord startCoords;

    int fontSize;

    Color fontColor;
} GuiText;

typedef struct
{
    LandscapeTypeID gameId;
    char *gameName;
    bool canBeOccupied;

    Color drawColor;
} LandscapeType;

typedef struct
{
    Coord lCoord;

    bool isSelected;
    bool isOccupied;

    LandscapeType landType;
} LandscapeCell;

typedef struct
{
    char *gameId;
    char *gameName;

    bool dwarvenness;
    bool isAlive;
    char *charValue;

    Coord coords;

    EntityMovingState movingState;
    int targetFoodId;

    Coord targetCellCoords;

    float hunger;
    float dieLevelHunger;
    float sleepiness;

    Color drawingColor;
} Entity;

typedef enum
{
    FOOD
} ItemType;

typedef struct
{
    Coord coords;

    ItemType imType;
    char *charValue;

    float number;
    bool isExist;
} Item;

typedef struct
{
    Coord *incomingCellsCoords;

    LandscapeType landscape;
    LandscapeType secondLandscape;
} LandscapeStructure;

typedef struct
{
    char *worldName;
    FILE *worldFile;

    LandscapeCell *map;
    Coord mapSize;

    Item *items;
    Entity *entities;

    LandscapeType *worldLandscapes;
} World;

typedef struct
{
    GuiPannel *allGuiPannels;
    GuiText *allGuiText;
} UILord;

typedef struct
{
    char *defaultName;

    int startEntitiesNumber;
    int startFoodOnMap;
    int structuresNumber;

    int entitiesAlive;
    int entitiesSelected;
    int foodExists;
} worldParamsDataLord;

typedef struct
{
    Coord windowSize;
    Coord rectSize;
    int textBufferSize;

    
} progParamsDataLord;

typedef struct
{
    int defaultFontSize;

    char* defaultHumanChar;
    char* defaultFoodChar;
} drawDataLord;

typedef struct
{
    struct tm *tm; 
    FILE *sourceLogFile;
    time_t rawTime;
} logDataLord;

// typedef struct
// {
//     worldParamsDataLord worldParamsData;
//     progParamsDataLord progParamsData;
// } mainDataLord;

#endif