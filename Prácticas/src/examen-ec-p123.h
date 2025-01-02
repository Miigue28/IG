/**
 * Nombre: Miguel Ángel Moreno Castro
 * Titulación: DGIIM
 * Email: miguelangelmc@correo.ugr.es
 * DNI: 20070272
**/

#ifndef EXAMEN_EC_P123_HPP
#define EXAMEN_EC_P123_HPP

#include "malla-ind.h"
#include "grafo-escena.h"

class Ejercicio01 : public MallaInd
{
public:
    Ejercicio01();
};

class Ejercicio02 : public MallaInd
{
public:
    Ejercicio02
    (
        unsigned n // Número de hélices
    );
};

class Ejercicio03 : public NodoGrafoEscena
{
protected:
    glm::mat4 *cuberotation = nullptr;
    glm::mat4 *cubescaling = nullptr;
    glm::mat4 *inducedtranslation = nullptr;
public:
    Ejercicio03
    (
        float w, // Anchura
        float h  // Altura
    );

    // Devuelve el número de parámetros de este objeto
   virtual unsigned leerNumParametros() const;
   
   // Actualiza el valor de un parámetro a un instante de tiempo
   virtual void actualizarEstadoParametro(const unsigned iParam, const float t_sec);
};

#endif