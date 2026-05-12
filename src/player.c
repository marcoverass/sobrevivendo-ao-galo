#include "player.h"

static Vector2 GetMovementDirection(void) {
    Vector2 direction = {0.0f, 0.0f};

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        direction.y -= 1.0f;
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        direction.y += 1.0f;
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        direction.x -= 1.0f;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        direction.x += 1.0f;
    }

    return direction;
}

void InitPlayer(Player *player, float startX, float startY) {
    player->body = (Rectangle){startX, startY, 28.0f, 28.0f};
    player->speed = 240.0f;
    player->color = BLUE;
}

void UpdatePlayer(Player *player, int screenWidth, int screenHeight) {
    Vector2 direction = GetMovementDirection();
    float deltaTime = GetFrameTime();

    player->body.x += direction.x * player->speed * deltaTime;
    player->body.y += direction.y * player->speed * deltaTime;

    if (player->body.x < 0) {
        player->body.x = 0;
    }
    if (player->body.y < 0) {
        player->body.y = 0;
    }
    if (player->body.x + player->body.width > screenWidth) {
        player->body.x = screenWidth - player->body.width;
    }
    if (player->body.y + player->body.height > screenHeight) {
        player->body.y = screenHeight - player->body.height;
    }
}

void DrawPlayer(const Player *player) {
    DrawRectangleRec(player->body, player->color);
    DrawText("VOCE", (int)player->body.x - 4, (int)player->body.y - 18, 14, DARKBLUE);
}

void ResetPlayer(Player *player, float startX, float startY) {
    player->body.x = startX;
    player->body.y = startY;
}
