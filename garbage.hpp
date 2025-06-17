#pragma once
#include "gameDefinitions.hpp" // Para TrashType

// --- Definição da classe Trash ---
enum TrashType {
    PAPER = 1,
    PLASTIC = 2,
    METAL = 3,
    GLASS = 4
};
class Trash {
public:
    float x, y, z;
    TrashType type;
    bool collect;
};

// --- Protótipos das funções ---
void drawTrash(); // Passa um único item de lixo
void desenharLixoPapel();
void desenharLixoPlastico();
void desenharLixoMetal();
void desenharLixoVidro();