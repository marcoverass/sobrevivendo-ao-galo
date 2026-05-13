#ifndef RANKING_H
#define RANKING_H

#define MAX_RANKING_SCORES 10
#define MAX_NOME_JOGADOR 32

typedef struct {
    char nome[MAX_NOME_JOGADOR];
    int pontuacao;
} RegistroRanking;

typedef struct {
    RegistroRanking registros[MAX_RANKING_SCORES];
    int quantidade;
} Ranking;

void InicializarRanking(Ranking *ranking);
void AdicionarPontuacaoAoRanking(Ranking *ranking, const char *nomeJogador, int pontuacao);
void OrdenarRanking(Ranking *ranking);
void CarregarRanking(Ranking *ranking, const char *nomeArquivo);
void SalvarRanking(const Ranking *ranking, const char *nomeArquivo);

#endif
