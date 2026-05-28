/*
    Ejercicio S6E02: Implementar una interfaz de inspección sencilla 
    de una tetera asociando el movimiento del ratón a giros sobre 
    los ejes X e Y cuando se mantenga pulsado el botón izquierdo del
    ratón. Dibujar unos ejes que no queden afectados por los giros.
*/

#define PROYECTO "S6E01::Pruebas con Cursor"
#include <iostream> // Biblioteca de entrada salida
#include <cmath> // Biblioteca matematica de C
#include "../codebase.h" // Biblioteca grafica

using namespace std;

bool wire = false;
bool solid = false;
GLdouble ojo[] = {0, 0, 5}; 

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
    
    gluLookAt(ojo[0], ojo[1], ojo[2], 0, 0, 0, 0, 0, 1);

    glLoadIdentity();    
    glutWireTeapot(0.5);
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
    if(tecla == 27)
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
void onMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON and state == GLUT_UP)
    {
        ojo[0] = GLdouble(x);
        ojo[1] = GLdouble(y);
    }
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
    glutMouseFunc(onMouse);

    init(); // Inicializacion propia

    glutMainLoop(); // Puesta en marcha del programa
}