#ifndef GAME_H
#define GAME_H

#include "menu.h"
#include "npc_list.h"
#include "player.h"
#include "ranking.h"

#define LARGURA_TELA 960
#define ALTURA_TELA 540

typedef enum {
    TELA_MENU = 0,
    TELA_JOGANDO,
    TELA_RANKING,
    TELA_GAME_OVER
} TelaJogo;

typedef struct {
    TelaJogo telaAtual;
    Jogador jogador;
    ListaFolioes listaFolioes;
    Ranking ranking;
    Menu menu;
    float tempoPontuacao;
    int pontuacao;
    float temporizadorSpawnNpc;
    float intervaloSpawnNpc;
    float temporizadorDificuldade;
    float velocidadeBaseFolioes;
    int eventoGaloAtivo;
    float temporizadorEventoGalo;
    float recargaEventoGalo;
    float duracaoEventoGalo;
    Rectangle zonaGalo;
    float velocidadeZonaGalo;
    int temporizadorAvisoGalo;
    Texture2D texturaChao;
    Texture2D texturaFoliaoA;
    Texture2D texturaFoliaoB;
    Texture2D texturaGalo;
    Texture2D texturaJogadorFrente;
    Texture2D texturaJogadorCostas;
    Texture2D texturaJogadorEsquerda;
    Texture2D texturaJogadorDireita;
    char nomeJogadorAtual[MAX_NOME_JOGADOR];
    int editandoNome;
    int executando;
} Jogo;

void InicializarJogo(Jogo *jogo);
void ResetarPartida(Jogo *jogo);
void AtualizarJogo(Jogo *jogo);
void DesenharJogo(const Jogo *jogo);
void EncerrarJogo(Jogo *jogo);

#endif
