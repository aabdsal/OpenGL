#include <iostream>	
#include <vector>
#include "../codebase.h"
#include <cmath> 

#define PROYECTO "Práctica 2 en OpenGL"

using namespace std;
using namespace cb; // Usar el espacio de nombres de codebase

// Variables globales del problema
static const unsigned int N_DIENTES = 40; // Número de dientes
static const float H_DIENTE = 0.4f; // Altura de los dientes
static const float D_PRIMITIVO = 1.4f; // Diametro primitivo
static const float D_EJE = 0.2f; // Diametro del eje;

static vector<Vec3> picos, valle, eje; // Puntos de los dientes, valles y ejes
static GLuint rueda;

vector<Vec3> circlePoints(float radio, float anguloInicial, unsigned int numPuntos) {
	
	vector<Vec3> puntos;
	if (numPuntos < 1) 
	{
		return puntos;
	}

	float paso = 2 * PI / numPuntos; // El paso entre cada punto
	float angulo = anguloInicial;            // El ángulo acumulado, empezando en la fase

	for (unsigned int i = 0; i < numPuntos; i++) 
	{
		puntos.push_back(Vec3(radio * cosf(angulo), radio * sinf(angulo), 0));
		angulo += paso; // Incrementamos el ángulo para el siguiente punto
	}
	return puntos;
}

void init()
{

    eje = circlePoints((D_EJE / 2.0f), (PI / 2.0f), N_DIENTES);
    valle = circlePoints((D_PRIMITIVO / 2.0f - H_DIENTE / 2.0f), (PI / 2.0f) - (PI / N_DIENTES), N_DIENTES);
    picos = circlePoints((D_PRIMITIVO / 2.0f + H_DIENTE / 2.0f), (PI / 2.0f), N_DIENTES);

    rueda = glGenLists(1);
    glNewList(rueda, GL_COMPILE);

    glColor3f(0.0f, 0.0f, 0.0f);

    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i < N_DIENTES; i++) 
    {
        glVertex3fv(valle[i]);
        glVertex3fv(picos[i]);
        glVertex3fv(eje[i]);

        glVertex3fv(valle[(i + 1) % N_DIENTES]);
        glVertex3fv(eje[(i + 1) % N_DIENTES]);
    }
    glEnd();
    glEndList();

	// Inicializaciones 
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void display()
{

    glClear(GL_COLOR_BUFFER_BIT); // Borra la pantalla

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glCallList(rueda);
    glFlush();

}
void reshape(GLint w, GLint h)
{

}
int main(int argc, char** argv)
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(50, 50);

	// Crear ventana
	glutCreateWindow(PROYECTO);

    init();

	// Registrar callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	// Bucle de atencion a eventos
    glutMainLoop();
}