/***************************************************
*	Primer_Proyecto.cpp
*
*	Codigo ejemplo inicial para practicas con OpenGL
*	y GLUT para ETSINF/UPV
*
*	@author	R.Vivo' <rvivo@upv.es>
*
***************************************************/
#define PROYECTO "Primer programa en OpenGL"

#include <iostream>	
#include <GLUT/glut.h>
						
float angle = 0;
// Funcion de atencion al evento de dibujo
void display()
{
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	//drawAxes
	glPushMatrix();
	glRotatef(angle, 1, 0, 0);
	glTranslatef((angle/180) * 3, 0.0 , 0.0);
	glutWireTeapot(0.5);
	glPopMatrix();
	glutSwapBuffers();
}
void timer(int){
	angle += 0.5;
	glutPostRedisplay();
	glutTimerFunc(16, timer, 0);
}
// Funcion de atencion al redimensionamiento
void reshape(GLint w, GLint h)
{
}

int main(int argc, char** argv)
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(50, 50);

	// Crear ventana
	glutCreateWindow(PROYECTO);

	// Registrar callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(16, timer, 0);
	// Bucle de atencion a eventos
	glutMainLoop();
}
