#include "uilord.h"
#include "draw.h"
#include <raylib.h>

UILord *initializeUILord (progParamsDataLord *progParamsData, int default_font_size) {
    GuiPannel mouseInfo = {.startCoords.x = 0, .startCoords.y = 0, .canvSizeCoords.x = 280, .canvSizeCoords.y = 150, .backgroundColor = BLACK};

    GuiText mouseCoordsText= {.text = (char *)malloc(progParamsData->textBufferSize), .startCoords.x = mouseInfo.startCoords.x + 15, .startCoords.y = mouseInfo.startCoords.y + 25, .fontSize = default_font_size, .fontColor = RED};

    GuiPannel selectedCellsInfo = {.startCoords.y = 0, .canvSizeCoords.x = 300, .canvSizeCoords.y = 200, .backgroundColor = BLACK};
    selectedCellsInfo.startCoords.x = progParamsData->windowSize.x - selectedCellsInfo.canvSizeCoords.x;

    GuiText selectedCellsText = {.text = (char *)malloc(progParamsData->textBufferSize), .startCoords.x = selectedCellsInfo.startCoords.x + 15, .startCoords.y = selectedCellsInfo.startCoords.y + 25, .fontSize = default_font_size, .fontColor = GOLD};

    int selectedCells[5];

    GuiPannel entitiesInfo = {.startCoords.x = 0, .canvSizeCoords.x = 300, .canvSizeCoords.y = 200, .backgroundColor = BLACK};
    entitiesInfo.startCoords.y = progParamsData->windowSize.y - entitiesInfo.canvSizeCoords.y;

    GuiText entitiesNumberText = {.text = (char *)malloc(progParamsData->textBufferSize), .startCoords.x = entitiesInfo.startCoords.x + 20, .startCoords.y = entitiesInfo.startCoords.y + 20, .fontSize = default_font_size, .fontColor = GREEN};
    GuiText entitiesSelectedText = {.text = (char *)malloc(progParamsData->textBufferSize), .startCoords.x = entitiesInfo.startCoords.x + 20, .startCoords.y = entitiesInfo.startCoords.y + 60, .fontSize = default_font_size, .fontColor = GREEN};

    GuiPannel zeroInfo = {.startCoords.x = progParamsData->windowSize.x - 200, .startCoords.y = progParamsData->windowSize.y- 200, .canvSizeCoords.x = 200, .canvSizeCoords.y = 200, .backgroundColor = BLACK};

    GuiText zeroInfoText = {.text = (char *)malloc(progParamsData->textBufferSize), .startCoords.x = zeroInfo.startCoords.x + 5, .startCoords.y = zeroInfo.startCoords.y + 5, .fontSize = default_font_size-5, .fontColor = RED};

    UILord *UIL = malloc(sizeof(UILord));
    UIL->allGuiPannels = malloc(sizeof(GuiPannel)*10);
    UIL->allGuiText = malloc(sizeof(GuiText)*10);

    UIL->allGuiPannels[0] = mouseInfo;
    UIL->allGuiPannels[1] = selectedCellsInfo;
    UIL->allGuiPannels[2] = entitiesInfo;
    UIL->allGuiPannels[3] = zeroInfo;

    UIL->allGuiText[0] = mouseCoordsText;
    UIL->allGuiText[1] = selectedCellsText;
    UIL->allGuiText[2] = entitiesNumberText;
    UIL->allGuiText[3] = zeroInfoText;
    UIL->allGuiText[4] = entitiesSelectedText;

    return UIL;
}

void deleteUILord (UILord *UIL) {
    free(UIL->allGuiPannels);
    free(UIL->allGuiText);
    free(UIL);
}

void updateUILord (UILord *UIL, Coord mousePosition, int* selectedCells, worldParamsDataLord *worldParamsData, int timer, bool isPaused) {
    sprintf(UIL->allGuiText[0].text, "X: %d Y: %d", mousePosition.x, mousePosition.y);

    sprintf(UIL->allGuiText[1].text, "Selected: \nbasic landscape: %d; \nwater: %d; \nmountains: %d; \nrocks: %d; deep water: %d", selectedCells[0], selectedCells[1], selectedCells[2], selectedCells[3], selectedCells[4]);

    sprintf(UIL->allGuiText[2].text, "Entities alive: %d", worldParamsData->entitiesAlive);
    sprintf(UIL->allGuiText[4].text, "      selected: %d", worldParamsData->entitiesSelected);

    if (isPaused) {
        sprintf(UIL->allGuiText[3].text, "Frame (from 1 to 60) :\n %d\n Is paused :\n true\n\n You can find FPS\n in log files", timer);
    } else {
        sprintf(UIL->allGuiText[3].text, "Frame (from 1 to 60) :\n %d\n Is paused :\n false\n\n You can find FPS\n in log files", timer);
    }
}

void drawUILord (UILord *UIL) {
    for (int x = 0; x < 4; x ++) {
        drawGuiPannel(UIL->allGuiPannels[x]);
    }

    for (int y = 0; y < 5; y ++) {
        drawGuiText(UIL->allGuiText[y]);
    }

    DrawText("Lmb to select, \nrmb to deselect area.", UIL->allGuiText[0].startCoords.x, UIL->allGuiText[0].startCoords.y + 30, 23, GREEN);
}