#define PROYECTO "Mecanismo animado"

#include "../codebase.h"

using namespace std;
using namespace cb;

vector<Vec3> puntosCircunferencia(float radio, float fase, unsigned int cantidad)
{
	vector<Vec3> puntos;
	if (cantidad < 1) return puntos;

	float angulo = 2 * PI / cantidad;
	for (unsigned int i = 0; i < cantidad; i++) {
		puntos.push_back(Vec3(radio * cosf(i * angulo + fase), radio * sinf(i * angulo + fase), 0));
	}

	return puntos;
}

// Listas para crear las geometrias necesarias
static GLuint diente, cara, doscaras;
static GLuint perfil1, perfil2, perfilT;
static GLuint unaRueda, rueda;
static GLuint engranaje, engranajeGrande, engranajePeque;
static GLuint unPalo, palos, palosGrande, palosPeque;
static GLuint pieza, unaC, dosC, unionC, uniCT, eje_interno, interno_completo, reloj;
static GLuint manCorta, manLarga;

vector<Vec3> ejesAux;

// Datos para la generar los engranajes
static const float HDIENTE = 0.1f;
static const float DPRIMITIVO = 1.7f;
static const float GROSOR = 0.3f; // ancho del engranaje, tambien lo uso para el ancho del reloj
static const int NDIENTES_CORONA = 40;
static const int NDIENTES_PINYON = 20;

bool check = false;
static const uint8_t delay = 2;

// Velocidades y angulos para la animación
static const float radio = 8.0f; 
static float ojo[] = {0, 0, radio}; 
static const int tasaFPS = 60;

static float angCam = 0.0f; 
static const float vCam = 2.0f*PI/60.0f; // vuelta por minuto

static float angCorona = 0.0f; 
static const float vCorona = 2.0f*PI/60.0f; // vuelta por minuto

static float angPinyon = PI/NDIENTES_PINYON; // offset para que engranen correctamente desde el principio 
static const float vPinyon = vCorona * (NDIENTES_CORONA)/(NDIENTES_PINYON); // velocidad proporcional a la de la corona

static float angManLarga = 0.0f;
static const float vManLarga = 2*PI/60.0f; // vuelta por minuto

static float angManCorta = 0.0f;
static const float vManCorta = 2*PI/3600.0f; // vuelta por minuto

void onTimer(int valor);

void crearEngranaje(int NDIENTES, float DEJE)
{
	vector<Vec3> picos = puntosCircunferencia(DPRIMITIVO / 2.0 + HDIENTE / 2.0, PI / 2.0, NDIENTES);
	vector<Vec3> valles = puntosCircunferencia(DPRIMITIVO / 2.0 - HDIENTE / 2.0, PI / 2.0 - PI / NDIENTES, NDIENTES);
	vector<Vec3> ejes = puntosCircunferencia(DEJE / 2.0, PI / 2.0, NDIENTES);

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
        glVertex3fv(valles[0] + Vec3(0, 0, GROSOR));
        glVertex3fv(picos[0] + Vec3(0,0, GROSOR));
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
        glVertex3fv(ejes[0] + Vec3(0, 0, GROSOR));
        glVertex3fv(ejes[1] + Vec3(0, 0, GROSOR));
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

void crearPalo(float Largo, int NDIENTES, vector<Vec3> &ejes)
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

    palos = glGenLists(1);
    glNewList(palos, GL_COMPILE);
        for (int i = 0; i < NDIENTES; i++)
        {
            glCallList(unPalo);
            glRotatef(360.0f/NDIENTES, 0.0, 0.0f, 1.0f);
        }
    glEndList();
}

void crearReloj(int NDIENTES, float DEJE)
{
	vector<Vec3> valles = puntosCircunferencia(DPRIMITIVO / 2.0 - HDIENTE / 2.0, PI / 2.0 - PI / NDIENTES, NDIENTES);
	vector<Vec3> ejes = puntosCircunferencia(DEJE / 2.0, PI / 2.0, NDIENTES);

    pieza = glGenLists(1);
    glNewList(pieza, GL_COMPILE);
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
            glCallList(pieza);
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
        glVertex3fv(valles[0] + Vec3(0, 0, GROSOR));
        glVertex3fv(valles[1] + Vec3(0,0, GROSOR));
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
        glVertex3fv(ejes[0] + Vec3(0, 0, GROSOR));
        glVertex3fv(ejes[1] + Vec3(0, 0, GROSOR));
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

void crearManecilla()
{
    float radioReloj = DPRIMITIVO / 2.0f - HDIENTE / 2.0f; 
    float base = radioReloj * 0.045f; // tamaño de la base
    float paloCorto = radioReloj * 0.45f; // altura de la manecilla de las horas
    float paloLargo = radioReloj * 0.9f; // altura de la manecilla de los minutos

    manCorta = glGenLists(1);
    glNewList(manCorta, GL_COMPILE);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLES);
        glVertex3f(-base, 0.0f, GROSOR);
        glVertex3f(base, 0.0f, GROSOR);
        glVertex3f(0.0f, paloCorto, GROSOR);
    glEnd();
    glEndList();

    manLarga = glGenLists(1);
    glNewList(manLarga, GL_COMPILE);
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

    crearEngranaje(NDIENTES_CORONA, 0.2f);
    engranajeGrande = engranaje;
    
    crearPalo(1.1f, NDIENTES_CORONA, ejesAux);
    palosGrande = palos;

    crearEngranaje(NDIENTES_PINYON, 0.65f);
    engranajePeque = engranaje;

    crearPalo(4.3f, NDIENTES_PINYON, ejesAux);
    palosPeque = palos;

    crearReloj(NDIENTES_CORONA, 0.08f);
    crearManecilla();
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
        glRotatef(deg(angCorona), 0, 0, 1);
        glCallList(engranajeGrande);
    glPopMatrix();

    glDisable(GL_LIGHTING);
    glPushMatrix();
        glColor3f(0.1f, 0.1f, 0.1f);
        glCallList(palosGrande);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.0f, 0.0f, 1.05f);
        glScalef(0.3f, 0.3f, 0.3f);
        glColor3f(0.72f, 0.72f, 0.72f);
        glRotatef(deg(angPinyon), 0, 0, 1);
        glCallList(engranajePeque);
    glPopMatrix();
    glEnable(GL_LIGHTING);

    glPushMatrix();
        glTranslatef(-1.15f, 0.0f, 1.2f);
        glRotatef(180.0f, 0, 1, 0);

        glPushMatrix();
            glRotatef(deg(angCorona), 0, 0, 1);
            glCallList(engranajeGrande);
        glPopMatrix();

        glDisable(GL_LIGHTING);
        glPushMatrix();
            glColor3f(0.1f, 0.1f, 0.1f);
            glCallList(palosGrande);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 1.05f);
            glScalef(0.3f, 0.3f, 0.3f);
            glRotatef(deg(angPinyon), 0, 0, 1);
            glColor3f(0.72f, 0.72f, 0.72f);
            glCallList(engranajePeque);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 1.05f);
            glScalef(0.3f, 0.3f, 0.3f);
            glColor3f(0.1f, 0.1f, 0.1f);
            glCallList(palosPeque); // donde se ubicara el reloj
        glPopMatrix();
    
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 1.5f);
            glScalef(2.5f, 2.5f, 2.5f);
            glColor3f(0.92f, 0.92f, 0.88f);
            glCallList(reloj);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 1.52f);
            glRotatef(-deg(angManCorta), 0, 0, 1);
            glScalef(2.5f, 2.5f, 2.5f);
            glColor3f(0.05f, 0.05f, 0.05f);
            glCallList(manCorta);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 1.55f);
            glRotatef(-deg(angManLarga), 0, 0, 1);
            glScalef(2.5f, 2.5f, 2.5f);
            glColor3f(0.9f, 0.1f, 0.1f);
            glCallList(manLarga);
        glPopMatrix();
        glEnable(GL_LIGHTING);
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

    gluPerspective(30, razon, 1, 35);
}

void onIdle()
{
    static int antes = 0;

    int ahora, tiempo_transcurrido;
    ahora = glutGet(GLUT_ELAPSED_TIME); 
    tiempo_transcurrido = ahora - antes;
    
    if(check)
    {
        angCam += vCam * (tiempo_transcurrido/1000.0);
        ojo[0] = radio * sin(angCam);
        ojo[2] = radio * cos(angCam);

        angCorona += vCorona * (tiempo_transcurrido/1000.0);
        
        angPinyon += vPinyon * (tiempo_transcurrido/1000.0);
        
        angManLarga += vManLarga * (tiempo_transcurrido/1000.0);
        
        angManCorta += vManCorta * (tiempo_transcurrido/1000.0);

        antes = ahora;
    }
    glutTimerFunc(1000/tasaFPS, onTimer, tasaFPS); 
    glutPostRedisplay();
}

void onTimer(int valor)
{
    check = true;
    onIdle();
}

int main(int argc, char** argv) // Programa principal
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(900, 600);
	glutCreateWindow(PROYECTO);
    
    init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
    glutIdleFunc(onIdle);
    glutTimerFunc(delay, onTimer, 0); 

	glutMainLoop();
}