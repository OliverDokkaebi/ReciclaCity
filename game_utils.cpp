#include "game_utils.hpp"
#include "player.hpp"
#include "lanes.hpp"
#include "garbage.hpp"
#include "cars.hpp"
#include "tree.hpp"
#include <iostream>
using namespace std;

void desenharTexto(float yPixel, const std::string& texto, float scale) {
    // calcula largura total do texto em unidades stroke (sem escala)
    float largura = 0;
    for (char c : texto)
        largura += glutStrokeWidth(GLUT_STROKE_ROMAN, c);

    float xPixel = (screenWidth - largura * scale) / 2.0f;

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

void subirNivel(){
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
//MENU

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
    gluOrtho2D(0, screenWidth, 0, screenHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Desenha um retângulo semitransparente por cima da tela
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f); // Preto com 50% de transparência
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(screenWidth, 0);
    glVertex2f(screenWidth, screenHeight);
    glVertex2f(0, screenHeight);
    glEnd();
    glDisable(GL_BLEND);

    // Desenha o título "Pausado"
    desenharTexto(screenHeight - 150, "Pausado", 0.7f);

    // Desenha os botões
    float buttonX = (screenWidth / 2.0f - BUTTON_WIDTH / 2.0f);
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
    glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    // Configura a projeção para 2D (ortográfica) para o menu
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, screenWidth, 0, screenHeight); 

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Desenha o título
    desenharTexto(screenHeight - 150, "ReciclaCity", 0.7f); 

    // Desenha os botões (posições relativas ao centro da windowWidth/windowHeight)
    float startButtonX = (screenWidth / 2.0f - BUTTON_WIDTH / 2.0f);
    float startButtonY = (screenHeight / 2.0f - 25.0f);
    float exitMenuButtonX = (screenWidth / 2.0f - BUTTON_WIDTH / 2.0f);
    float exitMenuButtonY = (screenHeight / 2.0f - BUTTON_HEIGHT - 45.0f);

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
        int invertedY = screenWidth - y; 

        float startButtonX = (screenWidth / 2.0f - BUTTON_WIDTH / 2.0f);
        float startButtonY = (screenHeight / 2.0f - 25.0f);
        float exitMenuButtonX = (screenWidth / 2.0f - BUTTON_WIDTH / 2.0f);
        float exitMenuButtonY = (screenHeight / 2.0f - BUTTON_HEIGHT - 45.0f);

        if (x >= startButtonX && x <= startButtonX + BUTTON_WIDTH &&
            invertedY >= startButtonY && invertedY <= startButtonY + BUTTON_HEIGHT) {
            startGame();
        }
        else if (x >= exitMenuButtonX && x <= exitMenuButtonX + BUTTON_WIDTH &&
                 invertedY >= exitMenuButtonY && invertedY <= exitMenuButtonY + BUTTON_HEIGHT) {
            exit(0);
        }
    } else if (currentGameState == PAUSED && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int invertedY = screenHeight - y;
        float buttonX = (screenWidth / 2.0f - BUTTON_WIDTH / 2.0f);

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
