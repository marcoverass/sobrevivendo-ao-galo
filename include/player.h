#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

typedef struct {
    Rectangle corpo;
    float velocidade;
    Color cor;
    int direcaoAtual;
} Jogador;

void InicializarJogador(Jogador *jogador, float posicaoXInicial, float posicaoYInicial);
void AtualizarJogador(Jogador *jogador, int larguraTela, int alturaTela);
void DesenharJogador(const Jogador *jogador,
                     const Texture2D *texturaFrente,
                     const Texture2D *texturaCostas,
                     const Texture2D *texturaEsquerda,
                     const Texture2D *texturaDireita);
void ResetarJogador(Jogador *jogador, float posicaoXInicial, float posicaoYInicial);

#endif
