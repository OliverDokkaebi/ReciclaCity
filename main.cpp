#include "gameDefinitions.hpp"
#include "player.hpp"
#include "lanes.hpp"
#include "garbage.hpp"
#include "bins.hpp"
#include "cars.hpp"
#include "tree.hpp"
#include "game_utils.hpp"

using namespace std;


GameState currentGameState = MENU;

int screenWidth = WIDTH;
int screenHeight = HEIGHT; 

int nivel = 1;
int qtdLixoDisponivel = 0;

bool mensagem = false;
bool menuStatus = true;
string text;
time_t tempoInicioMensagem = 0;

// Cria o jogador e os mapas
Player player(0.0f,5.0f,-5.0f);
deque<Lane> mapLanes;
deque<Trash> mapTrash;
deque<Tree> mapTree;
deque<Car> mapCars;
deque<Trash> mapDump;

// Controle do timer dos carros
bool carTimerActive = false;

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
    
     // Registra a função de callback do mouse
    glutMouseFunc(mouseClick);
}


void display() {
    if (currentGameState == MENU) {
        drawMenu();
    } else if (currentGameState == INSTRUCTIONS) {
        drawInstructionsMenu();
    } else if (currentGameState == PLAYING || currentGameState == PAUSED) { 
        glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Configura a câmera para o jogo
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, (float)screenWidth / (float)screenHeight, 1.0, 1000.0); 

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(
            player.x-15.0f, player.y + 45.0f, player.z-25.0f,  
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
            gluOrtho2D(0, WIDTH, 0, HEIGHT);

            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();

            desenharTexto(HEIGHT/2, text, 0.5f); // y em pixels

            glPopMatrix();
            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
        } else {
            mensagem = false;
        }
    }

    if(player.score >= (nivel*50)){
        subirNivel(); //Se o jogador tiver pontos suficientes para subir de nivel          
    }
    else{
        int aux = player.inv.plastic + player.inv.paper + player.inv.metal + player.inv.glass ;
        if((((aux+qtdLixoDisponivel)*10)+player.score) < (nivel*50)){ //Se mesmo ao coletar todos os lixos do mapa não for o suficiente para subir de nivel GAME OVER
            exibirMensagem("GAME OVER");
            glutTimerFunc(1000, [](int){ exit(0); }, 0);
        }
    }
    drawLane();
    player.drawPlayer(player);

    
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
            carTimerActive = false; // Para o timer quando pausa
            glutPostRedisplay();
            return;
        } else if (currentGameState == PAUSED) {
            currentGameState = PLAYING;
            carTimerActive = true; // Reativa o timer
            glutTimerFunc(16, updateCars, 0);
            glutPostRedisplay();
            return;
        } else if (currentGameState == INSTRUCTIONS) {
            currentGameState = MENU;
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

// Função de callback para redimensionamento da janela
void reshape(int w, int h) {
    screenWidth = w;
    screenHeight = h;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    if (currentGameState == MENU || currentGameState == PAUSED || currentGameState == INSTRUCTIONS) {
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
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Crossy Road");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(teclado);
    glutReshapeFunc(reshape);
    glutSpecialFunc(teclasEspeciais);
    //glutTimerFunc(0, updateCars, 0); // inicia animação imediatamente
    glutMainLoop();
}

