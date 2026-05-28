/**
*	TexturasAlmacen.cpp
*
* 	Movimiento de navegacion de la camara por un almacen
*   de cajas TEXTURADAS usando las flechas del teclado.
*		-Flecha arriba: acelera en la direccion del movimiento
* 		-Flecha abajo: desacelera en la direccion del movimiento
* 		-Flecha izquierda: gira a la izquierda
*		-Flecha derecha: gira a la derecha
*	La velocidad se limita a 5 km/h.
*   La marcha atras cambia el sentido del giro.
*   Cada caja tiene 4 caras texturadas.
*   El suelo se textura con una textura de repeticion.
*
*
*	@author R.Vivo' <rvivo@upv.es
*
*/

#include <sstream>
#include <iomanip>
#include <random>
#include "../codebase.h"

static const auto PROYECTO = "Texturas";

using namespace std;
using namespace cb;

// Parametros del movimiento
static float velocidad = 0.0f;				    // Velocidad de movimiento (m/sg)
static Vec3 direccion(0.0f, 0.0f, -1.0f);	// Direccion de movimiento
static float anguloDireccion = 0.0f;			// Angulo de direccion
static const float aceleracion = 0.1f;			// Incremento de la velocidad 0.36 Km/h
static const float deltaGiro = 0.5f;		    // Grados

// Parametros de la camara
static Vec3 ojo(0.0f, 1.5f, 0.0f);			// Posicion del ojo
static Vec3 poi(0.0f, 1.5f, -1.0f);			// Punto de interes
static Vec3 vup(0.0f, 1.0f, 0.0f);			// Vector de vista
static float fovy = 30.0f;						// Angulo de apertura
static int altomarco = 500;						// Altura del marco de la ventana

// Almacen
static const float ancho = 100.0f;				// Lado del almacen
static GLuint caja;								// Caja del almacen	
static GLuint almacen;							// Almacen


static bool luces = false;

// Texturas
static GLuint texturaSuelo;					// Textura del suelo	
static GLuint texturaEntornoDelante;				// Textura del entorno




// Funcion de ajustes y carga inicial
void init()
{
	// Configuracion de las luces ----------------------------------

	// Luz ambiente

	//GLfloat luzAmbiente[] = { 0.02f, 0.02f, 0.02f, 1.0f };
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	

	

	// Carga de texturas -------------------------------------------


	glGenTextures(1, &texturaSuelo);

	glGenTextures(1, &texturaEntornoDelante);

	


	// Suelo
	glBindTexture(GL_TEXTURE_2D, texturaSuelo);
	loadImageFile((char*)"../Imagenes/asfalto.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); // vertical

	// Entorno
	glBindTexture(GL_TEXTURE_2D, texturaEntornoDelante);
	loadImageFile((char*)"../Imagenes/valla.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // horizontal

	

	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	// Configuracion del motor de render ---------------------------

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glColor3f(0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	glEnable(GL_NORMALIZE);

	//antialiasing
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glEnable(GL_MULTISAMPLE);

	// Carga de geometria ------------------------------------------

	// Almacen de cajas

	// Configurar la semilla aleatoria
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
}


// Funcion de atencion al dibujo del frame
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Inicializacion de matrices
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	

	// Posicionamiento de la camara
	gluLookAt(ojo.x, ojo.y, ojo.z, poi.x, poi.y, poi.z, vup.x, vup.y, vup.z);

	glPolygonMode(GL_FRONT, GL_FILL);

	


	// Suelo

	glMaterialfv(GL_FRONT, GL_DIFFUSE, BLANCO);
	glMaterialfv(GL_FRONT, GL_SPECULAR, NEGRO);
	glBindTexture(GL_TEXTURE_2D, texturaSuelo);
	quadtex(Vec3(-50, 0, 50), Vec3(50, 0, 50), Vec3(50, 0, -50), Vec3(-50, 0, -50), 0, 10, 0, 10, 100, 100);


	

	// Entorno

	glMaterialfv(GL_FRONT, GL_DIFFUSE, GRISOSCURO);
	glMaterialfv(GL_FRONT, GL_SPECULAR, NEGRO);
	glBindTexture(GL_TEXTURE_2D, texturaEntornoDelante);
	quadtex(Vec3(50, 0, 50), Vec3(-50, 0, 50), Vec3(-50, 5, 50), Vec3(50, 5, 50), 0, 10, 0, 1);
	

	

	// Muestra de datos

	stringstream ss;
	ss << "Velocidad: ";
	ss << fixed << std::setprecision(1) << setw(4) << velocidad * 3.6 << " Km/h";
	texto(10, altomarco - 20, (char*)ss.str().c_str(), BLANCO, GLUT_BITMAP_HELVETICA_18, false);

	glutSwapBuffers();
}

// Funcion de atencion al redimensionamiento
void reshape(GLint w, GLint h)
{
	float ra = (float)w / h;
	glViewport(0, 0, w, h);
	altomarco = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, ra, 0.1, 300);
}

void onIdle()
{
	static int lastTime = glutGet(GLUT_ELAPSED_TIME);
	int time = glutGet(GLUT_ELAPSED_TIME);
	int elapsed = time - lastTime;

	

	// Calculo del nuevo punto de interes
	poi.x = ojo.x + direccion.x;
	poi.z = ojo.z + direccion.z;

	lastTime = time;
	glutPostRedisplay();

}

// Funcion de atencion a las teclas especiales
void onSpecialKey(int key, int x, int y)
{
	switch (key)
	{
        case GLUT_KEY_LEFT:
            anguloDireccion += deltaGiro;
            break;
        case GLUT_KEY_RIGHT:
            anguloDireccion -= deltaGiro;
            break;
	}

	

	// Calculo de la direccion de movimiento
	direccion.x = sinf(anguloDireccion * PI / 180.0f);
	direccion.z = cosf(anguloDireccion * PI / 180.0f);

	glutPostRedisplay();
}

void onKey(unsigned char key, int x, int y)
{
	switch (key)
	{
        case 'l':
            luces = !luces;
            if (luces) // se hace de noche
            {
                glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
            }
            else 
            {
                glDisable(GL_LIGHTING);
                glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            }
            break;
        case 27:
            exit(0);
            break;
	}

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	FreeImage_Initialise();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(altomarco * 4 / 3, altomarco);
	glutInitWindowPosition(50, 200);
	glutCreateWindow(PROYECTO);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutIdleFunc(onIdle);
	glutSpecialFunc(onSpecialKey);
	glutKeyboardFunc(onKey);

	init();

	glutMainLoop();
	FreeImage_DeInitialise();
	return 0;
}