#include "uilord.h"
#include "draw.h"
#include <raylib.h>

UILord *initializeUILord (progParamsDataLord *progParamsData, int default_font_size) {
    Color UIPannelsBack = (Color) {0, 0, 0, 185};

    GuiPannel mouseInfo = {.startCoords.x = 0, .startCoords.y = 0, .canvSizeCoords.x = 280, .canvSizeCoords.y = 150, .backgroundColor = UIPannelsBack};

    GuiText mouseCoordsText= {.text = (char *)malloc(progParamsData->textBufferSize), .startCoords.x = mouseInfo.startCoords.x + 15, .startCoords.y = mouseInfo.startCoords.y + 25, .fontSize = default_font_size, .fontColor = RED};

    GuiPannel selectedCellsInfo = {.startCoords.y = 0, .canvSizeCoords.x = 300, .canvSizeCoords.y = 210, .backgroundColor =  UIPannelsBack};
    selectedCellsInfo.startCoords.x = progParamsData->windowSize.x - selectedCellsInfo.canvSizeCoords.x;

    GuiText selectedCellsText = {.text = (char *)malloc(progParamsData->textBufferSize), .startCoords.x = selectedCellsInfo.startCoords.x + 15, .startCoords.y = selectedCellsInfo.startCoords.y + 25, .fontSize = default_font_size, .fontColor = GOLD};
    GuiText worldSaveText = {.text = (char *)malloc(progParamsData->textBufferSize), .startCoords.x = selectedCellsInfo.startCoords.x + 15, .startCoords.y = selectedCellsText.startCoords.y  + 140, .fontSize = default_font_size-5, .fontColor = RED};

    GuiPannel entitiesInfo = {.startCoords.x = 0, .canvSizeCoords.x = 300, .canvSizeCoords.y = 200, .backgroundColor =  UIPannelsBack};
    entitiesInfo.startCoords.y = progParamsData->windowSize.y - entitiesInfo.canvSizeCoords.y;

    GuiText entitiesNumberText = {.text = (char *)malloc(progParamsData->textBufferSize), .startCoords.x = entitiesInfo.startCoords.x + 5, .startCoords.y = entitiesInfo.startCoords.y + 25, .fontSize = default_font_size, .fontColor = GREEN};
    GuiText entitiesSelectedText = {.text = (char *)malloc(progParamsData->textBufferSize), .startCoords.x = entitiesInfo.startCoords.x + 5, .startCoords.y = entitiesInfo.startCoords.y + 60, .fontSize = default_font_size, .fontColor = GREEN};

    GuiPannel frameInfo = {.startCoords.x = progParamsData->windowSize.x - 200, .startCoords.y = progParamsData->windowSize.y- 200, .canvSizeCoords.x = 200, .canvSizeCoords.y = 200, .backgroundColor =  UIPannelsBack};

    GuiText frameInfoText = {.text = (char *)malloc(progParamsData->textBufferSize), .startCoords.x = frameInfo.startCoords.x + 5, .startCoords.y = frameInfo.startCoords.y + 25, .fontSize = default_font_size-5, .fontColor = RED};

    UILord *UIL = malloc(sizeof(UILord));
    UIL->allGuiPannels = malloc(sizeof(GuiPannel)*10);
    UIL->allGuiText = malloc(sizeof(GuiText)*10);

    UIL->allGuiPannels[0] = mouseInfo;
    UIL->allGuiPannels[1] = selectedCellsInfo;
    UIL->allGuiPannels[2] = entitiesInfo;
    UIL->allGuiPannels[3] = frameInfo;

    UIL->allGuiText[0] = mouseCoordsText;
    UIL->allGuiText[1] = selectedCellsText;
    UIL->allGuiText[2] = entitiesNumberText;
    UIL->allGuiText[3] = frameInfoText;
    UIL->allGuiText[4] = entitiesSelectedText;
    UIL->allGuiText[5] = worldSaveText;

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
    sprintf(UIL->allGuiText[5].text, "You cand find world map \nin prevTesting/worlds/");

    sprintf(UIL->allGuiText[2].text, "Entities alive: %d / %d", worldParamsData->entitiesAlive, worldParamsData->startEntitiesNumber);
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

    for (int y = 0; y < 6; y ++) {
        drawGuiText(UIL->allGuiText[y]);
    }

    DrawText("Lmb to select square, \nrmb to deselect area.", UIL->allGuiText[0].startCoords.x, UIL->allGuiText[0].startCoords.y + 30, 23, GREEN);
}