#ifndef LATA_PEONES_H
#define LATA_PEONES_H

#include "grafo-escena.h"

class LataPeones : public NodoGrafoEscena
{
public:
    LataPeones();
};

class Lata : public NodoGrafoEscena
{
public:
    Lata(const std::string & filename);
};

class Peon : public NodoGrafoEscena
{
public:
    Peon(int profiles);
};

#endif