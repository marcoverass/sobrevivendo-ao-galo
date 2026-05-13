# Sobrevivendo ao Galo

Jogo em C com Raylib desenvolvido para a atividade pratica de AED, dentro do tema **Na Vibe do Recife**.

O jogador controla um foliao em meio ao Carnaval do Recife e precisa sobreviver ao fluxo da multidao, desviando dos foliões e da passagem do Galo da Madrugada para conseguir a maior pontuacao possivel.

## Estrutura do projeto

- `src/`: arquivos `.c`
- `include/`: arquivos `.h`
- `assets/`: sprites, imagens e creditos
- `ranking.txt`: arquivo salvo com nomes e pontuacoes

## Modulos principais

- `main.c`: cria a janela e executa o loop principal
- `game.c`: controla telas, partida, pontuacao, evento do Galo e fluxo geral do jogo
- `menu.c`: menu inicial com teclado e mouse
- `player.c`: movimentacao e desenho do jogador
- `npc_list.c`: lista encadeada dos foliões ativos
- `ranking.c`: ranking de pontuacoes com Bubble Sort

## Mecanica atual

- movimentacao do jogador com `WASD` ou setas
- menu inicial interativo com teclado e mouse
- foliões surgindo em grupos e faixas de fluxo, simulando uma multidao
- dificuldade aumentando com o tempo
- evento especial da `Passagem do Galo`
- durante o evento, o Galo passa mais devagar pela avenida e atrai mais multidao para a regiao
- tela de game over com entrada de nome do jogador
- ranking salvo com nome e pontuacao

## Estrutura de dados principal

O jogo usa **lista encadeada** como estrutura central para controlar os foliões ativos da tela.

Cada foliao possui:

- posicao
- velocidade
- tipo
- dados de movimento
- ponteiro para o proximo elemento

### Funcoes ligadas a lista encadeada

O projeto possui mais de 6 funcoes relacionadas diretamente a estrutura de dados:

1. `InicializarListaFolioes`
2. `CriarFoliao`
3. `InserirFoliao`
4. `GerarGrupoAleatorio`
5. `GerarFluxoEmFaixaY`
6. `AtualizarFolioes`
7. `DesenharFolioes`
8. `VerificarColisaoJogadorFolioes`
9. `RemoverFolioesForaDaTela`
10. `LimparListaFolioes`

## Algoritmo de ordenacao

O ranking usa **Bubble Sort** para ordenar as pontuacoes do maior valor para o menor valor.

## Visual atual

- jogador com sprites em 4 direcoes
- NPCs com sprites simples
- Galo com sprite proprio em uma estrutura alegorica
- cenario de avenida carnavalesca
- creditos dos assets em `assets/credits/asset_sources.md`

## Como compilar no Windows

O projeto usa o Raylib instalado em:

- `C:/raylib/w64devkit`

### Compilar pelo arquivo `.bat`

```powershell
.\compilar.bat
```

### Executar o jogo

```powershell
.\sobrevivendo_ao_galo.exe
```

### Compilar direto pelo terminal

```powershell
C:\raylib\w64devkit\bin\gcc src/main.c src/game.c src/menu.c src/player.c src/npc_list.c src/ranking.c -o sobrevivendo_ao_galo -Iinclude -IC:/raylib/w64devkit/include -LC:/raylib/w64devkit/lib -lraylib -lopengl32 -lgdi32 -lwinmm
```

## Controles

- `WASD` ou setas: mover jogador
- `Mouse`: navegar e clicar no menu
- `Enter`: confirmar opcoes e salvar nome no game over
- `Esc`: voltar
- `R`: abrir ranking na tela de game over

## Observacao para entrega

Na hora de enviar o projeto da atividade, o ideal e compactar apenas os arquivos necessarios para reproducao do programa e **nao incluir os executaveis `.exe`**.
