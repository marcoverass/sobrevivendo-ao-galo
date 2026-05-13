#ifndef NPC_LIST_H
#define NPC_LIST_H

#include "raylib.h"

typedef struct Foliao {
    Rectangle corpo;
    float velocidadeX;
    float velocidadeY;
    float linhaFluxo;
    float faseBalanco;
    float velocidadeBalanco;
    float amplitudeBalanco;
    int eixoFluxo;
    int tipo;
    struct Foliao *proximo;
} Foliao;

typedef struct {
    Foliao *inicio;
    int quantidade;
} ListaFolioes;

void InicializarListaFolioes(ListaFolioes *lista);
Foliao *CriarFoliao(float posicaoX, float posicaoY, float largura, float altura, float velocidadeX, float velocidadeY, int tipo);
void InserirFoliao(ListaFolioes *lista, Foliao *novoFoliao);
void GerarGrupoAleatorio(ListaFolioes *lista, int larguraTela, int alturaTela, float velocidadeBase, int nivelMultidao);
void GerarFluxoEmFaixaY(ListaFolioes *lista, int larguraTela, float faixaY, float velocidadeBase, int tamanhoGrupo);
void AtualizarFolioes(ListaFolioes *lista, float deltaTime);
void DesenharFolioes(const ListaFolioes *lista, const Texture2D *texturaFoliaoA, const Texture2D *texturaFoliaoB);
int VerificarColisaoJogadorFolioes(const ListaFolioes *lista, Rectangle corpoJogador);
void RemoverFolioesForaDaTela(ListaFolioes *lista, int larguraTela, int alturaTela);
void LimparListaFolioes(ListaFolioes *lista);

#endif
