#include "player.hpp"

void Player::zerarPlayer(){
        this->x = 0.0f;
        this->z = -5.0f;
        this->size = 2.0f;
        this->score = 0;
        this->inv.paper = this->inv.plastic = this->inv.metal = this->inv.glass = 0;
}

void Player::drawPlayer(Player player){
    glPushMatrix();
    glColor3f(0.5f,0.5f,0.5f);
    glTranslated(player.x,player.y,player.z);
    glPushMatrix();
    glScalef(3,3,3);
    drawChicken();
    glPopMatrix();
    glPopMatrix();
}

float chickenX = 0.0f;
float chickenZ = 0.0f;
float rotationAngle = 0.0f;

void drawChicken() {
    glPushMatrix();
    glTranslatef(chickenX, 0.5f, chickenZ);
    glRotatef(rotationAngle, 0, 1, 0);
    
    // Corpo principal (branco)
    glColor3f(1.0f, 0.95f, 0.95f);
    glPushMatrix();
    glScalef(0.3f, 0.3f, 0.5f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Asa esquerda (branca)
    glColor3f(1.0f, 0.95f, 0.95f);
    glPushMatrix();
    glTranslatef(0.2f, 0.05f, 0.0f);
    glScalef(0.2f, 0.3f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Asa direita (branca)
    glColor3f(1.0f, 0.95f, 0.95f);
    glPushMatrix();
    glTranslatef(-0.2f, 0.05f, 0.0f);
    glScalef(0.2f, 0.3f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Cabeça (branca)
    glColor3f(1.0f, 0.95f, 0.95f);
    glPushMatrix();
    glTranslatef(0.0f, 0.35f, 0.1f);
    glScalef(0.35f, 0.35f, 0.35f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Olho esquerdo (preto)
    glColor3f(0.01f, 0.01f, 0.01f);
    glPushMatrix();
    glTranslatef(0.15f, 0.42f, 0.20f);
    glScalef(0.1f, 0.1f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Olho direito (preto)
    glColor3f(0.01f, 0.01f, 0.01f);
    glPushMatrix();
    glTranslatef(-0.15f, 0.42f, 0.20f);
    glScalef(0.1f, 0.1f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
        // Crista (vermelho) - sem iluminação
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, 0.6f, 0.1f);
    glScalef(0.2f, 0.2f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glEnable(GL_LIGHTING);
    
    // Bico (laranja)
    glColor3f(1.0f, 0.6f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.35f, 0.3f);
    glScalef(0.2f, 0.2f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Pernas (laranja)
    glColor3f(1.0f, 0.6f, 0.0f);
    // Perna esquerda
    glPushMatrix();
    glTranslatef(0.15f, -0.2f, 0.0f);
    glScalef(0.1f, 0.3f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Perna direita
    glPushMatrix();
    glTranslatef(-0.15f, -0.2f, 0.0f);
    glScalef(0.1f, 0.3f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Pés (laranja)
    // Pé esquerdo
    glPushMatrix();
    glTranslatef(0.15f, -0.35f, 0.1f);
    glScalef(0.15f, 0.1f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Pé direito
    glPushMatrix();
    glTranslatef(-0.15f, -0.35f, 0.1f);
    glScalef(0.15f, 0.1f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPopMatrix();
}