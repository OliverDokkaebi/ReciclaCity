#include <vector>

class Player {
public:
    float x, y, z;      
    float size; 
    int score;
    Player(int x, int y, int z){
        this->x = x;
        this->y = y;
        this->z = z;
        this->size = 2.0f;
        this->score = 0;
    }
    void drawPlayer();   
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
};

enum TrashType {PAPER = 1, PLASTIC = 2, METAL = 3, GLASS = 4};
class Trash{
public: 
    TrashType trash;
    bool isDump;
    float z;
    float x;
};
enum LaneType { ROAD=0, GRASS=1};

class Lane {
public:
    LaneType type;
    float z;
    std::vector<Car> carros;
    std::vector<Tree> arvores;
};