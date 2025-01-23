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
// ** Gestión de cámaras (declaraciones)
// ** Copyright (C) 2016-2024 Carlos Ureña
// **
// ** Declaraciones de las clases 'Viewport', 'Camara' y derivadas de 'Camara'
// **
// **   Camara: clase base
// **      + CamaraInteractiva: cámaras editables
// **          + CamaraOrbitalSimple: cámara orbital usada en las
// **            prácticas (implementada al 100%)
// **          + Camara3Modos: cámara con tres modos de funcionamiento (práctica 5)
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

#ifndef CAMARA_HPP
#define CAMARA_HPP

// #include "tup_mat.h"
#include "cauce.h"

// Clase: Viewport
class Viewport
{
public:
   // Origen en pixels (esquina inferior izquierda)
   int org_x, org_y;
   // Dimensiones en pixels (núm. columnas, núm. filas)
   int ancho, alto; 
   // Relación de aspecto   
   float ratio_yx;
   // Matriz de viewport (transforma NDC en DC)
   glm::mat4 matrizVp;
   // Matriz inversa (transforma DC en NDC)
   glm::mat4 matrizVpInv;
   // Crea viewport de 512 x 512 con origen en (0,0)
   Viewport();
   Viewport(int p_org_x, int p_org_y, int p_ancho, int p_alto);
};

// Clase base para cámaras
class Camara
{
public:
   // Fija las matrices model-view y projection en el cauce
   void activar(Cauce &cauce);

   // Cambio el valor de 'ratio_vp' (alto/ancho del viewport)
   void fijarRatioViewport(const float nuevo_ratio);

   // Lee la descripción de la cámara (y probablemente su estado)
   virtual std::string descripcion();

protected:
   bool matrices_actualizadas = false;
   // Matriz de vista
   glm::mat4 matriz_vista = glm::mat4(1.0);
   // Matriz de proyección
   glm::mat4 matriz_proye = glm::mat4(1);
   // Alto del viewport dividido por el ancho del viewport
   float ratio_vp = 1.0;

   // Actualiza 'matriz_vista' y 'matriz_proye' a partir de los parámetros
   // específicos de cada tipo de cámara
   virtual void actualizarMatrices();
};

// Clase (abstracta) para cámaras interactivas (cámaras manipulables)
class CamaraInteractiva : public Camara
{
public:
   // Desplazar o rotar la cámara en horizontal 'da' unidades y en vertical 'db' unidades
   // (en horizontal y en vertical aquí se entiende relativo al marco de camara)
   virtual void desplRotarXY(const float da, const float db) = 0;

   // Acercar/alejar o desplazar la camara en el eje Z un total de 'dz' unidades
   // (el eje es el eje Z, relativo al marco de la camara, es decir, perpendicular al plano de visión)
   virtual void moverZ(const float dz) = 0;

   // Hacer que apunte hacía el punto de atención y cambiar de modo a modo examinar
   // Por defecto imprime mensaje de advertencia de que la cámra no ofrece esta funcionalidad
   virtual void mirarHacia(const glm::vec3 &paten);

   // Cambiar el modo de la camara al siguiente modo o al primero
   // Por defecto imprime mensaje de advertencia de que la cámra no ofrece esta funcionalidad
   virtual void siguienteModo();

   virtual std::string descripcion()
   {
      return "Cámara interactiva";
   }
};

// Clase para una cámara orbital sencilla que siempre tiene el origen de
// coordenadas del mundo en el centro de la imagen
class CamaraOrbitalSimple : public CamaraInteractiva
{
public:
   // Cambia la longitud usando 'da' y la latitud usando 'db'
   virtual void desplRotarXY(const float da, const float db);

   // Acercar/alejar 'dz' unidades al origen (sin 'atravesarlo' nunca)
   virtual void moverZ(const float dz);

   // Lee la descripción de la cámara (y probablemente su estado)
   virtual std::string descripcion();

protected:
   // Posición de la cámara en coordenadas esféricas relativas al origen de coordenadas del mundo (y a sus ejes)
   float a = 0.0; // Ángulo horizontal (angulo del eje Z con el plano z == 0 en cc.m.)
   float b = 0.0; // Ángulo vértical (angulo del eje Z con el plano y == 0 en cc.m.)
   float d = 4.0; // Distancia entre origen de cámara y origen de cc. de mundo

   virtual void actualizarMatrices();
};

// Tipo enumerado para los modos de las cámaras interactivas
enum class ModoCam
{
   examinar,
   prim_pers_rot,
   prim_pers_despl
};

// Cámara interactiva completa, que puede funcionar en tres modos, y
// que puede centrarse en un punto (pasa a modo examinar)
//
// Los modos son:
//
//   - Modo examinar (o modo orbital), relativo a un punto de atención
//   - Modo primera persona con rotaciones (típico de los videojuegos)
//   - Modo primera persona con desplazamientos  (horizontal o vertical)
class Camara3Modos : public CamaraInteractiva
{
public:
   // Crea una cámara perspectiva con valores por defecto
   Camara3Modos();

   /**
    * @brief Crea una cámara, inicialmente en modo examinar, con el punto de atención en el origen
    * @param perspectiva_ini True si es una camara perspectiva, False si es ortográfica
    * @param origen_ini Punto de vista inicial (origen marco de cámara)
    * @param punto_aten_ini Punto de atención
    * @param fovy_grad_ini Si es perspectiva, la apertura de campo vertical, en grados
   **/
   Camara3Modos(const bool perspectiva_ini,
                const glm::vec3 &origen_ini, const float ratio_vp_ini,
                const glm::vec3 &punto_aten_ini, const float fovy_grad_ini = 70.0);

   // Desplazar o rotar la cámara en horizontal 'dx' unidades y en vertical 'dy' unidades
   // (en horizontal y en vertical aquí se entiende relativo al marco de camara)
   virtual void desplRotarXY(const float da, const float db);

   // Acercar/alejar la cámara respecto al punto de atención
   // (el eje es el eje Z, relativo al marco de la camara, es decir, perpendicular al plano de visión)
   virtual void moverZ(const float dz);

   // Hacer que apunte hacía el punto de atención 'paten' y pasar a modo examinar
   virtual void mirarHacia(const glm::vec3 &nuevo_punto_aten);

   // Cambiar el modo de la camara al siguiente modo o al primero
   virtual void siguienteModo();

   // Devuelve el punto de atencion actual
   virtual glm::vec3 puntoAtencion();

   // Lee la descripción de la cámara (y probablemente su estado)
   virtual std::string descripcion();

private:
   // Modo actual de la cámara,
   // (0=examinar, 1=prim.persona con desplazamientos, 2=prim.persona con rotaciones)
   ModoCam modo_actual = ModoCam::examinar;

   // true si la cámara es perspectiva, false en otro caso:
   bool perspectiva = true;

   // Apertura de campo vertical en grados
   float fovy_grad = 60.0;

   // Distancia inicial entre el origen del marco y el punto de atención
   const float d_ini = 3.0;

   // Punto de atención
   glm::vec3 punto_atencion = {0.0, 0.0, 0.0};

   // Coordenadas polares del origen de coordenadas del mcv, respecto del punto de atención
   // las componentes 0 y 1 son los ángulos de longitud y latitud, ambos EN RADIANES, inicialmente 0
   // la componente 2 es la distancia al origen, inicialmente 'd_ini'
   glm::vec3 org_polares = {0.0, 0.0, d_ini};

   // Coordenadas cartesianas del origen de coordenadas del mcv, respecto del punto de atención
   // ( == origen del MCV menos punto de atención)
   glm::vec3 org_cartesianas = {0.0, 0.0, d_ini};

   // Ejes del marco de coordenadas de vista
   glm::vec3 eje[3] = 
   {
      {1.0, 0.0, 0.0}, // Eje X
      {0.0, 1.0, 0.0}, // Eje Y
      {0.0, 0.0, 1.0}  // Eje Z
   };

   virtual void actualizarMatrices();
   void actualizarEjesMCV();
};

#endif
