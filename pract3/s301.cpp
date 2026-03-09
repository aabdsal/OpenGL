#include <iostream>	
#include <vector>
#include "../codebase.h" 

#define PROYECTO "Ejercicio 1 de la práctica 3"

using namespace std;
using namespace cb; 

GLfloat m[16] = 
{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.6f, 0.0f, 1.0f
};
void display()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glLoadMatrixf(m);

    glColor3f(1.0f, 0.0f, 0.0f);
    glutWireCube(0.7f);

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