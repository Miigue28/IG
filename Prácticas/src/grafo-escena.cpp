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
// ** Gestión de grafos de escena (implementación). Clase 'NodoGrafoEscena' y relacionadas.
// ** Copyright (C) 2016-2024 Carlos Ureña
// **
// ** Implementación de:
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

#include "ig-aux.h"
#include "grafo-escena.h"
#include "aplicacion-ig.h"
#include "seleccion.h" // para 'ColorDesdeIdent'
#include "malla-revol.h"


// Constructor para entrada de tipo Sub-objeto
EntradaNGE::EntradaNGE(Objeto3D *pObjeto)
{
   assert(pObjeto != nullptr);
   tipo = TipoEntNGE::objeto;
   objeto = pObjeto;
}

// Constructor para entrada de tipo Transformación
EntradaNGE::EntradaNGE(const glm::mat4 &pMatriz)
{
   tipo = TipoEntNGE::transformacion;
   matriz = new glm::mat4(); // matriz en el heap, puntero propietario
   *matriz = pMatriz;
}

// Constructor para entrada de tipo Material
EntradaNGE::EntradaNGE(Material *pMaterial)
{
   assert(pMaterial != nullptr);
   tipo = TipoEntNGE::material;
   material = pMaterial;
}

// Destructor de una entrada
EntradaNGE::~EntradaNGE()
{
   /**  no fnciona debido a que se hacen copias (duplicados) de punteros
   if ( tipo == TipoEntNGE::transformacion )
   {
      assert( matriz != NULL );
      delete matriz ;
      matriz = NULL ;
   }
   * **/
}

// *****************************************************************************
// Nodo del grafo de escena: Contiene una lista de entradas
// *****************************************************************************

NodoGrafoEscena::NodoGrafoEscena()
{

}

// Visualiza usando OpenGL
void NodoGrafoEscena::visualizarGL()
{
   using namespace std;
   assert(aplicacionIG != nullptr);

   // cout << "Entra " << __FUNCTION__ << ": " << leerNombre() << endl ;

   // Comprobar que hay un cauce y una pila de materiales y recuperarlos.
   Cauce *cauce = aplicacionIG->cauce;
   assert(cauce != nullptr);
   PilaMateriales *pila_materiales = aplicacionIG->pila_materiales;
   assert(pila_materiales != nullptr);

   // Si el objeto tiene un color asignado
   if (tieneColor())
   {
      // Hacer push del color actual del cauce
      cauce->pushColor();
      // Fijar el color en el cauce usando el color del objeto
      cauce->fijarColor(leerColor());
   }
   // Guardar copia de la matriz de modelado
   cauce->pushMM();
   // Para cada entrada del vector de entradas
   for  (EntradaNGE entrada : entradas)
   {
      switch (entrada.tipo)
      {
         // Si la entrada es de tipo objeto: llamar recursivamente a 'visualizarGL'
         case TipoEntNGE::objeto: entrada.objeto->visualizarGL(); break;
         // Si la entrada es de tipo transformación: componer la matriz
         case TipoEntNGE::transformacion: cauce->compMM(*(entrada.matriz)); break;
      }
   }
   // Restaurar la copia guardada de la matriz de modelado
   cauce->popMM();
   // Si el objeto tiene color asignado
   if (tieneColor())
   {
      // Restaurar el color original a la entrada
      cauce->popColor();
   }

   // COMPLETAR: práctica 4: añadir gestión de los materiales cuando la iluminación está activada
   //
   // Si 'apl->iluminacion' es 'true', se deben de gestionar los materiales:
   //
   //   1. al inicio, hacer 'push' de la pila de materiales (guarda material actual en la pila)
   //   2. si una entrada es de tipo material, activarlo usando a pila de materiales
   //   3. al finalizar, hacer 'pop' de la pila de materiales (restaura el material activo al inicio)

   // ......
}

// Visualizar pura y simplemente la geometría, sin colores, normales, coord. text. etc...
void NodoGrafoEscena::visualizarGeomGL()
{
   using namespace std;
   assert(aplicacionIG != nullptr);

   // Comprobar que hay un cauce
   Cauce *cauce = aplicacionIG->cauce;
   assert(cauce != nullptr);

   // Guardar copia de la matriz de modelado
   cauce->pushMM();

   // Para cada entrada del vector de entradas
   for (EntradaNGE entrada : entradas)
   {
      switch (entrada.tipo)
      {
         // Si la entrada es de tipo objeto: llamar recursivamente a 'visualizarGeomGL'
         case TipoEntNGE::objeto: (entrada.objeto)->visualizarGeomGL(); break;
         // Si la entrada es de tipo transformación: componer la matriz
         case TipoEntNGE::transformacion: cauce->compMM(*(entrada.matriz)); break;
      }
   }
   // Restaurar la copia guardada de la matriz de modelado
   cauce->popMM();
}

// Visualizar las normales de los objetos del nodo
void NodoGrafoEscena::visualizarNormalesGL()
{
   using namespace std;
   assert(aplicacionIG != nullptr);

   // Comprobar que hay un cauce
   Cauce *cauce = aplicacionIG->cauce;
   assert(cauce != nullptr);

   // COMPLETAR: práctica 4: visualizar las normales del nodo del grafo de escena
   //
   // Este método hace un recorrido de las entradas del nodo, parecido a 'visualizarGL', teniendo
   // en cuenta estos puntos:
   //
   // - usar push/pop de la matriz de modelado al inicio/fin (al igual que en visualizatGL)
   // - recorrer las entradas, llamando recursivamente a 'visualizarNormalesGL' en los nodos u objetos hijos
   // - ignorar el color o identificador del nodo (se supone que el color ya está prefijado antes de la llamada)
   // - ignorar las entradas de tipo material, y la gestión de materiales (se usa sin iluminación)

   // .......
}

// -----------------------------------------------------------------------------
// visualizar el objeto en 'modo seleccion', es decir, sin iluminación y con los colores
// basados en los identificadores de los objetos
void NodoGrafoEscena::visualizarModoSeleccionGL()
{
   using namespace std;
   assert(aplicacionIG != nullptr);
   Cauce *cauce = aplicacionIG->cauce;
   assert(cauce != nullptr);

   // COMPLETAR: práctica 5: visualizar este nodo en modo selección.
   //
   // Se debe escribir código para dar estos pasos:
   //
   // 2. Leer identificador (con 'leerIdentificador'), si el identificador no es -1
   //      + Guardar una copia del color actual del cauce (con 'pushColor')
   //      + Fijar el color del cauce de acuerdo al identificador, (usar 'ColorDesdeIdent').
   // 3. Guardar una copia de la matriz de modelado (con 'pushMM')
   // 4. Recorrer la lista de nodos y procesar las entradas transformación o subobjeto:
   //      + Para las entradas subobjeto, invocar recursivamente a 'visualizarModoSeleccionGL'
   //      + Para las entradas transformación, componer la matriz (con 'compMM')
   // 5. Restaurar la matriz de modelado original (con 'popMM')
   // 6. Si el identificador no es -1, restaurar el color previo del cauce (con 'popColor')
   //
   // ........
}

// Añadir una entrada genérica (de cualquier tipo de entrada) al final
unsigned NodoGrafoEscena::agregar(const EntradaNGE &entrada)
{
   entradas.push_back(entrada);
   return entradas.size()-1;
}

// Construir una entrada de tipo objeto y añadirla al final
unsigned NodoGrafoEscena::agregar(Objeto3D *pObjeto)
{
   return agregar(EntradaNGE(pObjeto));
}

// Construir una entrada de tipo matriz y añadirla al final
unsigned NodoGrafoEscena::agregar(const glm::mat4 &pMatriz)
{
   return agregar(EntradaNGE(pMatriz));
}

// Construir una entrada de tipo material y añadirla al final
unsigned NodoGrafoEscena::agregar(Material *pMaterial)
{
   return agregar(EntradaNGE(pMaterial));
}

// Devuelve el puntero a la matriz en la i-ésima entrada
glm::mat4 *NodoGrafoEscena::leerPtrMatriz(unsigned indice)
{
   // Verificamos que el índice está dentro de rango
   assert(indice < entradas.size());
   // Verificamos que la entrada sea de tipo transformacion
   assert(entradas[indice].tipo == TipoEntNGE::transformacion);
   // Verificamos que el puntero a la matriz no sea nulo
   assert(entradas[indice].matriz != nullptr);

   return entradas[indice].matriz;
}
// -----------------------------------------------------------------------------
// si 'centro_calculado' es 'false', recalcula el centro usando los centros
// de los hijos (el punto medio de la caja englobante de los centros de hijos)

void NodoGrafoEscena::calcularCentroOC()
{
   using namespace std;
   using namespace glm;

   // COMPLETAR: práctica 5: calcular y guardar el centro del nodo
   //    en coordenadas de objeto (hay que hacerlo recursivamente)
   //   (si el centro ya ha sido calculado, no volver a hacerlo)
   // ........
}
// -----------------------------------------------------------------------------
// método para buscar un objeto con un identificador y devolver un puntero al mismo

bool NodoGrafoEscena::buscarObjeto(
    const int ident_busc,       // identificador a buscar
    const glm::mat4 &mmodelado, // matriz de modelado
    Objeto3D **objeto,          // (salida) puntero al puntero al objeto
    glm::vec3 &centro_wc        // (salida) centro del objeto en coordenadas del mundo
)
{
   using namespace std;
   using namespace glm;

   assert(0 < ident_busc);

   // COMPLETAR: práctica 5: buscar un sub-objeto con un identificador
   // Se deben de dar estos pasos:

   // 1. calcula el centro del objeto, (solo la primera vez)
   // ........

   // 2. si el identificador del nodo es el que se busca, ya está (terminar)
   // ........

   // 3. El nodo no es el buscado: buscar recursivamente en los hijos
   //    (si alguna llamada para un sub-árbol lo encuentra, terminar y devolver 'true')
   // ........

   // ni este nodo ni ningún hijo es el buscado: terminar
   return false;
}

GrafoEstrellaX::GrafoEstrellaX(unsigned n)
{
   assert(n > 1);

   using namespace glm;

   const float angle = (2*M_PI)/static_cast<float>(2*n);
   const float radius = 1.3;

   NodoGrafoEscena *star = new NodoGrafoEscena();
   star->agregar(translate(vec3(0.0, -1.3, 1.3)));
   star->agregar(rotate(float(M_PI/2.0f), vec3(0.0, 1.0, 0.0)));
   star->agregar(scale(vec3(1.3/0.5, 1.3/0.5, 1.0)));
   star->agregar(new EstrellaZ(n));

   NodoGrafoEscena *cone = new NodoGrafoEscena();
   cone->agregar(translate(vec3(0.0, 0.0, radius)));
   cone->agregar(rotate(static_cast<float>(M_PI/2.0f), vec3(1.0, 0.0, 0.0)));
   cone->agregar(scale(vec3(0.14, 0.15, 0.14)));
   cone->agregar(new Cone(15, 15));

   unsigned index = agregar(rotate(0.0f, vec3(1.0, 0.0, 0.0)));
   for (unsigned i = 0; i < n; ++i)
   {
      NodoGrafoEscena *tipcone = new NodoGrafoEscena();
      tipcone->agregar(rotate(static_cast<float>(2*i*angle), vec3(1.0, 0.0, 0.0)));
      tipcone->agregar(cone);

      agregar(tipcone);
   }
   agregar(star);

   rotationalmovement = leerPtrMatriz(index);
}

unsigned GrafoEstrellaX::leerNumParametros() const
{
   // Un grado de libertad
   return 1;
}

void GrafoEstrellaX::actualizarEstadoParametro(const unsigned iParam, const float t_sec)
{
   using namespace glm;

   // Verificamos que el índice de parámetro sea válido
   assert(iParam < leerNumParametros());

   float n = 2.5;

   // Actualizar la matriz del nodo grafo de escena
   *rotationalmovement = rotate(float(2*M_PI*n*t_sec), vec3(1.0, 0.0, 0.0));
}

GrafoCubos::GrafoCubos()
{
   using namespace glm;

   NodoGrafoEscena *rejilla = new NodoGrafoEscena();
   rejilla->agregar(translate(vec3(-0.5, -0.5, -0.5)));
   rejilla->agregar(new RejillaY(10, 10));

   NodoGrafoEscena *base = new NodoGrafoEscena();
   base->agregar(rejilla);
   base->agregar(translate(vec3(0.0,-0.7, 0.0)));
   unsigned index = base->agregar(rotate(0.0f, vec3(0.0, 1.0, 0.0)));
   base->agregar(scale(vec3(0.1, 0.2, 0.1)));
   base->agregar(new Cubo());
   rotationalmovement = base->leerPtrMatriz(index);

   NodoGrafoEscena *tapas = new NodoGrafoEscena();
   tapas->agregar(base);
   tapas->agregar(rotate(static_cast<float>(M_PI), vec3(1.0, 0.0, 0.0)));
   tapas->agregar(base);

   agregar(tapas);
   agregar(rotate(static_cast<float>(M_PI/2.0f), vec3(0.0, 0.0, 1.0)));
   agregar(tapas);
   agregar(rotate(static_cast<float>(M_PI/2.0f), vec3(1.0, 0.0, 0.0)));
   agregar(tapas);
}

unsigned GrafoCubos::leerNumParametros() const
{
   // Un grado de libertad
   return 1;
}

void GrafoCubos::actualizarEstadoParametro(const unsigned iParam, const float t_sec)
{
   using namespace glm;

   // Verificamos que el índice de parámetro sea válido
   assert(iParam < leerNumParametros());

   // Actualizar la matriz del nodo grafo de escena
   *rotationalmovement = rotate(float(2*M_PI*t_sec), vec3(0.0, 1.0, 0.0));
}