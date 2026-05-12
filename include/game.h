#ifndef GAME_H
#define GAME_H

#include "menu.h"
#include "npc_list.h"
#include "player.h"
#include "ranking.h"

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 540
#define MAX_PLAYER_NAME 32

typedef enum {
    SCREEN_MENU = 0,
    SCREEN_PLAYING,
    SCREEN_RANKING,
    SCREEN_GAME_OVER
} GameScreen;

typedef struct {
    GameScreen currentScreen;
    Player player;
    NpcList npcs;
    Ranking ranking;
    Menu menu;
    float scoreTime;
    int score;
    float npcSpawnTimer;
    float npcSpawnInterval;
    float difficultyTimer;
    float npcBaseSpeed;
    int galoEventActive;
    float galoEventTimer;
    float galoEventCooldown;
    float galoEventDuration;
    Rectangle galoZone;
    float galoZoneSpeed;
    int galoWarningTimer;
    Texture2D tileTexture;
    Texture2D npcTextureA;
    Texture2D npcTextureB;
    Texture2D galoTexture;
    int running;
} Game;

void InitGame(Game *game);
void ResetMatch(Game *game);
void UpdateGame(Game *game);
void DrawGame(const Game *game);
void UnloadGame(Game *game);

#endif
