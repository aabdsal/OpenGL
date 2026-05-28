/*
    Ejercicio S6E01: Dibujar un objeto en modelo de alambres o modelo sólido según 
    se pulsen las teclas “a” o “s”. Indicar en todo momento en el título de la 
    ventana la posición del cursor en coordenadas del viewport. 
    Al pulsar la tecla “esc” forzar el cierre de la aplicación.
*/

#define PROYECTO "S6E01::Pruebas con Cursor"
#include <iostream> // Biblioteca de entrada salida
#include <cmath> // Biblioteca matematica de C
#include "../codebase.h" // Biblioteca grafica

using namespace std;

bool wire = false;
bool solid = false;

void init() // Funcion propia de inicializacion
{
    cout << "Version: OpenGL " << glGetString(GL_VERSION) << endl;
    glClearColor(1.0,1.0,1.0,1.0); 
    glColor3f(0,0,0); // Color de fondo a blanco

}
void display()
// Funcion de atencion al dibujo
{
    glClear(GL_COLOR_BUFFER_BIT); // Borra la pantalla
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if (wire)
    {
        glutWireTeapot(0.5);
    }
    else if(solid)
    {
        glutSolidCube(0.5);
    }

    glFlush(); // Finaliza el dibujo
}
void reshape(GLint w, GLint h)
{
    static const GLfloat ANCHO(2.0),ALTO(2.0);
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float razon = (float) w / h;
}

void onKey(unsigned char tecla, int x, int y)
{
    if (tecla == 'a')
    {
        wire = true;
    }
    else if (tecla == 's')
    {
        solid = true;
    }
    else if(tecla == 27)
    {
        exit(0);;
    }
    glutPostRedisplay();
}
void onPassiveMotion(int x, int y)
{
    
    string coordenadas = "Coordenadas: (" + to_string(x) + ", " + to_string(y) + ")" ;
    glutSetWindowTitle(coordenadas.c_str());
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv); // Inicializacion de GLUT
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB); // Alta de buffers a usar
    glutInitWindowSize(400,400); // Tamanyo inicial de la ventana
    glutCreateWindow(PROYECTO); // Creacion de la ventana con su titulo

    glutDisplayFunc(display); // Alta de la funcion de atencion a display
    glutReshapeFunc(reshape); // Alta de la funcion de atencion a reshape
    glutPassiveMotionFunc(onPassiveMotion);
    glutKeyboardFunc(onKey);

    init(); // Inicializacion propia

    glutMainLoop(); // Puesta en marcha del programa
}