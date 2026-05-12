@echo off
setlocal

set RAYLIB_PATH=C:\raylib\w64devkit
set RAYLIB_INCLUDE=%RAYLIB_PATH%\include
set RAYLIB_LIB=%RAYLIB_PATH%\lib
set RAYLIB_BIN=%RAYLIB_PATH%\bin
set PATH=%RAYLIB_BIN%;%PATH%

gcc src\main.c src\game.c src\menu.c src\player.c src\npc_list.c src\ranking.c ^
 -o sobrevivendo_ao_galo.exe ^
 -Iinclude -I%RAYLIB_INCLUDE% ^
 -L%RAYLIB_LIB% -lraylib -lopengl32 -lgdi32 -lwinmm

if %errorlevel% neq 0 (
    echo.
    echo Falha na compilacao.
    echo Verifique se o Raylib esta instalado em:
    echo %RAYLIB_INCLUDE%
    echo %RAYLIB_LIB%
    echo Se estiver em outra pasta, edite a linha RAYLIB_PATH deste arquivo.
    exit /b 1
)

echo.
echo Compilacao concluida com sucesso.
