#ifndef MENU_H
#define MENU_H

#include "raylib.h"

typedef enum {
    OPCAO_MENU_JOGAR = 0,
    OPCAO_MENU_RANKING,
    OPCAO_MENU_SAIR,
    TOTAL_OPCOES_MENU
} OpcaoMenu;

typedef struct {
    int opcaoSelecionada;
    Rectangle botoes[TOTAL_OPCOES_MENU];
} Menu;

void InicializarMenu(Menu *menu);
int AtualizarMenu(Menu *menu);
void DesenharMenu(const Menu *menu);

#endif
