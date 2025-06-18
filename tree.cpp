#include "tree.hpp"


void drawTree() {
    for (Tree &tree : mapTree) {
        glPushMatrix();
        glTranslatef(tree.x, tree.y, tree.z);

        // --- Tronco fofinho e mais visível ---
        glColor3f(0.55f, 0.27f, 0.07f); // marrom
        glPushMatrix();
        glTranslatef(0.0f, 1.2f, 0.0f);    // sobe metade da altura do tronco (tronco de 2.4)
        glScalef(0.8f, 2.4f, 0.8f);        // tronco um pouco mais alto e gordinho
        glutSolidCube(1.0f);
        glPopMatrix();

        // --- Copa um pouco mais alta ---
        glColor3f(0.0f, 0.6f, 0.0f);        // verde escuro
        glPushMatrix();
        glTranslatef(0.0f, 3.5f, 0.0f);     // sobe mais pra ajustar à altura do tronco
        glScalef(3.0f, 3.5f, 3.0f);         // copa mais altinha
        glutSolidCube(1.0f);
        glPopMatrix();

        glPopMatrix();
    }
}