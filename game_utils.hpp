#pragma once
#include "gameDefinitions.hpp" // Para screenWidth, screenHeight, mensagem, text, tempoInicioMensagem


// --- Protótipos das funções de utilidade ---
void desenharTexto(float yPixel, const std::string& texto, float scale);
void exibirMensagem(const std::string& texto);
