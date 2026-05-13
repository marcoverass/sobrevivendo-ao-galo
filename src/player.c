#include <stddef.h>

#include "player.h"

#define DIRECAO_FRENTE 0
#define DIRECAO_COSTAS 1
#define DIRECAO_ESQUERDA 2
#define DIRECAO_DIREITA 3

static Vector2 ObterDirecaoMovimento(void) {
    Vector2 direcao = {0.0f, 0.0f};

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        direcao.y -= 1.0f;
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        direcao.y += 1.0f;
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        direcao.x -= 1.0f;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        direcao.x += 1.0f;
    }

    return direcao;
}

void InicializarJogador(Jogador *jogador, float posicaoXInicial, float posicaoYInicial) {
    jogador->corpo = (Rectangle){posicaoXInicial, posicaoYInicial, 28.0f, 28.0f};
    jogador->velocidade = 240.0f;
    jogador->cor = BLUE;
    jogador->direcaoAtual = DIRECAO_FRENTE;
}

void AtualizarJogador(Jogador *jogador, int larguraTela, int alturaTela) {
    Vector2 direcao = ObterDirecaoMovimento();
    float deltaTime = GetFrameTime();

    if (direcao.x < 0) {
        jogador->direcaoAtual = DIRECAO_ESQUERDA;
    } else if (direcao.x > 0) {
        jogador->direcaoAtual = DIRECAO_DIREITA;
    } else if (direcao.y < 0) {
        jogador->direcaoAtual = DIRECAO_COSTAS;
    } else if (direcao.y > 0) {
        jogador->direcaoAtual = DIRECAO_FRENTE;
    }

    jogador->corpo.x += direcao.x * jogador->velocidade * deltaTime;
    jogador->corpo.y += direcao.y * jogador->velocidade * deltaTime;

    if (jogador->corpo.x < 0) {
        jogador->corpo.x = 0;
    }
    if (jogador->corpo.y < 0) {
        jogador->corpo.y = 0;
    }
    if (jogador->corpo.x + jogador->corpo.width > larguraTela) {
        jogador->corpo.x = larguraTela - jogador->corpo.width;
    }
    if (jogador->corpo.y + jogador->corpo.height > alturaTela) {
        jogador->corpo.y = alturaTela - jogador->corpo.height;
    }
}

void DesenharJogador(const Jogador *jogador,
                     const Texture2D *texturaFrente,
                     const Texture2D *texturaCostas,
                     const Texture2D *texturaEsquerda,
                     const Texture2D *texturaDireita) {
    const Texture2D *texturaSelecionada = texturaFrente;
    Rectangle areaVisual = {
        jogador->corpo.x - 8.0f,
        jogador->corpo.y - 8.0f,
        jogador->corpo.width + 16.0f,
        jogador->corpo.height + 16.0f
    };

    if (jogador->direcaoAtual == DIRECAO_COSTAS) {
        texturaSelecionada = texturaCostas;
    } else if (jogador->direcaoAtual == DIRECAO_ESQUERDA) {
        texturaSelecionada = texturaEsquerda;
    } else if (jogador->direcaoAtual == DIRECAO_DIREITA) {
        texturaSelecionada = texturaDireita;
    }

    if (texturaSelecionada != NULL && texturaSelecionada->id > 0) {
        Rectangle origem = {0.0f, 0.0f, (float)texturaSelecionada->width, (float)texturaSelecionada->height};
        DrawTexturePro(*texturaSelecionada, origem, areaVisual, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
    } else {
        DrawRectangleRec(jogador->corpo, jogador->cor);
    }

    DrawText("VOCE", (int)jogador->corpo.x - 4, (int)jogador->corpo.y - 18, 14, DARKBLUE);
}

void ResetarJogador(Jogador *jogador, float posicaoXInicial, float posicaoYInicial) {
    jogador->corpo.x = posicaoXInicial;
    jogador->corpo.y = posicaoYInicial;
}
