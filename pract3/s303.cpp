#include <iostream>	
#include <vector>
#include "../codebase.h" 

#define PROYECTO "Ejercicio 3 de la práctica 3"

using namespace std;
using namespace cb; 

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
        glColor3f(1.0f, 0.0f, 0.0f);
        glTranslatef(-0.35f, 0.0f, 0.0f);
        glutWireSphere(0.35f, 20.0f, 20.0f);
    glPopMatrix();
    
    glPushMatrix();
        glColor3f(0.0f, 0.0f, 1.0f);
        glTranslatef(0.35f, 0.0f, 0.0f);
        glutWireSphere(0.35f, 20.0f, 20.0f);
    glPopMatrix();

    glPushMatrix();
        glColor3f(1.0f, 0.0f, 1.0f);
        glutWireSphere(0.35f, 20.0f, 20.0f);
    glPopMatrix();

    glFlush();
}
void reshape(GLint w, GLint h)
{

}
int main(int argc, char** argv)
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(50, 50);

	// Crear ventana
	glutCreateWindow(PROYECTO);
	// Registrar callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	// Bucle de atencion a eventos
    glutMainLoop();
}