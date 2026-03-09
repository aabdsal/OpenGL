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
#include "../codebase.h"
#include "Engranajes.h"


using namespace std;
using namespace cb;

static float giroxTA = 0, giroyTA = 0;
static float giroxCam = 0, giroyCam = 0;
static int xanterior, yanterior;
static bool dragging = false;

// Diente
static GLuint mtl_diente;
static GLuint diente, cara, doscaras, perfil1, perfil2, perfilT, unaRueda, rueda, engranaje;
static const int NDIENTES = 30;
static const float HDIENTE = 0.2;
static const float DPRIMITIVO = 1.7;
static const float DEJE = 0.7;
static const float GROSOR = 0.3;

void init() // Inicializaciones
{
	cout << "GL version " << glGetString(GL_VERSION) << endl;

	// Configuracion motor
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);

	// Luces 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, BLANCO);
	glLightfv(GL_LIGHT0, GL_SPECULAR, BLANCO);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, GRISCLARO);
	glLightfv(GL_LIGHT1, GL_SPECULAR, BLANCO);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, GRISOSCURO);

	// Materiales
	mtl_diente = glGenLists(1);
	glNewList(mtl_diente, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, GRISOSCURO);
	glMaterialfv(GL_FRONT, GL_SPECULAR, GRISCLARO);
	glMaterialf(GL_FRONT, GL_SHININESS, 15);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
	glShadeModel(GL_FLAT);
	glEndList();

	// Geometrias
	vector<Vec3> picos = circlePoints(DPRIMITIVO / 2.0 + HDIENTE / 2.0, PI / 2.0, NDIENTES);
	vector<Vec3> valles = circlePoints(DPRIMITIVO / 2.0 - HDIENTE / 2.0, PI / 2.0 - PI / NDIENTES, NDIENTES);
	vector<Vec3> ejes = circlePoints(DEJE / 2.0, PI / 2.0, NDIENTES);

    diente = glGenLists(1);
    glNewList(diente, GL_COMPILE);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3fv(valles[0]);
    glVertex3fv(picos[0]);
    glVertex3fv(ejes[0]);
    glVertex3fv(valles[1]);
    glVertex3fv(ejes[1]);  
    glEnd();
    glEndList();

    cara = glGenLists(2);
    glNewList(cara, GL_COMPILE);
    for (int i = 0; i < NDIENTES; i++)
    {
        glCallList(diente);
        glRotatef(360.0f/NDIENTES, 0.0, 0.0f, 1.0f);
    }
    glEnd();
    glEndList();

    doscaras = glGenLists(2);
    glNewList(doscaras, GL_COMPILE);
    glCallList(cara);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -GROSOR);
    glCallList(cara);
    glEnd();
    glEndList();

    perfil1 = glGenLists(3);
    glNewList(perfil1, GL_COMPILE);
    glNormal3f(valles[0].x, valles[0].y, 0.0f);
    glBegin(GL_QUADS);
    glVertex3fv(valles[0]);
    glVertex3fv(valles[0] - Vec3(0, 0, -GROSOR));
    glVertex3fv(picos[0]- Vec3(0,0, -GROSOR));
    glVertex3fv(picos[0]);
    glEnd();
    glEndList();

    perfil2 = glGenLists(4);
    glNewList(perfil2, GL_COMPILE);
    for (int i = 0; i < NDIENTES; i++)
    {
        glCallList(perfil1);
        glRotatef(360.0f/NDIENTES, 0.0, 0.0f, 1.0f);
    }
    glEnd();
    glEndList();

    perfilT = glGenLists(5);
    glNewList(perfilT, GL_COMPILE);
    glCallList(perfil2);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -GROSOR);
    glCallList(perfil2);
    glEnd();
    glEndList();

	unaRueda = glGenLists(6);
	glNewList(unaRueda, GL_COMPILE);
	glNormal3f(-ejes[0].x, -ejes[0].y, 0.0f);
	glBegin(GL_QUADS);
	glVertex3fv(ejes[0]);
	glVertex3fv(ejes[0] - Vec3(0, 0, -GROSOR));
	glVertex3fv(ejes[1] - Vec3(0, 0, -GROSOR));
    glVertex3fv(ejes[1]);
	glEnd();
	glEndList();

    rueda = glGenLists(7);
    glNewList(rueda, GL_COMPILE);
    for (int i = 0; i < NDIENTES; i++)
    {
        glCallList(unaRueda);
        glRotatef(360.0f/NDIENTES, 0.0, 0.0f, 1.0f);
    }
    glEnd();
    glEndList();

    engranaje = glGenLists(7);
    glNewList(engranaje, GL_COMPILE);
    glCallList(doscaras);
    glCallList(perfilT);
    glCallList(rueda);
    glEnd();
    glEndList();

}

void display() // Funcion de atencion al dibujo
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Luces solidarias a la camara
	GLfloat posicionL1[] = { 0,1,0,1 };
	glLightfv(GL_LIGHT1, GL_POSITION, posicionL1);

	Vec3 posicionCamara(0, 0, 4);
	posicionCamara = posicionCamara.rotX(rad(giroxCam));
	posicionCamara = posicionCamara.rotY(rad(giroyCam));
	gluLookAt(posicionCamara.x, posicionCamara.y, posicionCamara.z, 0, 0, 0, 0, 1, 0);

	// Luces fijas en la escena
	GLfloat posicionL0[] = { 0, 1,0,0 };
	glLightfv(GL_LIGHT0, GL_POSITION, posicionL0);

	// Engranaje
	//glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glCallList(mtl_diente);

	glCallList(engranaje);
	//glPopAttrib();

	ejes();

	glutSwapBuffers();
}

void reshape(GLint w, GLint h) // Funcion de atencion al redimensionamiento
{
	// Marco dentro del area de dibujo
	glViewport(0, 0, w, h);
	float razon = (float)w / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, razon, 0.1, 20);
}


void onKey(unsigned char tecla, int x, int y)
{
	switch (tecla) {
	case 27:
		exit(0);
	}
	glutPostRedisplay();
}
void onClick(int boton, int estado, int x, int y)
{
	// Almaceno la coordenada que se pulso
	if (boton == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {
		xanterior = x;
		yanterior = y;
		dragging = true;
	}
	else if (estado == GLUT_UP) dragging = false;
}

void onDrag(int x, int y)
{
	if (!dragging) return;

	static const float pixel2grados = 1;

	giroyCam += (x - xanterior) * pixel2grados;
	giroxCam += (y - yanterior) * pixel2grados;
	// Limitar la latitud a ]-90,90[
	if (fabs(giroxCam) > 89) giroxCam = signo(giroxCam) * 89.0f;


	xanterior = x;
	yanterior = y;

	glutPostRedisplay();

}

int main(int argc, char** argv)
// Programa principal
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow(PROYECTO);
	init();

	// Registro de callbacks	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(onKey);
	glutMouseFunc(onClick);
	glutMotionFunc(onDrag);

	// Bucle de atencion a eventos
	glutMainLoop();

}