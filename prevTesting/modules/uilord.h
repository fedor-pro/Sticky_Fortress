#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#pragma once
#include "types.h"
#include "entities.h"
#include "logging.h"
#include "generate.h"
#include "spawn.h"

UILord *initializeUILord (int windowSizeX, int windowSizeY, int text_buffer_size, int default_font_size) {
    GuiPannel mouseInfo = {.startCoords.x = 0, .startCoords.y = 0, .canvSizeCoords.x = 280, .canvSizeCoords.y = 150, .backgroundColor = BLACK};

    GuiText mouseCoordsString = {.text = (char *)malloc(text_buffer_size), .startCoords.x = mouseInfo.startCoords.x + 15, .startCoords.y = mouseInfo.startCoords.y + 25, .fontSize = default_font_size, .fontColor = RED};

    GuiPannel selectedCellsInfo = {.startCoords.y = 0, .canvSizeCoords.x = 300, .canvSizeCoords.y = 200, .backgroundColor = BLACK};
    selectedCellsInfo.startCoords.x = windowSizeX - selectedCellsInfo.canvSizeCoords.x;

    GuiText selectedCellsString = {.text = (char *)malloc(text_buffer_size), .startCoords.x = selectedCellsInfo.startCoords.x + 15, .startCoords.y = selectedCellsInfo.startCoords.y + 25, .fontSize = default_font_size, .fontColor = GOLD};

    int selectedCells[5];

    GuiPannel entitiesInfo = {.startCoords.x = 0, .canvSizeCoords.x = 300, .canvSizeCoords.y = 200, .backgroundColor = BLACK};
    entitiesInfo.startCoords.y = windowSizeY - entitiesInfo.canvSizeCoords.y;

    GuiText entitiesNumberString = {.text = (char *)malloc(text_buffer_size), .startCoords.x = entitiesInfo.startCoords.x + 20, .startCoords.y = entitiesInfo.startCoords.y + 20, .fontSize = default_font_size, .fontColor = GREEN};

    UILord *UIL = malloc(sizeof(UILord));
    UIL->allGuiPannels = malloc(sizeof(GuiPannel)*10);
    UIL->allGuiText = malloc(sizeof(GuiText)*10);

    UIL->allGuiPannels[0] = mouseInfo;
    UIL->allGuiPannels[1] = selectedCellsInfo;
    UIL->allGuiPannels[2] = entitiesInfo;

    UIL->allGuiText[0] = mouseCoordsString;
    UIL->allGuiText[1] = selectedCellsString;
    UIL->allGuiText[2] = entitiesNumberString;

    return UIL;
}

void deleteUILord (UILord *UIL) {
    free(UIL->allGuiPannels);
    free(UIL->allGuiText);
    free(UIL);
}

void updateUILord (UILord *UIL, Coord mousePosition, int* selectedCells, int entitiesAlive) {
    sprintf(UIL->allGuiText[0].text, "X: %d Y: %d", mousePosition.x, mousePosition.y);
    DrawText("Lmb to select, \nrmb to deselect area.", UIL->allGuiText[0].startCoords.x, UIL->allGuiText[0].startCoords.y + 30, 23, GREEN);

    sprintf(UIL->allGuiText[1].text, "Selected: \nbasic landscape: %d; \nwater: %d; \nmountains: %d; \nrocks: %d; deep water: %d", selectedCells[0], selectedCells[1], selectedCells[2], selectedCells[3], selectedCells[4]);

    sprintf(UIL->allGuiText[2].text, "Entities alive: %d", entitiesAlive);
}

void drawUILord (UILord *UIL) {
    for (int x = 0; x < 3; x ++) {
        drawGuiPannel(UIL->allGuiPannels[x]);
    }

    for (int y = 0; y < 3; y ++) {
        drawGuiText(UIL->allGuiText[y]);
    }
}