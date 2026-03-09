#include <iostream>	
#include <vector>
#include "../codebase.h" 

#define PROYECTO "Ejercicio 2 de la práctica 3"

using namespace std;
using namespace cb; 

void display()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);

    glColor3f(1.0f, 0.0f, 0.0f);
    glTranslatef(0.5f, 0.6f, 0.0f);
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);

    //glLoadIdentity(); si cargamos la identidad no se pueden modificar los objetos

    glutWireCube(1.0f);

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