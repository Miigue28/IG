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
#include "modelo-jer.h"


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
         centro_acumulado = centro_acumulado + vec3(matriz_modelado * vec4(entrada.objeto->leerCentroOC(), 1.0f));
         contador_centros++;
      break;
      case TipoEntNGE::transformacion:
         matriz_modelado = matriz_modelado * *(entrada.matriz);
      break;
      default:
      break;
      }
   }

   // Hacemos el promedio de los centros
   //centro_acumulado /= static_cast<float>(contador_centros);

   for (int i = 0; i < 3; i++)
   {
      centro_acumulado[i] /= static_cast<float>(contador_centros);
   }

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
         matriz_modelado = matriz_modelado * *(entrada.matriz);
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

MallaDiscoP4::MallaDiscoP4()
{
   ponerColor({1.0f, 1.0f, 1.0f});
   const unsigned ni = 23, nj = 31;
   for (unsigned i = 0; i < ni; ++i)
   {
      for (unsigned j = 0; j < nj; ++j)
      {
         const float fi = static_cast<float>(i)/static_cast<float>(ni - 1.0f);
         const float fj = static_cast<float>(j)/static_cast<float>(nj - 1.0f);
         const float ai = 2.0f*M_PI*fi;
         const float x = fj * cos(ai);
         const float y = fj * sin(ai);
         const float z = 0.0;
         
         vertices.push_back({x, y, z}); // Ejercicio 1
         cc_tt_ver.push_back({0.5 + 0.5*x, 0.5 + 0.5*y});
         //cc_tt_ver.push_back({fj, fi}); // Ejercicio 2
      }
   }
   for (unsigned i = 0; i < ni - 1; ++i)
   {
      for (unsigned j = 0; j < nj - 1; ++j)
      {
         triangulos.push_back({i * nj + j, i * nj + (j + 1), (i + 1) * nj + (j + 1)});
         triangulos.push_back({i * nj + j, (i + 1) * nj + (j + 1), (i + 1)*nj + j});
      }
   }
}

NodoDiscoP4::NodoDiscoP4()
{
   ponerNombre("Nodo ejercicio adicional práctica 4, examen 27 enero 2021");
   agregar(new Material(new Textura("cuadricula.jpg"), 0.5, 0.4, 0.3, 40.0));
   agregar(new MallaDiscoP4());
}

Beethoven::Beethoven()
{
   ponerNombre("Beethoven");
   agregar(new Material(new Textura("text-madera.jpg"), 0.5, 0.4, 0.3, 40.0));
   agregar(new MallaPLY("beethoven.ply"));
}

EsferaGrafoP5::EsferaGrafoP5(unsigned i, unsigned j)
{
   this->i = i;
   this->j = j;
   ponerNombre("Esfera para el grafo de esferas del ejercicio 1 de la práctica 5");
   ponerIdentificador(i + j + 1);
   agregar(new Material(0.5, 0.4, 0.3, 40.0));
   agregar(new Sphere(30, 30));
}

bool EsferaGrafoP5::cuandoClick(const glm::vec3 &centro_wc)
{
   std::cout << "Se ha seleccionado la esfera número " << i+1 << " de la fila número " << j+1 << std::endl;
   return true;
}

GrafoEsferasP5::GrafoEsferasP5()
{
   const unsigned n_filas_esferas = 8;
   const unsigned n_esferas_x_fila = 5;
   const float e = 0.4/static_cast<float>(n_esferas_x_fila);

   ponerNombre("Grafo de esferas del ejercicio 1 de la práctica 5");
   agregar(glm::scale(glm::vec3(e, e, e)));

   for (unsigned i = 0; i < n_filas_esferas; ++i)
   {
      NodoGrafoEscena *fila_esferas = new NodoGrafoEscena();
      for (unsigned j = 0; j < n_esferas_x_fila; ++j)
      {
         EsferaGrafoP5 *esfera = new EsferaGrafoP5(i, j);
         esfera->ponerIdentificador(i*n_esferas_x_fila + j + 1);
         fila_esferas->agregar(glm::translate(glm::vec3(2.2, 0.0, 0.0)));
         fila_esferas->agregar(esfera);
      }
      agregar(fila_esferas);
      agregar(glm::translate(glm::vec3(0.0, 0.0, 5.0)));
   }
}

EsferaGrafoP5_2::EsferaGrafoP5_2()
{
   ponerNombre("Esfera para el grafo de esferas del ejercicio 2 de la práctica 5");
   agregar(new Material(0.5, 0.4, 0.3, 40.0));
   agregar(new Sphere(30, 30));
}

bool EsferaGrafoP5_2::cuandoClick(const glm::vec3 &centro_wc)
{
   if (leerColor() == glm::vec3(1.0f, 0.0f, 0.0f))
      ponerColor({1.0f, 1.0f, 1.0f});
   else
      ponerColor({1.0f, 0.0f, 0.0f});
   return true;
}

GrafoEsferasP5_2::GrafoEsferasP5_2()
{
   using namespace glm;
   const unsigned n_filas_esferas = 8;
   const unsigned n_esferas_x_fila = 5;
   const float e = 2.5/static_cast<float>(n_esferas_x_fila);

   ponerNombre("Grafo de esferas del ejercicio 2 de la práctica 5");
   agregar(glm::scale(glm::vec3(e, e, e)));

   for (unsigned i = 0; i < n_filas_esferas; ++i)
   {
      NodoGrafoEscena *fila_esferas = new NodoGrafoEscena();
      fila_esferas->agregar(glm::translate(glm::vec3(3.0, 0.0, 0.0)));
      for (unsigned j = 0; j < n_esferas_x_fila; ++j)
      {
         EsferaGrafoP5_2 *esfera = new EsferaGrafoP5_2();
         esfera->ponerColor({1.0f, 1.0f, 1.0f});
         esfera->ponerIdentificador(i*n_esferas_x_fila + j + 1);
         fila_esferas->agregar(glm::translate(glm::vec3(2.2, 0.0, 0.0)));
         fila_esferas->agregar(esfera);
      }
      agregar(fila_esferas);
      agregar(glm::rotate(glm::radians(360.0f/static_cast<float>(n_filas_esferas)), glm::vec3(0.0, 1.0, 0.0)));
   }
}

Piramide::Piramide() : MallaInd("Piramide con 16 vértices")
{
   vertices = 
   {
      // Cara frontal
      {-1.0, +0.0, +1.0},
      {+1.0, +0.0, +1.0},
      {+0.0, +2.0, +0.0},
      // Cara lateral derecha
      {+1.0, +0.0, +1.0},
      {+1.0, +0.0, -1.0},
      {+0.0, +2.0, +0.0},
      // Cara trasera
      {+1.0, +0.0, -1.0},
      {-1.0, +0.0, -1.0},
      {+0.0, +2.0, +0.0},
      // Cara lateral derecha
      {-1.0, +0.0, -1.0},
      {-1.0, +0.0, +1.0},
      {+0.0, +2.0, +0.0},
      // Base de la piramide
      {-1.0, +0.0, +1.0},
      {+1.0, +0.0, +1.0},
      {+1.0, +0.0, -1.0},
      {-1.0, +0.0, -1.0}
   };
   triangulos =
   {
      // Cara frontal
      {0, 1, 2},
      // Cara lateral derecha
      {3, 4, 5},
      // Cara trasera
      {6, 7, 8},
      // Cara lateral izquierda
      {9, 10, 11},
      // Base de la piramide
      {12, 13, 14},
      {12, 14, 15}
   };
   calcularNormales();
   cc_tt_ver = 
   {
      // Cara frontal
      {0.0, 1.0},
      {1.0, 1.0},
      {0.5, 0.0},
      // Cara lateral derecha
      {0.0, 1.0},
      {1.0, 1.0},
      {0.5, 0.0},
      // Cara trasera
      {0.0, 1.0},
      {1.0, 1.0},
      {0.5, 0.0},
      // Cara lateral izquierda
      {0.0, 1.0},
      {1.0, 1.0},
      {0.5, 0.0},
      // Base de la piramide
      {0.0, 0.0},
      {1.0, 0.0},
      {1.0, 1.0},
      {0.0, 1.0}
   };
}

NodoPiramideUGR::NodoPiramideUGR()
{
   ponerNombre("Piramide de la UGR");
   agregar(new Material(new Textura("window-icon.jpg"), 0.7, 0.5, 0.0, 30.0));
   agregar(new Piramide());
}

Triangle::Triangle(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c)
{
   vertices = {a, b, c};
   triangulos = {{0, 1, 2}};
   calcularNormales();
   ponerCentroOC((a + b + c)/3.0f);
   ponerNombre("Trianglulo" + std::to_string(leerIdentificador()));
   ponerColor({1.0f, 1.0f, 1.0f});
}

bool Triangle::cuandoClick(const glm::vec3 &centro_wc)
{
   MallaInd::cuandoClick(centro_wc);
   if (leerColor() == glm::vec3(1.0f, 0.0f, 0.0f))
      ponerColor({1.0f, 1.0f, 1.0f});
   else
      ponerColor({1.0f, 0.0f, 0.0f});
   return true;
}

NodoPiramideUGR_2::NodoPiramideUGR_2()
{
   Triangle *frontal = new Triangle({-1.0, 0.0, 1.0}, {1.0, 0.0, 1.0}, {0.0, 2.0, 0.0});
   frontal->ponerIdentificador(1);
   Triangle *derecho = new Triangle({1.0, 0.0, 1.0}, {1.0, 0.0, -1.0}, {0.0, 2.0, 0.0});
   derecho->ponerIdentificador(2);
   Triangle *trasero = new Triangle({1.0, 0.0, -1.0}, {-1.0, 0.0, -1.0}, {0.0, 2.0, 0.0});
   trasero->ponerIdentificador(3);
   Triangle *izquierdo = new Triangle({-1.0, 0.0, -1.0}, {-1.0, 0.0, 1.0}, {0.0, 2.0, 0.0});
   izquierdo->ponerIdentificador(4);
   Triangle *base_1 = new Triangle({-1.0, 0.0, 1.0}, {1.0, 0.0, 1.0}, {1.0, 0.0, -1.0});
   base_1->ponerIdentificador(5);
   Triangle *base_2 = new Triangle({-1.0, 0.0, 1.0}, {1.0, 0.0, -1.0}, {-1.0, 0.0, -1.0});
   base_2->ponerIdentificador(6);
   
   ponerNombre("Piramide de la UGR sin textura");
   agregar(frontal);
   agregar(derecho);
   agregar(trasero);
   agregar(izquierdo);
   agregar(base_1);
   agregar(base_2);
}

ExtremidadAndroid::ExtremidadAndroid()
{
   NodoGrafoEscena *semiesfera_superior = new NodoGrafoEscena();
   semiesfera_superior->agregar(glm::translate(glm::vec3(0.0, 3.0, 0.0)));
   semiesfera_superior->agregar(new SemiSphere(30, 30));

   NodoGrafoEscena *tronco = new NodoGrafoEscena();
   tronco->agregar(glm::scale(glm::vec3(1.0, 3.0, 1.0)));
   tronco->agregar(new Cilinder(30, 30));

   NodoGrafoEscena *semiesfera_inferior = new NodoGrafoEscena();
   semiesfera_inferior->agregar(glm::rotate(glm::pi<float>(), glm::vec3(0.0, 0.0, 1.0)));
   semiesfera_inferior->agregar(new SemiSphere(30, 30));

   agregar(semiesfera_superior);
   agregar(tronco);
   agregar(semiesfera_inferior);
   ponerColor({0.0f, 1.0f, 0.0f});
}

CabezaAndroid::CabezaAndroid()
{
   NodoGrafoEscena *ojo_izquierdo = new NodoGrafoEscena();
   ojo_izquierdo->agregar(glm::translate(glm::vec3(-0.35, 0.5, 0.75)));
   ojo_izquierdo->agregar(glm::scale(glm::vec3(0.1, 0.1, 0.1)));
   ojo_izquierdo->agregar(new Sphere(30, 30));
   ojo_izquierdo->ponerColor({0.0f, 0.0f, 0.0f});

   NodoGrafoEscena *ojo_derecho = new NodoGrafoEscena();
   ojo_derecho->agregar(glm::translate(glm::vec3(0.35, 0.5, 0.75)));
   ojo_derecho->agregar(glm::scale(glm::vec3(0.1, 0.1, 0.1)));
   ojo_derecho->agregar(new Sphere(30, 30));
   ojo_derecho->ponerColor({0.0f, 0.0f, 0.0f});

   NodoGrafoEscena *cabeza = new NodoGrafoEscena();
   cabeza->agregar(new SemiSphere(30, 30));
   cabeza->ponerColor({0.0f, 1.0f, 0.0f});

   NodoGrafoEscena *antena_derecha = new NodoGrafoEscena();
   antena_derecha->agregar(glm::translate(glm::vec3(0.35, 0.95, 0.0)));
   antena_derecha->agregar(glm::rotate(glm::pi<float>()/-6.0f, glm::vec3(0.0, 0.0, 1.0)));
   antena_derecha->agregar(glm::scale(glm::vec3(0.05, 0.5, 0.05)));
   antena_derecha->agregar(new Cilinder(30, 30));
   antena_derecha->ponerColor({0.0f, 1.0f, 0.0f});

   NodoGrafoEscena *antena_izquierda = new NodoGrafoEscena();
   antena_izquierda->agregar(glm::translate(glm::vec3(-0.35, 0.95, 0.0)));
   antena_izquierda->agregar(glm::rotate(glm::pi<float>()/6.0f, glm::vec3(0.0, 0.0, 1.0)));
   antena_izquierda->agregar(glm::scale(glm::vec3(0.05, 0.5, 0.05)));

   antena_izquierda->agregar(new Cilinder(30, 30));
   antena_izquierda->ponerColor({0.0f, 1.0f, 0.0f});

   agregar(ojo_izquierdo);
   agregar(ojo_derecho);
   agregar(cabeza);
   agregar(antena_derecha);
   agregar(antena_izquierda);
}

Android::Android(const float alpha, const float beta, const float gamma)
{
   NodoGrafoEscena *brazo_izquierdo = new NodoGrafoEscena();
   brazo_izquierdo->agregar(glm::translate(glm::vec3(-1.45, 1.0, 0.0)));
   unsigned int id_brazo_izq = brazo_izquierdo->agregar(glm::rotate(alpha, glm::vec3(1.0, 0.0, 0.0)));
   brazo_izquierdo->agregar(glm::scale(glm::vec3(-0.35, -0.35, 0.35)));
   brazo_izquierdo->agregar(new ExtremidadAndroid());

   rotacion_brazo_izquierdo = brazo_izquierdo->leerPtrMatriz(id_brazo_izq);

   NodoGrafoEscena *brazo_derecho = new NodoGrafoEscena();
   brazo_derecho->agregar(glm::translate(glm::vec3(1.45, 1.0, 0.0)));
   unsigned int id_brazo_drch = brazo_derecho->agregar(glm::rotate(beta, glm::vec3(1.0, 0.0, 0.0)));
   brazo_derecho->agregar(glm::scale(glm::vec3(-0.35, -0.35, 0.35)));
   brazo_derecho->agregar(new ExtremidadAndroid());

   rotacion_brazo_derecho = brazo_derecho->leerPtrMatriz(id_brazo_drch);

   NodoGrafoEscena *tronco = new NodoGrafoEscena();
   tronco->agregar(glm::scale(glm::vec3(1.0, 2.0, 1.0)));
   tronco->ponerColor({0.0, 1.0, 0.0});
   tronco->agregar(new Cilinder(30, 30));

   NodoGrafoEscena *cabeza = new NodoGrafoEscena();
   cabeza->agregar(glm::translate(glm::vec3(0.0, 2.1, 0.0)));
   unsigned id_cabeza = cabeza->agregar(glm::rotate(gamma, glm::vec3(0.0, 1.0, 0.0)));
   cabeza->ponerColor({0.0, 1.0, 0.0});
   cabeza->agregar(new CabezaAndroid());

   rotacion_cabeza = cabeza->leerPtrMatriz(id_cabeza);

   NodoGrafoEscena *pierna_izquierda = new NodoGrafoEscena();
   pierna_izquierda->agregar(glm::translate(glm::vec3(-0.5, -1.0, 0.0)));
   pierna_izquierda->agregar(glm::scale(glm::vec3(0.35, 0.35, 0.35)));
   pierna_izquierda->agregar(new ExtremidadAndroid());

   NodoGrafoEscena *pierna_derecha = new NodoGrafoEscena();
   pierna_derecha->agregar(glm::translate(glm::vec3(0.5, -1.0, 0.0)));
   pierna_derecha->agregar(glm::scale(glm::vec3(0.35, 0.35, 0.35)));
   pierna_derecha->agregar(new ExtremidadAndroid());

   agregar(brazo_izquierdo);
   agregar(brazo_derecho);
   agregar(tronco);
   agregar(cabeza);
   agregar(pierna_izquierda);
   agregar(pierna_derecha);
}

unsigned Android::leerNumParametros() const
{
   return 3;
}

void Android::actualizarEstadoParametro(const unsigned iParam, const float t_sec)
{
   assert(iParam < leerNumParametros());

   float min = -1.0f, max = 1.0f;
   float a = (min + max)/2, b = (max - min)/2; 
   float n = 0.25;

   switch (iParam)
   {
   case 0:
      *rotacion_brazo_izquierdo = glm::rotate(static_cast<float>(a + b*sin(2*M_PI*n*t_sec)), glm::vec3(1.0, 0.0, 0.0));
   break;
   case 1:
      *rotacion_brazo_derecho = glm::rotate(static_cast<float>(a + b*sin(2*M_PI*n*t_sec)), glm::vec3(1.0, 0.0, 0.0));
   break;
   case 2:
      *rotacion_cabeza = glm::rotate(static_cast<float>(a + b*sin(2*M_PI*n*t_sec)), glm::vec3(0.0, 1.0, 0.0));
   break;
   }
}