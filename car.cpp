#include "car.h"
#include<vector>

// Define as variáveis globais declaradas como extern no header
float gameZ = -120;
float gameX = 4;

float playerX = 0;
float playerY = -5;
float playerZ = 4;

int playerRelativeToGridX = 7;
int playerRelativeToGridZ = 31;
bool playerIsDead = false;

// Variável para ativar/desativar visualização da área de colisão (apenas para debug)
bool showCollisionAreas = false;

// Função para visualizar as áreas de colisão (útil para debug)
void debugDrawCollisionArea(float x, float z, float width, float length) {
    if (!showCollisionAreas) return;
    
    glPushMatrix();
    glColor4f(1.0f, 0.0f, 0.0f, 0.5f); // Vermelho semi-transparente
    glTranslatef(x+gameX, playerY, z+120+gameZ);
    glScaled(length, 0.1, width);
    glutWireCube(1.0);
    glPopMatrix();
}

Car::Car(float inputX, float inputZ, float inputSpeed, int inputDirection, int inputLane) {
    x = inputX;
    z = inputZ;
    speed = inputSpeed;
    direction = inputDirection;
    lane = inputLane;
    
    // Para compatibilidade com o código existente
    dir = inputDirection;
    
    type = (rand() % 4); // Número aleatório entre 0 e 3
}

void Car::Update() {
    
    if (direction == 0) {
        x+=speed;
        if (x > 60) {
            x = -60.0;
            type = (rand() % ((3 - 0) + 3)) + 0;
        }
    }
    else {
        x-=speed;
        if (x < -60) {
            x = 60;
            type = (rand() % ((3 - 0) + 3)) + 0;
        }
    }    // Verificação de colisão mais precisa baseada na posição real
    // A galinha deve estar na mesma estrada (lane) e a distância em X deve ser pequena
    if (fabs(playerZ - z) < 1.5f) { // Se estão na mesma estrada ou muito próximos
        float carWidth;
        float carLength;
        
        // Determina as dimensões aproximadas do carro com base no tipo
        switch (type) {
            case 0: // Small car
                carWidth = 1.2f;
                carLength = 1.8f;
                break;
            case 1: // Medium car - orange
            case 2: // Medium car purple
            case 3: // Taxi
                carWidth = 1.5f;
                carLength = 3.3f;
                break;
            default:
                carWidth = 1.5f;
                carLength = 3.0f;
        }
        
        // Calcula a distância real entre o centro do carro e o jogador
        float dx = playerX - x;
        
        // Ajusta a largura de colisão com base na direção do carro
        if ((direction == 0 && dx > 0) || (direction == 1 && dx < 0)) {
            // Colisão frontal - usa comprimento maior para simular a frente do carro
            if (fabs(dx) < carLength / 2) {
                playerIsDead = true;
            }
        } else {
            // Colisão lateral - usa largura menor para maior precisão
            if (fabs(dx) < carWidth) {
                playerIsDead = true;
            }
        }
    }
}

std::vector<Car> vectorOfCars;


void Car::draw() {
    // Usamos os membros da classe em vez de parâmetros
    int facing;
    if (direction == 0)
        facing = 1;
    else
        facing = -1;
    
    // Desenha área de colisão para debug
    float carWidth, carLength;
    switch (type) {
        case 0: // Small car
            carWidth = 1.2f;
            carLength = 1.8f;
            break;
        case 1: // Medium car - orange
        case 2: // Medium car purple
        case 3: // Taxi
            carWidth = 1.5f;
            carLength = 3.3f;
            break;
        default:
            carWidth = 1.5f;
            carLength = 3.0f;
    }
    debugDrawCollisionArea(x, z, carWidth, carLength);
    
    switch(type) {
            
            //Small car
        case 0:
            // Body
            glPushMatrix();
            glColor3f(25.0/255.0, 222.0/255.0, 254.0/255.0);
            glTranslatef(x+gameX, y-1.25, z+120+gameZ);
            glScaled(1.2, .5, .8);
            glutSolidCube(3);
            glPopMatrix();
            
            // Upper body
            glPushMatrix();
            glColor3f(1, 1, 1);
            glTranslatef(x+gameX, y+.25, z+120+gameZ);
            glScaled(.7, .4, .5);
            glutSolidCube(3);
            glPopMatrix();
            
            // Window side
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(x+gameX, y-.15, z+120+gameZ);
            glScaled(.5, .25, .51);
            glutSolidCube(3);
            glPopMatrix();
            
            // Window front/back
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(x+gameX, y-.15, z+120+gameZ);
            glScaled(.71, .25, .4);
            glutSolidCube(3);
            glPopMatrix();
            
            // Side mirrors
            glPushMatrix();
            glColor3f(4.0/255.0, 187.0/255.0, 253.0/255.0);
            glTranslatef(x+gameX+(.25*facing), y-.75, z+120+gameZ);
            glScaled(.3, .15, 1);
            glutSolidCube(3);
            glPopMatrix();
            
            // Wheels front
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(x+gameX+(1*facing), y-1.6, z+120+gameZ);
            glScaled(.3, .3, .815);
            glutSolidCube(3);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(1, 1, 1);
            glTranslatef(x+gameX+(1*facing), y-1.6, z+120+gameZ);
            glScaled(.125, .125, .816);
            glutSolidCube(3);
            glPopMatrix();
            
            
            // Wheels back
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(x+gameX-(1*facing), y-1.6, z+120+gameZ);
            glScaled(.3, .3, .815);
            glutSolidCube(3);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(1, 1, 1);
            glTranslatef(x+gameX-(1*facing), y-1.6, z+120+gameZ);
            glScaled(.125, .125, .816);
            glutSolidCube(3);
            glPopMatrix();
            break;
            
            // Medium car - orange
        case 1:
            // Body
            glPushMatrix();
            glColor3f(255.0/255.0, 112.0/255.0, 54.0/255.0);
            glTranslatef(x+gameX, y-1.25, z+120+gameZ);
            glScaled(2.2, .5, 1);
            glutSolidCube(3);
            glPopMatrix();
            
            // Upper body
            glPushMatrix();
            glColor3f(1, 1, 1);
            glTranslatef(x+gameX-(.8*facing), y+.25, z+120+gameZ);
            glScaled(1, .4, .75);
            glutSolidCube(3);
            glPopMatrix();
            
            // Window side
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(x+gameX-(.8*facing), y-.15, z+120+gameZ);
            glScaled(.5, .25, .751);
            glutSolidCube(3);
            glPopMatrix();
            
            // Window front/back
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(x+gameX-(.8*facing), y-.15, z+120+gameZ);
            glScaled(1.1, .25, .5);
            glutSolidCube(3);
            glPopMatrix();
            
            // Side mirrors
            glPushMatrix();
            glColor3f(255.0/255.0, 76.0/255.0, 43.0/255.0);
            glTranslatef(x+gameX+(.25*facing), y-.75, z+120+gameZ);
            glScaled(.3, .15, 1.25);
            glutSolidCube(3);
            glPopMatrix();
            
            // Wheels front
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(x+gameX+(2*facing), y-1.6, z+120+gameZ);
            glScaled(.3, .3, 1.1);
            glutSolidCube(3);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(1, 1, 1);
            glTranslatef(x+gameX+(2*facing), y-1.6, z+120+gameZ);
            glScaled(.125, .125, 1.11);
            glutSolidCube(3);
            glPopMatrix();
            
            
            // Wheels back
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(x+gameX-(2*facing), y-1.6, z+120+gameZ);
            glScaled(.3, .3, 1.1);
            glutSolidCube(3);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(1, 1, 1);
            glTranslatef(x+gameX-(2*facing), y-1.6, z+120+gameZ);
            glScaled(.125, .125, 1.11);
            glutSolidCube(3);
            glPopMatrix();
            break;
            
            // Medium car purple
        case 2:
            // Body
            glPushMatrix();
            glColor3f(161.0/255.0, 141.0/255.0, 255.0/255.0);
            glTranslatef(x+gameX, y-1.25, z+120+gameZ);
            glScaled(2.2, .5, 1);
            glutSolidCube(3);
            glPopMatrix();
            
            // Upper body
            glPushMatrix();
            glColor3f(1, 1, 1);
            glTranslatef(x+gameX-(.8*facing), y+.25, z+120+gameZ);
            glScaled(1, .4, .75);
            glutSolidCube(3);
            glPopMatrix();
            
            // Window side
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(x+gameX-(.8*facing), y-.15, z+120+gameZ);
            glScaled(.5, .25, .751);
            glutSolidCube(3);
            glPopMatrix();
            
            // Window front/back
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(x+gameX-(.8*facing), y-.15, z+120+gameZ);
            glScaled(1.1, .25, .5);
            glutSolidCube(3);
            glPopMatrix();
            
            // Side mirrors
            glPushMatrix();
            glColor3f(147.0/255.0, 97.0/255.0, 255.0/255.0);
            glTranslatef(x+gameX+(.25*facing), y-.75, z+120+gameZ);
            glScaled(.3, .15, 1.25);
            glutSolidCube(3);
            glPopMatrix();
            
            // Wheels front
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(x+gameX+(2*facing), y-1.6, z+120+gameZ);
            glScaled(.3, .3, 1.1);
            glutSolidCube(3);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(1, 1, 1);
            glTranslatef(x+gameX+(2*facing), y-1.6, z+120+gameZ);
            glScaled(.125, .125, 1.11);
            glutSolidCube(3);
            glPopMatrix();
            
            
            // Wheels back
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(x+gameX-(2*facing), y-1.6, z+120+gameZ);
            glScaled(.3, .3, 1.1);
            glutSolidCube(3);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(1, 1, 1);
            glTranslatef(x+gameX-(2*facing), y-1.6, z+120+gameZ);
            glScaled(.125, .125, 1.11);
            glutSolidCube(3);
            glPopMatrix();
            break;
            
            // Taxi
        case 3:
            // Body
            glPushMatrix();
            glColor3f(255.0/255.0, 254.0/255.0, 101.0/255.0);
            glTranslatef(x+gameX, y-1.25, z+120+gameZ);
            glScaled(2.2, .5, 1);
            glutSolidCube(3);
            glPopMatrix();
            
            // Upper body
            glPushMatrix();
            glColor3f(1, 1, 1);
            glTranslatef(x+gameX-(.8*facing), y+.25, z+120+gameZ);
            glScaled(1, .4, .75);
            glutSolidCube(3);
            glPopMatrix();
            
            // Light
            glPushMatrix();
            glColor3f(255.0/255.0, 254.0/255.0, 101.0/255.0);
            glTranslatef(x+gameX-(.8*facing), y+.4, z+120+gameZ);
            glScaled(.5, .4, .25);
            glutSolidCube(3);
            glPopMatrix();
            
            // Window side
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(x+gameX-(.8*facing), y-.15, z+120+gameZ);
            glScaled(.5, .25, .751);
            glutSolidCube(3);
            glPopMatrix();
            
            // Window front/back
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(x+gameX-(.8*facing), y-.15, z+120+gameZ);
            glScaled(1.1, .25, .5);
            glutSolidCube(3);
            glPopMatrix();
            
            // Side mirrors
            glPushMatrix();
            glColor3f(255.0/255.0, 233.0/255.0, 95.0/255.0);
            glTranslatef(x+gameX+(.25*facing), y-.75, z+120+gameZ);
            glScaled(.3, .15, 1.25);
            glutSolidCube(3);
            glPopMatrix();
            
            // Wheels front
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(x+gameX+(2*facing), y-1.6, z+120+gameZ);
            glScaled(.3, .3, 1.1);
            glutSolidCube(3);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(1, 1, 1);
            glTranslatef(x+gameX+(2*facing), y-1.6, z+120+gameZ);
            glScaled(.125, .125, 1.11);
            glutSolidCube(3);
            glPopMatrix();
            
            // Wheels back
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(x+gameX-(2*facing), y-1.6, z+120+gameZ);
            glScaled(.3, .3, 1.1);
            glutSolidCube(3);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(1, 1, 1);
            glTranslatef(x+gameX-(2*facing), y-1.6, z+120+gameZ);
            glScaled(.125, .125, 1.11);
            glutSolidCube(3);
            glPopMatrix();
            break;
            
            // Medium car - green
        case 4:
            // Body
            glPushMatrix();
            glColor3f(186.0/255.0, 241.0/255.0, 111.0/255.0);
            glTranslatef(x+gameX, y-1.25, z+120+gameZ);
            glScaled(2.2, .5, 1);
            glutSolidCube(3);
            glPopMatrix();
            
            // Upper body
            glPushMatrix();
            glColor3f(1, 1, 1);
            glTranslatef(x+gameX-(.8*facing), y+.25, z+120+gameZ);
            glScaled(1, .4, .75);
            glutSolidCube(3);
            glPopMatrix();
            
            // Window side
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(x+gameX-(.8*facing), y-.15, z+120+gameZ);
            glScaled(.5, .25, .751);
            glutSolidCube(3);
            glPopMatrix();
            
            // Window front/back
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(x+gameX-(.8*facing), y-.15, z+120+gameZ);
            glScaled(1.1, .25, .5);
            glutSolidCube(3);
            glPopMatrix();
            
            // Side mirrors
            glPushMatrix();
            glColor3f(145.0/255.0, 215.0/255.0, 93.0/255.0);
            glTranslatef(x+gameX+(.25*facing), y-.75, z+120+gameZ);
            glScaled(.3, .15, 1.25);
            glutSolidCube(3);
            glPopMatrix();
            
            // Wheels front
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(x+gameX+(2*facing), y-1.6, z+120+gameZ);
            glScaled(.3, .3, 1.1);
            glutSolidCube(3);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(1, 1, 1);
            glTranslatef(x+gameX+(2*facing), y-1.6, z+120+gameZ);
            glScaled(.125, .125, 1.11);
            glutSolidCube(3);
            glPopMatrix();
            
            
            // Wheels back
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(x+gameX-(2*facing), y-1.6, z+120+gameZ);
            glScaled(.3, .3, 1.1);
            glutSolidCube(3);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(1, 1, 1);
            glTranslatef(x+gameX-(2*facing), y-1.6, z+120+gameZ);
            glScaled(.125, .125, 1.11);
            glutSolidCube(3);
            glPopMatrix();
            break;
    }
}