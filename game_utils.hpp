#pragma once
#include "gameDefinitions.hpp" // Para screenWidth, screenHeight, mensagem, text, tempoInicioMensagem


// --- Protótipos das funções de utilidade ---
void desenharTexto(float yPixel, const std::string& texto, float scale);
void exibirMensagem(const std::string& texto);
void subirNivel(); 

enum GameState {
    MENU,
    PLAYING,
    PAUSED
};
extern GameState currentGameState; // Estado atual do jogo

// Protótipos de funções relacionadas ao menu
void drawMenu();
void drawPauseMenu();
void mouseClick(int button, int state, int x, int y);
void startGame();
void drawButton(float x, float y, float width, float height, const char* label);