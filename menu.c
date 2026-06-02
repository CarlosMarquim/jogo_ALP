#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef enum GameState
{
    MENU,
    GAME,
    INSTRUCTIONS,
    PAUSED,
    GAME_OVER
} GameState;

int main(void)
{

    InitWindow(800, 450, "Meu Jogo");
    SetTargetFPS(60);

    GameState estadoAtual = MENU;

    // Posição do jogador (reset ao reiniciar)
    Vector2 jogador = { 400, 225 };
    float velocidade = 4.0f;

    // =========================================================
    // DECLARE AQUI AS VARIÁVEIS DO SEU JOGO
    // Ex: int pontuacao = 0; int vidas = 3; etc.
    // =========================================================

    while (!WindowShouldClose())
    {
        if (estadoAtual == MENU)
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                // Reset do estado do jogo ao iniciar
                jogador = (Vector2){ 400, 225 };

                // =====================================================
                // RESET DAS VARIÁVEIS DO SEU JOGO AQUI
                // Ex: pontuacao = 0; vidas = 3; etc.
                // =====================================================

                estadoAtual = GAME;
            }
            if (IsKeyPressed(KEY_I))
            {
                estadoAtual = INSTRUCTIONS;
            }
            if (IsKeyPressed(KEY_ESCAPE))
            {
                break; // Fecha a janela
            }
        }

        // --- INSTRUÇÕES ---
        else if (estadoAtual == INSTRUCTIONS)
        {
            if (IsKeyPressed(KEY_M))
            {
                estadoAtual = MENU;
            }
        }

        // --- JOGO RODANDO ---
        else if (estadoAtual == GAME)
        {
            // Pausa com barra de espaço
            if (IsKeyPressed(KEY_SPACE))
            {
                estadoAtual = PAUSED;
            }

            // =====================================================
            // LÓGICA DO SEU JOGO AQUI
            // Ex: movimentação, colisões, spawns, etc.
            // =====================================================

            // Exemplo de movimentação (pode remover):
            if (IsKeyDown(KEY_RIGHT)) jogador.x += velocidade;
            if (IsKeyDown(KEY_LEFT))  jogador.x -= velocidade;
            if (IsKeyDown(KEY_UP))    jogador.y -= velocidade;
            if (IsKeyDown(KEY_DOWN))  jogador.y += velocidade;

            // =====================================================
            // CONDIÇÃO DE DERROTA
            // Quando o jogador perder, mude o estado:
            //     estadoAtual = GAME_OVER;
            // =====================================================
        }

        // --- PAUSADO ---
        else if (estadoAtual == PAUSED)
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                estadoAtual = GAME; // Retoma o jogo
            }
            if (IsKeyPressed(KEY_M))
            {
                estadoAtual = MENU; // Volta ao menu sem reiniciar
            }
        }

        // --- GAME OVER ---
        else if (estadoAtual == GAME_OVER)
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                // Reset completo e reinicia o jogo
                jogador = (Vector2){ 400, 225 };

                // =====================================================
                // RESET DAS VARIÁVEIS DO SEU JOGO AQUI (igual ao MENU)
                // =====================================================

                estadoAtual = GAME;
            }
            if (IsKeyPressed(KEY_M))
            {
                estadoAtual = MENU;
            }
        }

        // =========================
        // DRAW
        // =========================
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // --- DESENHA O JOGO (base, sempre visível em GAME e PAUSED) ---
        if (estadoAtual == GAME || estadoAtual == PAUSED)
        {
            // =====================================================
            // DESENHO DO SEU JOGO AQUI
            // =====================================================

            // Exemplo (pode remover):
            DrawText("JOGO RODANDO", 20, 20, 24, DARKBLUE);
            DrawRectangleV(jogador, (Vector2){ 40, 40 }, BLUE);
        }

        // --- MENU PRINCIPAL ---
        if (estadoAtual == MENU)
        {
            DrawRectangle(0, 0, 800, 450, RAYWHITE);

            DrawText("MEU JOGO",               290, 80,  42, DARKBLUE);
            DrawText("ENTER  -  Jogar",         295, 180, 24, BLACK);
            DrawText("I      -  Instrucoes",    295, 220, 24, DARKGRAY);
            DrawText("ESC    -  Sair",          295, 260, 24, MAROON);
        }

        // --- INSTRUÇÕES ---
        else if (estadoAtual == INSTRUCTIONS)
        {
            DrawRectangle(0, 0, 800, 450, RAYWHITE);

            DrawText("INSTRUCOES",                             290,  60, 36, DARKBLUE);

            // =====================================================
            // ESCREVA AS INSTRUÇÕES DO SEU JOGO AQUI
            // =====================================================
            DrawText("Use as setas para mover o jogador.",    160, 160, 22, BLACK);
            DrawText("Colete itens para ganhar pontos.",       160, 200, 22, BLACK);
            DrawText("Nao deixe sua vida chegar a zero!",      160, 240, 22, BLACK);

            DrawText("M  -  Voltar ao Menu",                  160, 340, 20, MAROON);
        }

        else if (estadoAtual == PAUSED)
        {
            DrawRectangle(0, 0, 800, 450, (Color){ 0, 0, 0, 120 });

            DrawRectangleRounded((Rectangle){ 250, 130, 300, 180 }, 0.2f, 8, LIGHTGRAY);
            DrawRectangleRoundedLines((Rectangle){ 250, 130, 300, 180 }, 0.2f, 8, DARKGRAY);

            DrawText("PAUSADO",          322, 155, 28, DARKBLUE);
            DrawText("ENTER - Continuar", 278, 210, 20, BLACK);
            DrawText("M     - Menu",      278, 245, 20, MAROON);
        }

        else if (estadoAtual == GAME_OVER)
        {
            DrawRectangle(0, 0, 800, 450, RAYWHITE);

            DrawText("GAME OVER",              255, 110, 54, RED);
            DrawText("ENTER  -  Jogar novamente", 240, 230, 22, BLACK);
            DrawText("M      -  Menu principal",  240, 270, 22, DARKGRAY);
        }

        EndDrawing();
    }

    // =====================================================
    // LIBERE RECURSOS DO SEU JOGO AQUI
    // Ex: UnloadTexture(), UnloadSound(), etc.
    // =====================================================

    CloseWindow();
    return 0;
}
