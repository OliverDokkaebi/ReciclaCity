#include "tree.hpp"


void drawTree(){
    glColor3f(0.0f, 0.78f, 0.28f);
    for( Tree &tree : mapTree){
        glPushMatrix();
        glTranslatef(tree.x,tree.y,tree.z);
        glutSolidCube(4.0f);
        glPopMatrix();
    }
}