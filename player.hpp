#pragma once
#include "gameDefinitions.hpp"

struct Inventory {
    int paper;
    int plastic;
    int metal;
    int glass;
};

// --- Declaração da classe Player ---
class Player {
public:
    float x, y, z;
    float size;
    int score;
    Inventory inv;

    Player(float x, float y, float z){
    this->x = x;
    this->y = y;
    this->z = z;
    this->size = 2.0f;
    this->score = 0;
    this->inv.paper = this->inv.plastic = this->inv.metal = this->inv.glass = 0;}
    void zerarPlayer();
    void drawPlayer(Player);
};

// Declaração da função drawChicken
void drawChicken();