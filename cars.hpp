#pragma once
#include "gameDefinitions.hpp" // Para ZONA_CARROS e outros

// --- Definição da struct Car ---
struct Car {
    float x, y, z;
    float speed;
    bool dir;
    int color;
    Car() : x(0.0f), y(0.0f), z(0.0f), speed(0.0f), dir(false), color(0) {}
};

// --- Protótipos das funções ---
void updateCars(int value);
void drawCar();