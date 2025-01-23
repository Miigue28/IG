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
// ** Gestión de eventos de teclado (implementación). Función 'FGE_PulsarLevantarTecla'
// ** Copyright (C) 2016-2024 Carlos Ureña
// **
// ** Implementación de la función gestora del evento de teclado
// ** (función 'FGE_PulsarLevantarTecla')
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

#include "ig-aux.h" // Incluye cabeceras de opengl / glut / glut / glew
#include "camara.h"
#include "seleccion.h"
#include "animacion.h"
#include "materiales-luces.h"
#include "eventos-teclado.h"

using namespace std;

// Función gestora del evento de pulsar o levantar una tecla
void FGE_PulsarLevantarTecla(GLFWwindow *window, int key, int scancode, int action, int mods)
{
   assert(aplicacionIG != nullptr);
   using namespace std;

   // Solo estamos interesados en el evento de levantar una tecla
   if (action == GLFW_PRESS) return;

   bool redib = true; // true sii al final de esta función es necesario redibujar

   // Recuperar y comprobar camara y objeto actual
   Escena *escena = aplicacionIG->escenas[aplicacionIG->ind_escena_act];
   assert(escena != nullptr);

   CamaraInteractiva *camara = escena->camaraActual();
   assert(camara != nullptr);

   Objeto3D *objeto = escena->objetoActual();
   assert(objeto != nullptr);

   // Si está pulsada la tecla 'L', actualizar la colección de fuentes de luz y terminar
   if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
   {
      // Procesar pulsación de una tecla en la configuración de las fuentes de luz
      if (ProcesaTeclaFuenteLuz(escena->colFuentes(), key))
         aplicacionIG->revisualizar_escena = true;

      // Finalizar la FGE, ya que si está la tecla L pulsada no se mira ninguna otra tecla.
      return; 
   }

   // Si está pulsada la tecla 'A', la tecla se procesa en 'animacion.cpp', actuando sobre el objeto que se está visualizando
   if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
   {
      // Procesar pulsación de una tecla de animación
      if (ProcesarTeclaAnimacion(objeto, key))
         // Forzar revisualizar escena
         aplicacionIG->revisualizar_escena = true;

      // Finalizar la FGE, ya que si está la tecla A pulsada no se mira ninguna otra tecla.
      return;
   }

   constexpr float
       cam_d_incre_tecla = 1.0,  // incremento de 'cam_d' por teclado (debe ser >= 1.0)
       cam_ab_incre_tecla = 3.0; // incremento de ángulos por teclado

   switch (key)
   {
   // Teclas de camara
   case GLFW_KEY_LEFT:
      camara->desplRotarXY(+cam_ab_incre_tecla, 0.0);
   break;

   case GLFW_KEY_RIGHT:
      camara->desplRotarXY(-cam_ab_incre_tecla, 0.0);
   break;

   case GLFW_KEY_UP:
      camara->desplRotarXY(0.0, +cam_ab_incre_tecla);
   break;

   case GLFW_KEY_DOWN:
      camara->desplRotarXY(0.0, -cam_ab_incre_tecla);
   break;

   case GLFW_KEY_SLASH: // Tecla con '-' y '_' en el teclado normal
   case GLFW_KEY_KP_SUBTRACT: // Tecla '-' en el teclado numérico
      camara->moverZ(+cam_d_incre_tecla);
   break;

   case GLFW_KEY_RIGHT_BRACKET: // Tecla '+' en el teclado normal
   case GLFW_KEY_KP_ADD: // Tecla '+' en el teclado numérico ¿?
      camara->moverZ(-cam_d_incre_tecla);
      break;

   case GLFW_KEY_C:
      camara->siguienteModo();
      break;

   case GLFW_KEY_V:
      escena->siguienteCamara();
      break;

   // Teclas para cambiar de escena y de objeto dentro de la escena
   case GLFW_KEY_O:
      escena->siguienteObjeto();
      break;

   case GLFW_KEY_P:
      assert(aplicacionIG->ind_escena_act < aplicacionIG->escenas.size());
      aplicacionIG->ind_escena_act = (aplicacionIG->ind_escena_act + 1) % aplicacionIG->escenas.size();
      cout << "Escena actual cambiada a: " << (aplicacionIG->ind_escena_act + 1) << endl
           << flush;
      break;

   // Tecla para terminar
   case GLFW_KEY_ESCAPE:
   case GLFW_KEY_Q:
      aplicacionIG->terminar_programa = true;
      break;

   // Teclas variadas:
   case GLFW_KEY_E:
      aplicacionIG->dibujar_ejes = !aplicacionIG->dibujar_ejes;
      cout << "Dibujar ejes: " << (aplicacionIG->dibujar_ejes ? "activado." : "desactivado.") << endl
           << flush;
      break;

   case GLFW_KEY_N:
      aplicacionIG->visualizar_normales = !aplicacionIG->visualizar_normales;
      cout << "Visualizar normales: " << (aplicacionIG->visualizar_normales ? "activado." : "desactivado.") << endl
           << flush;
      break;

   case GLFW_KEY_M:
      aplicacionIG->modo_visu = ModosVisu((int(aplicacionIG->modo_visu) + 1) % int(ModosVisu::num_modos));
      cout << "Modo de visualización cambiado a: '" << nombreModoVisu[int(aplicacionIG->modo_visu)] << "'" << endl
           << flush;
      break;

   case GLFW_KEY_W:
      aplicacionIG->dibujar_aristas = !aplicacionIG->dibujar_aristas;
      cout << "Dibujar aristas: " << (aplicacionIG->dibujar_aristas ? "activado" : "desactivado") << endl;
      break;

   case GLFW_KEY_I:
      aplicacionIG->iluminacion = !aplicacionIG->iluminacion;
      cout << "Iluminación : " << (aplicacionIG->iluminacion ? "activada" : "desactivada") << endl
           << flush;
      break;

   case GLFW_KEY_F:
      aplicacionIG->usar_normales_tri = !aplicacionIG->usar_normales_tri;
      cout << "La iluminación usa " << (aplicacionIG->usar_normales_tri ? "la normal del triángulo" : "las normales de vértices (interpoladas)") << endl
           << flush;
      break;

   case GLFW_KEY_G:
      aplicacionIG->suavizar_aristas = !aplicacionIG->suavizar_aristas;
      cout << "Suavizar_aristas : " << (aplicacionIG->suavizar_aristas ? "activado" : "desactivado") << endl
           << flush;
      break;

      // COMPLETAR: práctica 5: conmutar 'apl->visualizar_fbo' con la tecla 'Y'
      //
      // Añadir un caso nuevo: si se ha pulsado la tecla 'Y' (GLFW_KEY_Y), conmutar
      // el valor de 'apl->visualizar_fbo, e informar sobre si la visualización del
      // FBO ha quedado activada o desactivada
      //

   case GLFW_KEY_T:
      aplicacionIG->imprimir_tiempos = !aplicacionIG->imprimir_tiempos;
      cout << "imprimir tiempos : " << (aplicacionIG->imprimir_tiempos ? "activado" : "desactivado") << endl
           << flush;
      break;

   default:
      redib = false; // Para otras teclas, no es necesario redibujar
      break;
   }
   // Si se ha cambiado algo, forzar evento de redibujado
   if (redib)
      aplicacionIG->revisualizar_escena = true;
}
