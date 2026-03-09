#pragma once
/**
	Engranajes.h

	Clases y funciones auxiliares sobre aros para practicas 
	de Informatica Grafica

	@author R.Vivo' <rvivo@upv.es>
*/

#include <iostream>
#include <vector>
#include <array>
#include "../codebase.h"


using namespace std;
using namespace cb;

// Devuelve una cantidad de puntos en una circunferencia
// centrada en el origen dado el radio y la fase
vector<cb::Vec3> circlePoints(float radio, float fase, unsigned int cantidad)
{
	vector<cb::Vec3> puntos;
	if (cantidad < 1) return puntos;

	float angulo = 2 * PI / cantidad;
	for (unsigned int i = 0; i < cantidad; i++) {
		puntos.push_back(cb::Vec3(radio * cosf(i * angulo + fase), radio * sinf(i * angulo + fase), 0));
	}

	return puntos;

}

// Funcion interseccion de dos circunferencias dados sus centros y radios
// Devuelve un vector con los puntos de interseccion
vector<Vec3> interseccionCircunferencias(Vec3 c1, float r1, Vec3 c2, float r2)
{
	vector<Vec3> puntos;
	float d = sqrtf(powf(c2.x - c1.x, 2) + powf(c2.y - c1.y, 2));
	if (d > r1 + r2) return puntos; // No hay interseccion
	if (d < fabs(r1 - r2)) return puntos; // Una circunferencia esta dentro de la otra

	float a = (powf(r1, 2) - powf(r2, 2) + powf(d, 2)) / (2 * d);
	float h = sqrtf(powf(r1, 2) - powf(a, 2));
	Vec3 p2(c1.x + a * (c2.x - c1.x) / d, c1.y + a * (c2.y - c1.y) / d);
	puntos.push_back(Vec3(p2.x + h * (c2.y - c1.y) / d, p2.y - h * (c2.x - c1.x) / d));
	puntos.push_back(Vec3(p2.x - h * (c2.y - c1.y) / d, p2.y + h * (c2.x - c1.x) / d));

	return puntos;
}

class Aro
{
protected:
	float d_ext = 1.0f;
	float d_int = 0.4f;
	float grosor = 0.2f;
	unsigned int secciones = 40;
	float h_diente = 0.0f;
	vector<cb::Vec3> pext;
	vector<cb::Vec3> pint;
	vector<cb::Vec3> pdientes;
public:
	Aro(float dexterior, float dinterior, float ancho, unsigned int n = 40)
		: d_ext(dexterior), d_int(dinterior), grosor(ancho), secciones(n) {
		pext = circlePoints(d_ext/2, 0, secciones);
		pint = circlePoints(d_int/2, 0, secciones);
	};
	float getDExterior() const { return d_ext; };
	float getDInterior() const { return d_int; };
	float getGrosor() const { return grosor; };
	unsigned int getDientes() const { return secciones; };
	virtual void dibujar() const {
		// Caras del aro exterior como quads
		for (unsigned int i = 0; i < secciones; i++) {
			cb::Vec3 v0 = pext[i];
			cb::Vec3 v1 = pext[i] + cb::Vec3(0, 0, -grosor);
			cb::Vec3 v2 = pext[(i + 1) % secciones] + cb::Vec3(0, 0, -grosor);
			cb::Vec3 v3 = pext[(i + 1) % secciones];
			cb::quad(v0, v1, v2, v3, 1, 1);
		}
		
		// Caras laterales del aro como quads	
		for (unsigned int i = 0; i < secciones; i++) {
			cb::Vec3 v0 = pext[i];
			cb::Vec3 v1 = pext[(i + 1) % secciones];
			cb::Vec3 v2 = pint[(i + 1) % secciones];
			cb::Vec3 v3 = pint[i];
			cb::quad(v0, v1, v2, v3, 1, 1);
		}
		for (unsigned int i = 0; i < secciones; i++) {
			cb::Vec3 v0 = pext[i] + cb::Vec3(0, 0, -grosor);
			cb::Vec3 v1 = pext[(i + 1) % secciones] + cb::Vec3(0, 0, -grosor);
			cb::Vec3 v2 = pint[(i + 1) % secciones] + cb::Vec3(0, 0, -grosor);
			cb::Vec3 v3 = pint[i] + cb::Vec3(0, 0, -grosor);
			cb::quad(v3, v2, v1, v0, 1, 1);
		}

		// Caras del eje como quads
		for (unsigned int i = 0; i < secciones; i++) {
			cb::Vec3 v0 = pint[i];
			cb::Vec3 v1 = pint[i] + cb::Vec3(0, 0, -grosor);
			cb::Vec3 v2 = pint[(i + 1) % secciones] + cb::Vec3(0, 0, -grosor);
			cb::Vec3 v3 = pint[(i + 1) % secciones];
			cb::quad(v1, v0, v3, v2, 1, 1);
		}
	};
	virtual void dibujarDoble() const {

		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glDisable(GL_LIGHTING);

		glColor3f(0, 0, 0);
		glPolygonMode(GL_FRONT, GL_FILL);

		dibujar();

		glColor3f(1, 1, 1);
		glPolygonMode(GL_FRONT, GL_LINE);

		glPushMatrix();
		glScalef(1.01f, 1.01f, 0.9f);
		glTranslatef(0, 0, -0.01f);
		dibujar();
		glPopMatrix();

		glPopAttrib();
	};
	virtual void saveSTL(const char* filename) const {};
};

// Construye un engranaje de dientes en triangulo
class EngranajeExterior : public Aro
{
protected:
public:
	// Constructor. n: dientes, dprim: diametro primitivo exterior, hdiente: altura del diente,
	// deje: diametro del eje, ancho: grosor del aro 

	EngranajeExterior(unsigned int n, float dprim, float hdiente, float deje, float ancho)
		: Aro(dprim-hdiente, deje, ancho, n){
		h_diente = hdiente;
		float rext = d_ext / 2 + h_diente;
		pdientes = circlePoints(rext, PI/secciones, secciones);
	};

	float getModulo() const { return (d_ext + h_diente) / secciones; };
	float getDPrimitivo() const { return d_ext + h_diente; };
	void dibujar() const {
		if (secciones < 3)return;
		Aro::dibujar();

		// Caras de los dientes como quads
		for (unsigned int i = 0; i < secciones; i++) {
			cb::Vec3 v0 = pext[i];
			cb::Vec3 v1 = pext[i] + cb::Vec3(0, 0, -grosor);
			cb::Vec3 v2 = pdientes[i] + cb::Vec3(0, 0, -grosor);
			cb::Vec3 v3 = pdientes[i];
			// Recorto los picos de los dientes
			//v2 = v2 * 0.3f + v1 * 0.7f;
			//v3 = v3 * 0.3f + v0 * 0.7f;
			cb::quad(v0, v1, v2, v3, 1, 1);
			v0 = pext[(i + 1) % secciones];
			v1 = pext[(i + 1) % secciones] + cb::Vec3(0, 0, -grosor);
			cb::quad(v3, v2, v1, v0, 1, 1);
		}

		// Laterales de los dientes como triangulos
		glBegin(GL_TRIANGLES);
		cb::Vec3 v0, v1, v2;
		
		for (unsigned int i = 0; i < secciones; i++) {
			v0 = pext[i];
			v1 = pdientes[i];
			v2 = pext[(i + 1) % secciones];

			glNormal3f(0, 0, 1);
			glVertex3fv(v0);
			glVertex3fv(v1);
			glVertex3fv(v2);

			glNormal3f(0, 0, -1);
			glVertex3fv(v0 + cb::Vec3(0, 0, -grosor));
			glVertex3fv(v2 + cb::Vec3(0, 0, -grosor));
			glVertex3fv(v1 + cb::Vec3(0, 0, -grosor));
		}
		glEnd();
	};
	void dibujarDoble() const {

		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glDisable(GL_LIGHTING);

		glColor3f(0, 0, 0);
		glPolygonMode(GL_FRONT, GL_FILL);

		dibujar();

		glColor3f(1, 1, 1);
		glPolygonMode(GL_FRONT, GL_LINE);

		glPushMatrix();
		glScalef(1.01f, 1.01f, 0.9f);
		glTranslatef(0, 0, -0.01f);
		dibujar();
		glPopMatrix();

		glPopAttrib();
	}
	void saveSTL(const char* filename) const {

	};
};

class EngranajeInterior :  public Aro
{
protected:
public:
	// Constructor. n: dientes, dext: diametro exterior, hdiente: altura del diente,
	// dprimint: diametro primitivo interior, ancho: grosor del aro 
	EngranajeInterior(unsigned int n, float dext, float hdiente, float dprimint, float ancho)
		: Aro(dext, dprimint+hdiente, ancho, n) {
		h_diente = hdiente;	
		float rint = d_int / 2 - h_diente;
		pdientes = circlePoints(rint, PI / secciones, secciones);
	};
	float getModulo() const { return (d_int - h_diente) / secciones; };
	float getDPrimitivo() const { return d_int - h_diente; };
	void dibujar() const {
		if (secciones < 3)return;
		Aro::dibujar();

		
		// Caras de los dientes como quads
		for (unsigned int i = 0; i < secciones; i++) {
			cb::Vec3 v0 = pint[i];
			cb::Vec3 v1 = pint[i] + cb::Vec3(0, 0, -grosor);
			cb::Vec3 v2 = pdientes[i] + cb::Vec3(0, 0, -grosor);
			cb::Vec3 v3 = pdientes[i];
			cb::quad(v0, v1, v2, v3, 1, 1);
			v0 = pint[(i + 1) % secciones];
			v1 = pint[(i + 1) % secciones] + cb::Vec3(0, 0, -grosor);
			cb::quad(v3, v2, v1, v0, 1, 1);
		}

		// Laterales de los dientes como triangulos
		glBegin(GL_TRIANGLES);
		cb::Vec3 v0, v1, v2;

		for (unsigned int i = 0; i < secciones; i++) {
			v0 = pint[i];
			v1 = pdientes[i];
			v2 = pint[(i + 1) % secciones];

			glNormal3f(0, 0, 1);
			glVertex3fv(v0);
			glVertex3fv(v1);
			glVertex3fv(v2);

			glNormal3f(0, 0, -1);
			glVertex3fv(v0 + cb::Vec3(0, 0, -grosor));
			glVertex3fv(v2 + cb::Vec3(0, 0, -grosor));
			glVertex3fv(v1 + cb::Vec3(0, 0, -grosor));
		}
		glEnd();
	}
};

class EngranajeLateral : public Aro
{
protected:
	float a_diente = 0.2f;
public:
	// Constructor. n: dientes, dext: diametro exterior, hdiente: altura del diente,
	// dint: diametro interior, ancho: grosor del aro 
	// El ancho del diente es la diferencia entre los radios exterior e interior
	EngranajeLateral(unsigned int n, float dext, float hdiente, float dint, float ancho)
		: Aro(dext, dint, ancho, n), a_diente((dext-dint)/2) {
		h_diente = hdiente;
		// Picos de los dientes en el exterior
		pdientes = circlePoints(d_ext/2, PI/secciones, secciones);
	};
	float getModulo() const { return (d_ext - a_diente) / secciones; };
	float getDPrimitivo() const { return d_ext - a_diente; };
	void dibujar() const {
		if (secciones < 3)return;
		Aro::dibujar();

		// Caras de los dientes como quads
		for (unsigned int i = 0; i < secciones; i++) {
			cb::Vec3 v0 = pint[i];
			cb::Vec3 v1 = pext[i];
			cb::Vec3 v2 = pdientes[i] + cb::Vec3(0, 0, h_diente);
			cb::Vec3 v3 = pdientes[i] * d_int/d_ext + cb::Vec3(0, 0, h_diente);
			cb::quad(v0, v1, v2, v3, 1, 1);
			v1 = pext[(i + 1) % secciones];
			v0 = pint[(i + 1) % secciones];
			cb::quad(v3, v2, v1, v0, 1, 1);
		}

		// Laterales de los dientes como triangulos
		glBegin(GL_TRIANGLES);
		cb::Vec3 v0, v1, v2;

		for (unsigned int i = 0; i < secciones; i++) {
			v0 = pint[i];
			v1 = pdientes[i] * d_int / d_ext + cb::Vec3(0, 0, h_diente);
			v2 = pint[(i + 1) % secciones];

			glNormal3fv(Vec3()- pdientes[i] * d_int / d_ext);
			glVertex3fv(v0);
			glVertex3fv(v1);
			glVertex3fv(v2);

			v2 = pext[i];
			v1 = pdientes[i] + cb::Vec3(0, 0, h_diente);
			v0 = pext[(i + 1) % secciones];

			glNormal3fv(pdientes[i] * d_int / d_ext - Vec3());
			glVertex3fv(v0);
			glVertex3fv(v1);
			glVertex3fv(v2);
		}
		glEnd();
	}
};

// Funcion de dibujo de una saeta y la devuelve como
// una display list
GLuint saeta(float longitud, float grosor)
{


	GLuint saeta = glGenLists(1);
	glNewList(saeta, GL_COMPILE);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);

	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glVertex3f(-grosor / 2, 0, 0);
	glVertex3f(grosor / 2, 0, 0);
	glVertex3f(grosor / 2, longitud, 0);
	glVertex3f(-grosor / 2, longitud, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glNormal3f(0, 0, 1);
	glVertex3f(-grosor / 2, longitud, 0);
	glVertex3f(grosor / 2, longitud, 0);
	glVertex3f(0, longitud + grosor, 0);
	glEnd();

	glPopAttrib();
	glEndList();



	return saeta;
}
