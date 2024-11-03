// *********************************************************************
// **
// ** Asignatura: INFORMÁTICA GRÁFICA
// **
// ** Mallas indexadas creadas por revolución de un perfil (implementación). Clase 'MallaRevol' y derivadas.
// ** Copyright (C) 2016-2024 Carlos Ureña
// **
// ** Implementación de las clases
// **    + MallaRevol: malla indexada de triángulos obtenida por
// **      revolución de un perfil (derivada de MallaInd)
// **    + MallaRevolPLY: malla indexada de triángulos, obtenida
// **      por revolución de un perfil leído de un PLY (derivada de MallaRevol)
// **    + algunas clases derivadas de MallaRevol
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

#include "ig-aux.h"
#include "lector-ply.h"
#include "malla-revol.h"

using namespace std;

// Método que crea las tablas de vértices, triángulos, normales y cc.de.tt.
// a partir de un perfil y el número de copias que queremos de dicho perfil.
void MallaRevol::inicializar(const std::vector<glm::vec3> &perfil, const unsigned num_copias)
{
   using namespace glm;

   const double angle = (2*M_PI)/(num_copias-1);
   const size_t num_vertices = perfil.size();

   // Creación de la tabla de vértices por revolución del perfil
   for (size_t i = 0; i < num_copias; i++)
   {
      for (size_t j = 0; j < num_vertices; j++)
      {
         vec3 vertex = rotateVertexYAxis(angle*i, perfil[j]);
         vertices.push_back(vertex);
      }
   }

   size_t index;

   // Creación de la tabla de triángulos
   for (size_t i = 0; i < num_copias - 1; i++)
   {
      for (size_t j = 0; j < num_vertices - 1; j++)
      {
         index = i*num_vertices+j;
         triangulos.push_back({index, index+num_vertices, index+num_vertices+1});
         triangulos.push_back({index, index+num_vertices+1, index+1});
      }
   }
}

// Método que crea las tablas de vértices, triángulos, normales y cc.de.tt.
// a partir de un perfil y el número de copias que queremos de dicho perfil.
void MallaRevol::inicializarX(const std::vector<glm::vec3> &perfil, const unsigned num_copias)
{
   using namespace glm;

   const double angle = (2*M_PI)/(num_copias-1);
   const size_t num_vertices = perfil.size();

   // Creación de la tabla de vértices por revolución del perfil
   for (size_t i = 0; i < num_copias; i++)
   {
      for (size_t j = 0; j < num_vertices; j++)
      {
         vec3 vertex = rotateVertexXAxis(angle*i, perfil[j]);
         vertices.push_back(vertex);
      }
   }

   size_t index;

   // Creación de la tabla de triángulos
   for (size_t i = 0; i < num_copias - 1; i++)
   {
      for (size_t j = 0; j < num_vertices - 1; j++)
      {
         index = i*num_vertices+j;
         triangulos.push_back({index, index+num_vertices, index+num_vertices+1});
         triangulos.push_back({index, index+num_vertices+1, index+1});
      }
   }
}

// Método que crea las tablas de vértices, triángulos, normales y cc.de.tt.
// a partir de un perfil y el número de copias que queremos de dicho perfil.
void MallaRevol::inicializar(const std::vector<glm::vec3> &perfil, const unsigned num_copias, const double angle)
{
   using namespace glm;

   const double subangles = angle/(num_copias-1);
   const size_t num_vertices = perfil.size();

   // Creación de la tabla de vértices por revolución del perfil
   for (size_t i = 0; i < num_copias; i++)
   {
      for (size_t j = 0; j < num_vertices; j++)
      {
         vec3 vertex = rotateVertexYAxis(subangles*i, perfil[j]);
         vertices.push_back(vertex);
      }
   }

   size_t index;

   // Creación de la tabla de triángulos
   for (size_t i = 0; i < num_copias - 1; i++)
   {
      for (size_t j = 0; j < num_vertices - 2; j++)
      {
         index = i*num_vertices+j;
         triangulos.push_back({index, index+num_vertices, index+num_vertices+1});
         triangulos.push_back({index, index+num_vertices+1, index+1});
      }
   }
}

glm::vec3 MallaRevol::rotateVertexYAxis(const double angle, const glm::vec3 & vertex)
{
   glm::vec3 out;
   out.x = cos(angle)*vertex.x + sin(angle)*vertex.z;
   out.y = vertex.y;
   out.z = - sin(angle)*vertex.x + cos(angle)*vertex.z;
   return out;
}

glm::vec3 MallaRevol::rotateVertexXAxis(const double angle, const glm::vec3 & vertex)
{
   glm::vec3 out;
   out.x = vertex.x;
   out.z = cos(angle)*vertex.z + sin(angle)*vertex.y;
   out.y = - sin(angle)*vertex.z + cos(angle)*vertex.y;
   return out;
}

// -----------------------------------------------------------------------------
// Constructor, a partir de un archivo PLY

MallaRevolPLY::MallaRevolPLY(const std::string &nombre_arch, const unsigned nperfiles)
{
   std::vector<glm::vec3> perfil;

   ponerNombre(std::string("Malla por revolución del perfil en '" + nombre_arch + "'"));
   LeerVerticesPLY(nombre_arch, perfil);
   inicializar(perfil, nperfiles);
}

// -----------------------------------------------------------------------------
// Clase Cilindro

Cilindro::Cilindro(const int num_verts_per, const unsigned nperfiles)
{
   std::vector<glm::vec3> perfil;

   ponerNombre(std::string("Cilindro de Revolución"));

   // La base tiene el centro en el origen, el radio y la altura son 1
   for (int i = 0; i <= num_verts_per; i++)
   {
      perfil.push_back({+1.0, float(i)/float(num_verts_per) , 0.0});
   }
   inicializar(perfil, nperfiles);
}

// -----------------------------------------------------------------------------
// Clase Cono

Cono::Cono(const int num_verts_per, const unsigned nperfiles)
{
   std::vector<glm::vec3> perfil;

   ponerNombre(std::string("Cono de Revolución"));
   // La base tiene centro en el origen, el radio y la altura son 1
   for (int i = 0; i <= num_verts_per; i++)
   {
      float y = float(i)/float(num_verts_per);
      perfil.push_back({1-y, y, 0.0});
   }
   inicializar(perfil, nperfiles);
}

// -----------------------------------------------------------------------------
// Clase Esfera

Esfera::Esfera(const int num_verts_per, const unsigned nperfiles)
{
   std::vector<glm::vec3> perfil;
   const float subintervals = M_PI/float(num_verts_per);
   const float lowerbound = 1.5*M_PI;

   ponerNombre(std::string("Esfera de Revolución"));
   // La esfera tiene el centro en el origen, el radio es 1
   for (int i = 0; i <= num_verts_per; i++)
   {
      float angle = lowerbound + float(i)*subintervals;
      perfil.push_back({cos(angle), sin(angle), 0.0});
   }
   inicializar(perfil, nperfiles);
}