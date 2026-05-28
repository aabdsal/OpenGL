/*
    Ejercicio S6E04: Construir una aplicación que dibuje un cubo, 
    una esfera o una tetera según la opción de menú de pop-up seleccionada. 
    Cada opción de menú abrirá un submenú en cascada que permita
    seleccionar uno de los tres colores rojo, verde o azul como color de dibujo.
*/

#define PROYECTO "S7E04::Desplegar menus"
#include <iostream> // Biblioteca de entrada salida
#include <cmath> // Biblioteca matematica de C
#include "../codebase.h" // Biblioteca grafica

using namespace std;


int id_button = 0;

const int id_cubo = 1;
const int id_tetera = 2;
const int id_esfera = 3;
const int id_r = 4;
const int id_g = 5;
const int id_b = 6;


float r[3] = {1.0f, 0.0f, 0.0f};
float g[3] = {0.0f, 1.0f, 0.0f};
float b[3] = {0.0f, 0.0f, 1.0f};

void onMenu(int id);

void init() // Funcion propia de inicializacion
{
    cout << "Version: OpenGL " << glGetString(GL_VERSION) << endl;
    glClearColor(1.0,1.0,1.0,1.0); 
    glColor3f(0,0,0); // Color de fondo a blanco

    id_button = glutCreateMenu(onMenu);
    
    const char * mensaje_cubo = "Dibujar cubo";
    const char * mensaje_tetera = "Dibujar tetera";
    const char * mensaje_esfera = "Dibujar esfera";

    glutAddMenuEntry(mensaje_cubo, id_cubo);
    glutAddMenuEntry(mensaje_tetera, id_tetera);
    glutAddMenuEntry(mensaje_esfera, id_esfera);

    const char * mensaje_r = "De color rojo";
    const char * mensaje_g = "De color verde";
    const char * mensaje_b = "De color azul";
    
    glutAddSubMenu(mensaje_r, id_r);
    glutAddSubMenu(mensaje_g, id_g);
    glutAddSubMenu(mensaje_b, id_b);
    glutAttachMenu(GLUT_LEFT_BUTTON);
}
void display()
// Funcion de atencion al dibujo
{
    glClear(GL_COLOR_BUFFER_BIT); // Borra la pantalla
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
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
void onMenu(int id)
{  
    switch (id)
    {
    case id_cubo:
        break;
    case id_tetera:
        /* code */
        break;
    case id_esfera:
        /* code */
        break;
    
    default:
        break;
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
    

    init(); // Inicializacion propia

    glutMainLoop(); // Puesta en marcha del programa
}