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
// ** Gestión de escenas (implementaciones). Clase 'Escena' y derivadas.
// ** Copyright (C) 2016-2024 Carlos Ureña
// **
// ** Implementación de la clase 'Escena' y derivadas (una por cada práctica)
// **
// **  + Escena: clase con una colección de objetos para visualizar, las fuentes de luz
// **    y una cámara. En cada momento se visualiza uno de los objetos (se conmuta con 'o')
// **    Las clases derivadas incluyen cada una un constructor específico que crea el objeto,
// **    son estas:
// **
// **       + Escena1 : práctica 1
// **       + Escena2 : práctica 2
// **       + Escena3 : práctica 3
// **       + Escena4 : práctica 4
// **       + Escena5 : práctica 5
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

#include "objeto3d.h"
#include "malla-ind.h"
#include "malla-revol.h"
#include "camara.h"
#include "materiales-luces.h"
#include "escena.h"
#include "modelo-jer.h"
#include "examen-ec-p123.h"
#include "latapeones.h"
#include "examen-extr.h"

Escena::Escena()
{
   using namespace glm;

   // Iniciar la colección de fuentes y el material inicial
   col_fuentes = new Col2Fuentes();
   //material_ini = new Material();
   material_ini = new Material(0.4f, 0.8f, 0.0f, 2.0f);

   //camaras.push_back(new CamaraOrbitalSimple());
   camaras.push_back(new Camara3Modos(true, *(new vec3({2.0, 2.0, 2.0})), 1.0, *(new vec3({0.0, 0.0, 0.0})), 60.0));
   camaras.push_back(new Camara3Modos(false, *(new vec3({5.0, 5.0, 5.0})), 1.0, *(new vec3({0.0, 0.0, 0.0})), 60.0));
   camaras.push_back(new Camara3Modos(false, *(new vec3({-2.5, 2.5, 2.5})), 1.0, *(new vec3({0.0, 0.0, 0.0})), 50.0));
   camaras.push_back(new Camara3Modos(true, *(new vec3({5.0, -2.5, 5.0})), 1.0, *(new vec3({0.5, 3.0, 0.0})), 70.0));
}

// Visualiza la escena en la ventana actual, usando la configuración especificada en el cauce gráfico
void Escena::visualizarGL()
{
   assert(aplicacionIG != nullptr);
   assert(aplicacionIG->cauce != nullptr);

   using namespace std;
   CError();

   // Recuperar el cauce
   Cauce *cauce = aplicacionIG->cauce;

   // Desactivar el modo de selección, por si acaso
   aplicacionIG->modo_seleccion = false;

   // Activar el cauce
   cauce->activar();
   CError();

   // Recuperar la cámara actual de esta escena y:
   //   (1) Fijar la camara actual del contexto de visualización
   //   (2) Fijar las matrices 'modelview' y 'projection' en el
   //       cauce gráfico (es decir: activar la cámara actual)
   CamaraInteractiva *camara = camaras[ind_camara_actual];
   assert(camara != nullptr);

   const float ratio_vp = float(aplicacionIG->ventana_tam_y) / float(aplicacionIG->ventana_tam_x);

   // cout << "Escena::visualizarGL: dimen " << apl->ventana_tam_x << " x " << apl->ventana_tam_y << ", y/x == " << ratio_vp << endl ;

   camara->fijarRatioViewport(ratio_vp);
   // cout << endl << __FUNCTION__ << ": índice cámara actual == " << ind_camara_actual << endl ;
   camara->activar(*cauce);
   CError();

   // Dibujar los ejes, si procede
   if (aplicacionIG->dibujar_ejes)
      DibujarEjesSolido(*cauce); // ver 'ig-aux.cpp' para la definición.

   // Fijar el color por defecto en el cauce para dibujar los objetos
   // (es blanco debido a que el fondo se rellena con un color oscuro y debe contrastar).
   cauce->fijarColor(1.0, 1.0, 1.0);

   // Fijar el modo de normales (útil para la práctica 4)
   cauce->fijarUsarNormalesTri(aplicacionIG->usar_normales_tri);

   // Configurar el modo de visualización de polígonos
   if (aplicacionIG->modo_visu == ModosVisu::relleno)
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   if (aplicacionIG->modo_visu == ModosVisu::lineas)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   if (aplicacionIG->modo_visu == ModosVisu::puntos)
      glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

   CError();

   if (aplicacionIG->iluminacion)
   {
      // Habilitar evaluación del MIL en el cauce
      cauce->fijarEvalMIL(true);
      // Activar la colección de fuentes de la escena
      col_fuentes->activar();
      // Activar el material inicial (usando pila_materiales)
      (aplicacionIG->pila_materiales)->activar(material_ini);
   }
   else
   {
      // Deshabilitamos el MIL y las texturas
      cauce->fijarEvalMIL(false);
      cauce->fijarEvalText(false);
   }

   // log("recupero objeto");
   CError();

   // recuperar el objeto actual de esta escena
   Objeto3D *objeto = objetos[ind_objeto_actual];
   assert(objeto != nullptr);

   // Visualizar el objeto actual
   objeto->visualizarGL();

   // Visualizar las aristas del objeto, si procede (es decir: en modo relleno, con visualización de aristas activada)
   if (aplicacionIG->dibujar_aristas && aplicacionIG->modo_visu == ModosVisu::relleno)
   {
      // Desactivar iluminación y texturas (podrían estarlo a partir de prác. 4)
      cauce->fijarEvalMIL(false);
      cauce->fijarEvalText(false);

      // Fijar el color a negro
      cauce->fijarColor(0.0, 0.0, 0.0);

      // Fijar el modo de polígonos a modo 'lineas'
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

      // Visualizar únicamente las aristas del objeto actual
      objeto->visualizarGeomGL();
   }
}

// Visualiza el objeto actual de la cámara, pero en modo selección
void Escena::visualizarGL_Seleccion()
{
   assert(aplicacionIG != nullptr);
   Cauce *cauce = aplicacionIG->cauce;
   assert(cauce != nullptr);
   CError();

   // Fijar el viewport usando el tamaño de la ventana
   glViewport(0, 0, aplicacionIG->ventana_tam_x, aplicacionIG->ventana_tam_y);

   // Fijar el modo de polígonos a relleno
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   // Activar y configurar el cauce desactivando la iluminación y las texturas
   cauce->activar();
   cauce->fijarEvalMIL(false);
   cauce->fijarEvalText(false);

   // Poner el color actual del cauce a 0 (por defecto los objetos no son seleccionables)
   cauce->fijarColor(glm::vec4(0.0, 0.0, 0.0, 1.0));

   // Limpiar el framebuffer con color (0,0,0) para indicar que en ningún pixel hay nada seleccionable
   glClearColor(0, 0, 0, 1);
   glClear(GL_DEPTH_BUFFER_BIT);


   // Recuperar la cámara actual y activarla en el cauce
   camaraActual()->activar(*cauce);

   // Recuperar el objeto raiz actual de la escena y visualizarlo en modo seleccion
   objetoActual()->visualizarModoSeleccionGL();
}

// Visualiza las normales del objeto actual de la escena
void Escena::visualizarNormales()
{
   assert(aplicacionIG != nullptr);
   Cauce *cauce = aplicacionIG->cauce;
   assert(cauce != nullptr);

   // Desactivar la iluminación
   cauce->fijarEvalMIL(false);

   // Desactivar el uso de texturas
   cauce->fijarEvalText(false);

   // Fijar el color
   cauce->fijarColor(1.0, 1.0, 1.0);
   
   // Visualizar las normales del objeto actual de la escena
   objetoActual()->visualizarNormalesGL();

}

// Pasa la cámara actual a la siguiente
void Escena::siguienteCamara()
{
   assert(ind_camara_actual < camaras.size());
   ind_camara_actual = (ind_camara_actual + 1) % camaras.size();
   using namespace std;
   cout << "Cámara actual cambiada a: " << (ind_camara_actual + 1) << "/" << camaras.size() << ": " << camaras[ind_camara_actual]->descripcion() << endl;
}

// Pasa el objeto actual al siguiente
void Escena::siguienteObjeto()
{
   if (objetos.size() == 0)
      return;
   assert(ind_objeto_actual < objetos.size());
   ind_objeto_actual = (ind_objeto_actual + 1) % objetos.size();
   using namespace std;
   cout << "Objeto actual cambiado a: " << objetoActual()->leerNombre()
        << " (" << (ind_objeto_actual + 1) << "/" << objetos.size() << ")." << endl;
}

// Devuelve puntero al objeto actual
Objeto3D *Escena::objetoActual()
{
   assert(ind_objeto_actual < objetos.size());
   assert(objetos[ind_objeto_actual] != nullptr);
   return objetos[ind_objeto_actual];
}

// Devuelve un puntero a la cámara actual
CamaraInteractiva *Escena::camaraActual()
{
   assert(ind_camara_actual < camaras.size());
   assert(camaras[ind_camara_actual] != nullptr);
   return camaras[ind_camara_actual];
}

// Devuelve un puntero a la colección de fuentes actual
ColFuentesLuz *Escena::colFuentes()
{
   assert(col_fuentes != nullptr);
   return col_fuentes;
}

// -----------------------------------------------------------------------------------------------

constexpr bool lunes = false;

Escena1::Escena1()
{
   std::cout << "Creando objetos de la Práctica 1" << std::endl;

   //objetos.push_back(new Cubo());
   //objetos.push_back(new Tetraedro());
   //objetos.push_back(new CuboColores());
   //objetos.push_back(new CasaX());
   //objetos.push_back(new Triangulo());
   //objetos.push_back(new Cuadrado());
   //objetos.push_back(new PiramideL());
   //objetos.push_back(new EstrellaZ(8));
   //objetos.push_back(new Ejercicio01());
   //objetos.push_back(new NodoPiramideUGR());
   //objetos.push_back(new NodoPiramideUGR_2());
   objetos.push_back(new EjercicioExtr01());
}

Escena2::Escena2()
{
   std::cout << "Creando objectos de la Práctica 2" << std::endl;

   //objetos.push_back(new MallaPLY("beethoven.ply"));
   //objetos.push_back(new MallaPLY("big_dodge.ply"));
   //objetos.push_back(new MallaRevolPLY("peon.ply", 25));
   //objetos.push_back(new Cilinder(50, 50));
   //objetos.push_back(new Cone(50, 50));
   //objetos.push_back(new Sphere(50, 50));
   //objetos.push_back(new PiramideEstrellaZ(8));
   //objetos.push_back(new RejillaY(3, 3));
   //objetos.push_back(new MallaTorre(3));
   //objetos.push_back(new Ejercicio02(8));
   objetos.push_back(new EjercicioExtr02());
}

Escena3::Escena3()
{
   std::cout << "Creando objetos de la Práctica 3" << std::endl;

   //objetos.push_back(new GrafoEstrellaX(6));
   //objetos.push_back(new GrafoCubos());
   //objetos.push_back(new CubePair(0.5f, 0.25f));
   //objetos.push_back(new Trombone());
   //objetos.push_back(new Android(0.0f, 0.0f, 0.0f));
   //objetos.push_back(new Ejercicio03(2, 0.2));
   //objetos.push_back(new KeyCap(1, 1));
   objetos.push_back(new EjercicioExtr03(3, 3));
}

Escena4::Escena4()
{
   std::cout << "Creando objetos de la Práctica 4" << std::endl;

   //objetos.push_back(new NodoDiscoP4());
   //objetos.push_back(new Beethoven());
   //objetos.push_back(new NodoCubo24());
   //objetos.push_back(new LataPeones());
   objetos.push_back(new EjercicioExtr04(3, 3));
}

Escena5::Escena5()
{
   std::cout << "Creando objetos de la Práctica 5" << std::endl;

   //objetos.push_back(new GrafoEsferasP5());
   //objetos.push_back(new GrafoEsferasP5_2());
   //objetos.push_back(new VariasLatasPeones());
   objetos.push_back(new EjercicioExtr05(3, 3));
}