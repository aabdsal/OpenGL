#define PROYECTO "Ejercicio 1 de la práctica 4"
#define NDEBUG = true
#pragma warning(disable: 4305) 

#include <iostream>			
#include "../codebase.h"

using namespace std;
using namespace cb;

void display()
// Funcion de atencion al dibujo
{
    glClear(GL_COLOR_BUFFER_BIT); // Borra la pantalla
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,5,0,0,0,0,1,0); // Situa la camara
    glutWireSphere(1.0,20,20); // Dibuja la esfera
    glFlush();
}

void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
    // Usamos toda el area de dibujo
    glViewport(0,0,w,h);
    // Definimos la camara (matriz de proyeccion)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float razon = (float) w / h;
    /* CAMARA ORTOGRAFICA
    Ajustamos la vista a la dimenson más pequenya del viewport para
    poder ver la totalidad de la ventana del mundo real (2x2)
    if(w<h)
    glOrtho(-1,1,-1/razon,1/razon, 0,10);
    else
    glOrtho(-1*razon,1*razon,-1,1, 0,10); */
    /* CAMARA PERSPECTIVA
    La razon de aspecto se pasa directamente a la camara perspectiva
    Como damos fijo el angulovertical, el tamanyo del dibujo solo se
    modifica cuando variamos la altura del viewport */
    gluPerspective(45,razon,1,10);
}

int main(int argc, char** argv)
// Programa principal
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow(PROYECTO);

	// Registro de callbacks	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	// Bucle de atencion a eventos
	glutMainLoop();
}