#include <stdio.h>

#include "ranking.h"

void InitRanking(Ranking *ranking) {
    int index;

    ranking->count = 0;
    for (index = 0; index < MAX_RANKING_SCORES; index++) {
        ranking->values[index] = 0;
    }
}

void SortRanking(Ranking *ranking) {
    int i;
    int j;

    for (i = 0; i < ranking->count - 1; i++) {
        for (j = 0; j < ranking->count - 1 - i; j++) {
            if (ranking->values[j] < ranking->values[j + 1]) {
                int temp = ranking->values[j];
                ranking->values[j] = ranking->values[j + 1];
                ranking->values[j + 1] = temp;
            }
        }
    }
}

void AddScoreToRanking(Ranking *ranking, int score) {
    if (ranking->count < MAX_RANKING_SCORES) {
        ranking->values[ranking->count] = score;
        ranking->count++;
    } else {
        SortRanking(ranking);

        if (score > ranking->values[MAX_RANKING_SCORES - 1]) {
            ranking->values[MAX_RANKING_SCORES - 1] = score;
        } else {
            return;
        }
    }

    SortRanking(ranking);
}

void LoadRanking(Ranking *ranking, const char *fileName) {
    FILE *file = fopen(fileName, "r");

    InitRanking(ranking);

    if (file == NULL) {
        return;
    }

    while (ranking->count < MAX_RANKING_SCORES &&
           fscanf(file, "%d", &ranking->values[ranking->count]) == 1) {
        ranking->count++;
    }

    fclose(file);
    SortRanking(ranking);
}

void SaveRanking(const Ranking *ranking, const char *fileName) {
    FILE *file = fopen(fileName, "w");
    int index;

    if (file == NULL) {
        return;
    }

    for (index = 0; index < ranking->count; index++) {
        fprintf(file, "%d\n", ranking->values[index]);
    }

    fclose(file);
}
