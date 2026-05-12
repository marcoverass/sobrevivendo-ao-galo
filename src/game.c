#include "game.h"

#define RANKING_FILE "ranking.txt"
#define GALO_WARNING_FRAMES 180
#define TILE_SIZE 64

static Texture2D LoadTextureSafe(const char *fileName) {
    if (FileExists(fileName)) {
        return LoadTexture(fileName);
    }

    return (Texture2D){0};
}

static void DrawConfettiRow(int startY) {
    int x;

    for (x = 20; x < SCREEN_WIDTH; x += 70) {
        DrawRectangle(x, startY + 10, 10, 6, RED);
        DrawRectangle(x + 18, startY + 22, 8, 6, BLUE);
        DrawRectangle(x + 36, startY + 12, 9, 6, GOLD);
        DrawRectangle(x + 52, startY + 26, 8, 6, GREEN);
    }
}

static void DrawGaloEventShape(const Game *game) {
    Rectangle base = {
        game->galoZone.x + 8.0f,
        game->galoZone.y + game->galoZone.height - 18.0f,
        game->galoZone.width - 16.0f,
        18.0f
    };
    Vector2 body = {game->galoZone.x + 104.0f, game->galoZone.y + 54.0f};
    Vector2 chest = {game->galoZone.x + 124.0f, game->galoZone.y + 56.0f};
    Vector2 head = {game->galoZone.x + 165.0f, game->galoZone.y + 26.0f};
    Vector2 tailBase = {game->galoZone.x + 58.0f, game->galoZone.y + 42.0f};

    DrawRectangleRounded(base, 0.3f, 8, (Color){196, 34, 49, 255});
    DrawRectangle((int)base.x + 18, (int)base.y - 8, 18, 8, GOLD);
    DrawRectangle((int)base.x + 84, (int)base.y - 8, 18, 8, GOLD);
    DrawRectangle((int)base.x + 148, (int)base.y - 8, 18, 8, GOLD);

    DrawTriangle(
        (Vector2){tailBase.x - 30.0f, tailBase.y + 4.0f},
        (Vector2){tailBase.x - 8.0f, tailBase.y - 42.0f},
        (Vector2){tailBase.x + 8.0f, tailBase.y + 2.0f},
        BLUE
    );
    DrawTriangle(
        (Vector2){tailBase.x - 20.0f, tailBase.y + 10.0f},
        (Vector2){tailBase.x + 4.0f, tailBase.y - 36.0f},
        (Vector2){tailBase.x + 18.0f, tailBase.y + 12.0f},
        GREEN
    );
    DrawTriangle(
        (Vector2){tailBase.x - 10.0f, tailBase.y + 14.0f},
        (Vector2){tailBase.x + 20.0f, tailBase.y - 30.0f},
        (Vector2){tailBase.x + 28.0f, tailBase.y + 18.0f},
        ORANGE
    );

    DrawEllipse((int)body.x, (int)body.y, 44.0f, 28.0f, BLACK);
    DrawEllipse((int)chest.x, (int)chest.y, 24.0f, 22.0f, (Color){35, 35, 35, 255});
    DrawRectangle((int)body.x + 24, (int)body.y - 24, 16, 30, BLACK);
    DrawCircleV(head, 15.0f, BLACK);

    DrawLineEx((Vector2){body.x + 6.0f, body.y + 20.0f}, (Vector2){body.x + 2.0f, body.y + 44.0f}, 4.0f, ORANGE);
    DrawLineEx((Vector2){body.x + 18.0f, body.y + 20.0f}, (Vector2){body.x + 20.0f, body.y + 45.0f}, 4.0f, ORANGE);
    DrawLineEx((Vector2){body.x + 1.0f, body.y + 44.0f}, (Vector2){body.x - 7.0f, body.y + 50.0f}, 2.0f, ORANGE);
    DrawLineEx((Vector2){body.x + 1.0f, body.y + 44.0f}, (Vector2){body.x + 7.0f, body.y + 50.0f}, 2.0f, ORANGE);
    DrawLineEx((Vector2){body.x + 20.0f, body.y + 45.0f}, (Vector2){body.x + 12.0f, body.y + 51.0f}, 2.0f, ORANGE);
    DrawLineEx((Vector2){body.x + 20.0f, body.y + 45.0f}, (Vector2){body.x + 26.0f, body.y + 51.0f}, 2.0f, ORANGE);

    DrawTriangle(
        (Vector2){head.x + 12.0f, head.y},
        (Vector2){head.x + 31.0f, head.y - 5.0f},
        (Vector2){head.x + 12.0f, head.y + 8.0f},
        GOLD
    );
    DrawCircle(head.x + 4.0f, head.y - 4.0f, 3.0f, RAYWHITE);
    DrawCircle(head.x + 4.0f, head.y - 4.0f, 1.4f, BLACK);
    DrawTriangle(
        (Vector2){head.x - 10.0f, head.y - 12.0f},
        (Vector2){head.x - 2.0f, head.y - 28.0f},
        (Vector2){head.x + 6.0f, head.y - 12.0f},
        RED
    );
    DrawTriangle(
        (Vector2){head.x - 1.0f, head.y - 14.0f},
        (Vector2){head.x + 9.0f, head.y - 31.0f},
        (Vector2){head.x + 14.0f, head.y - 12.0f},
        RED
    );
    DrawCircle(head.x + 2.0f, head.y + 10.0f, 4.5f, RED);
    DrawCircle(head.x + 8.0f, head.y + 13.0f, 4.0f, RED);

    DrawText("GALO", (int)game->galoZone.x + 72, (int)game->galoZone.y + 72, 22, MAROON);
}

static void StartGaloEvent(Game *game) {
    game->galoEventActive = 1;
    game->galoEventTimer = 0.0f;
    game->galoEventCooldown = 0.0f;
    game->galoWarningTimer = GALO_WARNING_FRAMES;
    game->galoZone.width = 210.0f;
    game->galoZone.height = 92.0f;
    game->galoZone.x = -game->galoZone.width;
    game->galoZone.y = (float)GetRandomValue(150, SCREEN_HEIGHT - 170);
}

static void UpdateGaloEvent(Game *game, float deltaTime) {
    if (!game->galoEventActive) {
        game->galoEventCooldown += deltaTime;

        if (game->galoEventCooldown >= 14.0f) {
            StartGaloEvent(game);
        }

        return;
    }

    game->galoEventTimer += deltaTime;
    game->galoZone.x += game->galoZoneSpeed * deltaTime;

    if (game->galoEventTimer >= game->galoEventDuration ||
        game->galoZone.x > SCREEN_WIDTH + 20.0f) {
        game->galoEventActive = 0;
        game->galoEventTimer = 0.0f;
        game->galoZone.x = -game->galoZone.width;
    }
}

static int CheckGaloCollision(const Game *game) {
    if (!game->galoEventActive) {
        return 0;
    }

    return CheckCollisionRecs(game->player.body, game->galoZone);
}

static void UpdatePlaying(Game *game) {
    float deltaTime = GetFrameTime();
    float currentSpawnInterval = game->npcSpawnInterval;
    float currentNpcSpeed = game->npcBaseSpeed;
    int crowdLevel = 1 + (int)(game->scoreTime / 18.0f);

    UpdatePlayer(&game->player, SCREEN_WIDTH, SCREEN_HEIGHT);
    UpdateGaloEvent(game, deltaTime);

    game->scoreTime += deltaTime;
    game->score = (int)(game->scoreTime * 10.0f);

    game->npcSpawnTimer += deltaTime;
    game->difficultyTimer += deltaTime;

    if (game->difficultyTimer >= 8.0f) {
        game->difficultyTimer = 0.0f;
        game->npcBaseSpeed += 20.0f;

        if (game->npcSpawnInterval > 0.32f) {
            game->npcSpawnInterval -= 0.06f;
        }
    }

    if (game->galoEventActive) {
        currentSpawnInterval *= 0.55f;
        currentNpcSpeed += 80.0f;
        crowdLevel += 1;
    }

    if (game->npcSpawnTimer >= currentSpawnInterval) {
        game->npcSpawnTimer = 0.0f;
        if (crowdLevel > 5) {
            crowdLevel = 5;
        }

        SpawnRandomNpc(&game->npcs, SCREEN_WIDTH, SCREEN_HEIGHT, currentNpcSpeed, crowdLevel);
    }

    UpdateNpcs(&game->npcs, deltaTime);
    RemoveOffscreenNpcs(&game->npcs, SCREEN_WIDTH, SCREEN_HEIGHT);

    if (game->galoWarningTimer > 0) {
        game->galoWarningTimer--;
    }

    if (CheckPlayerNpcCollision(&game->npcs, game->player.body) ||
        CheckGaloCollision(game)) {
        AddScoreToRanking(&game->ranking, game->score);
        SaveRanking(&game->ranking, RANKING_FILE);
        game->currentScreen = SCREEN_GAME_OVER;
    }
}

static void DrawHud(const Game *game) {
    DrawRectangle(10, 10, 170, 70, Fade(WHITE, 0.85f));
    DrawText(TextFormat("Tempo: %.1f", game->scoreTime), 20, 20, 20, BLACK);
    DrawText(TextFormat("Pontuacao: %d", game->score), 20, 45, 20, BLACK);
    DrawText(TextFormat("Multidao: %d", game->npcs.count), 20, 68, 18, DARKGRAY);

    if (game->galoEventActive) {
        DrawRectangle(700, 10, 240, 70, Fade(GOLD, 0.85f));
        DrawText("PASSAGEM DO GALO!", 715, 20, 22, MAROON);
        DrawText("Saia da faixa!", 760, 48, 20, BLACK);
    }
}

static void DrawBackgroundDetails(const Game *game) {
    int x;

    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){244, 223, 170, 255});
    DrawRectangle(0, 0, SCREEN_WIDTH, 92, (Color){226, 205, 156, 255});
    DrawRectangle(0, SCREEN_HEIGHT - 92, SCREEN_WIDTH, 92, (Color){226, 205, 156, 255});
    DrawRectangle(0, 92, SCREEN_WIDTH, SCREEN_HEIGHT - 184, (Color){124, 132, 138, 255});

    for (x = 0; x < SCREEN_WIDTH; x += 90) {
        DrawRectangle(x + 22, 244, 46, 12, (Color){247, 237, 170, 255});
        DrawRectangle(x + 22, 284, 46, 12, (Color){247, 237, 170, 255});
        DrawRectangle(x + 22, 324, 46, 12, (Color){247, 237, 170, 255});
    }

    DrawRectangle(0, 92, SCREEN_WIDTH, 12, (Color){180, 160, 116, 255});
    DrawRectangle(0, SCREEN_HEIGHT - 104, SCREEN_WIDTH, 12, (Color){180, 160, 116, 255});
    DrawConfettiRow(10);
    DrawConfettiRow(SCREEN_HEIGHT - 70);

    DrawText("Avenida do Galo da Madrugada", 285, 18, 30, MAROON);
    DrawText("Sobreviva a multidao do Carnaval do Recife", 240, 52, 20, DARKBROWN);

    if (game->galoEventActive) {
        DrawRectangle((int)game->galoZone.x - 12, (int)game->galoZone.y + 30, (int)game->galoZone.width + 24, 20, Fade(WHITE, 0.45f));
        DrawGaloEventShape(game);
    }

    if (game->galoWarningTimer > 0) {
        DrawRectangle(250, 85, 460, 36, Fade(MAROON, 0.85f));
        DrawText("ATENCAO: o Galo esta passando pela avenida!", 270, 93, 20, RAYWHITE);
    }
}

static void DrawPlaying(const Game *game) {
    DrawBackgroundDetails(game);
    DrawText("Desvie da multidao e da passagem do Galo!", 20, SCREEN_HEIGHT - 35, 20, DARKGRAY);
    DrawPlayer(&game->player);
    DrawNpcs(&game->npcs, &game->npcTextureA, &game->npcTextureB);
    DrawHud(game);
}

static void DrawRankingScreen(const Game *game) {
    int index;

    DrawText("RANKING", 390, 60, 36, MAROON);
    DrawText("Bubble Sort organiza do maior para o menor", 250, 110, 20, DARKGRAY);

    for (index = 0; index < game->ranking.count; index++) {
        DrawText(TextFormat("%d. %d pontos", index + 1, game->ranking.values[index]),
                 360, 170 + index * 30, 24, BLACK);
    }

    if (game->ranking.count == 0) {
        DrawText("Nenhuma pontuacao registrada ainda.", 290, 220, 24, GRAY);
    }

    DrawText("Pressione ESC para voltar", 320, 470, 22, DARKGRAY);
}

static void DrawGameOverScreen(const Game *game) {
    DrawText("GAME OVER", 350, 130, 50, RED);
    DrawText(TextFormat("Pontuacao final: %d", game->score), 340, 220, 30, BLACK);
    DrawText("ENTER: jogar novamente", 330, 300, 24, DARKGRAY);
    DrawText("R: ver ranking", 380, 335, 24, DARKGRAY);
    DrawText("ESC: voltar ao menu", 350, 370, 24, DARKGRAY);
}

void InitGame(Game *game) {
    game->running = 1;
    game->currentScreen = SCREEN_MENU;
    game->tileTexture = LoadTextureSafe("assets/tiles/kenney_tilesheet_complete_2X.png");
    game->npcTextureA = LoadTextureSafe("assets/sprites/npc_man_blue.png");
    game->npcTextureB = LoadTextureSafe("assets/sprites/npc_woman_green.png");
    game->galoTexture = LoadTextureSafe("assets/sprites/galo_base_rooster.gif");

    InitPlayer(&game->player, 460.0f, 250.0f);
    InitNpcList(&game->npcs);
    InitMenu(&game->menu);
    LoadRanking(&game->ranking, RANKING_FILE);

    ResetMatch(game);
}

void ResetMatch(Game *game) {
    ResetPlayer(&game->player, 460.0f, 250.0f);
    ClearNpcList(&game->npcs);
    game->scoreTime = 0.0f;
    game->score = 0;
    game->npcSpawnTimer = 0.0f;
    game->npcSpawnInterval = 0.95f;
    game->difficultyTimer = 0.0f;
    game->npcBaseSpeed = 120.0f;
    game->galoEventActive = 0;
    game->galoEventTimer = 0.0f;
    game->galoEventCooldown = 0.0f;
    game->galoEventDuration = 5.5f;
    game->galoZone = (Rectangle){-210.0f, 180.0f, 210.0f, 92.0f};
    game->galoZoneSpeed = 240.0f;
    game->galoWarningTimer = 0;
}

void UpdateGame(Game *game) {
    if (game->currentScreen == SCREEN_MENU) {
        UpdateMenu(&game->menu);

        if (IsKeyPressed(KEY_ENTER)) {
            if (game->menu.selectedOption == MENU_OPTION_PLAY) {
                ResetMatch(game);
                game->currentScreen = SCREEN_PLAYING;
            } else if (game->menu.selectedOption == MENU_OPTION_RANKING) {
                game->currentScreen = SCREEN_RANKING;
            } else if (game->menu.selectedOption == MENU_OPTION_EXIT) {
                game->running = 0;
            }
        }
    } else if (game->currentScreen == SCREEN_PLAYING) {
        UpdatePlaying(game);

        if (IsKeyPressed(KEY_ESCAPE)) {
            game->currentScreen = SCREEN_MENU;
        }
    } else if (game->currentScreen == SCREEN_RANKING) {
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
            game->currentScreen = SCREEN_MENU;
        }
    } else if (game->currentScreen == SCREEN_GAME_OVER) {
        if (IsKeyPressed(KEY_ENTER)) {
            ResetMatch(game);
            game->currentScreen = SCREEN_PLAYING;
        } else if (IsKeyPressed(KEY_R)) {
            game->currentScreen = SCREEN_RANKING;
        } else if (IsKeyPressed(KEY_ESCAPE)) {
            game->currentScreen = SCREEN_MENU;
        }
    }
}

void DrawGame(const Game *game) {
    if (game->currentScreen == SCREEN_MENU) {
        DrawMenu(&game->menu);
    } else if (game->currentScreen == SCREEN_PLAYING) {
        DrawPlaying(game);
    } else if (game->currentScreen == SCREEN_RANKING) {
        DrawRankingScreen(game);
    } else if (game->currentScreen == SCREEN_GAME_OVER) {
        DrawGameOverScreen(game);
    }
}

void UnloadGame(Game *game) {
    ClearNpcList(&game->npcs);
    SaveRanking(&game->ranking, RANKING_FILE);

    if (game->tileTexture.id > 0) {
        UnloadTexture(game->tileTexture);
    }
    if (game->npcTextureA.id > 0) {
        UnloadTexture(game->npcTextureA);
    }
    if (game->npcTextureB.id > 0) {
        UnloadTexture(game->npcTextureB);
    }
    if (game->galoTexture.id > 0) {
        UnloadTexture(game->galoTexture);
    }
}
