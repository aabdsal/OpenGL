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
    // Razon de aspecto de la vista
    static const float razon = 2.0; // a/b = w'/h'
    
    // Razon de aspecto del area de dibujo
    float razonAD= float(w)/h;
    float wp,hp; // w',h'
    
    /* Centramos un viewport con la misma razon de la vista.
    Si el area tiene razon menor la vista se ajusta en horizontal (w)
    recortando el viewport por arriba y por abajo.
    Si el area tiene mayor razon que la vista se ajusta en vertical (h)
    y se recorta por la izquierda y la derecha. */

    if(razonAD<razon)
    {
        wp= float(w);
        hp= wp/razon;
        glViewport(0,int(h/2.0-hp/2.0),w,int(hp));
    }
    else
    {
        hp= float(h);
        wp= hp*razon;
        glViewport(int(w/2.0-wp/2.0),0,int(wp),h);
    }

    // Definimos la camara (matriz de proyeccion)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    /* CAMARA ORTOGRAFICA
    glOrtho(-1,1,-1/razon,1/razon, 0,10);*/
    
    /* CAMARA PERSPECTIVA */
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