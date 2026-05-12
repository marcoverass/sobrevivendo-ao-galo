#ifndef RANKING_H
#define RANKING_H

#define MAX_RANKING_SCORES 10

typedef struct {
    int values[MAX_RANKING_SCORES];
    int count;
} Ranking;

void InitRanking(Ranking *ranking);
void AddScoreToRanking(Ranking *ranking, int score);
void SortRanking(Ranking *ranking);
void LoadRanking(Ranking *ranking, const char *fileName);
void SaveRanking(const Ranking *ranking, const char *fileName);

#endif
