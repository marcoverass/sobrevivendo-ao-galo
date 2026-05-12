#include "menu.h"

static const char *menuOptions[MENU_OPTION_TOTAL] = {
    "JOGAR",
    "RANKING",
    "SAIR"
};

void InitMenu(Menu *menu) {
    menu->selectedOption = MENU_OPTION_PLAY;
}

void UpdateMenu(Menu *menu) {
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        menu->selectedOption--;
    }

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        menu->selectedOption++;
    }

    if (menu->selectedOption < 0) {
        menu->selectedOption = MENU_OPTION_TOTAL - 1;
    }

    if (menu->selectedOption >= MENU_OPTION_TOTAL) {
        menu->selectedOption = 0;
    }
}

void DrawMenu(const Menu *menu) {
    int optionIndex;

    DrawText("SOBREVIVENDO AO GALO", 230, 90, 40, MAROON);
    DrawText("Carnaval do Recife em modo sobrevivencia", 235, 140, 20, DARKGRAY);
    DrawText("Use W/S ou setas para navegar", 320, 190, 20, GRAY);
    DrawText("Enter para confirmar", 360, 220, 20, GRAY);

    for (optionIndex = 0; optionIndex < MENU_OPTION_TOTAL; optionIndex++) {
        Color optionColor = DARKGRAY;

        if (optionIndex == menu->selectedOption) {
            optionColor = RED;
        }

        DrawText(menuOptions[optionIndex], 415, 300 + optionIndex * 50, 30, optionColor);
    }
}
