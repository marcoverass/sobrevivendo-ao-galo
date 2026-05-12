#ifndef NPC_LIST_H
#define NPC_LIST_H

#include "raylib.h"

typedef struct Npc {
    Rectangle body;
    float speedX;
    float speedY;
    float flowLine;
    float swayPhase;
    float swaySpeed;
    float swayAmount;
    int flowAxis;
    int type;
    struct Npc *next;
} Npc;

typedef struct {
    Npc *head;
    int count;
} NpcList;

void InitNpcList(NpcList *list);
Npc *CreateNpc(float x, float y, float width, float height, float speedX, float speedY, int type);
void InsertNpc(NpcList *list, Npc *newNpc);
void SpawnRandomNpc(NpcList *list, int screenWidth, int screenHeight, float baseSpeed, int crowdLevel);
void UpdateNpcs(NpcList *list, float deltaTime);
void DrawNpcs(const NpcList *list, const Texture2D *npcTextureA, const Texture2D *npcTextureB);
int CheckPlayerNpcCollision(const NpcList *list, Rectangle playerBody);
void RemoveOffscreenNpcs(NpcList *list, int screenWidth, int screenHeight);
void ClearNpcList(NpcList *list);

#endif
