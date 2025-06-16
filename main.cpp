#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <deque>
#include <cstdlib>
#include <ctime>
#include "structGame.cpp"
#include "chicken.cpp"
#include "bins.cpp"
#include "garbage.cpp"

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
int qtdLixoDisponivel = 0;

bool mensagem = false;
string text;
time_t tempoInicioMensagem = 0;


Player player(0.0f,5.0f,-5.0f);
deque<Lane> mapLanes;
deque<Trash> mapTrash;
deque<Tree> mapTree;
deque<Car> mapCars;
deque<Trash> mapDump;

// Dimensões da Janela (agora variáveis globais)
int windowWidth = 600;
int windowHeight = 900;

// Estados do Jogo
enum GameState {
    MENU,
    PLAYING,
    PAUSED
};
GameState currentGameState = MENU;

// Dimensões e posições dos botões do menu
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50

// Dimensões e posições dos botões do menu de pausa
#define PAUSE_CONTINUE_BUTTON_Y (windowHeight / 2 + BUTTON_HEIGHT / 2 + 10)
#define PAUSE_RESTART_BUTTON_Y (windowHeight / 2 - BUTTON_HEIGHT / 2)
#define PAUSE_EXIT_BUTTON_Y (windowHeight / 2 - BUTTON_HEIGHT * 1.5 - 10)

// Protótipos de funções relacionadas ao menu
void drawMenu();
void drawPauseMenu();
void mouseClick(int button, int state, int x, int y);
void startGame();
void reshape(int w, int h);
void drawButton(float x, float y, float width, float height, const char* label);


void desenharTexto(float yPixel, const std::string& texto, float scale) {
    // calcula largura total do texto em unidades stroke (sem escala)
    float largura = 0;
    for (char c : texto)
        largura += glutStrokeWidth(GLUT_STROKE_ROMAN, c);

    float xPixel = (windowWidth - largura * scale) / 2.0f; // Usa windowWidth em vez de WIDTH

    glPushMatrix();
    glTranslatef(xPixel, yPixel, 0);
    glScalef(scale, scale, 1.0f);

    glColor3f(1.0f, 1.0f, 0.0f);  // amarelo

    for (char c : texto)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, c);

    glPopMatrix();
}


void exibirMensagem(const std::string& texto){
    mensagem = true;
    tempoInicioMensagem = time(0);
    text = texto;
    glutPostRedisplay();
}

void Player::zerarPlayer(){
        this->x = 0.0f;
        this->z = -5.0f;
        this->size = 2.0f;
        this->score = 0;
        this->inv.paper = this->inv.plastic = this->inv.metal = this->inv.glass = 0;
}

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

            if (dx < limite && dz < limite) {//Melhorar a detecção de colisão
                exibirMensagem("GAME OVER");
                glutTimerFunc(1000, [](int){ exit(0); }, 0);
                //Game Over
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
            int qtd = (rand() % (4 + nivel/2) +2); //OPCIONAL: Implementar nivel para aumentar a quantidade de carros não sei se poderia causar algum impacto na memória
            qtdLixoDisponivel += qtd;
            int dir = (rand()%2); //Sorteando direção da ROAD
                for(int i=0 ;i<qtd; i++){
                    Car car;
                    car.dir = dir;
                    car.x = car.dir ? (zonaCarros + (i*10)):((-1*zonaCarros) - (i*10)) ; //Setando extremos dir/esq | 1 - Esquerda /  0 = Direita
                    car.z = lane.z;
                    car.y = player.y;
                    car.speed = 0.1f + (rand() % (10*nivel)) / 50.0f; //Aumento de velocidade dos carros de acordo com o nível
                    car.color = (rand()%4 + 1);
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
        case 1: dump.type = PLASTIC;drawTrashBin(1.0f, 0.0f, 0.0f, "plastic", dump.x, dump.z); break; //Plastico - Vermelho
        case 2: dump.type = PAPER;drawTrashBin(0.0f, 0.0f, 1.0f, "paper", dump.x, dump.z); break; //Papel - Azul Por alfum motivo as cores amarelas e verde estao invertidas
        case 3: dump.type = GLASS;drawTrashBin(0.0f, 1.0f, 0.0f, "glass", dump.x, dump.z); break; //Vidro - Verde
        case 4: dump.type = METAL;drawTrashBin(1.0f, 1.0f, 0.0f, "metal", dump.x, dump.z); break; //Metal - Amarelo
        }
        mapDump.push_back(dump);
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
            glPushMatrix();
            glTranslatef(trash.x,trash.y,trash.z);
            switch (trash.type)
            {
            case PAPER:
                desenharLixoPapel();
                break;
            case PLASTIC:
                desenharLixoPlastico();
                break;
            case METAL:
                desenharLixoMetal();
                break;
            case GLASS: 
                desenharLixoVidro();
                break;
            }
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

void updateNivel(){

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
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);    glClearColor(0.38f, 0.78f, 0.28f, 1.0f);

    // Registra a função de callback do mouse
    glutMouseFunc(mouseClick);
}


void display() {
    if (currentGameState == MENU) {
        drawMenu();
    } else if (currentGameState == PLAYING || currentGameState == PAUSED) { 
        glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Configura a câmera para o jogo
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, (float)windowWidth / (float)windowHeight, 1.0, 1000.0); 

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(
            player.x-10.0f, player.y + 35.0f, player.z-20.0f,  
            player.x, player.y, player.z,     
            0.0f, 1.0f, 0.0f      
        );

        if (mensagem){
            double segundos = difftime(time(0), tempoInicioMensagem);
            if (segundos < 5) {
                float alpha = 1.0f - (segundos / 5.0f); 

                glMatrixMode(GL_PROJECTION);
                glPushMatrix();
                glLoadIdentity();
                gluOrtho2D(0, windowWidth, 0, windowHeight);

                glMatrixMode(GL_MODELVIEW);
                glPushMatrix();
                glLoadIdentity();

                desenharTexto(windowHeight/2, text, 0.5f);

                glPopMatrix();
                glMatrixMode(GL_PROJECTION);
                glPopMatrix();
                glMatrixMode(GL_MODELVIEW);
            } else {
                mensagem = false;
            }
        }

        if(player.score >= (nivel*50)){
            cout<<"subir nivel";
            nivel++;
            qtdLixoDisponivel = 0;
            mapTree.clear();
            mapCars.clear();
            mapTrash.clear();
            mapLanes.clear();
            drawInicialMap();
            player.zerarPlayer(); 
            exibirMensagem("NEXT LEVEL...");           
        }
        else{
            int aux = player.inv.plastic + player.inv.paper + player.inv.metal + player.inv.glass ;
            if((((aux+qtdLixoDisponivel)*10)+player.score) < (nivel*50)){
                exibirMensagem("GAME OVER");
                exit(0);
            }
        }
        
        drawLane();
        player.drawPlayer();

        if (currentGameState == PAUSED) { 
            drawPauseMenu();
        }

        glutSwapBuffers();
    } 
}

void teclado(unsigned char key, int x, int y){
    if (key == 27) { // Tecla ESC
        if (currentGameState == PLAYING) {
            currentGameState = PAUSED;
            glutPostRedisplay();
            return;
        } else if (currentGameState == PAUSED) {
            currentGameState = PLAYING;
            glutTimerFunc(16, updateCars, 0);
            glutPostRedisplay();
            return;
        }
    }

    if (currentGameState != PLAYING) {
        return; 
    }

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
    if (currentGameState != PLAYING) {
        return; 
    }
    
    for(Trash &trash : mapTrash){
        if(!trash.collect){//Verifica se tem lixo não coleatdo perto do jogador

            float dz = fabs(player.z - trash.z); 
            float dx = fabs(player.x - trash.x);
            float limite = 3.0f;
            if(dz < limite && dx <limite){

                if(key==GLUT_KEY_UP && trash.type==PAPER){
                    trash.collect = true;
                    qtdLixoDisponivel --;
                    player.inv.paper++;
                }
                else{
                    if(key==GLUT_KEY_DOWN && trash.type==PLASTIC){
                    trash.collect = true;
                    qtdLixoDisponivel --;
                    player.inv.plastic++;
                    }
                    else{
                        if(key==GLUT_KEY_RIGHT && trash.type==METAL){
                            trash.collect = true;
                            qtdLixoDisponivel --;
                            player.inv.metal++;
                        }
                        else{
                            if(key==GLUT_KEY_LEFT && trash.type==GLASS){
                            trash.collect = true;
                            qtdLixoDisponivel --;
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
    glutPostRedisplay();
}

// Função para desenhar os botões (usada por ambos os menus)
void drawButton(float x, float y, float width, float height, const char* label) {
    // Desenha o retângulo do botão
    glColor3f(0.7f, 0.7f, 0.7f); // Cor cinza para o botão
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // Desenha a borda do botão
    glColor3f(0.3f, 0.3f, 0.3f); // Cor cinza escuro para a borda
    glLineWidth(2.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // Desenha o texto do botão
    float textWidth = 0;
    for (const char* c = label; *c; ++c) {
        textWidth += glutStrokeWidth(GLUT_STROKE_ROMAN, *c);
    }
    textWidth *= 0.15f; 

    float textX = x + (width - textWidth) / 2;
    float textY = y + (height - 20) / 2 + 5; 

    glPushMatrix();
    glTranslatef(textX, textY, 0.1f); 
    glScalef(0.15f, 0.15f, 1.0f); 
    glColor3f(0.0f, 0.0f, 0.0f); 
    glLineWidth(1.0f); 
    for (const char* c = label; *c; ++c) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }
    glPopMatrix();
}

// Função para desenhar o menu de pausa
void drawPauseMenu() {
    // Configura para desenho 2D
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Desenha um retângulo semitransparente por cima da tela
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f); // Preto com 50% de transparência
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(windowWidth, 0);
    glVertex2f(windowWidth, windowHeight);
    glVertex2f(0, windowHeight);
    glEnd();
    glDisable(GL_BLEND);

    // Desenha o título "Pausado"
    desenharTexto(windowHeight - 150, "Pausado", 0.7f);

    // Desenha os botões
    float buttonX = (windowWidth / 2.0f - BUTTON_WIDTH / 2.0f);
    drawButton(buttonX, PAUSE_CONTINUE_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, "Continuar");
    drawButton(buttonX, PAUSE_RESTART_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, "Reiniciar");
    drawButton(buttonX, PAUSE_EXIT_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, "Sair");

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// Função para desenhar o menu inicial
void drawMenu() {
    glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    // Configura a projeção para 2D (ortográfica) para o menu
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight); 

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Desenha o título
    desenharTexto(windowHeight - 150, "ReciclaCity", 0.7f); 

    // Desenha os botões (posições relativas ao centro da windowWidth/windowHeight)
    float startButtonX = (windowWidth / 2.0f - BUTTON_WIDTH / 2.0f);
    float startButtonY = (windowHeight / 2.0f - 25.0f);
    float exitMenuButtonX = (windowWidth / 2.0f - BUTTON_WIDTH / 2.0f);
    float exitMenuButtonY = (windowHeight / 2.0f - BUTTON_HEIGHT - 45.0f);

    drawButton(startButtonX, startButtonY, BUTTON_WIDTH, BUTTON_HEIGHT, "Iniciar");
    drawButton(exitMenuButtonX, exitMenuButtonY, BUTTON_WIDTH, BUTTON_HEIGHT, "Sair");

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW); 

    glutSwapBuffers();
}

// Função para iniciar/reiniciar o jogo
void startGame() {
    currentGameState = PLAYING;
    nivel = 1; 
    qtdLixoDisponivel = 0;
    player.zerarPlayer(); 

    mapLanes.clear();
    mapTrash.clear();
    mapTree.clear();
    mapCars.clear();
    mapDump.clear();

    drawInicialMap(); 
    if (currentGameState == PLAYING) {
      glutTimerFunc(16, updateCars, 0); 
    }
    glutPostRedisplay();
}

// Função de callback para cliques do mouse
void mouseClick(int button, int state, int x, int y) {
    if (currentGameState == MENU && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int invertedY = windowHeight - y; 

        float startButtonX = (windowWidth / 2.0f - BUTTON_WIDTH / 2.0f);
        float startButtonY = (windowHeight / 2.0f - 25.0f);
        float exitMenuButtonX = (windowWidth / 2.0f - BUTTON_WIDTH / 2.0f);
        float exitMenuButtonY = (windowHeight / 2.0f - BUTTON_HEIGHT - 45.0f);

        if (x >= startButtonX && x <= startButtonX + BUTTON_WIDTH &&
            invertedY >= startButtonY && invertedY <= startButtonY + BUTTON_HEIGHT) {
            startGame();
        }
        else if (x >= exitMenuButtonX && x <= exitMenuButtonX + BUTTON_WIDTH &&
                 invertedY >= exitMenuButtonY && invertedY <= exitMenuButtonY + BUTTON_HEIGHT) {
            exit(0);
        }
    } else if (currentGameState == PAUSED && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int invertedY = windowHeight - y;
        float buttonX = (windowWidth / 2.0f - BUTTON_WIDTH / 2.0f);

        if (x >= buttonX && x <= buttonX + BUTTON_WIDTH &&
            invertedY >= PAUSE_CONTINUE_BUTTON_Y && invertedY <= PAUSE_CONTINUE_BUTTON_Y + BUTTON_HEIGHT) {
            currentGameState = PLAYING;
            glutTimerFunc(16, updateCars, 0);
            glutPostRedisplay();
        }
        else if (x >= buttonX && x <= buttonX + BUTTON_WIDTH &&
                 invertedY >= PAUSE_RESTART_BUTTON_Y && invertedY <= PAUSE_RESTART_BUTTON_Y + BUTTON_HEIGHT) {
            startGame(); 
        }
        else if (x >= buttonX && x <= buttonX + BUTTON_WIDTH &&
                 invertedY >= PAUSE_EXIT_BUTTON_Y && invertedY <= PAUSE_EXIT_BUTTON_Y + BUTTON_HEIGHT) {
            exit(0);
        }
    }
}

// Função de callback para redimensionamento da janela
void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    if (currentGameState == MENU || currentGameState == PAUSED) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, w, 0, h);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }
    glutPostRedisplay(); 
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("ReciclaCity");
    
    init();
    
    glutDisplayFunc(display);
    glutKeyboardFunc(teclado);
    glutReshapeFunc(reshape);
    glutSpecialFunc(teclasEspeciais);
    
    glutMainLoop();
    return 0;
}