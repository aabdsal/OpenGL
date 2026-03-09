#define PROYECTO "CAMARA"
#define NDEBUG = true
#pragma warning(disable: 4305) 

#include <iostream>			
#include "../codebase.h"

using namespace std;
using namespace cb;

// Variables globales
int ANCHO = 600, ALTO = 600;
bool perspectiva = true;

void init()
// Inicializaciones
{
	cout << "GL version " << glGetString(GL_VERSION) << endl;

	// Configuracion del motor de render
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void display()
// Funcion de atencion al dibujo
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Solidarios con la camara
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(0, 0, -3);
	glutWireCone(0.2,0.3,20,1);
	glPopMatrix();

	// Fijos es la escena
	ejes();

	glPushMatrix();
	glTranslatef(0.5, 0, 0);
	glColor3f(0, 0, 0.3);
	glutSolidTeapot(0.50);
	glColor3fv(AMARILLO);
	glutWireTeapot(0.51);
	glPopMatrix();

	glTranslatef(-0.5, 0, 0);
	glColor3f(0.3, 0, 0);
	glutSolidTeapot(0.50);
	glColor3fv(AMARILLO);
	glutWireTeapot(0.51);

	glFlush();
}

void onReshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
	// Marco dentro del area de dibujo
	glViewport(0, 0, w, h);
	float razon = (float)w / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (perspectiva) {
		gluPerspective(60, razon, 0.1, 20);
	}
	else {
		if (razon > 1) {    // El marco es mas ancho que alto
			glOrtho(-2 * razon, 2 * razon, -2, 2, -20, 20);
		}
		else {
			glOrtho(-2, 2, -2 / razon, 2 / razon, -20, 20);
		}
	}
}
void keyboard(unsigned char key, int x, int y)
{
	if (key == 'p' || key == 'P') {
		perspectiva = !perspectiva;
		onReshape(ANCHO, ALTO);
		glutPostRedisplay();
	}
}
int main(int argc, char** argv)
// Programa principal
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow(PROYECTO);
	init();

	// Registro de callbacks	
	glutDisplayFunc(display);
	glutReshapeFunc(onReshape);
	glutKeyboardFunc(keyboard);

	// Bucle de atencion a eventos
	glutMainLoop();
}