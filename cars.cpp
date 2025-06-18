#include "cars.hpp"
#include "player.hpp"
#include "game_utils.hpp"
//Função para desenhar os carros
void drawCar(){
    for(Car &carros : mapCars){    
        int facing = carros.dir ?  -1:1;

        glPushMatrix();    
            switch(carros.color){
                case 1: glColor3f(0.6f, 0.1f, 0.8f); break;
                case 2: glColor3f(1.0f, 0.0f, 0.6f); break;
                case 3: glColor3f(0.0f, 0.8f, 0.9f); break;
                case 4: glColor3f(1.0f, 0.4f, 0.0f); break;
            }
            glTranslatef(carros.x, carros.y-1.25, carros.z);
            glScaled(2.2, .5, 1);
            glutSolidCube(3);
            glPopMatrix();
            
            // Upper body
            glPushMatrix();
            glColor3f(1, 1, 1);
            glTranslatef(carros.x -(.8*facing), carros.y+.25,carros.z);
            glScaled(1, .4, .75);
            glutSolidCube(3);
            glPopMatrix();
            
            // Window side
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(carros.x-(.8*facing), carros.y-.15,carros.z);
            glScaled(.5, .25, .751);
            glutSolidCube(3);
            glPopMatrix();
            
            // Window front/back
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(carros.x-(.8*facing), carros.y-.15,carros.z);
            glScaled(1.1, .25, .5);
            glutSolidCube(3);
            glPopMatrix();
            
            // Side mirrors
            glPushMatrix();
            glColor3f(0.2, 0.2, 0.2);
            glTranslatef(carros.x+(.25*facing), carros.y-.75,carros.z);
            glScaled(.3, .15, 1.25);
            glutSolidCube(3);
            glPopMatrix();
            
            // Wheels front
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(carros.x+(2*facing), carros.y-1.6,carros.z);
            glScaled(.3, .3, 1.1);
            glutSolidCube(3);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(1, 1, 1);
            glTranslatef(carros.x+(2*facing), carros.y-1.6,carros.z);
            glScaled(.125, .125, 1.11);
            glutSolidCube(3);
            glPopMatrix();
            
            
            // Wheels back
            glPushMatrix();
            glColor3f(0, 0, 0);
            glTranslatef(carros.x-(2*facing), carros.y-1.6,carros.z);
            glScaled(.3, .3, 1.1);
            glutSolidCube(3);
            glPopMatrix();
            
            glPushMatrix();
            glColor3f(1, 1, 1);
            glTranslatef(carros.x-(2*facing), carros.y-1.6,carros.z);
            glScaled(.125, .125, 1.11);
            glutSolidCube(3);
            glPopMatrix();
    }
}

//Função para atualizar a posição dos carros
void updateCars(int value){
    // Se o timer não deveria estar ativo, não faz nada
    if (!carTimerActive || currentGameState != PLAYING) {
        return;
    }
    
        for(Car &carros : mapCars){
            if(carros.dir){
                if(carros.x<(-1*zonaCarros))
                    carros.x = zonaCarros;
                carros.x -= carros.speed;}
            else{
                if(carros.x>zonaCarros)
                    carros.x = (-1*zonaCarros);
                carros.x += carros.speed;}
            
            float dx = fabs(carros.x - player.x);
            float dz = fabs(carros.z - player.z);
            float limite = 1.5f; // Tolerância

            if (dx < limite && dz < limite) {//Melhorar a detecção de colisão
                carTimerActive = false; // Para o timer antes do game over
                exibirMensagem("GAME OVER");
                glutTimerFunc(1000, [](int){ exit(0); }, 0);
                //Game Over
                return;
            }
        }

    glutPostRedisplay();           // redesenha a tela
    if (carTimerActive && currentGameState == PLAYING) {
        glutTimerFunc(16, updateCars, 0); // chama de novo daqui a 16ms apenas se ainda ativo
    }
}