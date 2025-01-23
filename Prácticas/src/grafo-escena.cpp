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

   // Si la iluminación está activada, gestionamos los materiales
   if (aplicacionIG->iluminacion)
   {
      // Guardamos el material actual en la pila
      pila_materiales->push();
   }

   // Guardar copia de la matriz de modelado
   cauce->pushMM();
   
   // Para cada entrada del vector de entradas
   for  (EntradaNGE entrada : entradas)
   {
      switch (entrada.tipo)
      {
         // Si la entrada es de tipo objeto: llamar recursivamente a 'visualizarGL'
         case TipoEntNGE::objeto: 
            entrada.objeto->visualizarGL(); 
         break;
         // Si la entrada es de tipo transformación: componer la matriz
         case TipoEntNGE::transformacion: 
            cauce->compMM(*(entrada.matriz)); 
         break;
         // Si la entrada es de tipo material: activar el material
         case TipoEntNGE::material: 
            //if (aplicacionIG->iluminacion)
            pila_materiales->activar(entrada.material); 
         break;
         case TipoEntNGE::noInicializado:
            std::cout << "Entrada no inicializada" << std::endl;
         break;
      }
   }
   // Restaurar la copia guardada de la matriz de modelado
   cauce->popMM();

   // Restaurar el color original a la entrada
   if (tieneColor())
   {
      cauce->popColor();
   }
      
   // Restaurar el material activo al inicio
   if (aplicacionIG->iluminacion)
   {
      pila_materiales->pop();
   }
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
         // Ignoramos el resto
         case TipoEntNGE::material: break;
         case TipoEntNGE::noInicializado: break;
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
   Cauce *cauce = aplicacionIG->cauce;
   assert(cauce != nullptr);

   cauce->pushMM();

   // Recorrer las entradas, llamando recursivamente a 'visualizarNormalesGL' en los objetos
   for (EntradaNGE entrada : entradas)
   {
      switch (entrada.tipo)
      {
         case TipoEntNGE::objeto: 
            (entrada.objeto)->visualizarNormalesGL(); 
         break;
         case TipoEntNGE::transformacion: 
            cauce->compMM(*(entrada.matriz)); 
         break;
         // Ignoramos el resto
         case TipoEntNGE::material: break;
         case TipoEntNGE::noInicializado: break;
      }
   }

   cauce->popMM();
}

// Visualizar el objeto sin iluminación y con los colores basados en los identificadores de los objetos
void NodoGrafoEscena::visualizarModoSeleccionGL()
{
   using namespace std;
   assert(aplicacionIG != nullptr);
   Cauce *cauce = aplicacionIG->cauce;
   assert(cauce != nullptr);

   int id = leerIdentificador();

   // Si el nodo tiene identificador
   if (id != -1)
   {
      // Guardar una copia del color actual del cauce
      cauce->pushColor();

      // Fijar el color del cauce de acuerdo al identificador
      cauce->fijarColor(ColorDesdeIdent(id));
   }

   // Guardar una copia de la matriz de modelado
   cauce->pushMM();

   // Recorrer la lista de nodos y procesar las entradas
   for (auto & entrada : entradas)
   {
      switch (entrada.tipo)
      {
      case TipoEntNGE::objeto:
         entrada.objeto->visualizarModoSeleccionGL();
      break;
      case TipoEntNGE::transformacion:
         cauce->compMM(*(entrada.matriz));
      break;
      default:
      break;
      }
   }

   // Restaurar la matriz de modelado original
   cauce->popMM();

   if (id != -1)
   {
      // Restaurar el color previo del cauce
      cauce->popColor();
   }
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

// Calcula (solo una vez) el centro promediando los centros de los hijos
void NodoGrafoEscena::calcularCentroOC()
{
   using namespace std;
   using namespace glm;

   // Si el centro ya ha sido calculado, no volver a hacerlo
   if (centro_calculado)
      return;

   int contador_centros = 0;
   mat4 matriz_modelado = mat4(1.0f);
   vec3 centro_acumulado = {0.0, 0.0, 0.0};

   // Recorremos recursivamente todas las entradas
   for (auto & entrada : entradas)
   {
      switch (entrada.tipo)
      {
      case TipoEntNGE::objeto:
         // Calculamos el centro de cada subobjeto
         entrada.objeto->calcularCentroOC();
         centro_acumulado += vec3(matriz_modelado * vec4(entrada.objeto->leerCentroOC(), 1.0f));
         contador_centros++;
      break;
      case TipoEntNGE::transformacion:
         matriz_modelado *= *(entrada.matriz);
      break;
      default:
      break;
      }
   }

   // Hacemos el promedio de los centros
   centro_acumulado /= static_cast<float>(contador_centros);

   // Poner centro y apuntar que ya se ha calculado
   ponerCentroOC(centro_acumulado);
   centro_calculado = true;
}

// Método para buscar un objeto con un identificador y devolver un puntero al mismo
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

   // Calcula el centro del objeto
   calcularCentroOC();

   // Si el identificador del nodo es el que se busca, terminar
   if (ident_busc == leerIdentificador())
   {
      *objeto = this;
      centro_wc = leerCentroOC();
      return true;
   }

   mat4 matriz_modelado = mmodelado;

   // Si el nodo no es el buscado, buscar recursivamente en los hijos
   for (auto & entrada : entradas)
   {
      switch (entrada.tipo)
      {
      case TipoEntNGE::objeto:
         if (entrada.objeto->buscarObjeto(ident_busc, matriz_modelado, objeto, centro_wc))
            return true;
      break;
      case TipoEntNGE::transformacion:
         matriz_modelado *= *(entrada.matriz);
      break;
      default:
      break;
      }
   }

   // Si ni este nodo ni ningún hijo es el buscado, terminar
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

CubePair::CubePair(float h, float w)
{
   using namespace glm;

   NodoGrafoEscena *base = new NodoGrafoEscena();
   base->agregar(scale(vec3(w, h, w)));
   base->agregar(new Cubo());

   NodoGrafoEscena *cubo_rotatorio = new NodoGrafoEscena();
   unsigned visagraindex = cubo_rotatorio->agregar(rotate(0.0f, vec3(0.0, 0.0, 1.0)));
   cubo_rotatorio->agregar(translate(vec3(w, h, 0.0)));
   cubo_rotatorio->agregar(base);

   rotatingmovement = cubo_rotatorio->leerPtrMatriz(visagraindex);

   NodoGrafoEscena *cubo_escalable = new NodoGrafoEscena();
   unsigned baseindex = cubo_escalable->agregar(scale(vec3(1.0, 1.0, 1.0)));
   cubo_escalable->agregar(translate(vec3(-w, -h, 0.0)));
   cubo_escalable->agregar(base);

   scalingmovement = cubo_escalable->leerPtrMatriz(baseindex);

   agregar(translate(vec3(2*w, 2*h, w)));
   agregar(cubo_rotatorio);
   agregar(cubo_escalable);
}

unsigned CubePair::leerNumParametros() const
{
   // Dos grados de libertad
   return 2;
}

void CubePair::actualizarEstadoParametro(const unsigned iParam, const float t_sec)
{
   using namespace glm;

   // Verificamos que el índice de parámetro sea válido
   assert(iParam < leerNumParametros());

   float min = -M_PI/2.0, max = M_PI/4.0;
   float a = (min + max)/2, b = (max - min)/2;

   // Actualizar las matrices del nodo grafo de escena
   switch (iParam)
   {
   case 0:
      *rotatingmovement = rotate(static_cast<float>(a + b*sin(2*M_PI*t_sec)), vec3(0.0, 0.0, 1.0));
   break;
   case 1:
      *scalingmovement = scale(vec3(1.0, 1.0*abs(cos(t_sec)), 1.0));
   break;
   }
}

NodoCubo24::NodoCubo24()
{
   agregar(new Material(new Textura("window-icon.jpg"), 0.5, 0.4, 0.3, 40.0));
   agregar(new Cubo24());
}