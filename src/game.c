#include <math.h>
#include <string.h>

#include "game.h"

#define ARQUIVO_RANKING "ranking.txt"
#define FRAMES_AVISO_GALO 180

static Texture2D CarregarTexturaSeExistir(const char *nomeArquivo) {
    if (FileExists(nomeArquivo)) {
        return LoadTexture(nomeArquivo);
    }

    return (Texture2D){0};
}

static void DesenharTexturaAjustada(Texture2D textura, Rectangle destino, Color cor) {
    if (textura.id <= 0) {
        return;
    }

    DrawTexturePro(textura,
                   (Rectangle){0.0f, 0.0f, (float)textura.width, (float)textura.height},
                   destino,
                   (Vector2){0.0f, 0.0f},
                   0.0f,
                   cor);
}

static Rectangle AjustarTexturaAoRetangulo(Texture2D textura, Rectangle alvo) {
    Rectangle ajustado = alvo;
    float proporcaoTextura;
    float proporcaoAlvo;

    if (textura.id <= 0 || textura.width <= 0 || textura.height <= 0) {
        return alvo;
    }

    proporcaoTextura = (float)textura.width / (float)textura.height;
    proporcaoAlvo = alvo.width / alvo.height;

    if (proporcaoTextura > proporcaoAlvo) {
        ajustado.height = alvo.width / proporcaoTextura;
        ajustado.y = alvo.y + (alvo.height - ajustado.height) * 0.5f;
    } else {
        ajustado.width = alvo.height * proporcaoTextura;
        ajustado.x = alvo.x + (alvo.width - ajustado.width) * 0.5f;
    }

    return ajustado;
}

static void DesenharTexturaEmRetangulo(Texture2D textura, Rectangle destino, Color cor) {
    if (textura.id <= 0) {
        DrawRectangleRec(destino, cor);
        return;
    }

    DrawTexturePro(textura,
                   (Rectangle){0.0f, 0.0f, (float)textura.width, (float)textura.height},
                   destino,
                   (Vector2){0.0f, 0.0f},
                   0.0f,
                   cor);
}

static void DesenharFileiraConfetes(int posicaoYInicial) {
    int posicaoX;

    for (posicaoX = 20; posicaoX < LARGURA_TELA; posicaoX += 70) {
        DrawRectangle(posicaoX, posicaoYInicial + 10, 10, 6, RED);
        DrawRectangle(posicaoX + 18, posicaoYInicial + 22, 8, 6, BLUE);
        DrawRectangle(posicaoX + 36, posicaoYInicial + 12, 9, 6, GOLD);
        DrawRectangle(posicaoX + 52, posicaoYInicial + 26, 8, 6, GREEN);
    }
}

static void DesenharGaloReserva(const Jogo *jogo) {
    Rectangle base = {
        jogo->zonaGalo.x + 8.0f,
        jogo->zonaGalo.y + jogo->zonaGalo.height - 18.0f,
        jogo->zonaGalo.width - 16.0f,
        18.0f
    };

    DrawRectangleRounded(base, 0.3f, 8, (Color){196, 34, 49, 255});
    DrawText("GALO", (int)jogo->zonaGalo.x + 72, (int)jogo->zonaGalo.y + 36, 22, MAROON);
}

static void DesenharEventoDoGalo(const Jogo *jogo) {
    Rectangle sombraPlataforma = {
        jogo->zonaGalo.x - 8.0f,
        jogo->zonaGalo.y + jogo->zonaGalo.height - 8.0f,
        jogo->zonaGalo.width + 16.0f,
        12.0f
    };
    Rectangle basePlataforma = {
        jogo->zonaGalo.x - 4.0f,
        jogo->zonaGalo.y + jogo->zonaGalo.height - 20.0f,
        jogo->zonaGalo.width + 8.0f,
        18.0f
    };
    Rectangle topoPlataforma = {
        jogo->zonaGalo.x + 8.0f,
        jogo->zonaGalo.y + jogo->zonaGalo.height - 28.0f,
        jogo->zonaGalo.width - 16.0f,
        10.0f
    };
    Rectangle areaGalo = {
        jogo->zonaGalo.x + 26.0f,
        jogo->zonaGalo.y - 6.0f,
        jogo->zonaGalo.width - 52.0f,
        jogo->zonaGalo.height - 16.0f
    };
    Rectangle destinoGalo = AjustarTexturaAoRetangulo(jogo->texturaGalo, areaGalo);
    float oscilacaoCortejo = sinf(jogo->temporizadorEventoGalo * 5.0f) * 4.0f;
    Rectangle foliaoA = {jogo->zonaGalo.x - 28.0f, jogo->zonaGalo.y + 34.0f + oscilacaoCortejo, 26.0f, 26.0f};
    Rectangle foliaoB = {jogo->zonaGalo.x + jogo->zonaGalo.width + 2.0f, jogo->zonaGalo.y + 30.0f - oscilacaoCortejo, 26.0f, 26.0f};
    Rectangle foliaoC = {jogo->zonaGalo.x + 12.0f, jogo->zonaGalo.y + jogo->zonaGalo.height - 52.0f - oscilacaoCortejo, 24.0f, 24.0f};
    Rectangle foliaoD = {jogo->zonaGalo.x + jogo->zonaGalo.width - 36.0f, jogo->zonaGalo.y + jogo->zonaGalo.height - 50.0f + oscilacaoCortejo, 24.0f, 24.0f};

    DrawRectangleRounded(sombraPlataforma, 0.3f, 6, Fade(BLACK, 0.22f));
    DrawRectangleRounded(basePlataforma, 0.2f, 8, (Color){181, 32, 56, 255});
    DrawRectangleRounded(topoPlataforma, 0.2f, 8, (Color){247, 197, 53, 255});
    DrawRectangle((int)topoPlataforma.x + 12, (int)topoPlataforma.y + 2, 18, 6, BLUE);
    DrawRectangle((int)topoPlataforma.x + 42, (int)topoPlataforma.y + 2, 18, 6, GREEN);
    DrawRectangle((int)topoPlataforma.x + 72, (int)topoPlataforma.y + 2, 18, 6, RED);
    DrawRectangle((int)topoPlataforma.x + 102, (int)topoPlataforma.y + 2, 18, 6, BLUE);

    DesenharTexturaEmRetangulo(jogo->texturaFoliaoA, foliaoA, WHITE);
    DesenharTexturaEmRetangulo(jogo->texturaFoliaoB, foliaoB, WHITE);
    DesenharTexturaEmRetangulo(jogo->texturaFoliaoB, foliaoC, WHITE);
    DesenharTexturaEmRetangulo(jogo->texturaFoliaoA, foliaoD, WHITE);

    if (jogo->texturaGalo.id > 0) {
        DesenharTexturaAjustada(jogo->texturaGalo, destinoGalo, WHITE);
    } else {
        DesenharGaloReserva(jogo);
    }
}

static void IniciarEventoGalo(Jogo *jogo) {
    jogo->eventoGaloAtivo = 1;
    jogo->temporizadorEventoGalo = 0.0f;
    jogo->recargaEventoGalo = 0.0f;
    jogo->temporizadorAvisoGalo = FRAMES_AVISO_GALO;
    jogo->zonaGalo.width = 210.0f;
    jogo->zonaGalo.height = 92.0f;
    jogo->zonaGalo.x = -jogo->zonaGalo.width;
    jogo->zonaGalo.y = (float)GetRandomValue(150, ALTURA_TELA - 170);
}

static void AtualizarEventoGalo(Jogo *jogo, float deltaTime) {
    if (!jogo->eventoGaloAtivo) {
        jogo->recargaEventoGalo += deltaTime;

        if (jogo->recargaEventoGalo >= 14.0f) {
            IniciarEventoGalo(jogo);
        }

        return;
    }

    jogo->temporizadorEventoGalo += deltaTime;
    jogo->zonaGalo.x += jogo->velocidadeZonaGalo * deltaTime;

    if (jogo->temporizadorEventoGalo >= jogo->duracaoEventoGalo ||
        jogo->zonaGalo.x > LARGURA_TELA + 20.0f) {
        jogo->eventoGaloAtivo = 0;
        jogo->temporizadorEventoGalo = 0.0f;
        jogo->zonaGalo.x = -jogo->zonaGalo.width;
    }
}

static int VerificarColisaoComGalo(const Jogo *jogo) {
    if (!jogo->eventoGaloAtivo) {
        return 0;
    }

    return CheckCollisionRecs(jogo->jogador.corpo, jogo->zonaGalo);
}

static void PrepararEntradaNome(Jogo *jogo) {
    strncpy(jogo->nomeJogadorAtual, "JOGADOR", MAX_NOME_JOGADOR - 1);
    jogo->nomeJogadorAtual[MAX_NOME_JOGADOR - 1] = '\0';
    jogo->editandoNome = 1;
}

static void ProcessarDigitacaoNome(Jogo *jogo) {
    int caractere = GetCharPressed();

    while (caractere > 0) {
        int tamanhoAtual = (int)strlen(jogo->nomeJogadorAtual);

        if (caractere >= 32 && caractere <= 125 && tamanhoAtual < MAX_NOME_JOGADOR - 1) {
            jogo->nomeJogadorAtual[tamanhoAtual] = (char)caractere;
            jogo->nomeJogadorAtual[tamanhoAtual + 1] = '\0';
        }

        caractere = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
        int tamanhoAtual = (int)strlen(jogo->nomeJogadorAtual);

        if (tamanhoAtual > 0) {
            jogo->nomeJogadorAtual[tamanhoAtual - 1] = '\0';
        }
    }
}

static void SalvarPontuacaoAtual(Jogo *jogo) {
    if (strlen(jogo->nomeJogadorAtual) == 0) {
        strncpy(jogo->nomeJogadorAtual, "JOGADOR", MAX_NOME_JOGADOR - 1);
        jogo->nomeJogadorAtual[MAX_NOME_JOGADOR - 1] = '\0';
    }

    AdicionarPontuacaoAoRanking(&jogo->ranking, jogo->nomeJogadorAtual, jogo->pontuacao);
    SalvarRanking(&jogo->ranking, ARQUIVO_RANKING);
    jogo->editandoNome = 0;
}

static void AtualizarTelaJogando(Jogo *jogo) {
    float deltaTime = GetFrameTime();
    float intervaloAtualSpawn = jogo->intervaloSpawnNpc;
    float velocidadeAtualFolioes = jogo->velocidadeBaseFolioes;
    int nivelMultidao = 1 + (int)(jogo->tempoPontuacao / 18.0f);
    static float temporizadorMultidaoDoGalo = 0.0f;

    AtualizarJogador(&jogo->jogador, LARGURA_TELA, ALTURA_TELA);
    AtualizarEventoGalo(jogo, deltaTime);

    jogo->tempoPontuacao += deltaTime;
    jogo->pontuacao = (int)(jogo->tempoPontuacao * 10.0f);
    jogo->temporizadorSpawnNpc += deltaTime;
    jogo->temporizadorDificuldade += deltaTime;

    if (jogo->temporizadorDificuldade >= 8.0f) {
        jogo->temporizadorDificuldade = 0.0f;
        jogo->velocidadeBaseFolioes += 20.0f;

        if (jogo->intervaloSpawnNpc > 0.32f) {
            jogo->intervaloSpawnNpc -= 0.06f;
        }
    }

    if (jogo->eventoGaloAtivo) {
        intervaloAtualSpawn *= 0.55f;
        velocidadeAtualFolioes += 80.0f;
        nivelMultidao += 1;
        temporizadorMultidaoDoGalo += deltaTime;
    } else {
        temporizadorMultidaoDoGalo = 0.0f;
    }

    if (jogo->temporizadorSpawnNpc >= intervaloAtualSpawn) {
        jogo->temporizadorSpawnNpc = 0.0f;

        if (nivelMultidao > 5) {
            nivelMultidao = 5;
        }

        GerarGrupoAleatorio(&jogo->listaFolioes, LARGURA_TELA, ALTURA_TELA, velocidadeAtualFolioes, nivelMultidao);
    }

    if (jogo->eventoGaloAtivo && temporizadorMultidaoDoGalo >= 0.55f) {
        float faixaProxima = jogo->zonaGalo.y + jogo->zonaGalo.height * 0.5f;
        int tamanhoExtra = 3 + nivelMultidao;

        temporizadorMultidaoDoGalo = 0.0f;

        if (tamanhoExtra > 7) {
            tamanhoExtra = 7;
        }

        GerarFluxoEmFaixaY(&jogo->listaFolioes, LARGURA_TELA, faixaProxima - 18.0f, velocidadeAtualFolioes + 20.0f, tamanhoExtra);
        GerarFluxoEmFaixaY(&jogo->listaFolioes, LARGURA_TELA, faixaProxima + 20.0f, velocidadeAtualFolioes + 10.0f, tamanhoExtra - 1);
    }

    AtualizarFolioes(&jogo->listaFolioes, deltaTime);
    RemoverFolioesForaDaTela(&jogo->listaFolioes, LARGURA_TELA, ALTURA_TELA);

    if (jogo->temporizadorAvisoGalo > 0) {
        jogo->temporizadorAvisoGalo--;
    }

    if (VerificarColisaoJogadorFolioes(&jogo->listaFolioes, jogo->jogador.corpo) ||
        VerificarColisaoComGalo(jogo)) {
        PrepararEntradaNome(jogo);
        jogo->telaAtual = TELA_GAME_OVER;
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        jogo->telaAtual = TELA_MENU;
    }
}

static void AtualizarTelaGameOver(Jogo *jogo) {
    if (jogo->editandoNome) {
        ProcessarDigitacaoNome(jogo);

        if (IsKeyPressed(KEY_ENTER)) {
            SalvarPontuacaoAtual(jogo);
        }

        return;
    }

    if (IsKeyPressed(KEY_ENTER)) {
        ResetarPartida(jogo);
        jogo->telaAtual = TELA_JOGANDO;
    } else if (IsKeyPressed(KEY_R)) {
        jogo->telaAtual = TELA_RANKING;
    } else if (IsKeyPressed(KEY_ESCAPE)) {
        jogo->telaAtual = TELA_MENU;
    }
}

static void DesenharHud(const Jogo *jogo) {
    DrawRectangle(10, 10, 190, 92, Fade(WHITE, 0.85f));
    DrawText(TextFormat("Tempo: %.1f", jogo->tempoPontuacao), 20, 18, 20, BLACK);
    DrawText(TextFormat("Pontuacao: %d", jogo->pontuacao), 20, 44, 20, BLACK);
    DrawText(TextFormat("Multidao: %d", jogo->listaFolioes.quantidade), 20, 70, 18, DARKGRAY);

    if (jogo->eventoGaloAtivo) {
        DrawRectangle(690, 10, 250, 72, Fade(GOLD, 0.85f));
        DrawText("PASSAGEM DO GALO!", 707, 18, 22, MAROON);
        DrawText("Saia da faixa!", 760, 46, 20, BLACK);
    }
}

static void DesenharCenario(const Jogo *jogo) {
    int posicaoX;

    DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, (Color){244, 223, 170, 255});
    DrawRectangle(0, 0, LARGURA_TELA, 92, (Color){226, 205, 156, 255});
    DrawRectangle(0, ALTURA_TELA - 92, LARGURA_TELA, 92, (Color){226, 205, 156, 255});
    DrawRectangle(0, 92, LARGURA_TELA, ALTURA_TELA - 184, (Color){124, 132, 138, 255});

    for (posicaoX = 0; posicaoX < LARGURA_TELA; posicaoX += 90) {
        DrawRectangle(posicaoX + 22, 244, 46, 12, (Color){247, 237, 170, 255});
        DrawRectangle(posicaoX + 22, 284, 46, 12, (Color){247, 237, 170, 255});
        DrawRectangle(posicaoX + 22, 324, 46, 12, (Color){247, 237, 170, 255});
    }

    DrawRectangle(0, 92, LARGURA_TELA, 12, (Color){180, 160, 116, 255});
    DrawRectangle(0, ALTURA_TELA - 104, LARGURA_TELA, 12, (Color){180, 160, 116, 255});
    DesenharFileiraConfetes(10);
    DesenharFileiraConfetes(ALTURA_TELA - 70);

    DrawText("Avenida do Galo da Madrugada", 285, 18, 30, MAROON);
    DrawText("Sobreviva a multidao do Carnaval do Recife", 240, 52, 20, DARKBROWN);

    if (jogo->eventoGaloAtivo) {
        DrawRectangle((int)jogo->zonaGalo.x - 12, (int)jogo->zonaGalo.y + 30, (int)jogo->zonaGalo.width + 24, 20, Fade(WHITE, 0.45f));
        DesenharEventoDoGalo(jogo);
    }

    if (jogo->temporizadorAvisoGalo > 0) {
        DrawRectangle(250, 85, 460, 36, Fade(MAROON, 0.85f));
        DrawText("ATENCAO: o Galo esta passando pela avenida!", 270, 93, 20, RAYWHITE);
    }
}

static void DesenharTelaJogando(const Jogo *jogo) {
    DesenharCenario(jogo);
    DrawText("Desvie da multidao e da passagem do Galo!", 20, ALTURA_TELA - 35, 20, DARKGRAY);
    DesenharJogador(&jogo->jogador,
                    &jogo->texturaJogadorFrente,
                    &jogo->texturaJogadorCostas,
                    &jogo->texturaJogadorEsquerda,
                    &jogo->texturaJogadorDireita);
    DesenharFolioes(&jogo->listaFolioes, &jogo->texturaFoliaoA, &jogo->texturaFoliaoB);
    DesenharHud(jogo);
}

static void DesenharTelaRanking(const Jogo *jogo) {
    int indice;

    DrawText("RANKING", 385, 50, 38, MAROON);
    DrawText("Bubble Sort organiza do maior para o menor", 250, 100, 20, DARKGRAY);

    for (indice = 0; indice < jogo->ranking.quantidade; indice++) {
        DrawText(TextFormat("%d. %s - %d", indice + 1, jogo->ranking.registros[indice].nome, jogo->ranking.registros[indice].pontuacao),
                 250, 160 + indice * 30, 24, BLACK);
    }

    if (jogo->ranking.quantidade == 0) {
        DrawText("Nenhuma pontuacao registrada ainda.", 290, 220, 24, GRAY);
    }

    DrawText("Pressione ESC ou ENTER para voltar", 280, 475, 22, DARKGRAY);
}

static void DesenharTelaGameOver(const Jogo *jogo) {
    DrawText("GAME OVER", 340, 110, 54, RED);
    DrawText(TextFormat("Pontuacao final: %d", jogo->pontuacao), 330, 190, 32, BLACK);

    if (jogo->editandoNome) {
        Rectangle caixaNome = {250.0f, 250.0f, 460.0f, 54.0f};

        DrawText("Digite seu nome para o ranking:", 275, 220, 24, DARKGRAY);
        DrawRectangleRounded(caixaNome, 0.15f, 8, Fade(WHITE, 0.9f));
        DrawRectangleRoundedLinesEx(caixaNome, 0.15f, 8, 2.0f, MAROON);
        DrawText(jogo->nomeJogadorAtual, 270, 266, 28, BLACK);
        DrawText("ENTER para salvar", 360, 330, 22, DARKGRAY);
    } else {
        DrawText("ENTER: jogar novamente", 325, 280, 24, DARKGRAY);
        DrawText("R: ver ranking", 380, 315, 24, DARKGRAY);
        DrawText("ESC: voltar ao menu", 345, 350, 24, DARKGRAY);
    }
}

void InicializarJogo(Jogo *jogo) {
    jogo->executando = 1;
    jogo->telaAtual = TELA_MENU;
    jogo->texturaChao = CarregarTexturaSeExistir("assets/tiles/kenney_tilesheet_complete_2X.png");
    jogo->texturaFoliaoA = CarregarTexturaSeExistir("assets/sprites/npc_man_blue.png");
    jogo->texturaFoliaoB = CarregarTexturaSeExistir("assets/sprites/npc_woman_green.png");
    jogo->texturaGalo = CarregarTexturaSeExistir("assets/sprites/galo_evento.png");
    jogo->texturaJogadorFrente = CarregarTexturaSeExistir("assets/sprites/jogador_frente.png");
    jogo->texturaJogadorCostas = CarregarTexturaSeExistir("assets/sprites/jogador_costas.png");
    jogo->texturaJogadorEsquerda = CarregarTexturaSeExistir("assets/sprites/jogador_esquerda.png");
    jogo->texturaJogadorDireita = CarregarTexturaSeExistir("assets/sprites/jogador_direita.png");

    InicializarJogador(&jogo->jogador, 460.0f, 250.0f);
    InicializarListaFolioes(&jogo->listaFolioes);
    InicializarMenu(&jogo->menu);
    CarregarRanking(&jogo->ranking, ARQUIVO_RANKING);
    ResetarPartida(jogo);
}

void ResetarPartida(Jogo *jogo) {
    ResetarJogador(&jogo->jogador, 460.0f, 250.0f);
    LimparListaFolioes(&jogo->listaFolioes);
    jogo->tempoPontuacao = 0.0f;
    jogo->pontuacao = 0;
    jogo->temporizadorSpawnNpc = 0.0f;
    jogo->intervaloSpawnNpc = 0.95f;
    jogo->temporizadorDificuldade = 0.0f;
    jogo->velocidadeBaseFolioes = 120.0f;
    jogo->eventoGaloAtivo = 0;
    jogo->temporizadorEventoGalo = 0.0f;
    jogo->recargaEventoGalo = 0.0f;
    jogo->duracaoEventoGalo = 8.5f;
    jogo->zonaGalo = (Rectangle){-210.0f, 180.0f, 210.0f, 92.0f};
    jogo->velocidadeZonaGalo = 135.0f;
    jogo->temporizadorAvisoGalo = 0;
    jogo->nomeJogadorAtual[0] = '\0';
    jogo->editandoNome = 0;
}

void AtualizarJogo(Jogo *jogo) {
    if (jogo->telaAtual == TELA_MENU) {
        int opcaoEscolhida = AtualizarMenu(&jogo->menu);

        if (opcaoEscolhida == OPCAO_MENU_JOGAR) {
            ResetarPartida(jogo);
            jogo->telaAtual = TELA_JOGANDO;
        } else if (opcaoEscolhida == OPCAO_MENU_RANKING) {
            jogo->telaAtual = TELA_RANKING;
        } else if (opcaoEscolhida == OPCAO_MENU_SAIR) {
            jogo->executando = 0;
        }
    } else if (jogo->telaAtual == TELA_JOGANDO) {
        AtualizarTelaJogando(jogo);
    } else if (jogo->telaAtual == TELA_RANKING) {
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            jogo->telaAtual = TELA_MENU;
        }
    } else if (jogo->telaAtual == TELA_GAME_OVER) {
        AtualizarTelaGameOver(jogo);
    }
}

void DesenharJogo(const Jogo *jogo) {
    if (jogo->telaAtual == TELA_MENU) {
        DesenharMenu(&jogo->menu);
    } else if (jogo->telaAtual == TELA_JOGANDO) {
        DesenharTelaJogando(jogo);
    } else if (jogo->telaAtual == TELA_RANKING) {
        DesenharTelaRanking(jogo);
    } else if (jogo->telaAtual == TELA_GAME_OVER) {
        DesenharTelaGameOver(jogo);
    }
}

void EncerrarJogo(Jogo *jogo) {
    LimparListaFolioes(&jogo->listaFolioes);
    SalvarRanking(&jogo->ranking, ARQUIVO_RANKING);

    if (jogo->texturaChao.id > 0) {
        UnloadTexture(jogo->texturaChao);
    }
    if (jogo->texturaFoliaoA.id > 0) {
        UnloadTexture(jogo->texturaFoliaoA);
    }
    if (jogo->texturaFoliaoB.id > 0) {
        UnloadTexture(jogo->texturaFoliaoB);
    }
    if (jogo->texturaGalo.id > 0) {
        UnloadTexture(jogo->texturaGalo);
    }
    if (jogo->texturaJogadorFrente.id > 0) {
        UnloadTexture(jogo->texturaJogadorFrente);
    }
    if (jogo->texturaJogadorCostas.id > 0) {
        UnloadTexture(jogo->texturaJogadorCostas);
    }
    if (jogo->texturaJogadorEsquerda.id > 0) {
        UnloadTexture(jogo->texturaJogadorEsquerda);
    }
    if (jogo->texturaJogadorDireita.id > 0) {
        UnloadTexture(jogo->texturaJogadorDireita);
    }
}
