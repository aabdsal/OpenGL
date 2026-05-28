#define PROYECTO "Caja delante de la camara"

#include "../codebase.h"
#include <sstream>
#include <iomanip>
#include <random>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace cb;

struct Cajacord 
{
    float x;
    float z;
    bool rotada; // true en 90 grados, false en 0 grados
};

vector <Cajacord> cajas;
GLuint caja;

float camX = 0.0f;
float camZ = 0.0f;
float camY = 1.5f;   // altura fija

float angulo = 45.0f;    // grados, dirección inicial
float velocidad = 0.0f; // velocidad actual

const float VMAX = 10.0f;
const float acc = 1.0f;     // aceleración
const float giro = 1.0f;     // grados por pulsación
const float rozamiento = 0.98f;

int lastTime = 0;

const int FPS = 60;
const int FRAME_TIME = 1000 / FPS;

// Texturas
static GLuint texturaSuelo;					// Textura del suelo	
static GLuint texturaEntornoDelante;				// Textura del entorno
static GLuint texturaCaja;				// Textura del entorno
static GLuint texturaCielo;				// Textura del entorno

bool modoNoche = true;
/*

    TODO : Falta poner las caras a los contenedores, el cielo y los limites, y poner contenedores en los limites de la zona

*/
void aplicarModo()
{
    if (modoNoche) 
    {
        glClearColor(0.0f, 0.0f, 0.05f, 1.0f);

        GLfloat amb[] = { 0.05f, 0.05f, 0.05f, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

        // Faro fuerte
        GLfloat Dl0[] = { 1.5f, 1.5f, 1.5f, 1.0f };
        glLightfv(GL_LIGHT0, GL_DIFFUSE, Dl0);

        // Luces fijas activas
        for (int i = 1; i <= 6; i++)
            glEnable(GL_LIGHT0 + i);
    }
    else 
    {
        glClearColor(0.7f, 0.85f, 1.0f, 1.0f);

        GLfloat amb[] = { 0.4f, 0.4f, 0.4f, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

        // Faro débil
        GLfloat Dl0[] = { 0.4f, 0.4f, 0.4f, 1.0f };
        glLightfv(GL_LIGHT0, GL_DIFFUSE, Dl0);

        // Luces fijas apagadas
        for (int i = 1; i <= 6; i++)
        {
            glDisable(GL_LIGHT0 + i);
        }
    }
}

GLuint crearContenedor()
{
    GLuint id = glGenLists(1);

    float hx = 1.5f; //ancho
    float hy = 1.5f; //alto
    float hz = 3.0f; //fondo

    glNewList(id, GL_COMPILE);
    glBegin(GL_QUADS);

    // Cara frontal (+Z)
    glNormal3f(0, 0, 1);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-hx, -hy, hz);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(hx, -hy, hz);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(hx, hy, hz);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-hx, hy, hz);

    // Cara trasera (-Z)
    glNormal3f(0, 0, -1);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-hx, -hy, -hz);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-hx, hy, -hz);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(hx, hy, -hz);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(hx, -hy, -hz);

    // Cara izquierda (-X)
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-hx, -hy, -hz);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-hx, -hy, hz);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-hx, hy, hz);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-hx, hy, -hz);

    // Cara derecha (+X)
    glNormal3f(1, 0, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(hx, -hy, -hz);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(hx, hy, -hz);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(hx, hy, hz);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(hx, -hy, hz);

    // Cara superior (+Y)
    glNormal3f(0, 1, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-hx, hy, -hz);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-hx, hy, hz);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(hx, hy, hz);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(hx, hy, -hz);

    // Cara inferior (-Y)
    glNormal3f(0, -1, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-hx, -hy, -hz);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(hx, -hy, -hz);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(hx, -hy, hz);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-hx, -hy, hz);

    glEnd();
    glEndList();

    return id;
}

float clamp(float v, float min, float max)
{
    if (v < min) return min;
    if (v > max) return max;
    return v;
}

void generarCajas(int numCajas, float tamEscenario)
{
    cajas.clear();

    for (int i = 0; i < numCajas; i++) 
    {
        Cajacord c;

        c.rotada = (i % 2 == 0);

        // Dimensiones según rotación
        float hx = c.rotada ? 3.0f : 1.5f;
        float hz = c.rotada ? 1.5f : 3.0f;

        c.x = static_cast<float>(rand()) / RAND_MAX * tamEscenario;
        c.z = static_cast<float>(rand()) / RAND_MAX * tamEscenario;

        // Clamp teniendo en cuenta el tamaño REAL
        c.x = clamp(c.x, hx, tamEscenario - hx);
        c.z = clamp(c.z, hz, tamEscenario - hz);

        cajas.push_back(c);
    }
}


void onSpecialKey(int key, int, int)
{
    switch (key) 
    {
        case GLUT_KEY_UP:
            velocidad += acc;
            if (velocidad > VMAX) velocidad = VMAX;
            break;

        case GLUT_KEY_DOWN:
            velocidad -= acc;
            if (velocidad < -VMAX) velocidad = -VMAX;
            break;

        case GLUT_KEY_LEFT:
            // Si vamos hacia atrás, el giro se invierte
            if (velocidad >= 0)
                angulo -= giro;
            else
                angulo += giro;
            break;

        case GLUT_KEY_RIGHT:
            if (velocidad >= 0)
                angulo += giro;
            else
                angulo -= giro;
            break;
    }
}

void actualizarCamara(float dt)
{
    float rad = angulo * 3.141592f / 180.0f;

    camX += velocidad * dt * cos(rad);
    camZ += velocidad * dt * sin(rad);

    velocidad *= pow(rozamiento, dt * 60.0f);
}

// Inicialización
void init()
{
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glColor3f(0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);     // LIGHT0 como faro

    glEnable(GL_NORMALIZE); // para las cajas
    glShadeModel(GL_SMOOTH);

    glDisable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    GLfloat Al0[] = {0.05, 0.05, 0.05, 1.0};
    GLfloat Dl0[] = {0.5, 0.7, 1.0, 1.0};
    GLfloat Sl0[] = {1.0, 1.0, 1.0, 1.0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, Al0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Dl0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, Sl0);

    // Parámetros del foco
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 50.0f);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 60.0f);

    GLfloat Al[] = {0.05f, 0.05f, 0.05f, 1.0f};
    GLfloat Dl[] = {0.9f, 0.9f, 0.8f, 1.0f};
    GLfloat Sl[] = {0.5f, 0.5f, 0.5f, 1.0f};

    for (int i = 1; i <= 6; i++) 
    {
        glLightfv(GL_LIGHT0 + i, GL_AMBIENT, Al);
        glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, Dl);
        glLightfv(GL_LIGHT0 + i, GL_SPECULAR, Sl);

        glLightf(GL_LIGHT0 + i, GL_SPOT_CUTOFF, 35.0f);
        glLightf(GL_LIGHT0 + i, GL_SPOT_EXPONENT, 80.0f);

        glEnable(GL_LIGHT0 + i);
    }

    GLfloat especular[] = { 0.4f, 0.4f, 0.4f, 1 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, especular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 30.0f);

    caja = crearContenedor();

    generarCajas(30, 100.0f);  // numero de cajas en un suelo de 100x100

    aplicarModo();

    glGenTextures(1, &texturaSuelo);
	glGenTextures(1, &texturaEntornoDelante);

	glBindTexture(GL_TEXTURE_2D, texturaSuelo);
	loadImageFile((char*)"../Imagenes/desierto.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); // vertical

	glBindTexture(GL_TEXTURE_2D, texturaCielo);
	loadImageFile((char*)"../Imagenes/cielo2.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); // vertical

	// Entorno
	glBindTexture(GL_TEXTURE_2D, texturaEntornoDelante);
	loadImageFile((char*)"../Imagenes/sanmiguelfrontal.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // horizontal

	glBindTexture(GL_TEXTURE_2D, texturaCaja);
	loadImageFile((char*)"../Imagenes/cama_del.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // horizontal

	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glEnable(GL_MULTISAMPLE);

    srand(static_cast<unsigned int>(time(nullptr)));
}

// Render 
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // ---- LUZ FARO SOLIDARIA A LA CAMARA ----
    GLfloat pos[] = { 0.0f, 0.3f, 0.0f, 1.0f };   // en la cámara
    GLfloat dir[] = { 0.0f, -0.2f, -1.0f };      // hacia delante (−Z)

    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);


    gluLookAt(
        camX, camY, camZ, // posición cámara
        camX + cos(angulo * 3.141592f / 180.0f), // punto de interés
        camY, camZ + sin(angulo * 3.141592f / 180.0f),
        0, 1, 0
    );

    // ---- LUCES FIJAS DISTRIBUIDAS EN EL TERRENO ----
    GLfloat dir_down[] = {0.0f, -1.0f, 0.0f}; // apuntan hacia abajo

    GLfloat posiciones[6][4] = 
    {
        {20.0f, 8.0f, 20.0f, 1.0f},
        {50.0f, 8.0f, 20.0f, 1.0f},
        {80.0f, 8.0f, 20.0f, 1.0f},

        {20.0f, 8.0f, 60.0f, 1.0f},
        {50.0f, 8.0f, 60.0f, 1.0f},
        {80.0f, 8.0f, 60.0f, 1.0f}
    };

    for (int i = 0; i < 6; i++) 
    {

        // Posicionar la luz real
        glLightfv(GL_LIGHT1 + i, GL_POSITION, posiciones[i]);
        glLightfv(GL_LIGHT1 + i, GL_SPOT_DIRECTION, dir_down);

        // ---- MATERIAL EMISIVO (bombilla) ----
        GLfloat emision[] = { 0.8f, 0.8f, 0.4f, 1.0f };
        GLfloat negro[] = { 0.0f, 0.0f, 0.0f, 1.0f };

        if (modoNoche) 
        {
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emision);
        }
        else 
        {
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, negro);
        }

        // Dibujar la "bombilla"
        glPushMatrix();
        glTranslatef(posiciones[i][0], posiciones[i][1], posiciones[i][2]);
        glutSolidSphere(0.3, 12, 12);
        glPopMatrix();

        // ---- QUITAR EMISIÓN ----
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, negro);
    }

    // ---- MATERIAL DE LAS CAJAS (SEMIBRILLANTE) ----
    GLfloat caja_amb[] = { 0.15f, 0.05f, 0.05f, 1.0f };
    GLfloat caja_dif[] = { 0.8f,  0.2f,  0.2f,  1.0f };
    GLfloat caja_spec[] = { 0.8f,  0.8f,  0.8f,  1.0f };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, caja_amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, caja_dif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, caja_spec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0f);
    glBindTexture(GL_TEXTURE_2D, texturaCaja);
    
    for (const auto& c : cajas) 
    {
        glPushMatrix();
        glTranslatef(c.x, 1.5f, c.z);

        if (c.rotada)
        {
            glRotatef(90.0f, 0, 1, 0);
        }

        glCallList(caja);
        glPopMatrix();
    }

    // ---- MATERIAL DEL SUELO (MATE) ----
    GLfloat suelo_amb[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat suelo_dif[] = { 0.2f, 0.25f, 0.3f, 1.0f };
    GLfloat suelo_spec[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glPolygonMode(GL_FRONT, GL_FILL);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, suelo_amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, suelo_dif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, suelo_spec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
    glBindTexture(GL_TEXTURE_2D, texturaSuelo);
    quadtex(Vec3(-50, 0, 50), Vec3(50, 0, 50), Vec3(50, 0, -50), Vec3(-50, 0, -50), 0, 10, 0, 10, 100, 100);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, suelo_amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, suelo_dif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, suelo_spec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
    glBindTexture(GL_TEXTURE_2D, texturaCielo);
    quadtex(Vec3(-50, 10, 50), Vec3(50, 10, 50), Vec3(50, 10, -50), Vec3(-50, 10, -50), 0, 10, 0, 10, 100, 100);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, GRISOSCURO);
	glMaterialfv(GL_FRONT, GL_SPECULAR, NEGRO);
	glBindTexture(GL_TEXTURE_2D, texturaEntornoDelante);
	quadtex(Vec3(50, 0, 50), Vec3(-50, 0, 50), Vec3(-50, 5, 50), Vec3(50, 5, 50), 0, 10, 0, 1);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, GRISOSCURO);
	glMaterialfv(GL_FRONT, GL_SPECULAR, NEGRO);
	glBindTexture(GL_TEXTURE_2D, texturaEntornoDelante);
	quadtex(Vec3(0, 0, 50), Vec3(-0, 0, 50), Vec3(-0, 5, 50), Vec3(0, 5, 50), 0, 10, 0, 1);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, GRISOSCURO);
	glMaterialfv(GL_FRONT, GL_SPECULAR, NEGRO);
	glBindTexture(GL_TEXTURE_2D, texturaEntornoDelante);
	quadtex(Vec3(50, 0, 50), Vec3(-50, 0, 50), Vec3(-50, 5, 50), Vec3(50, 5, 50), 0, 10, 0, 1);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, GRISOSCURO);
	glMaterialfv(GL_FRONT, GL_SPECULAR, NEGRO);
	glBindTexture(GL_TEXTURE_2D, texturaEntornoDelante);
	quadtex(Vec3(50, 0, 50), Vec3(-50, 0, 50), Vec3(-50, 5, 50), Vec3(50, 5, 50), 0, 10, 0, 1);
    
    glutSwapBuffers();
}

void timer(int)
{
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    float delta = (currentTime - lastTime) / 1000.0f; // en segundos
    lastTime = currentTime;

    actualizarCamara(delta);

    glutPostRedisplay();
    glutTimerFunc(FRAME_TIME, timer, 0);
}

// Ajuste de ventana
void reshape(int w, int h)
{
    float aspecto = float(w) / float(h == 0 ? 1 : h);
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, aspecto, 0.1, 100);
}

void onKey(unsigned char key, int, int)
{
    if (key == 'l' || key == 'L') 
    {
        modoNoche = !modoNoche;
        aplicarModo();
        glutPostRedisplay();
    }

    if (key == 27) exit(0);
}

int main(int argc, char** argv)
{
    FreeImage_Initialise();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1440, 1080);
    glutCreateWindow(PROYECTO);

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(onKey);
    glutSpecialFunc(onSpecialKey);
    lastTime = glutGet(GLUT_ELAPSED_TIME);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    FreeImage_DeInitialise();
    return 0;
}