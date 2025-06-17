#include "bins.hpp"
#include "garbage.hpp"
#include "player.hpp"

float angleX = 0.0f, angleY = 0.0f;
float zoom = -8.0f;
int lastX, lastY;
bool mousePressed = false;


//Dsenhar a lixeira
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

// Função para desenhar um cilindro (corpo da lixeira)
void drawCylinder(float radius, float height, int slices) {
    float angleStep = 2.0f * 3.14159265358979323846 / slices;
    
    // Tampa superior
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, height, 0.0f);
    for (int i = 0; i <= slices; i++) {
        float angle = i * angleStep;
        glVertex3f(radius * cos(angle), height, radius * sin(angle));
    }
    glEnd();
    
    // Base inferior
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for (int i = slices; i >= 0; i--) {
        float angle = i * angleStep;
        glVertex3f(radius * cos(angle), 0.0f, radius * sin(angle));
    }
    glEnd();
    
    // Laterais do cilindro
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; i++) {
        float angle = i * angleStep;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        glNormal3f(cos(angle), 0.0f, sin(angle));
        glVertex3f(x, 0.0f, z);
        glVertex3f(x, height, z);
    }
    glEnd();
}

// Função para desenhar a tampa da lixeira
void drawLid(float radius, float height) {
    glPushMatrix();
    glTranslatef(0.0f, height + 0.1f, 0.0f);
    
    // Tampa principal
    glPushMatrix();
    glScalef(1.0f, 0.3f, 1.0f);
    drawCylinder(radius * 1.1f, 0.3f, 20);
    glPopMatrix();
    
    // Alça da tampa
    glPushMatrix();
    glTranslatef(0.0f, 0.2f, 0.0f);
    glScalef(0.3f, 0.2f, 0.3f);
    drawCylinder(0.5f, 0.5f, 12);
    glPopMatrix();
    
    glPopMatrix();
}

// Função para desenhar o símbolo de reciclagem
void drawRecycleSymbol(float size) {
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    // Símbolo simplificado de reciclagem (triângulo com setas)
    for (int i = 0; i < 3; i++) {
        float angle = i * 2.0f * 3.14159265358979323846 / 3.0f;
        float x = size * cos(angle);
        float y = size * sin(angle);
        glVertex3f(x, y, 0.01f);
    }
    glEnd();
    
    // Setas do símbolo
    glBegin(GL_LINES);
    for (int i = 0; i < 3; i++) {
        float angle = i * 2.0f * 3.14159265358979323846 / 3.0f;
        float x1 = size * cos(angle);
        float y1 = size * sin(angle);
        float x2 = size * 0.7f * cos(angle + 0.3f);
        float y2 = size * 0.7f * sin(angle + 0.3f);
        glVertex3f(x1, y1, 0.01f);
        glVertex3f(x2, y2, 0.01f);
    }
    glEnd();
}

// Função para desenhar texto 3D simples
void drawText3D(const char* text, float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(0.001f, 0.001f, 0.001f);
    
    for (const char* c = text; *c != '\0'; c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }
    glPopMatrix();
}

// Função para desenhar uma lixeira completa
void drawTrashBin(float r, float g, float b, const char* label, float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    
    // Configurar material com a cor desejada
    GLfloat matDiffuse[] = {r, g, b, 1.0f};
    GLfloat matAmbient[] = {r * 0.3f, g * 0.3f, b * 0.3f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    
    // Corpo da lixeira
    drawCylinder(0.8f, 2.5f, 20);
      // Tampa
    drawLid(0.8f, 2.5f);
    
    // Configurar material preto para símbolos e texto
    GLfloat blackDiffuse[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat blackAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, blackDiffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, blackAmbient);
    
    // Símbolo de reciclagem na frente
    glPushMatrix();
    glTranslatef(0.0f, 1.8f, 0.85f);
    drawRecycleSymbol(0.3f);
    glPopMatrix();
    
    // Texto da categoria
    glPushMatrix();
    glTranslatef(-0.3f, 1.2f, 0.85f);
    drawText3D(label, 0, 0, 0);
    glPopMatrix();
    
    glPopMatrix();
}