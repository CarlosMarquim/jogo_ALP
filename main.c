// =========================================
// IMPORTS
// =========================================

#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// =========================================
// CONSTANTES
// =========================================

#define MAX_PARTICULAS 200
#define MAX_INIMIGOS     1

// =========================================
// STRUCT BLOCO DE MEMÓRIA
// =========================================

typedef struct BlocoMemoria
{
    Vector2 posicao;
    float tamanho;
    bool ativo;

} BlocoMemoria;

// =========================================
// STRUCT PARTICULA
// =========================================

typedef struct Particula
{
    Vector2 posicao;
    Vector2 velocidade;
    float tamanho;
    float tempoVida;
    bool ativa;
    Color cor;

} Particula;

// =========================================
// STRUCT INIMIGO
// =========================================

typedef struct Inimigo
{
    Vector2 posicao;
    bool ativo;

} Inimigo;

// =========================================
// FUNÇÃO: CRIAR EXPLOSÃO
// =========================================

void CriarExplosao(Particula particulas[], Vector2 origem, Color cor)
{
    for (int i = 0; i < MAX_PARTICULAS; i++)
    {
        if (!particulas[i].ativa)
        {
            particulas[i].ativa = true;
            particulas[i].posicao = origem;

            particulas[i].velocidade.x =
                GetRandomValue(-200, 200) / 100.0f;

            particulas[i].velocidade.y =
                GetRandomValue(-200, 200) / 100.0f;

            particulas[i].tamanho =
                GetRandomValue(3, 8);

            particulas[i].tempoVida = 1.0f;
            particulas[i].cor = cor;
        }
    }
}

// =========================================
// MAIN
// =========================================

int main(void)
{
    // =========================================
    // JANELA E ÁUDIO
    // =========================================

    InitWindow(800, 450, "Jogo dos Ponteiros e malloc");
    InitAudioDevice();
    SetTargetFPS(60);

    // =========================================
    // MÚSICA E EFEITOS SONOROS
    // =========================================

    Music musica = LoadMusicStream("fundo.ogg");
    Sound coleta = LoadSound("coleta.wav");
    PlayMusicStream(musica);

    // =========================================
    // JOGADOR
    // =========================================

    Vector2 jogador = { 400, 225 };
    float velocidade = 4.0f;

    // =========================================
    // VIDAS
    // =========================================

    int vidas = 3;
    float invencivelTimer = 0.0f;

    // =========================================
    // GAME OVER
    // =========================================

    bool gameOver = false;

    // =========================================
    // CONTROLE DE MEMÓRIA
    // =========================================

    int capacidade = 3;
    int quantidade = 3;
    int pontos = 0;

    // =========================================
    // MALLOC
    // =========================================

    BlocoMemoria *blocos =
        malloc(capacidade * sizeof(BlocoMemoria));

    if (blocos == NULL)
    {
        printf("Erro ao alocar memoria!\n");
        CloseAudioDevice();
        CloseWindow();
        return 1;
    }

    // =========================================
    // INICIALIZA BLOCOS
    // =========================================

    for (int i = 0; i < quantidade; i++)
    {
        blocos[i].posicao.x = GetRandomValue(50, 750);
        blocos[i].posicao.y = GetRandomValue(50, 400);
        blocos[i].tamanho = 25;
        blocos[i].ativo = true;
    }

    // =========================================
    // PARTICULAS
    // =========================================

    Particula particulas[MAX_PARTICULAS] = { 0 };

    // =========================================
    // INIMIGOS
    // =========================================

    Inimigo inimigos[MAX_INIMIGOS] = { 0 };

    float timerSpawn = 0.0f;

    // Inicializa 1 inimigo de começo
    inimigos[0].ativo = true;
    inimigos[0].posicao = (Vector2){ 0, 200 };

    // =========================================
    // GAME LOOP
    // =========================================

    while (!WindowShouldClose())
    {
        float delta = GetFrameTime();

        UpdateMusicStream(musica);

        // =========================================
        // TELA DE GAME OVER
        // =========================================

        if (gameOver)
        {
            BeginDrawing();

            ClearBackground(BLACK);

            DrawText(
                "GAME OVER",
                250, 160, 60, RED
            );

            DrawText(
                TextFormat("Pontos: %d", pontos),
                320, 250, 30, WHITE
            );

            DrawText(
                "Pressione R para reiniciar",
                220, 320, 24, GRAY
            );

            EndDrawing();

            if (IsKeyPressed(KEY_R))
            {
                // Reseta jogador
                jogador = (Vector2){ 400, 225 };
                vidas = 3;
                invencivelTimer = 0;
                gameOver = false;

                // Reseta blocos
                pontos = 0;
                capacidade = 3;
                quantidade = 3;
                free(blocos);
                blocos = malloc(capacidade * sizeof(BlocoMemoria));
                for (int i = 0; i < quantidade; i++)
                {
                    blocos[i].posicao.x = GetRandomValue(50, 750);
                    blocos[i].posicao.y = GetRandomValue(50, 400);
                    blocos[i].tamanho = 25;
                    blocos[i].ativo = true;
                }

                // Reseta inimigo
                for (int i = 0; i < MAX_INIMIGOS; i++)
                    inimigos[i].ativo = false;

                inimigos[0].ativo = true;
                inimigos[0].posicao = (Vector2){ 0, 200 };

                // Reseta particulas
                for (int i = 0; i < MAX_PARTICULAS; i++)
                    particulas[i].ativa = false;

                timerSpawn = 0;
            }

            continue;
        }

        // =========================================
        // TIMERS
        // =========================================

        timerSpawn += delta;
        if (invencivelTimer > 0)
            invencivelTimer -= delta;

        // =========================================
        // INPUT - MOVIMENTO
        // =========================================

        if (IsKeyDown(KEY_RIGHT))
            jogador.x += velocidade;

        if (IsKeyDown(KEY_LEFT))
            jogador.x -= velocidade;

        if (IsKeyDown(KEY_UP))
            jogador.y -= velocidade;

        if (IsKeyDown(KEY_DOWN))
            jogador.y += velocidade;

        // =========================================
        // HITBOX DO JOGADOR
        // =========================================

        Rectangle rectJogador =
        {
            jogador.x,
            jogador.y,
            30,
            30
        };

        // =========================================
        // UPDATE - INIMIGO (persegue o jogador)
        // =========================================

        // Respawna inimigo após 3 segundos se não estiver ativo
        if (!inimigos[0].ativo && timerSpawn >= 1.5f)
        {
            timerSpawn = 0;
            inimigos[0].ativo = true;

            int lado = GetRandomValue(0, 3);
            if (lado == 0) inimigos[0].posicao = (Vector2){   0, GetRandomValue(0, 450) };
            if (lado == 1) inimigos[0].posicao = (Vector2){ 800, GetRandomValue(0, 450) };
            if (lado == 2) inimigos[0].posicao = (Vector2){ GetRandomValue(0, 800),   0 };
            if (lado == 3) inimigos[0].posicao = (Vector2){ GetRandomValue(0, 800), 450 };
        }

        for (int i = 0; i < MAX_INIMIGOS; i++)
        {
            if (!inimigos[i].ativo) continue;

            // Persegue o jogador
            float dx = jogador.x - inimigos[i].posicao.x;
            float dy = jogador.y - inimigos[i].posicao.y;
            float dist = sqrtf(dx*dx + dy*dy);

            if (dist > 0)
            {
                inimigos[i].posicao.x += (dx / dist) * 3.0f;
                inimigos[i].posicao.y += (dy / dist) * 3.0f;
            }
        }

        // =========================================
        // COLISÃO: INIMIGO x JOGADOR (dano)
        // =========================================

        if (invencivelTimer <= 0)
        {
            for (int i = 0; i < MAX_INIMIGOS; i++)
            {
                if (!inimigos[i].ativo) continue;

                Rectangle rectInimigo =
                {
                    inimigos[i].posicao.x,
                    inimigos[i].posicao.y,
                    20, 20
                };

                if (CheckCollisionRecs(rectJogador, rectInimigo))
                {
                    vidas--;
                    invencivelTimer = 2.0f;
                    inimigos[i].ativo = false;
                    timerSpawn = 0;

                    // Explosão vermelha no jogador
                    CriarExplosao(particulas, jogador, RED);

                    if (vidas <= 0)
                        gameOver = true;

                    break;
                }
            }
        }

        // =========================================
        // COLISÃO: JOGADOR x BLOCOS
        // =========================================

        for (int i = 0; i < quantidade; i++)
        {
            if (blocos[i].ativo)
            {
                Rectangle rectBloco =
                {
                    blocos[i].posicao.x,
                    blocos[i].posicao.y,
                    blocos[i].tamanho,
                    blocos[i].tamanho
                };

                if (CheckCollisionRecs(rectJogador, rectBloco))
                {
                    blocos[i].ativo = false;
                    pontos++;
                    PlaySound(coleta);
                }
            }
        }

        // =========================================
        // REALLOC
        // =========================================

        if (pontos == quantidade)
        {
            capacidade += 3;
            quantidade += 3;

            BlocoMemoria *novoEndereco =
                realloc(blocos, capacidade * sizeof(BlocoMemoria));

            if (novoEndereco == NULL)
            {
                printf("Erro ao realocar memoria!\n");
                free(blocos);
                UnloadSound(coleta);
                UnloadMusicStream(musica);
                CloseAudioDevice();
                CloseWindow();
                return 1;
            }

            blocos = novoEndereco;

            for (int i = pontos; i < quantidade; i++)
            {
                blocos[i].posicao.x = GetRandomValue(50, 750);
                blocos[i].posicao.y = GetRandomValue(50, 400);
                blocos[i].tamanho = 25;
                blocos[i].ativo = true;
            }
        }

        // =========================================
        // UPDATE - PARTICULAS
        // =========================================

        for (int i = 0; i < MAX_PARTICULAS; i++)
        {
            if (!particulas[i].ativa) continue;

            particulas[i].posicao.x +=
                particulas[i].velocidade.x * 100 * delta;

            particulas[i].posicao.y +=
                particulas[i].velocidade.y * 100 * delta;

            particulas[i].tempoVida -= delta;
            particulas[i].tamanho   -= delta * 3;

            if (particulas[i].tempoVida <= 0 ||
                particulas[i].tamanho   <= 0)
            {
                particulas[i].ativa = false;
            }
        }

        // =========================================
        // RENDERIZAÇÃO
        // =========================================

        BeginDrawing();

        ClearBackground(RAYWHITE);

        // =========================================
        // DESENHA PARTICULAS
        // =========================================

        for (int i = 0; i < MAX_PARTICULAS; i++)
        {
            if (particulas[i].ativa)
            {
                DrawCircleV(
                    particulas[i].posicao,
                    particulas[i].tamanho,
                    particulas[i].cor
                );
            }
        }

        // =========================================
        // DESENHA BLOCOS
        // =========================================

        for (int i = 0; i < quantidade; i++)
        {
            if (blocos[i].ativo)
            {
                DrawRectangleV(
                    blocos[i].posicao,
                    (Vector2){ blocos[i].tamanho, blocos[i].tamanho },
                    GREEN
                );
            }
        }

        // =========================================
        // DESENHA INIMIGO
        // =========================================

        for (int i = 0; i < MAX_INIMIGOS; i++)
        {
            if (inimigos[i].ativo)
            {
                DrawRectangleV(
                    inimigos[i].posicao,
                    (Vector2){ 20, 20 },
                    RED
                );
            }
        }

        // =========================================
        // DESENHA JOGADOR (pisca se invencível)
        // =========================================

        bool piscar = (int)(invencivelTimer * 10) % 2 == 0;

        if (invencivelTimer <= 0 || piscar)
        {
            DrawRectangleV(
                jogador,
                (Vector2){ 30, 30 },
                BLUE
            );
        }

        // =========================================
        // HUD
        // =========================================

        DrawText(
            "Jogo dos Ponteiros e malloc",
            20, 20, 24, DARKBLUE
        );

        DrawText(
            "Use as setas para coletar blocos de memoria",
            20, 55, 18, DARKGRAY
        );

        DrawText(
            TextFormat("Pontos: %d", pontos),
            20, 90, 20, BLACK
        );

        DrawText(
            TextFormat("Blocos alocados: %d", quantidade),
            20, 115, 20, BLACK
        );

        DrawText(
            TextFormat("Capacidade da memoria: %d", capacidade),
            20, 140, 20, BLACK
        );

        // Vidas (corações)
        DrawText("Vidas:", 20, 170, 20, DARKGRAY);
        for (int i = 0; i < 3; i++)
        {
            Color corCoracao = (i < vidas) ? RED : LIGHTGRAY;
            DrawRectangle(80 + i * 28, 170, 20, 20, corCoracao);
        }

        DrawText(
            "malloc cria | realloc aumenta | free libera",
            20, 410, 18, MAROON
        );

        EndDrawing();
    }

    // =========================================
    // FREE
    // =========================================

    free(blocos);
    UnloadSound(coleta);
    UnloadMusicStream(musica);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
