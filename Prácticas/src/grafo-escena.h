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
// ** Gestión de grafos de escena (declaraciones).  Clase 'NodoGrafoEscena' y relacionadas.
// ** Copyright (C) 2016-2024 Carlos Ureña
// **
// ** Declaraciones de:
// **     + Clase 'NodoGrafoEscena' (derivada de 'Objeto3D')
// **     + Clase 'EntradaNGE' (una entrada de un nodo del grafo de escena)
// **     + Tipo enumerado 'TipoEntNGE' (tipo de entradas del nodo del grafo de escena)
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

#ifndef GRAFO_ESCENA_HPP
#define GRAFO_ESCENA_HPP

// #include <tup_mat.h>

#include "objeto3d.h"
#include "malla-ind.h"
#include "materiales-luces.h"

// using namespace tup_mat ;

// Declaración adelantada de estructura para un nodo del grafo de escena
class NodoGrafoEscena;

// Tipo enumerado con los tipos de entradas del nodo del grafo de escena
enum class TipoEntNGE
{
   objeto,
   transformacion,
   material,
   noInicializado
};

// Entrada del nodo del Grafo de Escena
struct EntradaNGE
{
   TipoEntNGE tipo = TipoEntNGE::noInicializado; // Objeto, transformacion, material
   union
   {
      Objeto3D *objeto = nullptr; // Ptr. a un objeto (no propietario)
      glm::mat4 *matriz;          // Ptr. a matriz 4x4 transf. (propietario)
      Material *material;         // Ptr. a material (no propietario)
   };
   // Constructores
   EntradaNGE() = delete;                // Prohibe constructor sin parámetros
   EntradaNGE(Objeto3D *pObjeto);        // (copia solo puntero)
   EntradaNGE(const glm::mat4 &pMatriz); // (crea copia en el heap)
   EntradaNGE(Material *pMaterial);      // (copia solo puntero)
   ~EntradaNGE();
};

// Nodo del grafo de escena: es un objeto 3D parametrizado, que contiene una lista de entradas
class NodoGrafoEscena : public Objeto3D
{
protected:
   // Vector de entradas
   std::vector<EntradaNGE> entradas;
public:
   NodoGrafoEscena();

   // Visualiza usando OpenGL
   virtual void visualizarGL();

   // Visualizar pura y simplemente la geometría, sin colores, normales, coord. text. etc...
   virtual void visualizarGeomGL();

   // Visualizar las normales de los objetos del nodo
   virtual void visualizarNormalesGL();

   // Visualizar el objeto en 'modo seleccion', es decir, sin iluminación y con los colores
   // basados en los identificadores de los objetos
   virtual void visualizarModoSeleccionGL();

   // Añadir una entrada al final, hace copia de la entrada
   // devuelve indice de la entrada dentro del vector de entradas
   unsigned agregar(const EntradaNGE &entrada);

   // Construir una entrada y añadirla (al final)
   unsigned agregar(Objeto3D *pObjeto);        // Objeto (copia solo puntero)
   unsigned agregar(const glm::mat4 &pMatriz); // Matriz (copia objeto)
   unsigned agregar(Material *pMaterial);      // Material (copia solo puntero)

   // Devuelve el puntero a la matriz en la i-ésima entrada
   glm::mat4 *leerPtrMatriz(unsigned iEnt);

   // Método para buscar un objeto con un identificador
   virtual bool buscarObjeto(const int ident_busc, const glm::mat4 &mmodelado, Objeto3D **objeto, glm::vec3 &centro_wc);

   // Si 'centro_calculado' es 'false', recalcula el centro usando los centros
   // de los hijos (el punto medio de la caja englobante de los centros de hijos)
   virtual void calcularCentroOC();
};

class GrafoEstrellaX : public NodoGrafoEscena
{
protected:
   glm::mat4 *rotationalmovement = nullptr;
public:
   GrafoEstrellaX
   (
      unsigned n // Número de puntas
   );

   // Devuelve el número de parámetros de este objeto
   virtual unsigned leerNumParametros() const;

   // Actualiza el valor de un parámetro a un instante de tiempo
   virtual void actualizarEstadoParametro(const unsigned iParam, const float t_sec);
};

class GrafoCubos : public NodoGrafoEscena
{
protected:
   glm::mat4 *rotationalmovement = nullptr;
public:
   GrafoCubos();

   // Devuelve el número de parámetros de este objeto
   virtual unsigned leerNumParametros() const;
   
   // Actualiza el valor de un parámetro a un instante de tiempo
   virtual void actualizarEstadoParametro(const unsigned iParam, const float t_sec);
};

class CubePair : public NodoGrafoEscena
{
protected:
   glm::mat4 *scalingmovement = nullptr;
   glm::mat4 *rotatingmovement = nullptr;
public:
   CubePair
   (
      float h, // Altura
      float w  // Anchura
   );

   // Devuelve el número de parámetros de este objeto
   virtual unsigned leerNumParametros() const;
   
   // Actualiza el valor de un parámetro a un instante de tiempo
   virtual void actualizarEstadoParametro(const unsigned iParam, const float t_sec);
};

#endif // GRAFO_ESCENA_HPP
