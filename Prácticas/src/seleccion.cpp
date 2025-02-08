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
// ** Funciones y método de gestión de la selección (implementación).
// ** Copyright (C) 2016-2024 Carlos Ureña
// **
// ** Implementación de las funciones de gestión de identificadores
// **   + FijarColVertsIdent
// **   + LeerIdentEnPixel
// ** Implementación del método de 'AplicacionIG'
// **   + Seleccion
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
#include "camara.h"
#include "fbo.h"
#include "seleccion.h"
#include "grafo-escena.h"

// Calcula un color usando un identificador entero no negativo
glm::vec4 ColorDesdeIdent(const int ident) // 0 ≤ ident < 2^24
{
   using namespace glm;
   assert(0 <= ident);

   // Decodificar: extraer los tres bytes menos significativos del valor entero.
   const unsigned char byteR = (ident) % 0x100U;            // Rojo  = byte menos significativo
   const unsigned char byteG = (ident / 0x100U) % 0x100U;   // Verde = byte intermedio
   const unsigned char byteB = (ident / 0x10000U) % 0x100U; // Azul  = byte más significativo

   // log(" RGB == ",(unsigned int)byteR,", ",(unsigned int)byteG,", ", (unsigned int)byteB);

   // Fijar el color normalizando desde [0..255] hacia [0..1]
   // (otra posibilidad es pasarle los bytes directamente a 'glVertexAttrib4Nubv')
   // (ver: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttrib.xhtml)

   return vec4(float(byteR) / 255.0f, float(byteG) / 255.0f, float(byteB) / 255.0f, 1.0f);
}

// Lee un identificador codificado en el color de un pixel en el framebuffer activo
int LeerIdentEnPixel(int xpix, int ypix)
{
   unsigned char bytes[3]; 

   // Lee los 3 bytes del frame-buffer
   glReadPixels(xpix, ypix, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void *)bytes);

   // Reconstruir el identificador y devolverlo
   return int(bytes[0]) + (int(0x100U)*int(bytes[1])) + (int(0x10000U) * int(bytes[2]));
}

// Función principal de selección, se llama al hacer click con el botón izquierdo
//
// (x,y) = posición donde se ha hecho click en coordenadas del sistema de ventanas (enteras)
//
// devuelve: true si se ha seleccionado algún objeto, false si no
bool AplicacionIG::seleccion(int x, int y)
{
   using namespace std;
   using namespace glm;

   // Comprobar algunas precondiciones
   assert(0 < ventana_tam_x);
   assert(0 < ventana_tam_y);

   Escena *escena = escenas[ind_escena_act];
   assert(escena != nullptr);

   // Nos aseguramos que solo se crea la primera vez
   if (aplicacionIG->fbo == nullptr)
   {
      // Crear el framebuffer
      aplicacionIG->fbo = new Framebuffer(ventana_tam_x, ventana_tam_y);
   }

   // Activar el framebuffer
   aplicacionIG->fbo->activar(ventana_tam_x, ventana_tam_y);

   // Visualizar la escena actual en modo selección
   escena->visualizarGL_Seleccion();
   
   // Leer el identificador del pixel en las coordenadas (x, y)
   int id = LeerIdentEnPixel(x, y);

   // Desactivar el FBO (vuelve a activar el FBO por defecto, con identificador '0')
   aplicacionIG->fbo->desactivar();

   // Si el identificador del pixel es 0, imprimir mensaje y terminar
   if (id == 0)
   {
      cout << "Objeto no seleccionable" << endl;
      return false;
   }

   Objeto3D * objeto = escena->objetoActual();
   Objeto3D * target = nullptr;
   vec3 centro = {0.0, 0.0, 0.0};
   mat4 * identity = new mat4(1.0f);

   // Buscar el identificador del objeto raiz de la escena y ejecutar 'cuandoClick'
   if (objeto->buscarObjeto(id, *identity, &target, centro))
   {
      return target->cuandoClick(centro);
   }

   // Si el flujo de control llega aquí, es que no se encuentra ese identificador
   cout << "El identificador del objeto en el pixel no se encuentra en el objeto raíz que se está visualizando." << endl;
   return false;
}
