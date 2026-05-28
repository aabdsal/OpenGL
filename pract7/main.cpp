#define PROYECTO "Interaccion"

#include <iostream>			
#include "../codebase.h"

using namespace std;
using namespace cb;
GLuint cara;
GLuint suelo;

void contenedor()
{
    float x = 1.0f;
    float y = 1.0f;
    float z = 3.0f;

    cara = glGenLists(1);
    glNewList(cara, GL_COMPILE);
    glBegin(GL_QUADS);

        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-x, -y, z);
        glVertex3f(x, -y, z);
        glVertex3f(x, y, z);
        glVertex3f(-x, y, z);

        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-x, -y, 0);
        glVertex3f(x, -y, 0);
        glVertex3f(x, y, 0);
        glVertex3f(-x, y, 0);
    
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(x, -y, 0);
        glVertex3f(x, -y, z);
        glVertex3f(x, y, z);
        glVertex3f(x, y, 0);

        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-x, -y, 0);
        glVertex3f(-x, -y, z);
        glVertex3f(-x, y, z);
        glVertex3f(-x, y, 0);

        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-x, y, 0);
        glVertex3f(x, y, 0);
        glVertex3f(x, y, z);
        glVertex3f(-x, y, z);

        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-x, -y, 0);
        glVertex3f(x, -y, 0);
        glVertex3f(x, -y, z);
        glVertex3f(-x, -y, z);
        
    glEnd();
    glEndList();

}
void crearSuelo(int nz, int nx, float dx, float dz)
{
    suelo = glGenLists(1);
    glNewList(suelo, GL_COMPILE);
    glBegin(GL_QUADS);
    for(int i = 0; i < nx; i++)
    {
        float x0 = i * dx;
        float x1 = (i + 1) * dx;
        for (int j = 0; j < nz; j++)
        {
            float z = j * dz;
            glVertex3f(x0, 0, z);
            glVertex3f(x1, 0, z);
        }
    }
    glEnd();
    glEndList();

}
// Inicializaciones (solo una vez)
void init()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    contenedor();
    crearSuelo(1, 1, 1, 1);
}
// Callback de atencion al redibujado
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Seleccionar la MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Situar y orientar la camara
	gluLookAt(0, 1.5f, 0.0f, 0, 1.3f, 1, 0, 1, 0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glPushMatrix();
        glColor3f(1.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, 6.0f);
        glCallList(cara);
    glPopMatrix();
    

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    glPushMatrix();
        glCallList(suelo);
    glPopMatrix();

	glutSwapBuffers();
}

// Callback de atencion al redimensionado
void reshape(GLint w, GLint h)
{
	float relacionAspecto = float(w) / h;
	glViewport(0, 0, w, h);

	// Definir la camara
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(30, relacionAspecto, 0.1, 35);
}

// Callback de atencion a los eventos de teclas alfanumericas
void onKey(unsigned char tecla, int x, int y)
{
	switch (tecla) {
	case 27:
		exit(0);
        break;
	}

	glutPostRedisplay();
}

// Programa principal
int main(int argc, char** argv)
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
	glutKeyboardFunc(onKey);

	// Bucle de atencion a eventos
	glutMainLoop();
}