#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <deque>
#include <cstdlib>
#include <ctime>
#include "structGame.cpp"
#include "chicken.cpp"

/*MUDANÇAS REALIZADAS:
*Acrescentei nivel que influencia na lógica de velocidade dos carros
*Mudanças na estrtura do jogo agora temos: mapTrash, mapTree, mapCars disvunculando tudo de Lane (Usa menos laços de repetição e espaço sequencial na memoria)
*Mudei a estrtura da class Player adicionando uma estrtura INVENTARIO que contem quantidade de lixo coletado de cada tipo 
*Redenrização dos lixos ao longo das ROADS do mapa
*Definicão de zona de atuação do jogador e zona dos carros
*Função para detecção de colisão tanto para Tree quanto para Cars -> Quando redefinir os desenho dos objetos precisa ajustar o limite da colisão
*Sistema de coleta de acordo com as setas

O QUE FALTA:
*Melhorar a aparencia do jogo
*Acrescentar linha de chegada + lixeira ao final
*Implementar Lógica de depositar lixo nas lixeira (calcular Score)
*Implementar Lógica de "Subir Nivel" + reinicio do mapa de player
*Implementar Lógica de score ->Acredito que seja interessante colocar algo como score minimo para passar tipo 60% do lixo coletado
*Implementar menu (controles do jogo) - OPCIONAL PODEMOS COLOCAR ORIENTAÇÃO NO READ.ME

*/

#define WIDTH 600
#define HEIGHT 900
#define pass 2.0f
#define zonaJogador 50 
#define zonaCarros 80
#define tamMapa 20
using namespace std;

int nivel = 1;

Player player(0.0f,5.0f,-5.0f);
deque<Lane> mapLanes;
deque<Trash> mapTrash;
deque<Tree> mapTree;
deque<Car> mapCars;
deque<Trash> mapDump;

void Player::drawPlayer(){
    glPushMatrix();
    glColor3f(0.5f,0.5f,0.5f);
    glTranslated(player.x,player.y,player.z);
    glPushMatrix();
    glScalef(3,3,3);
    drawChicken();
    glPopMatrix();
    glPopMatrix();
}

void updateCars(int value){
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

            if (dx < limite && dz < limite) //Melhorar a detecção de colisão
                exit(0); //Game Over
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
            int qtd = (rand() % 3+1); //OPCIONAL: Implementar nivel para aumentar a quantidade de carros não sei se poderia causar algum impacto na memória
            int dir = (rand()%2); //Sorteando direção da ROAD
                for(int i=0 ;i<qtd; i++){
                    Car car;
                    car.dir = dir;
                    car.x = car.dir ? zonaCarros:(-1*zonaCarros); //Setando extremos dir/esq | 1 - Esquerda /  0 = Direita
                    car.z = lane.z;
                    car.y = player.y;
                    car.speed = 0.1f + (rand() % (10*nivel)) / 100.0f; //Aumento de velocidade dos carros de acordo com o nível
                    mapCars.push_back(car);

                    Trash lixo;
                    lixo.x = (rand()%(zonaJogador*2 + 1)) - zonaJogador; //Gerando lixo na zona de jogabilidade
                    lixo.z = lane.z;
                    lixo.y = player.y;
                    lixo.collect = false;
                    int type = (rand()%4 + 1);
                    switch (type)
                    { case 1: lixo.type = PAPER; break;
                    case 2: lixo.type = PLASTIC; break;
                    case 3: lixo.type = METAL;break;
                    case 4: lixo.type = GLASS; break;
                    default: lixo.type = PAPER; break;
                    }
                    mapTrash.push_back(lixo);

                }
        }
        else{ //Gerar arvores
            int qtd = rand()%7 + 1;
            for(int i = 0; i<qtd;i++){
                int pos = (rand()%(zonaJogador*2 + 1)) - zonaJogador; //Gerando arvores na zona de jogabilidade
                Tree tree;
                tree.x = pos;
                tree.z = lane.z;
                tree.y = player.y;
                mapTree.push_back(tree);
            }
        }
    }
    mapLanes.push_back(lane);
}

void drawDump(int z){
    z += 20.0f; // Espaço
    float pos = -10.0f;
    for(int i=1; i<5;i++,pos += 10){
        Trash dump;
        dump.y = player.y;
        dump.z = z;
        dump.x = pos;
        dump.collect = 1;
        switch (i){ //Setar cores
        case 1: dump.type = PLASTIC; glColor3f(1.0f,0.0f,0.0f); break; //Plastico - Vermelho
        case 2: dump.type = PAPER; glColor3f(0.0f,1.0f,0.0f); break; //Papel - Azul Por alfum motivo as cores amarelas e verde estao invertidas
        case 3: dump.type = GLASS; glColor3f(0.0f,0.0f,1.0f); break; //Vidro - Verde
        case 4: dump.type = METAL; glColor3f(1.0f,1.0f,0.0f);break; //Metal - Amarelo
        }
        mapDump.push_back(dump);
        glPushMatrix();
            glTranslatef(dump.x,dump.y,dump.z);
            glutSolidCube(3.0f);
        glPopMatrix();
    }
}

void drawInicialMap(){
    int z = 0.0f;

    for(int i = 0; i<tamMapa; i++){
        randomLane(z);
        z += 10.0f;
    }

}

void drawTrash(){
    for(Trash &trash : mapTrash){
        if(!trash.collect){ //Apenas lixos nao coletados sao redenrizados em cena
            switch (trash.type)
            {
            case PAPER:
                glColor3f(0.0f,1.0f,1.0f);
                break;
            case PLASTIC:
                glColor3f(1.0f,0.0f,0.0f);
                break;
            case METAL:
                glColor3f(1.0f,1.0f,0.0f);
                break;
            case GLASS: 
                glColor3f(0.0f,0.0f,1.0f);
                break;
            }
            glPushMatrix();
            glTranslatef(trash.x,trash.y,trash.z);
            glutSolidCube(1.5f);
            glPopMatrix();
        }
    }
}

void drawTree(){
    glColor3f(0.0f, 0.78f, 0.28f);
    for(Tree &tree : mapTree){
        glPushMatrix();
        glTranslatef(tree.x,tree.y,tree.z);
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
void drawCar(){
    for(Car &carros : mapCars){
        glColor3f(1.0f, 0.0f, 0.0f);
        glPushMatrix();
        glTranslatef(carros.x, carros.y, carros.z);
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
    for(Lane &lane : mapLanes){ // Desenhar cada terreno e seus objetos
        glDisable(GL_LIGHTING);
        if(lane.type == GRASS){
            drawGrass(lane);
            drawTree();
        }
        else{
            drawRoad(lane);
            drawCar();
        } 
        drawTrash();
    }

    drawDump(tamMapa*10);
    glEnable(GL_LIGHTING);
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
    bool go = true;
    int passX = 0;
    int passZ = 0;
    switch (key){ 
        case 'w': passZ += pass; break;
        case 's': passZ -= pass; break;
        case 'a': passX += pass; break;
        case 'd': passX -= pass; break;
    } 
    for(Tree &tree : mapTree){ //Verifica colisão com alguma arvore
        float dx = fabs((player.x+passX) - tree.x);
        float dz = fabs((player.z+passZ) - tree.z);
        float limite = 2.0f;
        if(dx<limite && dz<(limite+1)) // limite + 1 para dar o espaço do bico
            go = false;
    }
    if(go)
        switch (key)
        {
        case 'w': // Frente (z+)
            player.z += pass;
            break;
        case 's': // Trás (z-)
            player.z -= pass;
            break;
        case 'a': // Esquerda (x-)
            if (player.x + pass < zonaJogador)
                player.x += pass;
            break;
        case 'd': // Direita (x+)
            if (player.x - pass > (-1*zonaJogador))
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
        for(Trash &trash : mapTrash){
            if(!trash.collect){//Verifica se tem lixo não coleatdo perto do jogador

                float dz = fabs(player.z - trash.z); 
                float dx = fabs(player.x - trash.x);
                float limite = 3.0f;
                if(dz < limite && dx <limite){

                    if(key==GLUT_KEY_UP && trash.type==PAPER){
                        trash.collect = true;
                        player.inv.paper++;
                    }
                    else{
                        if(key==GLUT_KEY_DOWN && trash.type==PLASTIC){
                        trash.collect = true;
                        player.inv.plastic++;
                        }
                        else{
                            if(key==GLUT_KEY_RIGHT && trash.type==METAL){
                                trash.collect = true;
                                player.inv.metal++;
                            }
                            else{
                                if(key==GLUT_KEY_LEFT && trash.type==GLASS){
                                trash.collect = true;
                                player.inv.glass++;
                                }
                            }
                        }
                    }
                }

            }
        }
        for (Trash &dump : mapDump) {
    float dz = fabs(player.z - dump.z); 
    float dx = fabs(player.x - dump.x);
    float limite = 4.0f;
    if (dx < limite && dz < limite) {
        switch (dump.type) {
            case PLASTIC:
                player.score += (key == GLUT_KEY_DOWN) ? (10 * player.inv.plastic) : (-10 * player.inv.plastic);
                player.inv.plastic = 0;
                break;
            case PAPER: 
                player.score += (key == GLUT_KEY_UP) ? (10 * player.inv.paper) : (-10 * player.inv.paper); 
                player.inv.paper = 0; 
                break;
            case METAL: 
                player.score += (key == GLUT_KEY_RIGHT) ? (10 * player.inv.metal) : (-10 * player.inv.metal); 
                player.inv.metal = 0; 
                break;
            case GLASS: 
                player.score += (key == GLUT_KEY_LEFT) ? (10 * player.inv.glass) : (-10 * player.inv.glass); 
                player.inv.glass = 0; 
                break;
        }
    }
}
        cout<<player.inv.plastic << player.inv.paper << player.inv.metal << player.inv.glass << "\n";
        cout<<player.score <<"\n";
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