// *********************************************************************
// **
// ** Asignatura: INFORMÁTICA GRÁFICA
// **
// ** Mallas indexadas (implementación). Clase 'MallaInd' y derivadas.
// ** Copyright (C) 2016-2024 Carlos Ureña
// **
// ** Implementación de las clases
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

#include "ig-aux.h"
#include "aplicacion-ig.h"
#include "malla-ind.h" // declaración de 'ContextoVis'
#include "lector-ply.h"
#include "seleccion.h" // para 'ColorDesdeIdent'

// *****************************************************************************
// funciones auxiliares

// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd()
{
   // nombre por defecto
   ponerNombre("malla indexada, anónima");
}
// -----------------------------------------------------------------------------

MallaInd::MallaInd(const std::string &nombreIni)
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni);
}

//-----------------------------------------------------------------------------
// calcula la tabla de normales de triángulos una sola vez, si no estaba calculada

void MallaInd::calcularNormalesTriangulos()
{

   // si ya está creada la tabla de normales de triángulos, no es necesario volver a crearla
   const unsigned nt = triangulos.size();
   assert(1 <= nt);
   if (0 < nor_tri.size())
   {
      assert(nt == nor_tri.size());
      return;
   }

   // COMPLETAR: Práctica 4: creación de la tabla de normales de triángulos
   // ....
}

// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcularNormales()
{
   using namespace glm;
   // COMPLETAR: en la práctica 4: calculo de las normales de la malla
   // se debe invocar en primer lugar 'calcularNormalesTriangulos'
   // .......
}

// --------------------------------------------------------------------------------------------

void MallaInd::visualizarGL()
{
   // Comprobar algunas precondiciones básicas
   using namespace std;
   assert(aplicacionIG != nullptr);
   Cauce *cauce = aplicacionIG->cauce;
   assert(cauce != nullptr);
   CError();

   // Si la malla no tiene vértices o no tiene triángulos, imprimir advertencia y salir.
   if (triangulos.size() == 0 || vertices.size() == 0)
   {
      cout << "Advertencia: Intentando dibujar malla vacía '" << leerNombre() << "'" << endl << flush;
      return;
   }

   // Si el objeto tiene un color asignado
   if (tieneColor())
   {
      // Insertar el color actual en el cauce
      cauce->pushColor();
      // Fijar el color en el cauce usando el color del objeto
      cauce->fijarColor(leerColor());
   }

   // Crear el descriptor de VAO, si no está creado
   if (dvao == nullptr)
   {
      // Crear un descriptor de VAO
      dvao = new DescrVAO(numero_atributos_cauce, new DescrVBOAtribs(ind_atrib_posiciones, vertices));
      // Añadir descriptor de VBO con la tabla de índices
      dvao->agregar(new DescrVBOInds(triangulos));
      // Añadir al VAO los descriptores VBOs con tablas de atributos no vacías
      if (col_ver.size() > 0)
         dvao->agregar(new DescrVBOAtribs(ind_atrib_colores, col_ver));
      if (nor_ver.size() > 0)
         dvao->agregar(new DescrVBOAtribs(ind_atrib_normales, nor_ver));
      if (cc_tt_ver.size() > 0)
         dvao->agregar(new DescrVBOAtribs(ind_atrib_coord_text, cc_tt_ver));
   }

   // Visualizar el VAO usando el método 'draw' de 'DescrVAO'
   dvao->draw(GL_TRIANGLES);

   // Si el objeto tiene un color asignado
   if (tieneColor())
      // Eliminar el color actual del cauce
      cauce->popColor();
}

// -----------------------------------------------------------------------------
// Visualizar el objeto con OpenGL
// Usa las tablas de normales, colores y coordenadas de textura, si no están vacías.

void MallaInd::visualizarGeomGL()
{
   // Comprobar que el descriptor de VAO ya está creado
   // (es decir, este método únicamente se podrá invocar después de que
   // se haya llamado a 'visualizaGL')

   assert(dvao != nullptr);

   // Desactivar todas las tablas de atributos para los cuales la tabla no estén vacías
   if (col_ver.size() > 0)
      dvao->habilitarAtrib(ind_atrib_colores, GL_FALSE);
   if (nor_ver.size() > 0)
      dvao->habilitarAtrib(ind_atrib_normales, GL_FALSE);
   if (cc_tt_ver.size() > 0)
      dvao->habilitarAtrib(ind_atrib_coord_text, GL_FALSE);

   // Dibujar la malla
   dvao->draw(GL_TRIANGLES);

   // Volver a activar todos los atributos para los cuales la tabla no esté vacía
   if (col_ver.size() > 0)
      dvao->habilitarAtrib(ind_atrib_colores, GL_TRUE);
   if (nor_ver.size() > 0)
      dvao->habilitarAtrib(ind_atrib_normales, GL_TRUE);
   if (cc_tt_ver.size() > 0)
      dvao->habilitarAtrib(ind_atrib_coord_text, GL_TRUE);
}

// -----------------------------------------------------------------------------
// Visualizar las normales del objeto, si no tiene tabla de normales imprime
// advertencia y no hace nada.

void MallaInd::visualizarNormalesGL()
{
   using namespace std;
   assert(aplicacionIG != nullptr);
   Cauce *cauce = aplicacionIG->cauce;
   assert(cauce != nullptr);

   if (nor_ver.size() == 0)
   {
      cout << "Advertencia: intentando dibujar normales de una malla que no tiene tabla (" << leerNombre() << ")." << endl;
      return;
   }

   if (nor_ver.size() != vertices.size())
   {
      cout << "Error visu. normales: tabla de normales no vacía y de tamaño distinto a la de vértices." << endl;
      cout << "Nombre del objeto        : " << leerNombre() << endl;
      cout << "Tamaño tabla vértices    : " << vertices.size() << endl;
      cout << "Tamaño tabla de normales : " << nor_ver.size() << endl;
      exit(1);
   }
   CError();

   // COMPLETAR: práctica 4: visualizar las normales del objeto MallaInd
   //
   // *1* Si el puntero al descriptor de VAO de normales ('dvao_normales') es nulo,
   //    debemos de crear dicho descriptor, con estos pasos:
   //
   //       * Para cada posición 'v_i' de un vértice en el vector 'vertices':
   //             - Leer la correspondiente normal 'n_i' del vector de normales ('nor_ver').
   //             - Añadir 'v_i' al vector 'segmentos_normales'.
   //             - Añadir 'v_i+a*n_i' al vector 'segmentos_normales'.
   //
   //       * Crear el objeto descriptor del VAO de normales, para ello se usa el vector
   //          'segmentos_normales' y se tiene en cuenta que esa descriptor únicamente gestiona
   //          una tabla de atributos de vértices (la de posiciones, ya que las otras no se
   //          necesitan).
   //
   // *2* Visualizar el VAO de normales, usando el método 'draw' del descriptor, con el
   //       tipo de primitiva 'GL_LINES'.

   //  ..........
}

// -----------------------------------------------------------------------------
// visualizar el objeto en 'modo seleccion', es decir, sin iluminación y con los colores
// basados en los identificadores de los objetos
void MallaInd::visualizarModoSeleccionGL()
{

   using namespace std;
   assert(aplicacionIG != nullptr);
   Cauce *cauce = aplicacionIG->cauce;
   assert(cauce != nullptr);

   // COMPLETAR: práctica 5: visualizar la malla en modo selección
   //
   // Se debe escribir código para visualizar únicamente la geometría, pero usando el color
   // obtenido a partir del identificador. El código da estos pasos:
   //
   // 1. Leer el identificador del objeto (con 'leerIdentificador'). Si el objeto tiene
   //    identificador (es decir, si su identificador no es -1)
   //       + Hacer push del color del cauce, con 'pushColor'.
   //       + Fijar el color del cauce (con 'fijarColor') usando un color obtenido a
   //         partir del identificador (con 'ColorDesdeIdent').
   // 2. Invocar 'visualizarGeomGL' para visualizar la geometría.
   // 3. Si tiene identificador: hacer pop del color, con 'popColor'.
   //
}

// -----------------------------------------------------------------------------
// Clase MallaPLY

MallaPLY::MallaPLY(const std::string &nombre_arch)
{
   ponerNombre(std::string("Malla leída del archivo '") + nombre_arch + "'");
   LeerPLY(nombre_arch, vertices, triangulos);

   // COMPLETAR: práctica 4: invocar  a 'calcularNormales' para el cálculo de normales
   // .................
}

// -----------------------------------------------------------------------------
// Clase Cubo

Cubo::Cubo() : MallaInd("Cubo 8 vértices")
{
   vertices =
   {
      {-1.0, -1.0, -1.0}, // 0
      {-1.0, -1.0, +1.0}, // 1
      {-1.0, +1.0, -1.0}, // 2
      {-1.0, +1.0, +1.0}, // 3
      {+1.0, -1.0, -1.0}, // 4
      {+1.0, -1.0, +1.0}, // 5
      {+1.0, +1.0, -1.0}, // 6
      {+1.0, +1.0, +1.0}, // 7
   };
   triangulos =
   {
     {0, 1, 3}, 
     {0, 3, 2}, // X-
     {4, 7, 5},
     {4, 6, 7}, // X+ (+4)
     {0, 5, 1},
     {0, 4, 5}, // Y-
     {2, 3, 7},
     {2, 7, 6}, // Y+ (+2)
     {0, 6, 4},
     {0, 2, 6}, // Z-
     {1, 5, 7},
     {1, 7, 3} // Z+ (+1)
   };
}

Tetraedro::Tetraedro() : MallaInd("Tetraedro 4 vértices")
{
   this->ponerColor({0.75, 0.0, 0.75});
   vertices = 
   {
      {-1.0, -1.0, 0.0},// A
      {1.0, -1.0, 0.0}, // B
      {0.0, 1.0, 1.0},  // C
      {0.0, -1.0, -1.0}   // D
   };
   triangulos =
   {
      {0, 1, 2},
      {0, 1, 3},
      {1, 2, 3},
      {0, 2, 3}
   };
}

CuboColores::CuboColores() : MallaInd("Cubo 8 vértices colorido")
{
   vertices =
   {
      {-1.0, -1.0, -1.0}, // 0
      {-1.0, -1.0, +1.0}, // 1
      {-1.0, +1.0, -1.0}, // 2
      {-1.0, +1.0, +1.0}, // 3
      {+1.0, -1.0, -1.0}, // 4
      {+1.0, -1.0, +1.0}, // 5
      {+1.0, +1.0, -1.0}, // 6
      {+1.0, +1.0, +1.0}, // 7
   };
   triangulos =
   {
     {0, 1, 3}, 
     {0, 3, 2}, // X-
     {4, 7, 5},
     {4, 6, 7}, // X+ (+4)
     {0, 5, 1},
     {0, 4, 5}, // Y-
     {2, 3, 7},
     {2, 7, 6}, // Y+ (+2)
     {0, 6, 4},
     {0, 2, 6}, // Z-
     {1, 5, 7},
     {1, 7, 3} // Z+ (+1)
   };
   col_ver =
   {
      {0.0, 0.0, 0.0},
      {0.0, 0.0, +1.0},
      {0.0, +1.0, 0.0}, 
      {0.0, +1.0, +1.0}, 
      {+1.0, 0.0, 0.0}, 
      {+1.0, 0.0, +1.0}, 
      {+1.0, +1.0, 0.0}, 
      {+1.0, +1.0, +1.0}, 
   };
}

CasaX::CasaX() : MallaInd("Casa 10 vértices colorida")
{
   vertices =
   {
      {-1.0, -1.0, -1.0}, // 0
      {-1.0, -1.0, +1.0}, // 1
      {-1.0, +1.0, -1.0}, // 2
      {-1.0, +1.0, +1.0}, // 3
      {+1.0, -1.0, -1.0}, // 4
      {+1.0, -1.0, +1.0}, // 5
      {+1.0, +1.0, -1.0}, // 6
      {+1.0, +1.0, +1.0}, // 7
      {0.0, +2.0, +1.0},  // 8
      {0.0, +2.0, -1.0}   // 9
   };
   triangulos =
   {
     {0, 1, 3}, 
     {0, 3, 2},
     {4, 7, 5},
     {4, 6, 7},
     {0, 6, 4},
     {0, 2, 6},
     {1, 5, 7},
     {1, 7, 3},
     {3, 7, 8}, // Tejado delantero
     {6, 2, 9}, // Tejado trasero
     {3, 8, 9}, // Tejado izquierdo
     {3, 2, 9},
     {7, 8, 9}, // Tejado derecho
     {7, 6, 9}
   };
   col_ver =
   {
      {-1.0, -1.0, -1.0},
      {-1.0, -1.0, +1.0},
      {-1.0, +1.0, -1.0},
      {-1.0, +1.0, +1.0},
      {+1.0, -1.0, -1.0},
      {+1.0, -1.0, +1.0},
      {+1.0, +1.0, -1.0},
      {+1.0, +1.0, +1.0},
      {0.0, +2.0, +1.0}, 
      {0.0, +2.0, -1.0}  
   };
}

Triangulo::Triangulo() : MallaInd("Triángulo 3 vértices")
{
   vertices =
   {
      {-0.5, 0.0, 0.0},
      {+0.5, 0.0, 0.0},
      {0.0, sqrt(2), 0.0}
   };
   triangulos = 
   {
      {0, 1, 2}
   };
}

Cuadrado::Cuadrado() : MallaInd("Cuadrado 4 vértices")
{
   vertices = 
   {
      {-1.0, -1.0, 0.0},
      {+1.0, +1.0, 0.0},
      {-1.0, +1.0, 0.0},
      {+1.0, -1.0, 0.0}
   };
   triangulos =
   {
      {0, 1, 2},
      {0, 1, 3}
   };
}

// -----------------------------------------------------------------------------------------------
