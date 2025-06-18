#include "lanes.hpp"
#include "garbage.hpp"
#include "cars.hpp"
#include "tree.hpp"
#include "player.hpp"
using namespace std;

//Função para desenhar o mapaLanes
// Desenha cada lane de acordo com o tipo (grama ou estrada) e seus objetos
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

// Função para gerar o mapaLanes inicial
void drawInicialMap(){
    int z = 0.0f;

    for(int i = 0; i<tamMapa; i++){
        randomLane(z);
        z += 10.0f;
    }

}

//Gerar uma nova lane aleatória
void randomLane(int z) {
    Lane lane;
    lane.z = z;   
    if(!z)
        lane.type = GRASS;
    else{
        lane.type = (rand()%2 == 0) ? ROAD:GRASS;

        if(lane.type==ROAD){
            int qtd = (rand() % (3 + nivel/2) +2); //OPCIONAL: Implementar nivel para aumentar a quantidade de carros não sei se poderia causar algum impacto na memória
            qtdLixoDisponivel += qtd;
            int dir = (rand()%2); //Sorteando direção da ROAD
                for(int i=0 ;i<qtd; i++){
                    Car car;
                    car.dir = dir;
                    car.x = car.dir ? (zonaCarros + (i*10)):((-1*zonaCarros) - (i*10)) ; //Setando extremos dir/esq | 1 - Esquerda /  0 = Direita
                    car.z = lane.z;
                    car.y = player.y;
                    car.speed = 0.1f + (rand() % (10*nivel)) / 20.0f; //Aumento de velocidade dos carros de acordo com o nível
                    car.color = (rand()%4 + 1);
                    mapCars.push_back(car);

                    Trash lixo;
                    lixo.x = (rand()%(zonaJogador*2 + 1)) - zonaJogador; //Gerando lixo na zona de jogabilidade
                    lixo.z = lane.z;
                    lixo.y = player.y;
                    lixo.collect = false;
                    int type = (rand()%4 + 1);
                    switch (type){ 
                        case 1: lixo.type = TrashType::PAPER; break;
                        case 2: lixo.type = TrashType::PLASTIC; break;
                        case 3: lixo.type = TrashType::METAL; break;
                        case 4: lixo.type = TrashType::GLASS; break;
                        default: lixo.type = TrashType::PAPER; break;
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

// Função para desenhar uma lane de grama
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

// Função para desenhar uma lane de estrada
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