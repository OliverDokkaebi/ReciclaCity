#include "game_utils.hpp"

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