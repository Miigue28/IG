#ifndef MODELO_JERARQUICO_HPP
#define MODELO_JERARQUICO_HPP

#include "malla-revol.h"
#include "grafo-escena.h"

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

class Trombone : public NodoGrafoEscena
{
protected:
public:
    Trombone();
};

#endif // MODELO_JERARQUICO_HPP