#pragma once
#include "gameDefinitions.hpp"

enum LaneType {
    GRASS,
    ROAD
};
class Lane {
public:
LaneType type;
float z;
};

// --- Protótipos das funções ---
void drawLane();
void drawGrass(Lane lane);
void drawRoad(Lane lane);
void randomLane(int z);
void drawInicialMap();
void drawDump(int); 