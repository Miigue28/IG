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
// ** Gestión de escenas (declaraciones). Clase 'Escena' y derivadas.
// ** Copyright (C) 2016-2024 Carlos Ureña
// **
// ** Declaración de la clase 'Escena' y derivadas (una por cada práctica)
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

#ifndef ESCENA_H
#define ESCENA_H

#include <vector>
#include "aplicacion-ig.h"
// #include "objeto3d.h"
// #include "camara.h"

// ---------------------------------------------------------------------
// Clase para objetos visualizables de tipo escena: Incluye objetos, cámaras,
// un material inicial, una colección de fuentes de luz, etc...

class Escena
{
public:
   // Crea una escena con una cámara orbital simple, materiales y luces por defecto.
   Escena();

   // Visualiza el objeto actual de la escena
   void visualizarGL();

   // Visualiza el objeto actual de la escena, pero en modo selección
   void visualizarGL_Seleccion();

   // Pasa la cámara actual a la siguiente
   void siguienteCamara();

   // Devuelve un puntero a la cámara actual
   CamaraInteractiva *camaraActual();

   // Pasa el objeto actual al siguiente
   void siguienteObjeto();

   // Devuelve puntero al objeto actual
   Objeto3D *objetoActual();

   // Devuelve un puntero a la colección de fuentes actual
   ColFuentesLuz *colFuentes();

   // Visualiza las normales del objeto actual de la escena
   void visualizarNormales();

protected:
   // Vector de objetos (alternativos: se visualiza uno de ellos nada más)
   std::vector<Objeto3D*> objetos;

   // Vector de cámaras (se usa una en cada momento)
   std::vector<CamaraInteractiva*> camaras;

   // Material por defecto que se activa antes de dibujar (con iluminación activada)
   Material *material_ini = nullptr;

   // Colección de fuentes de luz que se usarán para dibujar, (con iluminación activada)
   ColFuentesLuz *col_fuentes = nullptr;

   // Índice de la cámara activa (en el vector 'camaras')
   unsigned ind_camara_actual = 0;

   // Índice del objeto activo (en el vector 'objetos')
   unsigned ind_objeto_actual = 0;
};

// -------------------------------------------------------------------------
// Clases derivadas de 'Escena'

class Escena1 : public Escena
{
public:
   Escena1();
};

class Escena2 : public Escena
{
public:
   Escena2();
};

class Escena3 : public Escena
{
public:
   Escena3();
};

class Escena4 : public Escena
{
public:
   Escena4();
};

class Escena5 : public Escena
{
public:
   Escena5();
};

#endif // ESCENA_H
