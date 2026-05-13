#include <math.h>
#include <stdlib.h>

#include "npc_list.h"

#define EIXO_FLUXO_HORIZONTAL 0
#define EIXO_FLUXO_VERTICAL 1

static float SortearFloat(float valorMinimo, float valorMaximo) {
    return valorMinimo + ((float)GetRandomValue(0, 1000) / 1000.0f) * (valorMaximo - valorMinimo);
}

static int EscolherFaixaComPeso(const int *pesos, int quantidadeFaixas) {
    int somaPesos = 0;
    int valorSorteado;
    int indiceFaixa;

    for (indiceFaixa = 0; indiceFaixa < quantidadeFaixas; indiceFaixa++) {
        somaPesos += pesos[indiceFaixa];
    }

    valorSorteado = GetRandomValue(1, somaPesos);

    for (indiceFaixa = 0; indiceFaixa < quantidadeFaixas; indiceFaixa++) {
        valorSorteado -= pesos[indiceFaixa];

        if (valorSorteado <= 0) {
            return indiceFaixa;
        }
    }

    return 0;
}

static void ConfigurarEstiloDeMovimento(Foliao *foliao) {
    if (fabsf(foliao->velocidadeX) > fabsf(foliao->velocidadeY)) {
        foliao->eixoFluxo = EIXO_FLUXO_HORIZONTAL;
        foliao->linhaFluxo = foliao->corpo.y;
    } else {
        foliao->eixoFluxo = EIXO_FLUXO_VERTICAL;
        foliao->linhaFluxo = foliao->corpo.x;
    }

    foliao->faseBalanco = SortearFloat(0.0f, 6.28f);
    foliao->velocidadeBalanco = SortearFloat(2.2f, 3.8f);
    foliao->amplitudeBalanco = SortearFloat(4.0f, 10.0f);
}

static void GerarGrupoFolioes(ListaFolioes *lista,
                              int tamanhoGrupo,
                              float posicaoXInicial,
                              float posicaoYInicial,
                              float espacamentoX,
                              float espacamentoY,
                              float velocidadeX,
                              float velocidadeY) {
    int indice;
    int tamanhoLinha = 2;

    for (indice = 0; indice < tamanhoGrupo; indice++) {
        float tamanho = SortearFloat(24.0f, 31.0f);
        int linha = indice / tamanhoLinha;
        int coluna = indice % tamanhoLinha;
        float deslocamentoX;
        float deslocamentoY;
        float velocidadeVariadaX = velocidadeX + SortearFloat(-10.0f, 10.0f);
        float velocidadeVariadaY = velocidadeY + SortearFloat(-10.0f, 10.0f);
        Foliao *foliao;

        if (fabsf(velocidadeX) > fabsf(velocidadeY)) {
            deslocamentoX = espacamentoX * linha + SortearFloat(-5.0f, 5.0f);
            deslocamentoY = espacamentoY * coluna + SortearFloat(-18.0f, 18.0f);
        } else {
            deslocamentoX = espacamentoX * coluna + SortearFloat(-18.0f, 18.0f);
            deslocamentoY = espacamentoY * linha + SortearFloat(-5.0f, 5.0f);
        }

        foliao = CriarFoliao(posicaoXInicial + deslocamentoX,
                             posicaoYInicial + deslocamentoY,
                             tamanho,
                             tamanho,
                             velocidadeVariadaX,
                             velocidadeVariadaY,
                             GetRandomValue(0, 2));

        if (foliao == NULL) {
            continue;
        }

        ConfigurarEstiloDeMovimento(foliao);
        InserirFoliao(lista, foliao);
    }
}

void InicializarListaFolioes(ListaFolioes *lista) {
    lista->inicio = NULL;
    lista->quantidade = 0;
}

Foliao *CriarFoliao(float posicaoX, float posicaoY, float largura, float altura, float velocidadeX, float velocidadeY, int tipo) {
    Foliao *novoFoliao = (Foliao *)malloc(sizeof(Foliao));

    if (novoFoliao == NULL) {
        return NULL;
    }

    novoFoliao->corpo = (Rectangle){posicaoX, posicaoY, largura, altura};
    novoFoliao->velocidadeX = velocidadeX;
    novoFoliao->velocidadeY = velocidadeY;
    novoFoliao->linhaFluxo = 0.0f;
    novoFoliao->faseBalanco = 0.0f;
    novoFoliao->velocidadeBalanco = 0.0f;
    novoFoliao->amplitudeBalanco = 0.0f;
    novoFoliao->eixoFluxo = EIXO_FLUXO_HORIZONTAL;
    novoFoliao->tipo = tipo;
    novoFoliao->proximo = NULL;

    return novoFoliao;
}

void InserirFoliao(ListaFolioes *lista, Foliao *novoFoliao) {
    if (novoFoliao == NULL) {
        return;
    }

    novoFoliao->proximo = lista->inicio;
    lista->inicio = novoFoliao;
    lista->quantidade++;
}

void GerarFluxoEmFaixaY(ListaFolioes *lista, int larguraTela, float faixaY, float velocidadeBase, int tamanhoGrupo) {
    if (tamanhoGrupo < 2) {
        tamanhoGrupo = 2;
    }
    if (tamanhoGrupo > 8) {
        tamanhoGrupo = 8;
    }

    if (GetRandomValue(0, 1) == 0) {
        GerarGrupoFolioes(lista, tamanhoGrupo, -26.0f, faixaY, -24.0f, 18.0f, velocidadeBase, 0.0f);
    } else {
        GerarGrupoFolioes(lista, tamanhoGrupo, (float)larguraTela + 26.0f, faixaY, 24.0f, 18.0f, -velocidadeBase, 0.0f);
    }
}

void GerarGrupoAleatorio(ListaFolioes *lista, int larguraTela, int alturaTela, float velocidadeBase, int nivelMultidao) {
    static const float faixasHorizontais[] = {150.0f, 205.0f, 260.0f, 315.0f, 370.0f};
    static const float faixasVerticais[] = {190.0f, 315.0f, 445.0f, 580.0f, 720.0f};
    static const int pesosFaixasHorizontais[] = {2, 4, 5, 4, 2};
    static const int pesosFaixasVerticais[] = {1, 2, 3, 2, 1};
    int padraoMultidao;
    int indiceFaixa;
    int tamanhoGrupo;
    int deveGerarGrupoExtra;

    if (lista->quantidade > 90) {
        return;
    }

    tamanhoGrupo = GetRandomValue(3, 5) + nivelMultidao;
    padraoMultidao = GetRandomValue(0, 99);
    deveGerarGrupoExtra = nivelMultidao >= 3 && GetRandomValue(0, 99) < 35;

    if (tamanhoGrupo > 10) {
        tamanhoGrupo = 10;
    }

    if (padraoMultidao < 70) {
        indiceFaixa = EscolherFaixaComPeso(pesosFaixasHorizontais, 5);

        if (GetRandomValue(0, 1) == 0) {
            GerarGrupoFolioes(lista, tamanhoGrupo, -26.0f, faixasHorizontais[indiceFaixa], -26.0f, 20.0f, velocidadeBase, 0.0f);

            if (deveGerarGrupoExtra) {
                indiceFaixa = EscolherFaixaComPeso(pesosFaixasHorizontais, 5);
                GerarGrupoFolioes(lista, tamanhoGrupo - 1, -26.0f, faixasHorizontais[indiceFaixa], -26.0f, 20.0f, velocidadeBase + 12.0f, 0.0f);
            }
        } else {
            GerarGrupoFolioes(lista, tamanhoGrupo, (float)larguraTela + 26.0f, faixasHorizontais[indiceFaixa], 26.0f, 20.0f, -velocidadeBase, 0.0f);

            if (deveGerarGrupoExtra) {
                indiceFaixa = EscolherFaixaComPeso(pesosFaixasHorizontais, 5);
                GerarGrupoFolioes(lista, tamanhoGrupo - 1, (float)larguraTela + 26.0f, faixasHorizontais[indiceFaixa], 26.0f, 20.0f, -(velocidadeBase + 12.0f), 0.0f);
            }
        }
    } else {
        indiceFaixa = EscolherFaixaComPeso(pesosFaixasVerticais, 5);

        if (GetRandomValue(0, 1) == 0) {
            GerarGrupoFolioes(lista, tamanhoGrupo - 1, faixasVerticais[indiceFaixa], -26.0f, 20.0f, -26.0f, 0.0f, velocidadeBase * 0.75f);

            if (deveGerarGrupoExtra) {
                indiceFaixa = EscolherFaixaComPeso(pesosFaixasVerticais, 5);
                GerarGrupoFolioes(lista, tamanhoGrupo - 2, faixasVerticais[indiceFaixa], -26.0f, 20.0f, -26.0f, 0.0f, velocidadeBase * 0.82f);
            }
        } else {
            GerarGrupoFolioes(lista, tamanhoGrupo - 1, faixasVerticais[indiceFaixa], (float)alturaTela + 26.0f, 20.0f, 26.0f, 0.0f, -velocidadeBase * 0.75f);

            if (deveGerarGrupoExtra) {
                indiceFaixa = EscolherFaixaComPeso(pesosFaixasVerticais, 5);
                GerarGrupoFolioes(lista, tamanhoGrupo - 2, faixasVerticais[indiceFaixa], (float)alturaTela + 26.0f, 20.0f, 26.0f, 0.0f, -(velocidadeBase * 0.82f));
            }
        }
    }
}

void AtualizarFolioes(ListaFolioes *lista, float deltaTime) {
    Foliao *foliaoAtual = lista->inicio;

    while (foliaoAtual != NULL) {
        foliaoAtual->corpo.x += foliaoAtual->velocidadeX * deltaTime;
        foliaoAtual->corpo.y += foliaoAtual->velocidadeY * deltaTime;
        foliaoAtual->faseBalanco += foliaoAtual->velocidadeBalanco * deltaTime;

        if (foliaoAtual->eixoFluxo == EIXO_FLUXO_HORIZONTAL) {
            foliaoAtual->corpo.y = foliaoAtual->linhaFluxo + sinf(foliaoAtual->faseBalanco) * foliaoAtual->amplitudeBalanco;
        } else {
            foliaoAtual->corpo.x = foliaoAtual->linhaFluxo + sinf(foliaoAtual->faseBalanco) * foliaoAtual->amplitudeBalanco;
        }

        foliaoAtual = foliaoAtual->proximo;
    }
}

void DesenharFolioes(const ListaFolioes *lista, const Texture2D *texturaFoliaoA, const Texture2D *texturaFoliaoB) {
    const Foliao *foliaoAtual = lista->inicio;

    while (foliaoAtual != NULL) {
        const Texture2D *texturaSelecionada = texturaFoliaoA;

        if (foliaoAtual->tipo == 1) {
            texturaSelecionada = texturaFoliaoB;
        } else if (foliaoAtual->tipo == 2) {
            texturaSelecionada = texturaFoliaoA;
        }

        if (texturaSelecionada != NULL && texturaSelecionada->id > 0) {
            Rectangle origem = {0.0f, 0.0f, (float)texturaSelecionada->width, (float)texturaSelecionada->height};
            DrawTexturePro(*texturaSelecionada, origem, foliaoAtual->corpo, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
        } else {
            DrawRectangleRec(foliaoAtual->corpo, RED);
        }

        foliaoAtual = foliaoAtual->proximo;
    }
}

int VerificarColisaoJogadorFolioes(const ListaFolioes *lista, Rectangle corpoJogador) {
    const Foliao *foliaoAtual = lista->inicio;

    while (foliaoAtual != NULL) {
        if (CheckCollisionRecs(corpoJogador, foliaoAtual->corpo)) {
            return 1;
        }
        foliaoAtual = foliaoAtual->proximo;
    }

    return 0;
}

void RemoverFolioesForaDaTela(ListaFolioes *lista, int larguraTela, int alturaTela) {
    Foliao *foliaoAtual = lista->inicio;
    Foliao *foliaoAnterior = NULL;

    while (foliaoAtual != NULL) {
        int saiuEsquerda = foliaoAtual->corpo.x + foliaoAtual->corpo.width < -80;
        int saiuDireita = foliaoAtual->corpo.x > larguraTela + 80;
        int saiuCima = foliaoAtual->corpo.y + foliaoAtual->corpo.height < -80;
        int saiuBaixo = foliaoAtual->corpo.y > alturaTela + 80;
        int deveRemover = saiuEsquerda || saiuDireita || saiuCima || saiuBaixo;

        if (deveRemover) {
            Foliao *foliaoRemovido = foliaoAtual;

            if (foliaoAnterior == NULL) {
                lista->inicio = foliaoAtual->proximo;
            } else {
                foliaoAnterior->proximo = foliaoAtual->proximo;
            }

            foliaoAtual = foliaoAtual->proximo;
            free(foliaoRemovido);
            lista->quantidade--;
        } else {
            foliaoAnterior = foliaoAtual;
            foliaoAtual = foliaoAtual->proximo;
        }
    }
}

void LimparListaFolioes(ListaFolioes *lista) {
    Foliao *foliaoAtual = lista->inicio;

    while (foliaoAtual != NULL) {
        Foliao *proximoFoliao = foliaoAtual->proximo;
        free(foliaoAtual);
        foliaoAtual = proximoFoliao;
    }

    lista->inicio = NULL;
    lista->quantidade = 0;
}
