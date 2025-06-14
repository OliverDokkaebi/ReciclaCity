#ifndef CAR_H
#define CAR_H

#include <gl/glut.h>

// Declarando variáveis globais usadas na detecção de colisão
extern float gameZ;
extern float gameX;
extern int playerRelativeToGridX;
extern int playerRelativeToGridZ;
extern bool playerIsDead;

// Classe Car
class Car {
    
public:
    // Construtor da classe
    Car(float inputX = -60.0, float inputZ = 0.0, float inputSpeed = 0.1, int inputDirection = 0, int inputLane = 0);
    
    // Método para atualizar posição e verificar colisões
    void Update();
    
    // Método para desenhar o carro
    void draw();
    
    // Variáveis de membro públicas
    float x = -60.0, y = -5.0, z = 0.0;
    int type, direction, lane;
    int dir; // Para compatibilidade com o código existente (1 = esquerda, 0 = direita)
    
private:
    float speed;
};

#endif // CAR_H
