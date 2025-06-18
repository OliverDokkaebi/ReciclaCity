#include "garbage.hpp"

//Desenhando lixo de acordo com o tipo
void drawTrash(){
    for(Trash &trash : mapTrash){
        if(!trash.collect){ //Apenas lixos nao coletados sao redenrizados em cena
            glPushMatrix();
            glTranslatef(trash.x,trash.y,trash.z);
            switch (trash.type){ 
            case PAPER: desenharLixoPapel();break;
            case PLASTIC: desenharLixoPlastico();break;
            case METAL: desenharLixoMetal();break;
            case GLASS: desenharLixoVidro();break;
            }
            glPopMatrix();
        }
    }
}

void desenharLixoMetal() {
    glPushMatrix();
    glTranslatef(-3.0f, 0.0f, 0.0f);
    
    // Cor metálica (cinza brilhante)
    glColor3f(0.7f, 0.7f, 0.8f);
    
    // Corpo da lata (cilindro)
    GLUquadric* cilindro = gluNewQuadric();
    gluCylinder(cilindro, 0.5, 0.5, 1.5, 20, 20);
    
    // Tampa inferior
    glPushMatrix();
    glRotatef(180, 1, 0, 0);
    gluDisk(cilindro, 0, 0.5, 20, 1);
    glPopMatrix();
    
    // Tampa superior
    glPushMatrix();
    glTranslatef(0, 0, 1.5);
    gluDisk(cilindro, 0, 0.5, 20, 1);
    glPopMatrix();
    
    // Amassados na lata (esferas pequenas para simular)
    glColor3f(0.5f, 0.5f, 0.6f);
    glPushMatrix();
    glTranslatef(0.3f, 0.0f, 0.8f);
    glutSolidSphere(0.1, 10, 10);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-0.2f, 0.3f, 1.0f);
    glutSolidSphere(0.08, 10, 10);
    glPopMatrix();
    
    gluDeleteQuadric(cilindro);
    glPopMatrix();
}

// Função para desenhar papel amassado (esfera irregular)
void desenharLixoPapel() {
    glPushMatrix();
    glTranslatef(-1.0f, 0.0f, 0.0f);
    
    // Cor do papel (branco/bege)
    glColor3f(0.9f, 0.9f, 0.8f);
    
    // Papel amassado principal
    glutSolidSphere(0.6, 12, 12);
    
    // Adicionar irregularidades (pequenas esferas)
    glColor3f(0.8f, 0.8f, 0.7f);
    glPushMatrix();
    glTranslatef(0.4f, 0.2f, 0.3f);
    glutSolidSphere(0.2, 8, 8);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-0.3f, 0.4f, -0.1f);
    glutSolidSphere(0.15, 8, 8);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.1f, -0.4f, 0.4f);
    glutSolidSphere(0.18, 8, 8);
    glPopMatrix();
    
    // Simular texto no papel
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.6f);
    glScalef(0.1f, 0.05f, 0.01f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glPopMatrix();
}

// Função para desenhar vidro quebrado (fragmentos)
void desenharLixoVidro() {
    glPushMatrix();
    glTranslatef(1.0f, 0.0f, 0.0f);
    
    // Cor do vidro (verde translúcido)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.2f, 0.7f, 0.3f, 0.7f);
    
    // Fragmento principal (cubo irregular)
    glPushMatrix();
    glRotatef(45, 1, 0, 1);
    glScalef(0.8f, 0.3f, 1.2f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    // Fragmentos menores
    glPushMatrix();
    glTranslatef(0.6f, 0.3f, 0.4f);
    glRotatef(30, 0, 1, 1);
    glScalef(0.4f, 0.2f, 0.6f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-0.4f, -0.2f, 0.7f);
    glRotatef(-60, 1, 1, 0);
    glScalef(0.3f, 0.5f, 0.4f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.2f, -0.5f, -0.3f);
    glRotatef(90, 0, 0, 1);
    glScalef(0.5f, 0.2f, 0.8f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    // Pequenos cacos
    glColor4f(0.1f, 0.8f, 0.2f, 0.8f);
    for(int i = 0; i < 8; i++) {
        glPushMatrix();
        float angle = i * 45;
        float x = 0.8f * cos(angle * 3.14159265358979323846f / 180);
        float z = 0.8f * sin(angle * 3.14159265358979323846f / 180);
        glTranslatef(x, -0.8f, z);
        glScalef(0.1f, 0.05f, 0.15f);
        glutSolidCube(1.0);
        glPopMatrix();
    }
    
    glDisable(GL_BLEND);
    glPopMatrix();
}

// Função para desenhar garrafa plástica
void desenharLixoPlastico() {
    glPushMatrix();
    glTranslatef(3.0f, 0.0f, 0.0f);
    
    // Cor do plástico (azul translúcido)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.2f, 0.4f, 0.8f, 0.6f);
    
    GLUquadric* garrafa = gluNewQuadric();
    
    // Corpo da garrafa (cone truncado)
    glPushMatrix();
    gluCylinder(garrafa, 0.6, 0.4, 2.0, 16, 16);
    glPopMatrix();
    
    // Base da garrafa
    glPushMatrix();
    glRotatef(180, 1, 0, 0);
    gluDisk(garrafa, 0, 0.6, 16, 1);
    glPopMatrix();
    
    // Gargalo
    glPushMatrix();
    glTranslatef(0, 0, 2.0);
    gluCylinder(garrafa, 0.4, 0.2, 0.5, 12, 12);
    glPopMatrix();
    
    // Tampa
    glColor4f(0.8f, 0.2f, 0.2f, 0.9f);
    glPushMatrix();
    glTranslatef(0, 0, 2.5);
    gluCylinder(garrafa, 0.22, 0.22, 0.2, 12, 12);
    gluDisk(garrafa, 0, 0.22, 12, 1);
    glPopMatrix();
    
    // Rótulo
    glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
    glPushMatrix();
    glTranslatef(0, 0, 1.0);
    gluCylinder(garrafa, 0.61, 0.5, 0.6, 16, 16);
    glPopMatrix();
    
    // Amassados na garrafa
    glColor4f(0.1f, 0.3f, 0.7f, 0.7f);
    glPushMatrix();
    glTranslatef(0.4f, 0.0f, 1.2f);
    glutSolidSphere(0.1, 8, 8);
    glPopMatrix();
    
    gluDeleteQuadric(garrafa);
    glDisable(GL_BLEND);
    glPopMatrix();
}