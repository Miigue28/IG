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
// ** Mallas indexadas (declaraciones). Clase 'MallaInd' y derivadas.
// ** Copyright (C) 2016-2024 Carlos Ureña
// **
// ** Declaración de las clases
// **        + MallaInd: malla indexada de triángulos (derivada de Objeto3D)
// **        + MallaPLY: malla indexada de triángulos, leída de un PLY (derivada de MallaInd)
// **        + algunas clases derivadas de MallaInd.
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

#ifndef IG_MALLAIND_HPP
#define IG_MALLAIND_HPP

#include <vector>
#include <ig-aux.h>
#include <vaos-vbos.h>

#include "objeto3d.h" // Declaración de 'Objeto3D'

// ---------------------------------------------------------------------
// Clase para objetos gráficos genéricos

class MallaInd : public Objeto3D
{
protected:
   // COMPLETAR: incluir aquí las variables y métodos privados que sean
   // necesarios para una malla indexada (y que no aparezcan ya declarados en esta plantilla)
   // ......

   std::vector<glm::vec3> vertices;
   std::vector<glm::uvec3> triangulos;

   std::vector<glm::vec3> col_ver;   // Colores de los vértices
   std::vector<glm::vec3> nor_ver;   // Normales de vértices
   std::vector<glm::vec3> nor_tri;   // Normales de triángulos
   std::vector<glm::vec2> cc_tt_ver; // Coordenadas de textura de los vértices

   // Descriptor del VAO con los vértices, triángulos y atributos de esta malla indexada
   // (se crea bajo demanda en 'visualizarGL')
   DescrVAO *dvao = nullptr;

   // VAO con los segmentos de las normales (vis. con GL_LINES)
   // (se crea bajo demanda en 'visualizarNormales')
   DescrVAO *dvao_normales = nullptr;

   std::vector<glm::vec3> segmentos_normales; // Guarda los segmentos de normales

   // Normales de triángulos y vértices
   void calcularNormales();

   // Cálculo de las normales de triángulos (solo si no están creadas ya)
   void calcularNormalesTriangulos();

public:
   // Crea una malla vacía (nombre: "malla indexada nueva vacía")
   MallaInd();

   // Crea una malla vacía con un nombre concreto
   MallaInd(const std::string &nombreIni);

   // Visualizar el objeto con OpenGL
   // usa las tablas de normales, colores y coordenadas de textura, si no están vacías.
   virtual void visualizarGL();

   // Visualizar pura y simplemente la geometría, sin colores, normales, coord. text. etc...
   // (se supone que el estado de OpenGL está fijado antes de esta llamada de alguna forma adecuada)
   virtual void visualizarGeomGL();

   // Visualizar las normales del objeto, si no tiene tabla de normales imprime advertencia y no hace nada.
   virtual void visualizarNormalesGL();

   // Visualizar el objeto en 'modo seleccion', es decir, sin iluminación y con los colores
   // basados en los identificadores de los objetos
   virtual void visualizarModoSeleccionGL();

   // Métodos para acceder a las tablas de una malla (acceso de lectura)
   const std::vector<glm::vec3> &refVertices() const { return vertices; }
   const std::vector<glm::uvec3> &refTriangulos() const { return triangulos; }
};
// ---------------------------------------------------------------------
// Clase para mallas obtenidas de un archivo 'ply'
// es un tipo de malla indexada que define un nuevo constructor
// que recibe el nombre del archivo ply como parámetro

class MallaPLY : public MallaInd
{
public:
   MallaPLY(const std::string &nombre_arch);
};

// ---------------------------------------------------------------------

class Cubo : public MallaInd
{
public:
   Cubo();
};

class Tetraedro : public MallaInd
{
public:
   Tetraedro();
};

class CuboColores : public MallaInd
{
public:
   CuboColores();
};

class EstrellaZ : public MallaInd
{
public:
   EstrellaZ
   (
      unsigned n // Número de puntas de la estrella
   );
};

class PiramideEstrellaZ : public MallaInd
{
public:
   PiramideEstrellaZ
   (
      unsigned n
   );
};

class CasaX : public MallaInd
{
public:
   CasaX();
};

class Triangulo : public MallaInd
{
public:
   Triangulo();
};

class Cuadrado : public MallaInd
{
public:
   Cuadrado();
};

class PiramideL : public MallaInd
{
public:
   PiramideL();
};

class RejillaY : public MallaInd
{
public:
   RejillaY
   (
      unsigned n, // Número de filas
      unsigned m  // Número de columnas
   );
};

class MallaTorre : public MallaInd
{
public:
   MallaTorre
   (
      unsigned n // Número de plantas de la torre
   );
};

#endif
