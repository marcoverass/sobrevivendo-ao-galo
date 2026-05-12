#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

typedef struct {
    Rectangle body;
    float speed;
    Color color;
} Player;

void InitPlayer(Player *player, float startX, float startY);
void UpdatePlayer(Player *player, int screenWidth, int screenHeight);
void DrawPlayer(const Player *player);
void ResetPlayer(Player *player, float startX, float startY);

#endif
