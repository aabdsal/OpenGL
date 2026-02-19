#include <iostream>	
#include <vector>

// Incluir la librería codebase (ajusta la ruta relativa si es necesario)
#include "../codebase.h" 

#define PROYECTO "Práctica 2 en OpenGL"

using namespace std;
using namespace cb; // Usar el espacio de nombres de codebase

// Variables globales del problema
static const unsigned int N_DIENTES = 30; // Número de dientes
static const float H_DIENTE = 0.05f; // Altura de los dientes
static const float D_PRIMITIVO = 1.0f; // Diametro primitivo
static const float D_EJE = 0.4f; // Diametro del eje;

static vector<Vec3> picos, valle, eje; // Puntos de los dientes, valles y ejes
static GLuint rueda;

vector<Vec3> circlePoints(float radio, float fase, unsigned int cantidad) {
	
	vector<Vec3> puntos;
	if (cantidad < 1) 
	{
		return puntos;
	}

	float paso = 2 * PI / cantidad; // El paso entre cada punto
	float angulo = fase;            // El ángulo acumulado, empezando en la fase

	for (unsigned int i = 0; i < cantidad; i++) 
	{
		puntos.push_back(Vec3(radio * cosf(angulo), radio * sinf(angulo), 0));
		angulo += paso; // Incrementamos el ángulo para el siguiente punto
	}
	return puntos;
}

void init()
{

    eje = circlePoints((D_EJE / 2), (PI / 2), N_DIENTES);
    picos = circlePoints((D_PRIMITIVO / 2 + H_DIENTE), (PI / 2), N_DIENTES);
    valle = circlePoints((D_PRIMITIVO / 2 - H_DIENTE / 2), (PI / 2 - PI / N_DIENTES), N_DIENTES);
    
    rueda = glGenLists(1);

    glNewList(rueda, GL_COMPILE);
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i < N_DIENTES; i++) {
        glVertex3fv(valle[i]);
        glVertex3fv(picos[i]);
        glVertex3fv(eje[i]);
        glVertex3fv(valle[(i + 1) % N_DIENTES]);
        glVertex3fv(eje[(i + 1) % N_DIENTES]);
    }
    glEnd();
    glEndList();

	// Inicializaciones 
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void display(){
    init();
    //glClear(GL_COLOR_BUFFER_BIT); // Borra la pantalla
    /* Dibujo como POLYGON
    glBegin(GL_POLYGON);
    glVertex3f(0.5*cos(0*2*3.1415926/5), 0.5*sin(0*2*3.1415926/5), 0.0);
    glVertex3f(0.5*cos(1*2*3.1415926/5), 0.5*sin(1*2*3.1415926/5), 0.0);
    glVertex3f(0.5*cos(2*2*3.1415926/5), 0.5*sin(2*2*3.1415926/5), 0.0);
    glVertex3f(0.5*cos(3*2*3.1415926/5), 0.5*sin(3*2*3.1415926/5), 0.0);
    glVertex3f(0.5*cos(4*2*3.1415926/5), 0.5*sin(4*2*3.1415926/5), 0.0);
    glEnd();
     Dibujo como TRIANGLE_STRIP*/
    /*
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f(0.5*cos(0*2*3.1415926/5), 0.5*sin(0*2*3.1415926/5), 0.0);
    glVertex3f(0.5*cos(1*2*3.1415926/5), 0.5*sin(1*2*3.1415926/5), 0.0);
    glVertex3f(0.5*cos(4*2*3.1415926/5), 0.5*sin(4*2*3.1415926/5), 0.0);
    glVertex3f(0.5*cos(2*2*3.1415926/5), 0.5*sin(2*2*3.1415926/5), 0.0);
    glVertex3f(0.5*cos(3*2*3.1415926/5), 0.5*sin(3*2*3.1415926/5), 0.0);
    glEnd();
    */
    // Dibujo como TRIANGLE_FAN
    /*
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.5*cos(0*2*3.1415926/5), 0.5*sin(0*2*3.1415926/5), 0.0);
    glVertex3f(0.5*cos(1*2*3.1415926/5), 0.5*sin(1*2*3.1415926/5), 0.0);
    glVertex3f(0.5*cos(2*2*3.1415926/5), 0.5*sin(2*2*3.1415926/5), 0.0);
    glVertex3f(0.5*cos(3*2*3.1415926/5), 0.5*sin(3*2*3.1415926/5), 0.0);
    glVertex3f(0.5*cos(4*2*3.1415926/5), 0.5*sin(4*2*3.1415926/5), 0.0);
    glEnd();
    */
    glClear(GL_COLOR_BUFFER_BIT); // Borra la pantalla
    glPolygonMode(GL_FRONT, GL_LINE);
    glPolygonMode(GL_BACK, GL_FILL);
    glCallList(rueda);
    glFlush();
}
void reshape(GLint w, GLint h){

}
int main(int argc, char** argv){
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(50, 50);

	// Crear ventana
	glutCreateWindow(PROYECTO);
	// Registrar callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	// Bucle de atencion a eventos
    glutMainLoop();
}