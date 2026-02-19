/*!
	Geometria.cpp

	Programa para dibujar un pentagono con listas
	Falta 
	@author		Roberto Vivo' <rvivo@upv.es>
	@date		Feb,2024
 */

#define PROYECTO "IGR::S#02::Listas de dibujo"

#include <iostream>			
#include <GLUT/glut.h>
#include <vector>

using namespace std;

// Definición local de PI
#ifndef PI
#define PI 3.14159265358979323846
#endif

// Estructura Vec3 local para eliminar dependencias externas
struct Vec3 {
	float x, y, z;
	Vec3(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}
};

static GLuint pentagono;
static float radio = 1;
static float offset = PI / 2;
static int numPuntos = 5;

// Funcion para muestrear una circunferencia. Devuelve un vector de puntos en el plano XY
// Parametros
// radio: radio de la circunferencia
// anguloInicial: desplazamiento angular inicial en radianes
// numPuntos: numero de puntos a muestrear
vector<Vec3> puntosCircunferencia(float radio, float anguloInicial, int numPuntos) {

	vector<Vec3> puntos;
	float angulo = anguloInicial;
	float incremento = 2 * PI / numPuntos;
	for (int i = 0; i < numPuntos; i++) {
		puntos.push_back(Vec3(radio * cosf(angulo), radio * sinf(angulo), 0));
		angulo += incremento;
	}
	return puntos;
}

void init()
// Inicializaciones
{
	cout << "GL version " << glGetString(GL_VERSION) << endl;

	// Calculo de coordenadas
	vector<Vec3> puntos = puntosCircunferencia(radio, offset, numPuntos);

	// Lista de dibujo
	pentagono = glGenLists(1);

	glNewList(pentagono, GL_COMPILE);
	glBegin(GL_POLYGON);
	for (int i = 0; i < numPuntos; i++) {
		glVertex3f(puntos[i].x, puntos[i].y, 0);
	}
	glEnd();
	glEndList();

	// Inicializaciones 
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
}

void display()
// Funcion de atencion al dibujo
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Uso de Display Lists

	// Dibujar el pentagono relleno
	glPolygonMode(GL_FRONT, GL_FILL);
	glColor3f(0.5f, 0, 0);
	glCallList(pentagono);

	// Dibujar el pentagono en modo alambrico
	glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);
	glLineWidth(4);
	glPolygonMode(GL_FRONT, GL_LINE);
	glColor3f(1, 1, 1);
	glCallList(pentagono);
	glPopAttrib();

	// Dibujar el pentagono en modo puntos sin usar listas
	vector<Vec3> coordenadas = puntosCircunferencia(radio, offset, numPuntos);
	glPushAttrib(GL_CURRENT_BIT | GL_POINT_BIT);
	glPointSize(10);
	glColor3f(1, 1, 0);
	glBegin(GL_POINTS);
	for (int i = 0; i < numPuntos; i++) {
		glVertex3f(coordenadas[i].x, coordenadas[i].y, 0);
	}
	glEnd();
	glPopAttrib();

	// Dibujar un pentagono menor con triangle fan y desfase diferente
	vector<Vec3> coordenadasMenor = puntosCircunferencia(radio / 2, offset / 2, numPuntos);
	glPolygonMode(GL_FRONT, GL_FILL);
	glColor3f(0, 1, 0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, 0);
	for (int i = 0; i <= numPuntos; i++) {
		glVertex3f(coordenadasMenor[i % numPuntos].x, coordenadasMenor[i % numPuntos].y, 0);
	}
	glEnd();

	//ejes();


	glFlush();
}

void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
}

int main(int argc, char** argv)
// Programa principal
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow(PROYECTO);
	init();

	// Registro de callbacks	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	// Bucle de atencion a eventos
	glutMainLoop();
}