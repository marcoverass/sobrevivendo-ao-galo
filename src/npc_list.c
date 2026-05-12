#include <stdlib.h>
#include <math.h>

#include "npc_list.h"

#define FLOW_AXIS_HORIZONTAL 0
#define FLOW_AXIS_VERTICAL 1

static float RandomFloatInRange(float minValue, float maxValue) {
    return minValue + ((float)GetRandomValue(0, 1000) / 1000.0f) * (maxValue - minValue);
}

static int ChooseLaneWeighted(const int *weights, int laneCount) {
    int totalWeight = 0;
    int randomValue;
    int laneIndex;

    for (laneIndex = 0; laneIndex < laneCount; laneIndex++) {
        totalWeight += weights[laneIndex];
    }

    randomValue = GetRandomValue(1, totalWeight);

    for (laneIndex = 0; laneIndex < laneCount; laneIndex++) {
        randomValue -= weights[laneIndex];

        if (randomValue <= 0) {
            return laneIndex;
        }
    }

    return 0;
}

static void SetupNpcMovementStyle(Npc *npc) {
    if (fabsf(npc->speedX) > fabsf(npc->speedY)) {
        npc->flowAxis = FLOW_AXIS_HORIZONTAL;
        npc->flowLine = npc->body.y;
    } else {
        npc->flowAxis = FLOW_AXIS_VERTICAL;
        npc->flowLine = npc->body.x;
    }

    npc->swayPhase = RandomFloatInRange(0.0f, 6.28f);
    npc->swaySpeed = RandomFloatInRange(2.2f, 3.8f);
    npc->swayAmount = RandomFloatInRange(4.0f, 10.0f);
}

static void SpawnNpcGroup(NpcList *list,
                          int groupSize,
                          float startX,
                          float startY,
                          float spacingX,
                          float spacingY,
                          float speedX,
                          float speedY) {
    int index;
    int rowSize = 2;

    for (index = 0; index < groupSize; index++) {
        float size = RandomFloatInRange(24.0f, 31.0f);
        int row = index / rowSize;
        int column = index % rowSize;
        float offsetX;
        float offsetY;
        float variedSpeedX = speedX + RandomFloatInRange(-10.0f, 10.0f);
        float variedSpeedY = speedY + RandomFloatInRange(-10.0f, 10.0f);

        if (fabsf(speedX) > fabsf(speedY)) {
            offsetX = spacingX * row + RandomFloatInRange(-5.0f, 5.0f);
            offsetY = spacingY * column + RandomFloatInRange(-18.0f, 18.0f);
        } else {
            offsetX = spacingX * column + RandomFloatInRange(-18.0f, 18.0f);
            offsetY = spacingY * row + RandomFloatInRange(-5.0f, 5.0f);
        }

        {
            Npc *npc = CreateNpc(startX + offsetX,
                                 startY + offsetY,
                                 size,
                                 size,
                                 variedSpeedX,
                                 variedSpeedY,
                                 GetRandomValue(0, 2));

            if (npc == NULL) {
                continue;
            }

            SetupNpcMovementStyle(npc);
            InsertNpc(list, npc);
        }
    }
}

void InitNpcList(NpcList *list) {
    list->head = NULL;
    list->count = 0;
}

Npc *CreateNpc(float x, float y, float width, float height, float speedX, float speedY, int type) {
    Npc *newNpc = (Npc *)malloc(sizeof(Npc));

    if (newNpc == NULL) {
        return NULL;
    }

    newNpc->body = (Rectangle){x, y, width, height};
    newNpc->speedX = speedX;
    newNpc->speedY = speedY;
    newNpc->flowLine = 0.0f;
    newNpc->swayPhase = 0.0f;
    newNpc->swaySpeed = 0.0f;
    newNpc->swayAmount = 0.0f;
    newNpc->flowAxis = FLOW_AXIS_HORIZONTAL;
    newNpc->type = type;
    newNpc->next = NULL;

    return newNpc;
}

void InsertNpc(NpcList *list, Npc *newNpc) {
    if (newNpc == NULL) {
        return;
    }

    newNpc->next = list->head;
    list->head = newNpc;
    list->count++;
}

void SpawnRandomNpc(NpcList *list, int screenWidth, int screenHeight, float baseSpeed, int crowdLevel) {
    static const float horizontalLanes[] = {150.0f, 205.0f, 260.0f, 315.0f, 370.0f};
    static const float verticalLanes[] = {190.0f, 315.0f, 445.0f, 580.0f, 720.0f};
    static const int horizontalLaneWeights[] = {2, 4, 5, 4, 2};
    static const int verticalLaneWeights[] = {1, 2, 3, 2, 1};
    int crowdPattern;
    int laneIndex;
    int groupSize;
    int shouldSpawnExtraGroup;

    if (list->count > 90) {
        return;
    }

    groupSize = GetRandomValue(3, 5) + crowdLevel;
    crowdPattern = GetRandomValue(0, 99);
    shouldSpawnExtraGroup = crowdLevel >= 3 && GetRandomValue(0, 99) < 35;

    if (groupSize > 10) {
        groupSize = 10;
    }

    /* A maior parte da multidao segue o fluxo da avenida em faixas parecidas. */
    if (crowdPattern < 70) {
        laneIndex = ChooseLaneWeighted(horizontalLaneWeights, 5);

        if (GetRandomValue(0, 1) == 0) {
            SpawnNpcGroup(list,
                          groupSize,
                          -26.0f,
                          horizontalLanes[laneIndex],
                          -26.0f,
                          20.0f,
                          baseSpeed,
                          0.0f);

            if (shouldSpawnExtraGroup) {
                laneIndex = ChooseLaneWeighted(horizontalLaneWeights, 5);
                SpawnNpcGroup(list,
                              groupSize - 1,
                              -26.0f,
                              horizontalLanes[laneIndex],
                              -26.0f,
                              20.0f,
                              baseSpeed + 12.0f,
                              0.0f);
            }
        } else {
            SpawnNpcGroup(list,
                          groupSize,
                          (float)screenWidth + 26.0f,
                          horizontalLanes[laneIndex],
                          26.0f,
                          20.0f,
                          -baseSpeed,
                          0.0f);

            if (shouldSpawnExtraGroup) {
                laneIndex = ChooseLaneWeighted(horizontalLaneWeights, 5);
                SpawnNpcGroup(list,
                              groupSize - 1,
                              (float)screenWidth + 26.0f,
                              horizontalLanes[laneIndex],
                              26.0f,
                              20.0f,
                              -(baseSpeed + 12.0f),
                              0.0f);
            }
        }
    } else {
        laneIndex = ChooseLaneWeighted(verticalLaneWeights, 5);

        if (GetRandomValue(0, 1) == 0) {
            SpawnNpcGroup(list,
                          groupSize - 1,
                          verticalLanes[laneIndex],
                          -26.0f,
                          20.0f,
                          -26.0f,
                          0.0f,
                          baseSpeed * 0.75f);

            if (shouldSpawnExtraGroup) {
                laneIndex = ChooseLaneWeighted(verticalLaneWeights, 5);
                SpawnNpcGroup(list,
                              groupSize - 2,
                              verticalLanes[laneIndex],
                              -26.0f,
                              20.0f,
                              -26.0f,
                              0.0f,
                              baseSpeed * 0.82f);
            }
        } else {
            SpawnNpcGroup(list,
                          groupSize - 1,
                          verticalLanes[laneIndex],
                          (float)screenHeight + 26.0f,
                          20.0f,
                          26.0f,
                          0.0f,
                          -baseSpeed * 0.75f);

            if (shouldSpawnExtraGroup) {
                laneIndex = ChooseLaneWeighted(verticalLaneWeights, 5);
                SpawnNpcGroup(list,
                              groupSize - 2,
                              verticalLanes[laneIndex],
                              (float)screenHeight + 26.0f,
                              20.0f,
                              26.0f,
                              0.0f,
                              -(baseSpeed * 0.82f));
            }
        }
    }
}

void UpdateNpcs(NpcList *list, float deltaTime) {
    Npc *current = list->head;

    while (current != NULL) {
        current->body.x += current->speedX * deltaTime;
        current->body.y += current->speedY * deltaTime;
        current->swayPhase += current->swaySpeed * deltaTime;

        if (current->flowAxis == FLOW_AXIS_HORIZONTAL) {
            current->body.y = current->flowLine + sinf(current->swayPhase) * current->swayAmount;
        } else {
            current->body.x = current->flowLine + sinf(current->swayPhase) * current->swayAmount;
        }

        current = current->next;
    }
}

void DrawNpcs(const NpcList *list, const Texture2D *npcTextureA, const Texture2D *npcTextureB) {
    const Npc *current = list->head;

    while (current != NULL) {
        const Texture2D *selectedTexture = npcTextureA;

        if (current->type == 1) {
            selectedTexture = npcTextureB;
        } else if (current->type == 2) {
            selectedTexture = npcTextureA;
        }

        if (selectedTexture != NULL && selectedTexture->id > 0) {
            Rectangle source = {0.0f, 0.0f, (float)selectedTexture->width, (float)selectedTexture->height};
            Vector2 position = {current->body.x, current->body.y};

            DrawTexturePro(*selectedTexture, source, current->body, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
        } else {
            DrawRectangleRec(current->body, RED);
        }

        current = current->next;
    }
}

int CheckPlayerNpcCollision(const NpcList *list, Rectangle playerBody) {
    const Npc *current = list->head;

    while (current != NULL) {
        if (CheckCollisionRecs(playerBody, current->body)) {
            return 1;
        }
        current = current->next;
    }

    return 0;
}

void RemoveOffscreenNpcs(NpcList *list, int screenWidth, int screenHeight) {
    Npc *current = list->head;
    Npc *previous = NULL;

    while (current != NULL) {
        int outLeft = current->body.x + current->body.width < -80;
        int outRight = current->body.x > screenWidth + 80;
        int outTop = current->body.y + current->body.height < -80;
        int outBottom = current->body.y > screenHeight + 80;
        int shouldRemove = outLeft || outRight || outTop || outBottom;

        if (shouldRemove) {
            Npc *removed = current;

            if (previous == NULL) {
                list->head = current->next;
            } else {
                previous->next = current->next;
            }

            current = current->next;
            free(removed);
            list->count--;
        } else {
            previous = current;
            current = current->next;
        }
    }
}

void ClearNpcList(NpcList *list) {
    Npc *current = list->head;

    while (current != NULL) {
        Npc *next = current->next;
        free(current);
        current = next;
    }

    list->head = NULL;
    list->count = 0;
}
