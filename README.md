# Sobrevivendo ao Galo

Projeto inicial em C com Raylib para a atividade pratica de AED.

## Estrutura

- `src/`: arquivos `.c`
- `include/`: arquivos `.h`
- `ranking.txt`: arquivo gerado para guardar as pontuacoes

## Modulos

- `main.c`: abre a janela e roda o loop principal
- `game.c`: controla estados do jogo, pontuacao e dificuldade
- `menu.c`: menu inicial e navegacao
- `player.c`: movimentacao e desenho do jogador
- `npc_list.c`: lista encadeada dos NPCs ativos
- `ranking.c`: ranking com Bubble Sort

## Mecanica atual

- jogador se move pela avenida com `WASD` ou setas
- NPCs entram pelas bordas da tela usando lista encadeada
- a pontuacao aumenta com o tempo de sobrevivencia
- de tempos em tempos acontece a `Passagem do Galo`
- durante a passagem, surge uma faixa de perigo que atravessa a avenida
- nesse momento, a quantidade e a velocidade dos NPCs aumentam

## Visual atual

- fundo desenhado com tiles do pack `Kenney Top-down Shooter`
- NPCs com sprites simples em vez de apenas retangulos
- base visual do Galo carregada por imagem
- fontes e licencas anotadas em `assets/credits/asset_sources.md`

## Funcoes da lista encadeada

O projeto ja comeca com mais de 6 funcoes ligadas a estrutura de dados:

1. `InitNpcList`
2. `CreateNpc`
3. `InsertNpc`
4. `SpawnRandomNpc`
5. `UpdateNpcs`
6. `DrawNpcs`
7. `CheckPlayerNpcCollision`
8. `RemoveOffscreenNpcs`
9. `ClearNpcList`

## Como compilar no Windows

O `Makefile` e o `compilar.bat` usam a variavel `RAYLIB_PATH`, que por padrao esta em:

- `C:/raylib/w64devkit`

Dentro dela devem existir:

- `include/raylib.h`
- `lib/libraylib.a`

Compile com:

```powershell
mingw32-make
```

Se o `mingw32-make` falhar no seu Windows, use o arquivo:

```powershell
.\compilar.bat
```

Se o Raylib ficar em outra pasta, altere apenas a variavel `RAYLIB_PATH` no `Makefile` e no `compilar.bat`.

Se preferir compilar direto:

```powershell
C:\raylib\w64devkit\bin\gcc src/main.c src/game.c src/menu.c src/player.c src/npc_list.c src/ranking.c -o sobrevivendo_ao_galo -Iinclude -IC:/raylib/w64devkit/include -LC:/raylib/w64devkit/lib -lraylib -lopengl32 -lgdi32 -lwinmm
```

## Controles

- `WASD` ou setas: mover
- `Enter`: confirmar no menu
- `Esc`: voltar
- `R`: abrir ranking na tela de game over
