#include <GL/glut.h>
#include <iostream>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.5, -0.5);
        glVertex2f(0.5, -0.5);
        glVertex2f(0.0, 0.5);
    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    std::cout << "Iniciando teste OpenGL..." << std::endl;
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Teste OpenGL");
    
    std::cout << "Janela criada com sucesso!" << std::endl;
    
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glutDisplayFunc(display);
    
    std::cout << "Entrando no main loop..." << std::endl;
    glutMainLoop();
    
    return 0;
}
