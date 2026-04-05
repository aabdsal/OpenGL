/***************************************************
	Animacion.cpp

	Variacion del punto de vista sobre la escena

	@author		R. Vivo'
	@version	2024, March

***************************************************/
#define PROYECTO "ANIMACION GL"
#define NDEBUG = true
#pragma warning(disable: 4305) 

#include <iostream>		
#include <sstream>
#include "../codebase.h"


using namespace std;
using namespace cb;

static const int tasaFPS = 60;

// variables dependientes del tiempo
static float gradosTeapot = 0;
static float gradosCamara = 0;

void init()
// Inicializaciones
{
	cout << "GL version " << glGetString(GL_VERSION) << endl;

	// Configuracion del motor de render
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

// Muestra en la barra de titulo los FPS
void muestraFPS()
{
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	static int fotogramas = 0;
	int ahora, tiempo_transcurrido;

	// Cada que pase por aqui incremento los fotogramas
	fotogramas++;

	// Calculo el tiempo transcurrido
	ahora = glutGet(GLUT_ELAPSED_TIME);
	tiempo_transcurrido = ahora - antes;
	// si ha transcurrido un segundo, mostrar los fotogramas
	if (tiempo_transcurrido > 1000) {
		// modificar la barra de titulo
		stringstream titulo;
		titulo << "FPS= " << fotogramas;
		glutSetWindowTitle(titulo.str().c_str());
		// reinicio a cuenta
		fotogramas = 0;
		antes = ahora;
	}

}

void display()
// Funcion de atencion al dibujo
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Vec3 camPosition(2*cosf(rad(gradosCamara)), cosf(rad(2*gradosCamara)), -2*sinf(rad(gradosCamara)));
	gluLookAt(camPosition.x, camPosition.y, camPosition.z, 0, 0, 0, 0, 1, 0);

	ejes();

	glPushMatrix();
	glTranslatef(0.5, 0, 0);
	glRotatef(gradosTeapot, 0, 1, 0);
	glColor3f(0, 0, 0.3);
	glutSolidTeapot(0.50);
	glColor3fv(AMARILLO);
	glutWireTeapot(0.51);
	glPopMatrix();

	glTranslatef(-0.5, 0, 0);
	glRotatef(gradosTeapot / 2, 0, 1, 0);
	glColor3f(0.3, 0, 0);
	glutSolidTeapot(0.50);
	glColor3fv(AMARILLO);
	glutWireTeapot(0.51);

	muestraFPS();

	glutSwapBuffers();
}

void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
	// Marco dentro del area de dibujo
	glViewport(0, 0, w, h);
	float razon = (float)w / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, razon, 0.1, 20);
}

void update()
{
	/*
	// Variar la variable entre frames sin control de tiempo
	gradosTeapot += 0.5;
	*/

	// Animacion coherente con el tiempo transcurrido
	static const float omega = 1;	// vueltas por segundo

	// Incialmente la hora de arranque
	static int antes = glutGet(GLUT_ELAPSED_TIME);

	// Hora actual
	int ahora = glutGet(GLUT_ELAPSED_TIME);

	// Tiempo transcurrido
	float tiempo_transcurrido = (ahora - antes) / 1000.0f;

	gradosTeapot += omega * 360 * tiempo_transcurrido;
	gradosCamara += omega / 60 * 360 * tiempo_transcurrido;
	antes = ahora;

	// Encolar un evento de redibujo
	glutPostRedisplay();
}

void onTimer(int tiempo)
{
	// Sirve para re-arrancar el reloj de cuenta atras
	glutTimerFunc(tiempo, onTimer, tiempo);

	update();
}

int main(int argc, char** argv)
// Programa principal
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow(PROYECTO);
	init();

	// Registro de callbacks	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	//glutIdleFunc(update);
	glutTimerFunc(1000 / tasaFPS, onTimer, 1000 / tasaFPS);

	// Bucle de atencion a eventos
	glutMainLoop();
}