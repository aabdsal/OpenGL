#include <iostream>	
#include <vector>
#include "../codebase.h" 

#define PROYECTO "Ejercicio 1 de la práctica 3"

using namespace std;
using namespace cb; 

void display()
{
    glClear(GL_COLOR_BUFFER_BIT); // Borra la pantalla
    glMatrixMode(GL_MODELVIEW); // Selecciona la modelview
    glLoadIdentity(); // Carga la matriz identidad
    glScalef(0.2,0.2,0.2);// Escalado general
    glutSolidSphere(1.0,30,30); // Dibuja el sol
    glPushMatrix();// Salva el estado de la Tr sol
        glRotatef(45,0,0,1); // Giro alrededor del sol
        glTranslatef(3.0,0.0,0.0); // Puesta en Orbita
        glutWireSphere(0.5,30,30);// Dibujo el planeta
        glPushMatrix(); // Salva el estado Tr planeta
            glRotatef(100,0,0,1);// Gira alrededor del planeta
            glTranslatef(1.0,0.0,0.0); // Orbita de la luna
            glutWireSphere(0.1,5,5);// Dibujo la luna
        glPopMatrix(); // Restaura Tr del planeta
    glPopMatrix(); // Restaura Tr del sol
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