#include "menu.h"

static const char *opcoesMenu[TOTAL_OPCOES_MENU] = {
    "JOGAR",
    "RANKING",
    "SAIR"
};

void InicializarMenu(Menu *menu) {
    int indice;

    menu->opcaoSelecionada = OPCAO_MENU_JOGAR;

    for (indice = 0; indice < TOTAL_OPCOES_MENU; indice++) {
        menu->botoes[indice] = (Rectangle){330.0f, 270.0f + indice * 62.0f, 300.0f, 46.0f};
    }
}

int AtualizarMenu(Menu *menu) {
    int indice;
    Vector2 posicaoMouse = GetMousePosition();

    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        menu->opcaoSelecionada--;
    }

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        menu->opcaoSelecionada++;
    }

    if (menu->opcaoSelecionada < 0) {
        menu->opcaoSelecionada = TOTAL_OPCOES_MENU - 1;
    }

    if (menu->opcaoSelecionada >= TOTAL_OPCOES_MENU) {
        menu->opcaoSelecionada = 0;
    }

    for (indice = 0; indice < TOTAL_OPCOES_MENU; indice++) {
        if (CheckCollisionPointRec(posicaoMouse, menu->botoes[indice])) {
            menu->opcaoSelecionada = indice;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                return indice;
            }
        }
    }

    if (IsKeyPressed(KEY_ENTER)) {
        return menu->opcaoSelecionada;
    }

    return -1;
}

void DesenharMenu(const Menu *menu) {
    int indice;

    DrawText("SOBREVIVENDO AO GALO", 220, 80, 42, MAROON);
    DrawText("Carnaval do Recife em modo sobrevivencia", 230, 132, 22, DARKGRAY);
    DrawText("Use teclado ou mouse para navegar", 300, 178, 20, GRAY);

    for (indice = 0; indice < TOTAL_OPCOES_MENU; indice++) {
        Color corBotao = Fade(LIGHTGRAY, 0.85f);
        Color corTexto = DARKGRAY;
        Rectangle botao = menu->botoes[indice];

        if (indice == menu->opcaoSelecionada) {
            corBotao = (Color){241, 198, 93, 255};
            corTexto = MAROON;
        }

        DrawRectangleRounded(botao, 0.22f, 8, corBotao);
        DrawRectangleRoundedLinesEx(botao, 0.22f, 8, 2.0f, Fade(MAROON, 0.6f));
        DrawText(opcoesMenu[indice], (int)botao.x + 92, (int)botao.y + 10, 28, corTexto);
    }
}
