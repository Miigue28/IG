/**
 * Nombre: Miguel Ángel Moreno Castro
 * Titulación: DGIIM
 * Email: miguelangelmc@correo.ugr.es
 * DNI: 20070272
**/

// *********************************************************************
// **
// ** Asignatura: INFORMÁTICA GRÁFICA
// **
// ** Gestión de materiales y texturas (implementación)
// ** Copyright (C) 2016-2024 Carlos Ureña.
// **
// ** Declaraciones de:
// **    + clase 'Textura' (y derivadas 'TexturaXY', 'TexturaXZ')
// **    + clase 'Material'
// **    + clase 'FuenteLuz'
// **    + clase 'ColFuentesLuz' (y la clase derivada 'Col2Fuentes')
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************

#ifndef MATERIALES_HPP
#define MATERIALES_HPP

#include <vector>
#include "ig-aux.h"
#include "lector-jpg.h"

// Algunes declaraciones auxiliares importantes
class Material;
class Textura;

// Posibles modos de generacion de coords. de textura
typedef enum
{
   mgct_desactivada,
   mgct_coords_objeto,
   mgct_coords_ojo
} ModoGenCT;

// Clase Textura: Encapsula una imagen de textura de OpenGL, así como los parámetros relativos 
// a como se aplica  a las primitivas que se dibujen mientras está activa
class Textura
{
public:
   // Carga una imagen de textura en la memoria de vídeo, e
   // inicializa los atributos de la textura a valores por defecto.
   // El nombre del archivo debe ir sin el 'path', se busca en 'materiales/imgs' y si
   // no está se busca en 'archivos-alumno'
   Textura(const std::string &nombreArchivoJPG);

   // Libera la memoria dinámica usada por la textura, si hay alguna
   ~Textura();

   // Activar una textura en un cauce
   void activar();

protected:
   // Envia la imagen a la GPU (gluBuild2DMipmaps)
   void enviar();

   bool enviada = false;

   // Identificador de textura para OpenGL
   GLuint ident_textura = -1;

   // Número de columnas de la imagen
   unsigned ancho = 0;

   // Número de filas de la imagen
   unsigned alto = 0;

   // Pixels de la imagen, por filas.
   unsigned char * imagen = nullptr;

   // Modo de generacion de coordenadas de textura
   ModoGenCT modo_gen_ct = mgct_desactivada;

   // Coeficientes de generación de coordenadas de textura
   float coefs_s[4] = {1.0, 0.0, 0.0, 0.0}; 
   float coefs_t[4] = {0.0, 1.0, 0.0, 0.0};
};

// Clase TexturaXY: Textura con generación automática de coords. de textura (s = x, t = y)
class TexturaXY : public Textura
{
public:
   TexturaXY(const std::string &nom);
};
class TexturaXZ : public Textura
{
public:
   TexturaXZ(const std::string &nom);
};

// Clase Material: Encapsula los atributos de un material, incluyendo la
// textura del mismo.
class Material
{
public:
   Material(){};

   // Crea un material usando el color plano del objeto
   Material(/** const Tupla3f & p_color_ad,**/ const float p_k_amb, const float p_k_dif,
            const float p_k_pse, const float p_exp_pse);

   // Crea un material usando una textura
   Material(Textura *p_textura, const float p_k_amb, const float p_k_dif,
            const float p_k_pse, const float p_exp_pse);

   // Libera la memoria dinámica ocupada por el material (básicamente la textura)
   ~Material();

   // Activa un material en el cauce actual.
   void activar();

   // Poner y leer el nombre del material
   void ponerNombre(const std::string &nuevo_nombre);
   std::string nombre() const;
protected:
   // void coloresCero();// pone todos los colores y reflectividades a cero

   // Nombre del material
   std::string nombre_mat = "Material anónimo";

   // Color para componentes difusa y ambiental
   //Tupla3f color_ad = { 0.6f, 0.6f, 0.6f };

   // Si textura != nullptr, se usa color de la textura en lugar de 'color_ad'
   Textura * textura = nullptr;

   float k_amb = 0.2f;   // Coeficiente de reflexión ambiente
   float k_dif = 0.8f;   // Coeficiente de reflexión difusa
   float k_pse = 0.0f;   // Coeficiente de reflexión pseudo-especular
   float exp_pse = 0.0f; // Exponente de brillo para reflexion pseudo-especular
};

// Clase FuenteLuz: Contiene los atributos de una fuente de luz OpenGL
class FuenteLuz
{
public:
   // Inicializa la fuente de luz
   //
   // p_longi_ini == Valor inicial del ángulo horizontal en grados
   // p_lati_ini  == Valor inicial del ángulo vértical en grados
   // p_color     == Color de la fuente de luz (amb, dif y spec)
   FuenteLuz(GLfloat p_longi_ini, GLfloat p_lati_ini, const glm::vec3 & p_color);

   // cambia los atributos de la instancia en respuesta a una pulsación
   // de una tecla 'especial' (según la terminología de 'glut')
   // bool gestionarEventoTeclaEspecial( int key ) ;

   // Para fuentes diraccionales, incrementar o decrementar la longitud
   // (en las puntuales no hace nada)
   void actualizarLongi(const float incre);
   // Para fuentes diraccionales, incrementar o decrementar la longitud
   // (en las puntuales no hace nada)
   void actualizarLati(const float incre);

public:
   float longi; // Longitud actual de la fuente direccional (en grados, entre 0 y 360)
   float lati;  // Latitud actual de la fuente direccional (en grados, entre -90 y 90)

protected:
   glm::vec3 color; // Color de la fuente de luz (sirve para todas las componentes del MIL)

   float longi_ini; // Valor inicial de 'longi'
   float lati_ini;  // Valor inicial de 'lati'

   friend class ColFuentesLuz;
};

// Clase ConjuntoFuentes: Contiene los atributos de una coleccion de fuentes de luz OpenGL
class ColFuentesLuz
{
public:
   ColFuentesLuz();
   ~ColFuentesLuz();

   // Inserta una nueva fuente de luz
   void insertar(FuenteLuz *pf);

   // Activa esta colección de fuentes en el cauce actual
   void activar();

   // Pasa a la siguiente fuente de luz (si d == +1), o a la anterior (si d == -1)
   // Aborta si 'd' no es -1 o +1, inicialmente es la fuente 0
   void sigAntFuente(int d);

   // Devuelve un puntero a la fuente de luz actual
   FuenteLuz *fuenteLuzActual();
private:
   // Vector de punteros a fuentes
   std::vector<FuenteLuz*> vpf; 

   // Máximo número de fuentes
   GLint max_num_fuentes = 0;

   // Indice de la fuente actual 
   unsigned i_fuente_actual = 0;
};

// PilaMateriales: Clase para un vector (usado como pila FIFO) con punteros a materiales
class PilaMateriales
{
public:
   PilaMateriales(){}
   // Guarda el material actual en la pila (NO puede ser nulo)
   void push();
   // Cambia el material actual y lo activa
   void activar(Material *nuevo_actual);
   // Invoca 'activar' para el material en el tope y lo elimina del tope
   void pop();

private:
   std::vector<Material *> vector_materiales;
   Material *actual = nullptr;
};

// --------------------------------------------------------------------------
// Actualizar una colección de fuentes en función de una tecla GLFW pulsada (se usa el código glfw de la tecla, se llama desde 'main.cpp' con L pulsada)
// Devuelve 'true' sii se ha actualizado algo
bool ProcesaTeclaFuenteLuz(ColFuentesLuz *col_fuentes, int glfw_key);

// Clase Col2Fuentes: Incluye dos fuentes de luz predefinidas
class Col2Fuentes : public ColFuentesLuz
{
public:
   Col2Fuentes();
};

#endif
