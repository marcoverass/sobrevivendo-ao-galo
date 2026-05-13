#include <stdio.h>
#include <string.h>

#include "ranking.h"

void InicializarRanking(Ranking *ranking) {
    int indice;

    ranking->quantidade = 0;
    for (indice = 0; indice < MAX_RANKING_SCORES; indice++) {
        ranking->registros[indice].nome[0] = '\0';
        ranking->registros[indice].pontuacao = 0;
    }
}

void OrdenarRanking(Ranking *ranking) {
    int i;
    int j;

    for (i = 0; i < ranking->quantidade - 1; i++) {
        for (j = 0; j < ranking->quantidade - 1 - i; j++) {
            if (ranking->registros[j].pontuacao < ranking->registros[j + 1].pontuacao) {
                RegistroRanking temporario = ranking->registros[j];
                ranking->registros[j] = ranking->registros[j + 1];
                ranking->registros[j + 1] = temporario;
            }
        }
    }
}

void AdicionarPontuacaoAoRanking(Ranking *ranking, const char *nomeJogador, int pontuacao) {
    if (ranking->quantidade < MAX_RANKING_SCORES) {
        strncpy(ranking->registros[ranking->quantidade].nome, nomeJogador, MAX_NOME_JOGADOR - 1);
        ranking->registros[ranking->quantidade].nome[MAX_NOME_JOGADOR - 1] = '\0';
        ranking->registros[ranking->quantidade].pontuacao = pontuacao;
        ranking->quantidade++;
    } else {
        OrdenarRanking(ranking);

        if (pontuacao > ranking->registros[MAX_RANKING_SCORES - 1].pontuacao) {
            strncpy(ranking->registros[MAX_RANKING_SCORES - 1].nome, nomeJogador, MAX_NOME_JOGADOR - 1);
            ranking->registros[MAX_RANKING_SCORES - 1].nome[MAX_NOME_JOGADOR - 1] = '\0';
            ranking->registros[MAX_RANKING_SCORES - 1].pontuacao = pontuacao;
        } else {
            return;
        }
    }

    OrdenarRanking(ranking);
}

void CarregarRanking(Ranking *ranking, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    char nome[MAX_NOME_JOGADOR];
    int pontuacao;

    InicializarRanking(ranking);

    if (arquivo == NULL) {
        return;
    }

    while (ranking->quantidade < MAX_RANKING_SCORES &&
           fscanf(arquivo, " %31[^;];%d", nome, &pontuacao) == 2) {
        strncpy(ranking->registros[ranking->quantidade].nome, nome, MAX_NOME_JOGADOR - 1);
        ranking->registros[ranking->quantidade].nome[MAX_NOME_JOGADOR - 1] = '\0';
        ranking->registros[ranking->quantidade].pontuacao = pontuacao;
        ranking->quantidade++;
    }

    fclose(arquivo);
    OrdenarRanking(ranking);
}

void SalvarRanking(const Ranking *ranking, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    int indice;

    if (arquivo == NULL) {
        return;
    }

    for (indice = 0; indice < ranking->quantidade; indice++) {
        fprintf(arquivo, "%s;%d\n", ranking->registros[indice].nome, ranking->registros[indice].pontuacao);
    }

    fclose(arquivo);
}
