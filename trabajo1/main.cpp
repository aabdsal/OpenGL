#define PROYECTO "Mecanismo animado"

#include <iostream>	
#include "../codebase.h"
#include "../Engranajes.h"

using namespace std;
using namespace cb;

// Listas para crear las geometrias necesarias
static GLuint diente, cara, doscaras;
static GLuint perfil1, perfil2, perfilT;
static GLuint unaRueda, rueda;
static GLuint engranaje, dosEngranajes, engranajeGrande, engranajePeque;
static GLuint unPalo, Palos, PalosGrande, PalosPeque;
static GLuint unaC, dosC, unionC, uniCT, eje_interno, interno_completo, reloj;
static GLuint ManCorta, ManLarga;

vector<Vec3> ejesAux;

// Datos para la generar el engranaje
static const float HDIENTE = 0.1f;
static const float DPRIMITIVO = 1.7f;
static const float DENGRANAJES = 1.20f;
static const float GROSOR = 0.3f;

// Velocidades y angulos para la animación
static const float radio = 8.0f; 

static float anguloGrande = 0.0f; 
static float ojo[] = {0, 0, radio}; 
static const float velocidadGrande = 2.0f*3.1415926f/60.0f; // Radio de giro de la camara y el engranaje grande
static const int tasaFPS = 60;

static float anguloPeque = 3.1415926f/20.0f; 
static const float velocidadPeque = 0.2094333333f; // Radio de giro del engranaje pequeño, calculado por la relacion de las velocidades angulares y el numero de dientes con la grande.

static float anguloManLarga = 0.0f;
static const float vManLarga = 0.2094333333f; 

void crearEngranaje(float NDIENTES, float DEJE)
{
	vector<Vec3> picos = circlePoints(DPRIMITIVO / 2.0 + HDIENTE / 2.0, PI / 2.0, NDIENTES);
	vector<Vec3> valles = circlePoints(DPRIMITIVO / 2.0 - HDIENTE / 2.0, PI / 2.0 - PI / NDIENTES, NDIENTES);
	vector<Vec3> ejes = circlePoints(DEJE / 2.0, PI / 2.0, NDIENTES);

    ejesAux = ejes;

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
    glEndList();

    doscaras = glGenLists(1);
    glNewList(doscaras, GL_COMPILE);
        glCallList(cara);
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, -GROSOR);
        glCallList(cara);
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
    glEndList();

    perfilT = glGenLists(1);
    glNewList(perfilT, GL_COMPILE);
        glCallList(perfil2);
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, -GROSOR);
        glCallList(perfil2);
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
    glEndList();

    engranaje = glGenLists(1);
    glNewList(engranaje, GL_COMPILE);
        glCallList(doscaras);
        glCallList(perfilT);
        glCallList(rueda);
    glEndList();

}

void crearPalo(float Largo, float NDIENTES, vector<Vec3> ejes)
{
    unPalo = glGenLists(1);
    glNewList(unPalo, GL_COMPILE);
	glBegin(GL_QUADS);
	glVertex3fv(ejes[0]);
	glVertex3fv(ejes[0] + Vec3(0, 0, Largo));
	glVertex3fv(ejes[1] + Vec3(0, 0, Largo));
    glVertex3fv(ejes[1]);
    glEnd();
    glEndList();

    Palos = glGenLists(1);
    glNewList(Palos, GL_COMPILE);
    for (int i = 0; i < NDIENTES; i++)
    {
        glCallList(unPalo);
        glRotatef(360.0f/NDIENTES, 0.0, 0.0f, 1.0f);
    }
    glEndList();
}

void crearReloj(float NDIENTES, float DEJE)
{
	vector<Vec3> valles = circlePoints(DPRIMITIVO / 2.0 - HDIENTE / 2.0, PI / 2.0 - PI / NDIENTES, NDIENTES);
	vector<Vec3> ejes = circlePoints(DEJE / 2.0, PI / 2.0, NDIENTES);

    diente = glGenLists(1);
    glNewList(diente, GL_COMPILE);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glBegin(GL_TRIANGLE_STRIP);
        glVertex3fv(valles[0]);
        glVertex3fv(ejes[0]);
        glVertex3fv(valles[1]);
        glVertex3fv(ejes[1]);  
    glEnd();
    glEndList();

    unaC = glGenLists(1);
    glNewList(unaC, GL_COMPILE);
        for (int i = 0; i < NDIENTES; i++)
        {
            glCallList(diente);
            glRotatef(360.0f/NDIENTES, 0.0, 0.0f, 1.0f);
        }
    glEndList();

    dosC = glGenLists(1);
    glNewList(dosC, GL_COMPILE);
        glCallList(unaC);
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, -GROSOR);
        glCallList(unaC);
    glEndList();

    unionC = glGenLists(1);
    glNewList(unionC, GL_COMPILE);
    glNormal3f(valles[0].x, valles[0].y, 0.0f);
    glBegin(GL_QUADS);
        glVertex3fv(valles[0]);
        glVertex3fv(valles[0] - Vec3(0, 0, -GROSOR));
        glVertex3fv(valles[1]- Vec3(0,0, -GROSOR));
        glVertex3fv(valles[1]);
    glEnd();
    glEndList();

    uniCT = glGenLists(1);
    glNewList(uniCT, GL_COMPILE);
    for (int i = 0; i < NDIENTES; i++)
    {
        glCallList(unionC);
        glRotatef(360.0f/NDIENTES, 0.0, 0.0f, 1.0f);
    }
    glEndList();

	eje_interno= glGenLists(1);
	glNewList(eje_interno, GL_COMPILE);
	glNormal3f(-ejes[0].x, -ejes[0].y, 0.0f);
	glBegin(GL_QUADS);
        glVertex3fv(ejes[0]);
        glVertex3fv(ejes[0] - Vec3(0, 0, -GROSOR));
        glVertex3fv(ejes[1] - Vec3(0, 0, -GROSOR));
        glVertex3fv(ejes[1]);
    glEnd();
	glEndList();

    interno_completo = glGenLists(1);
    glNewList(interno_completo, GL_COMPILE);
    for (int i = 0; i < NDIENTES; i++)
    {
        glCallList(eje_interno);
        glRotatef(360.0f/NDIENTES, 0.0, 0.0f, 1.0f);
    }
    glEndList();

    reloj = glGenLists(1);
    glNewList(reloj, GL_COMPILE);
        glCallList(dosC);
        glCallList(uniCT);
        glCallList(interno_completo);
    glEndList();
}

void crearManecilla(float NDIENTES, float DEJE)
{
    float radioEsfera = DPRIMITIVO / 2.0f - HDIENTE / 2.0f;
    float base = radioEsfera * 0.045f;
    float paloCorto = radioEsfera * 0.45f;
    float paloLargo = radioEsfera * 0.9f;

    ManCorta = glGenLists(1);
    glNewList(ManCorta, GL_COMPILE);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLES);
        glVertex3f(-base, 0.0f, GROSOR);
        glVertex3f(base, 0.0f, GROSOR);
        glVertex3f(0.0f, paloCorto, GROSOR);
    glEnd();
    glEndList();

    ManLarga = glGenLists(1);
    glNewList(ManLarga, GL_COMPILE);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLES);
        glVertex3f(-base, 0.0f, GROSOR);
        glVertex3f(base, 0.0f, GROSOR);
        glVertex3f(0.0f, paloLargo, GROSOR);
    glEnd();
    glEndList();
}
void init() 
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, GRISCLARO);
	glLightfv(GL_LIGHT0, GL_SPECULAR, BLANCO);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, GRISCLARO);
	glLightfv(GL_LIGHT1, GL_SPECULAR, BLANCO);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, GRISOSCURO);

    crearEngranaje(40, 0.2f);
    engranajeGrande = engranaje;
    
    crearPalo(1.1f, 40, ejesAux);
    PalosGrande = Palos;

    crearEngranaje(20, 0.65f);
    engranajePeque = engranaje;

    crearPalo(4.3f, 20, ejesAux);
    PalosPeque = Palos;

    crearReloj(40, 0.08f);
    crearManecilla(40, 0.08f);

}

void display() 
{  
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    gluLookAt(ojo[0], ojo[1], ojo[2], 0, 0, 0, 0, 1, 0); 

	glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glPushMatrix();
        glRotatef(deg(anguloGrande), 0, 0, 1);
        glCallList(engranajeGrande);
    glPopMatrix();

    glPushMatrix();
        glDisable(GL_LIGHTING);
        glColor3f(0.1f, 0.1f, 0.1f);
        glCallList(PalosGrande);
        glEnable(GL_LIGHTING);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.0f, 0.0f, 1.05f);
        glScalef(0.3f, 0.3f, 0.3f);
        glDisable(GL_LIGHTING);
        glColor3f(0.72f, 0.72f, 0.72f);
        glRotatef(deg(anguloPeque), 0, 0, 1);
        glCallList(engranajePeque);
        glEnable(GL_LIGHTING);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-1.15f, 0.0f, 1.2f);
        glRotatef(180.0f, 0, 1, 0);

        glPushMatrix();
            glRotatef(deg(anguloGrande), 0, 0, 1);
            glCallList(engranajeGrande);
        glPopMatrix();

        glPushMatrix();
            glDisable(GL_LIGHTING);
            glColor3f(0.1f, 0.1f, 0.1f);
            glCallList(PalosGrande);
            glEnable(GL_LIGHTING);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 1.05f);
            glScalef(0.3f, 0.3f, 0.3f);
            glRotatef(deg(anguloPeque), 0, 0, 1);
            glDisable(GL_LIGHTING);
            glColor3f(0.72f, 0.72f, 0.72f);
            glCallList(engranajePeque);
            glEnable(GL_LIGHTING);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 1.05f);
            glScalef(0.3f, 0.3f, 0.3f);
            glDisable(GL_LIGHTING);
            glColor3f(0.1f, 0.1f, 0.1f);
            glCallList(PalosPeque); // donde se ubicara el reloj
            glEnable(GL_LIGHTING);
        glPopMatrix();
    
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 1.5f);
            glScalef(2.5f, 2.5f, 2.5f);
            glDisable(GL_LIGHTING);
            glColor3f(0.92f, 0.92f, 0.88f);
            glCallList(reloj);
            glEnable(GL_LIGHTING);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 1.52f);
            glRotatef(-deg(anguloPeque * 0.2f), 0, 0, 1);
            glScalef(2.5f, 2.5f, 2.5f);
            glDisable(GL_LIGHTING);
            glColor3f(0.05f, 0.05f, 0.05f);
            glCallList(ManCorta);
            glEnable(GL_LIGHTING);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 1.55f);
            glRotatef(-deg(anguloPeque), 0, 0, 1);
            glScalef(2.5f, 2.5f, 2.5f);
            glDisable(GL_LIGHTING);
            glColor3f(0.9f, 0.1f, 0.1f);
            glCallList(ManLarga);
            glEnable(GL_LIGHTING);
        glPopMatrix();
    glPopMatrix();
	
    glPopAttrib();
    glutSwapBuffers();
}

void reshape(GLint w, GLint h) 
{
	glViewport(0, 0, w, h);
	float razon = (float)w / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(30, razon, 1, 30);
}

void onTimer(int valor)
{
    static int antes = 0;
    static bool check = false;
    
    int ahora, tiempo_transcurrido;
    ahora = glutGet(GLUT_ELAPSED_TIME); 
    tiempo_transcurrido = ahora - antes; // Tiempo transcurrido desde el inicio
    
    if(tiempo_transcurrido > 2000)
    {
        check = true;
    }
    if(check)
    {
        // camara i el engranaje grande
        anguloGrande += velocidadGrande * (tiempo_transcurrido/1000.0);
        ojo[0] = radio * sin(anguloGrande);
        ojo[2] = radio * cos(anguloGrande);

        // engranaje pequeño
        anguloPeque += velocidadPeque * (tiempo_transcurrido/1000.0);
        antes = ahora;
    }
    glutTimerFunc(1000/tasaFPS , onTimer, tasaFPS); // Se encola un nuevo timer
    glutPostRedisplay();
}

int main(int argc, char** argv) // Programa principal
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(900, 600);
    glutInitWindowPosition(1900, 600);
	glutCreateWindow(PROYECTO);
    
    init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
    glutTimerFunc(1000/tasaFPS, onTimer, tasaFPS); 

	glutMainLoop();
}