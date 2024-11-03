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
       const float angle        // Ángulo de rotación
    );
};

class Bell : public MallaRevol
{
public:
    Bell
    (
       const int num_verts_per, // Número de vértices del perfil original
       const unsigned nperfiles // Número de perfiles
    );
};

class Slide : public NodoGrafoEscena
{
protected:
public:
    // Constructor
    Slide();
};

#endif // MODELO_JERARQUICO_HPP