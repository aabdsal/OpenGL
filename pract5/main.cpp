#define PROYECTO "Practica 5 de OpenGL"

#include <iostream>	
#include <sstream>
#include "../codebase.h"
#include "../Engranajes.h"


using namespace std;
using namespace cb;

static float giroxTA = 0, giroyTA = 0;
static float giroxCam = 0, giroyCam = 0;
static int xanterior, yanterior;
static bool dragging = false;

// Diente
static GLuint materialEngranaje;
static GLuint diente, cara, doscaras, perfil1, perfil2, perfilT, unaRueda, rueda, engranaje, dosEngranajes, engranajeGrande, engranajePeque;
static const float HDIENTE = 0.2f;
static const float DPRIMITIVO = 1.7f;
static const float DENGRANAJES = 1.33f;
static const float DEJE = 0.7f;
static const float GROSOR = 0.3f;

// animacion
static const float radio = 5.0f; 
static float angulo = 0.0f; 
static float ojo[] = {0,0,radio}; 
static const float velocidad = 2.0f*3.1415926f/60.0f; // Radio de giro de la camara y el engranaje grande
static const int tasaFPS = 60;

static float anguloPeque = 3.1415926f/20.0f; 
static const float velocidadPeque = 0.2094333333f; // Radio de giro del engranaje pequeño, calculado por la relacion de las velocidades angulares y el numero de dientes con la grande.

void muestraFPS()
// Calcula la frecuencia y la muestra en el título de la ventana cada segundo
{
    int ahora, tiempo_transcurrido;
    static int antes = 0;
    static int FPS = 0;
    stringstream titulo;

    //Cuenta de llamadas a esta función en el último segundo
    FPS++;
    ahora = glutGet(GLUT_ELAPSED_TIME); tiempo_transcurrido = ahora - antes; 

    if(tiempo_transcurrido > 1000)
    { 
        titulo << "FPS: " << FPS; 
        glutSetWindowTitle(titulo.str().c_str());
        antes = ahora; 
        FPS = 0; //Tiempo transcurrido desde el inicio
        //Tiempo transcurrido desde antes
        //Acaba de rebasar el segundo
        //Se muestra una vez por segundo
        //Ahora ya es antes
        //Reset del conteo
    }
}
void crearEngranaje(float NDIENTES)
{
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

    cara = glGenLists(1);
    glNewList(cara, GL_COMPILE);
    for (int i = 0; i < NDIENTES; i++)
    {
        glCallList(diente);
        glRotatef(360.0f/NDIENTES, 0.0, 0.0f, 1.0f);
    }
    glEnd();
    glEndList();

    doscaras = glGenLists(1);
    glNewList(doscaras, GL_COMPILE);
    glCallList(cara);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -GROSOR);
    glCallList(cara);
    glEnd();
    glEndList();

    perfil1 = glGenLists(1);
    glNewList(perfil1, GL_COMPILE);
    glNormal3f(valles[0].x, valles[0].y, 0.0f);
    glBegin(GL_QUADS);
    glVertex3fv(valles[0]);
    glVertex3fv(valles[0] - Vec3(0, 0, -GROSOR));
    glVertex3fv(picos[0]- Vec3(0,0, -GROSOR));
    glVertex3fv(picos[0]);
    glEnd();
    glEndList();

    perfil2 = glGenLists(1);
    glNewList(perfil2, GL_COMPILE);
    for (int i = 0; i < NDIENTES; i++)
    {
        glCallList(perfil1);
        glRotatef(360.0f/NDIENTES, 0.0, 0.0f, 1.0f);
    }
    glEnd();
    glEndList();

    perfilT = glGenLists(1);
    glNewList(perfilT, GL_COMPILE);
    glCallList(perfil2);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -GROSOR);
    glCallList(perfil2);
    glEnd();
    glEndList();

	unaRueda = glGenLists(1);
	glNewList(unaRueda, GL_COMPILE);
	glNormal3f(-ejes[0].x, -ejes[0].y, 0.0f);
	glBegin(GL_QUADS);
	glVertex3fv(ejes[0]);
	glVertex3fv(ejes[0] - Vec3(0, 0, -GROSOR));
	glVertex3fv(ejes[1] - Vec3(0, 0, -GROSOR));
    glVertex3fv(ejes[1]);
	glEnd();
	glEndList();

    rueda = glGenLists(1);
    glNewList(rueda, GL_COMPILE);
    for (int i = 0; i < NDIENTES; i++)
    {
        glCallList(unaRueda);
        glRotatef(360.0f/NDIENTES, 0.0, 0.0f, 1.0f);
    }
    glEnd();
    glEndList();

    engranaje = glGenLists(1);
    glNewList(engranaje, GL_COMPILE);
    glCallList(doscaras);
    glCallList(perfilT);
    glCallList(rueda);
    glEnd();
    glEndList();

}

void init() // Inicializaciones
{
	// Configuracion motor
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);

	// Luces
	glLightfv(GL_LIGHT0, GL_DIFFUSE, GRISCLARO);
	glLightfv(GL_LIGHT0, GL_SPECULAR, BLANCO);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, GRISCLARO);
	glLightfv(GL_LIGHT1, GL_SPECULAR, BLANCO);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, GRISOSCURO);

    crearEngranaje(40);
    engranajeGrande = engranaje;

    crearEngranaje(20);
    engranajePeque = engranaje;
}

void display() // Funcion de atencion al dibujo
{  
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    gluLookAt(ojo[0], ojo[1], ojo[2], 0, 0, 0, 0, 1, 0); // Situa la camara

	// Engranaje
	glPushAttrib(GL_ALL_ATTRIB_BITS);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // rellenar ambas caras del poligono 
    glPushMatrix();
    glRotatef(deg(angulo), 0, 0, 1);
	glCallList(engranajeGrande);
    glPopMatrix();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // dibujar ambas caras del poligono en modo alámbrico
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glColor3fv(BLANCO);
    glRotatef(deg(angulo), 0, 0, 1);
	glCallList(engranajeGrande);
    glEnable(GL_LIGHTING);
    glPopMatrix();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPushMatrix();
    glTranslatef(DENGRANAJES, 0, 0.05f);
    glScalef(0.5f, 0.5f, 0.5f);
    glRotatef(-deg(anguloPeque), 0, 0, 1);
	glCallList(engranajePeque);
    glPopMatrix();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glColor3fv(BLANCO);
    glTranslatef(DENGRANAJES, 0, 0.05f);
    glScalef(0.5f, 0.5f, 0.5f);
    glRotatef(-deg(anguloPeque), 0, 0, 1);
    glCallList(engranajePeque);
    glEnable(GL_LIGHTING);
    glPopMatrix();

	glPopAttrib();
    
	//ejes();
    muestraFPS();
	glutSwapBuffers();
}

void reshape(GLint w, GLint h) // Funcion de atencion al redimensionamiento
{
	// Marco dentro del area de dibujo
	glViewport(0, 0, w, h);
	float razon = (float)w / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(30, razon, 1, 10);

}

void onTimer(int valor)
{
    //Calculamos el tiempo transcurrido desde la última vez
    static int antes = 0;
    static bool check = false;
    int ahora, tiempo_transcurrido;
    ahora = glutGet(GLUT_ELAPSED_TIME); 
    tiempo_transcurrido = ahora - antes; // Tiempo transcurrido desde el inicio
    // Tiempo transcurrido desde antes en msg.
    // angulo = angulo anterior + velocidad x tiempo
    if(tiempo_transcurrido > 5000)
    {
        check = true;
    }
    if(check)
    {
        // camara i el engranaje grande
        angulo += velocidad * (tiempo_transcurrido/1000.0);
        ojo[0] = radio * sin(angulo);
        ojo[2] = radio * cos(angulo);

        // engranaje pequeño
        anguloPeque += velocidadPeque * (tiempo_transcurrido/1000.0);
        antes = ahora;
    }
    glutTimerFunc(1000/tasaFPS,onTimer,tasaFPS); // Se encola un nuevo timer
    glutPostRedisplay();
}
int main(int argc, char** argv) // Programa principal
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(900, 600);
	glutCreateWindow(PROYECTO);
	init();

	// Registro de callbacks	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
    glutTimerFunc(1000/tasaFPS, onTimer, tasaFPS); // Alta de la funcion de atencion al timer

	// Bucle de atencion a eventos
	glutMainLoop();
}