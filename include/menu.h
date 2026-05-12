#ifndef MENU_H
#define MENU_H

#include "raylib.h"

typedef enum {
    MENU_OPTION_PLAY = 0,
    MENU_OPTION_RANKING,
    MENU_OPTION_EXIT,
    MENU_OPTION_TOTAL
} MenuOption;

typedef struct {
    int selectedOption;
} Menu;

void InitMenu(Menu *menu);
void UpdateMenu(Menu *menu);
void DrawMenu(const Menu *menu);

#endif
