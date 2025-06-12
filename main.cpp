#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <deque>
#include <cstdlib>
#include <ctime>
#include "structGame.cpp"

#define WIDTH 600
#define HEIGHT 900
#define pass 2.0f
using namespace std;

Player player(0.0f,5.0f,-5.0f);
deque<Lane> mapLanes;

void Player::drawPlayer(){
    glPushMatrix();
    glColor3f(0.5f,0.5f,0.5f);
    glTranslated(player.x,player.y,player.z);
    glutSolidCube(player.size);
    glPopMatrix();
}

void updateCars(int value){
    for(Lane &lane : mapLanes){
        for(Car &carros : lane.carros){
            if(carros.dir){
                if(carros.x<-100.0f)
                    carros.x = 100.0f;
                carros.x -= carros.speed;}
            else{
                if(carros.x>100.0f)
                    carros.x = -100.0f;
                carros.x += carros.speed;}
            
            float dx = fabs(carros.x - player.x);
            float dz = fabs(lane.z - player.z);
            float limite = 1.5f; // Tolerância

            if (dx < limite && dz < limite)
                exit(0); //Game Over
        }
    }

    glutPostRedisplay();           // redesenha a tela
    glutTimerFunc(16, updateCars, 0); // chama de novo daqui a 16ms
}

void randomLane(int z) {
    Lane lane;
    lane.z = z;   
    if(!z)
        lane.type = GRASS;
    else{
        lane.type = (rand()%2 == 0) ? ROAD:GRASS;

        if(lane.type==ROAD){
            int qtd = (rand() % 3+1);
            int dir = (rand()%2);
                for(int i=0 ;i<qtd; i++){
                    Car car;
                    car.dir = dir;
                    car.x = car.dir ? 100.0f:-100.0f; // 1 - Esquerda /  0 = Direita
                    car.z = lane.z;
                    car.y = player.y;
                    car.speed = 0.1f + (rand() % 10) / 100.0f;
                    lane.carros.push_back(car);
                }
        }
        else{
            int qtd = rand()%7 + 1;
            for(int i = 0; i<qtd;i++){
                int pos = (rand() % 121) - 60;
                Tree tree;
                tree.x = pos;
                tree.z = lane.z;
                lane.arvores.push_back(tree);
            }
        }
    }
    mapLanes.push_back(lane);
}

void drawInicialMap(){
    int z = 0.0f;

    for(int i = 0; i<20; i++){
        randomLane(z);
        z += 10.0f;
    }

}

void drawTree(Lane lane){
    glColor3f(0.0f, 0.78f, 0.28f);
    for(Tree &tree : lane.arvores){
        glPushMatrix();
        glTranslatef(tree.x,0.0f,tree.z);
        glutSolidCube(4.0f);
        glPopMatrix();
    }
}

void drawGrass(Lane lane){
    glColor3f(0.38f, 0.78f, 0.28f);

    glPushMatrix();
        glTranslatef(0.0f, 0.0f, lane.z);
        glBegin(GL_QUADS);
            glVertex3f(-150.0f, 0.0f, -5.0f);
            glVertex3f(-150.0f, 0.0f,  5.0f);
            glVertex3f( 150.0f, 0.0f,  5.0f);
            glVertex3f( 150.0f, 0.0f, -5.0f);
    glEnd();
    glPopMatrix();
}
void drawCar(Lane lane){
    for(Car &carros : lane.carros){
        glColor3f(1.0f, 0.0f, 0.0f);
        glPushMatrix();
        glTranslatef(carros.x, 0.0f, lane.z);
            glutSolidCube(2.0f);
        glPopMatrix();
    }
}
void drawRoad(Lane lane){

    glColor3f(0.15f, 0.15f, 0.15f);
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, lane.z);
        glBegin(GL_QUADS);
            glVertex3f(-150.0f, 0.0f, -5.0f);
            glVertex3f(-150.0f, 0.0f,  5.0f);
            glVertex3f( 150.0f, 0.0f,  5.0f);
            glVertex3f( 150.0f, 0.0f, -5.0f);
    glEnd();
    glPopMatrix();
}
void drawLane(){
    for(Lane &lane : mapLanes){
        glDisable(GL_LIGHTING);
        if(lane.type == GRASS){
            drawGrass(lane);
            drawTree(lane);
        }
        else{
            drawRoad(lane);
            drawCar(lane);
        } 
        glEnable(GL_LIGHTING);
    }
}

void init() {
    // Ativa o teste de profundidade
    glEnable(GL_DEPTH_TEST);

    // Ativa o uso de cor como material
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Configura material especular (opcional: pode ser zero para estilo cartoon)
    GLfloat specular[] = {0.0f, 0.0f, 0.0f, 1.0f}; // brilho desativado para visual cartoon
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f); // sem brilho especular

    // Ativa a iluminação e a luz 0
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Define luz branca difusa e sem especular
    GLfloat light_diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light_ambient[]  = {1.0f, 1.0f, 1.0f, 1.0f}; // luz ambiente total
    GLfloat light_position[] = {0.0f, 20.0f, 10.0f, 1.0f}; // posição acima da cena (tipo ponto)

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glClearColor(0.38f, 0.78f, 0.28f, 1.0f);

    // Gera o mapa inicial
    drawInicialMap();
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Configura a câmera aqui
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)WIDTH / (float)HEIGHT, 1.0, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        player.x-10.0f, player.y + 35.0f, player.z-20.0f,  // posição da câmera em relação ao jogador
        player.x, player.y, player.z,     // acomphamento da camera ao jogador
        0.0f, 1.0f, 0.0f      // eixo "para cima"
    );
    drawLane();
    player.drawPlayer();
    glutSwapBuffers();
}

void teclado(unsigned char key, int x, int y){
    switch (key)
    {
    case 'w': // Frente (z+)
        player.z += pass;
        break;
    case 's': // Trás (z-)
        player.z -= pass;
        break;
    case 'a': // Esquerda (x-)
        if (player.x + pass < 100.0f)
            player.x += pass;
        break;
    case 'd': // Direita (x+)
        if (player.x - pass > -100.0f)
            player.x -= pass;
        break;
    case 'q': // Sair
        exit(0);
        break;
    case 'r': // Reiniciar
        break;
    }
    glutPostRedisplay();
}


void teclasEspeciais(int key, int x, int y){
        switch (key)
        {
        case GLUT_KEY_DOWN:
            //Coletar/Deixar lixeira tipo1
            break;
        case GLUT_KEY_UP:
            break;
        case GLUT_KEY_RIGHT:
            break;
        case GLUT_KEY_LEFT:
            break;
        }
        glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Crossy Road");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(teclasEspeciais);
    glutTimerFunc(0, updateCars, 0); // inicia animação imediatamente
    glutMainLoop();
}