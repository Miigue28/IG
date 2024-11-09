#ifndef MODELO_JERARQUICO_HPP
#define MODELO_JERARQUICO_HPP

#include "malla-revol.h"
#include "grafo-escena.h"

class SemiSphere : public MallaRevol
{
public:
    SemiSphere
    (
       const int num_verts_per, // Número de vértices del perfil original
       const unsigned nperfiles // Número de perfiles
    );
};

class Torus : public MallaRevol
{
public:
    Torus
    (
       const int num_verts_per, // Número de vértices del perfil original
       const unsigned nperfiles,// Número de perfiles
       const float angle,       // Ángulo de rotación
       const float center         // Centro
    );
};

class OuterBell : public MallaRevol
{
public:
    OuterBell
    (
       const int num_verts_per, // Número de vértices del perfil original
       const unsigned nperfiles // Número de perfiles
    );
};

class TunningSlide : public NodoGrafoEscena
{
protected:
public:
    // Constructor
    TunningSlide();
};

class Bell : public NodoGrafoEscena
{
protected:
public:
    Bell();
};

class InnerSlide : public NodoGrafoEscena
{
protected:
public:
    // Constructor
    InnerSlide();
};

class OuterSlide : public NodoGrafoEscena
{
protected:
public:
    // Constructor
    OuterSlide();
};

class Slide : public NodoGrafoEscena
{
protected:
public:
    // Constructor
    Slide();
};

class Mouthpiece : public NodoGrafoEscena
{
protected:
public:
    Mouthpiece();
};

class Trombone : public NodoGrafoEscena
{
protected:
public:
    Trombone();
    // Devuelve el número de parámetros de este objeto
    virtual unsigned leerNumParametros() const;

    // Actualiza el valor de un parámetro a un instante de tiempo
    virtual void actualizarEstadoParametro(const unsigned iParam, const float t_sec);
};

#endif // MODELO_JERARQUICO_HPP