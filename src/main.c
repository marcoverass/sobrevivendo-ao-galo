#include "game.h"
#include "raylib.h"

int main(void) {
    Game game;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sobrevivendo ao Galo");
    SetTargetFPS(60);

    InitGame(&game);

    while (!WindowShouldClose() && game.running) {
        UpdateGame(&game);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawGame(&game);
        EndDrawing();
    }

    UnloadGame(&game);
    CloseWindow();

    return 0;
}
