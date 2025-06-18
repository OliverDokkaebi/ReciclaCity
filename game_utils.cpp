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
        
        // Reinicia o timer para o novo nível
        carTimerActive = false;
        carTimerActive = true;
        
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

    // Desenha o título com sombra
    // Sombra do título
    glColor3f(0.0f, 0.0f, 0.0f);
    desenharTexto(screenHeight - 152, "ReciclaCity", 0.7f);
    // Título principal
    glColor3f(0.5f, 1.0f, 0.5f);
    desenharTexto(screenHeight - 150, "ReciclaCity", 0.7f); 

    // Linha decorativa dupla sob o título
    glLineWidth(4.0f);
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(screenWidth * 0.15f, screenHeight - 170);
    glVertex2f(screenWidth * 0.85f, screenHeight - 170);
    glEnd();
    
    glLineWidth(2.0f);
    glColor3f(0.8f, 0.8f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(screenWidth * 0.15f, screenHeight - 175);
    glVertex2f(screenWidth * 0.85f, screenHeight - 175);
    glEnd();

    // Desenha os botões com sombras
    float buttonX = (screenWidth / 2.0f - BUTTON_WIDTH / 2.0f);
    
    // Sombras dos botões
    glColor3f(0.0f, 0.0f, 0.0f);
    // Sombra Iniciar
    glBegin(GL_QUADS);
    glVertex2f(buttonX + 3, MENU_START_BUTTON_Y - 3);
    glVertex2f(buttonX + BUTTON_WIDTH + 3, MENU_START_BUTTON_Y - 3);
    glVertex2f(buttonX + BUTTON_WIDTH + 3, MENU_START_BUTTON_Y + BUTTON_HEIGHT - 3);
    glVertex2f(buttonX + 3, MENU_START_BUTTON_Y + BUTTON_HEIGHT - 3);
    glEnd();
    
    // Sombra Instruções
    glBegin(GL_QUADS);
    glVertex2f(buttonX + 3, MENU_INSTRUCTIONS_BUTTON_Y - 3);
    glVertex2f(buttonX + BUTTON_WIDTH + 3, MENU_INSTRUCTIONS_BUTTON_Y - 3);
    glVertex2f(buttonX + BUTTON_WIDTH + 3, MENU_INSTRUCTIONS_BUTTON_Y + BUTTON_HEIGHT - 3);
    glVertex2f(buttonX + 3, MENU_INSTRUCTIONS_BUTTON_Y + BUTTON_HEIGHT - 3);
    glEnd();
    
    // Sombra Sair
    glBegin(GL_QUADS);
    glVertex2f(buttonX + 3, MENU_EXIT_BUTTON_Y - 3);
    glVertex2f(buttonX + BUTTON_WIDTH + 3, MENU_EXIT_BUTTON_Y - 3);
    glVertex2f(buttonX + BUTTON_WIDTH + 3, MENU_EXIT_BUTTON_Y + BUTTON_HEIGHT - 3);
    glVertex2f(buttonX + 3, MENU_EXIT_BUTTON_Y + BUTTON_HEIGHT - 3);
    glEnd();
    
    // Botões principais
    drawButton(buttonX, MENU_START_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, "Iniciar");
    drawButton(buttonX, MENU_INSTRUCTIONS_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, "Instrucoes");
    drawButton(buttonX, MENU_EXIT_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, "Sair");

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
    
    // Para qualquer timer anterior e inicia um novo
    carTimerActive = false;
    if (currentGameState == PLAYING) {
      carTimerActive = true;
      glutTimerFunc(16, updateCars, 0); 
    }
    glutPostRedisplay();
}

// Função de callback para cliques do mouse
void mouseClick(int button, int state, int x, int y) {
    if (currentGameState == MENU && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int invertedY = screenHeight - y; 
        float buttonX = (screenWidth / 2.0f - BUTTON_WIDTH / 2.0f);

        // Botão Iniciar
        if (x >= buttonX && x <= buttonX + BUTTON_WIDTH &&
            invertedY >= MENU_START_BUTTON_Y && invertedY <= MENU_START_BUTTON_Y + BUTTON_HEIGHT) {
            startGame();
        }
        // Botão Instruções
        else if (x >= buttonX && x <= buttonX + BUTTON_WIDTH &&
                 invertedY >= MENU_INSTRUCTIONS_BUTTON_Y && invertedY <= MENU_INSTRUCTIONS_BUTTON_Y + BUTTON_HEIGHT) {
            currentGameState = INSTRUCTIONS;
            glutPostRedisplay();
        }
        // Botão Sair
        else if (x >= buttonX && x <= buttonX + BUTTON_WIDTH &&
                 invertedY >= MENU_EXIT_BUTTON_Y && invertedY <= MENU_EXIT_BUTTON_Y + BUTTON_HEIGHT) {
            exit(0);
        }
    } else if (currentGameState == INSTRUCTIONS && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int invertedY = screenHeight - y;
        float buttonX = (screenWidth / 2.0f - BUTTON_WIDTH / 2.0f);

        // Botão Voltar
        if (x >= buttonX && x <= buttonX + BUTTON_WIDTH &&
            invertedY >= 50 && invertedY <= 50 + BUTTON_HEIGHT) {
            currentGameState = MENU;
            glutPostRedisplay();
        }
    } else if (currentGameState == PAUSED && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int invertedY = screenHeight - y;
        float buttonX = (screenWidth / 2.0f - BUTTON_WIDTH / 2.0f);

        if (x >= buttonX && x <= buttonX + BUTTON_WIDTH &&
            invertedY >= PAUSE_CONTINUE_BUTTON_Y && invertedY <= PAUSE_CONTINUE_BUTTON_Y + BUTTON_HEIGHT) {
            currentGameState = PLAYING;
            carTimerActive = true; // Reativa o timer
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

void drawInstructionsMenu() {
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

    // Título principal
    desenharTexto(screenHeight - 60, "COMO JOGAR", 0.5f);

    float startY = screenHeight - 120;
    float lineHeight = 30;
    float currentY = startY;

    // Seção Movimento
    glColor3f(1.0f, 1.0f, 0.0f); // Amarelo para títulos das seções
    desenharTexto(currentY, "MOVIMENTO:", 0.3f);
    currentY -= lineHeight;
    
    glColor3f(1.0f, 1.0f, 1.0f); // Branco para texto normal
    desenharTexto(currentY, "W - Mover para frente", 0.22f);
    currentY -= 22;
    desenharTexto(currentY, "S - Mover para tras", 0.22f);
    currentY -= 22;
    desenharTexto(currentY, "A - Mover para esquerda", 0.22f);
    currentY -= 22;
    desenharTexto(currentY, "D - Mover para direita", 0.22f);
    currentY -= 35;

    // Seção Coletar Lixo
    glColor3f(1.0f, 1.0f, 0.0f);
    desenharTexto(currentY, "COLETAR LIXO:", 0.3f);
    currentY -= lineHeight;
    
    glColor3f(1.0f, 1.0f, 1.0f);
    desenharTexto(currentY, "Seta CIMA - Coletar papel", 0.22f);
    currentY -= 22;
    desenharTexto(currentY, "Seta BAIXO - Coletar plastico", 0.22f);
    currentY -= 22;
    desenharTexto(currentY, "Seta DIREITA - Coletar metal", 0.22f);
    currentY -= 22;
    desenharTexto(currentY, "Seta ESQUERDA - Coletar vidro", 0.22f);
    currentY -= 35;

    // Seção Descartar Lixo
    glColor3f(1.0f, 1.0f, 0.0f);
    desenharTexto(currentY, "DESCARTAR LIXO:", 0.3f);
    currentY -= lineHeight;
    
    glColor3f(1.0f, 1.0f, 1.0f);
    desenharTexto(currentY, "Aproxime-se da lixeira correta", 0.22f);
    currentY -= 22;
    desenharTexto(currentY, "Use a mesma tecla para descartar", 0.22f);
    currentY -= 22;
    desenharTexto(currentY, "Tecla correta = +10 pontos", 0.22f);
    currentY -= 22;
    desenharTexto(currentY, "Tecla errada = -10 pontos", 0.22f);
    currentY -= 35;

    // Seção Objetivo
    glColor3f(1.0f, 1.0f, 0.0f);
    desenharTexto(currentY, "OBJETIVO:", 0.3f);
    currentY -= lineHeight;
    
    glColor3f(1.0f, 1.0f, 1.0f);
    desenharTexto(currentY, "Colete lixo e descarte corretamente", 0.22f);
    currentY -= 22;
    desenharTexto(currentY, "Evite os carros na estrada", 0.22f);
    currentY -= 22;
    desenharTexto(currentY, "Alcance 50 pontos por nivel", 0.22f);
    currentY -= 35;

    // Controles extras
    glColor3f(1.0f, 1.0f, 0.0f);
    desenharTexto(currentY, "CONTROLES EXTRAS:", 0.3f);
    currentY -= lineHeight;
    
    glColor3f(1.0f, 1.0f, 1.0f);
    desenharTexto(currentY, "ESC - Pausar/Continuar jogo", 0.22f);

    // Botão Voltar
    float buttonX = (screenWidth / 2.0f - BUTTON_WIDTH / 2.0f);
    drawButton(buttonX, 30, BUTTON_WIDTH, BUTTON_HEIGHT, "Voltar");

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glutSwapBuffers();
}
