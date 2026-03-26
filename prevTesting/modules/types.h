// STANDALONE MODULE

#pragma once

#include <stdio.h>  // for printf
#include <raylib.h> // for graphics
#include <stdlib.h>
#include <string.h>
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
    int index;

    bool isSelected;
    bool isOccupied;

    LandscapeType landType;
} LandscapeCell;

typedef struct
{
    char *gameId;
    char *gameName;

    bool humanity;
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
} ResourceType;

typedef struct
{
    ResourceType resType;
    char *charValue;

    float number;
    bool isExist;

    Coord coords;
} Resource;

typedef struct
{
    Coord *incomingCellsCoords;

    LandscapeType landscape;
} LandscapeStructure;

typedef struct
{
    LandscapeCell *map;
    Resource *resources;
    Entity *entities;

    Coord mapSize;
} World;
