#include <vector>

typedef struct {
    int paper;
    int plastic;
    int metal;
    int glass;
}INVENTARIO;

class Player {
public:
    float x, y, z;      
    float size; 
    int score;
    INVENTARIO inv;
    Player(int x, int y, int z){
        this->x = x;
        this->y = y;
        this->z = z;
        this->size = 2.0f;
        this->score = 0;
        this->inv.paper = this->inv.plastic = this->inv.metal = this->inv.glass = 0;
    }
    void drawPlayer();   
    void zerarPlayer();
};

class Car {
public:
    float x, y, z;
    float speed;
    int dir;         // 1 para direita, 0 para esquerda
};

class Tree{
public:
    float x;
    float z;
    float y;
};

enum TrashType {PAPER = 1, PLASTIC = 2, METAL = 3, GLASS = 4};
class Trash{
public: 
    TrashType type;
    float z; 
    float x;
    float y;
    bool collect;
};
enum LaneType { ROAD=0, GRASS=1};

class Lane {
public:
    LaneType type;
    float z;
};