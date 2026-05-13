#include "game.h"
#include "raylib.h"

int main(void) {
    Jogo jogo;

    InitWindow(LARGURA_TELA, ALTURA_TELA, "Sobrevivendo ao Galo");
    SetTargetFPS(60);

    InicializarJogo(&jogo);

    while (!WindowShouldClose() && jogo.executando) {
        AtualizarJogo(&jogo);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DesenharJogo(&jogo);
        EndDrawing();
    }

    EncerrarJogo(&jogo);
    CloseWindow();

    return 0;
}
