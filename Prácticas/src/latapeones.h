/**
 * Nombre: Miguel Ángel Moreno Castro
 * Titulación: DGIIM
 * Email: miguelangelmc@correo.ugr.es
 * DNI: 20070272
**/

#ifndef LATA_PEONES_H
#define LATA_PEONES_H

#include "grafo-escena.h"

class VariasLatasPeones : public NodoGrafoEscena
{
public:
    VariasLatasPeones();
};

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
protected:
    glm::mat4 * traslation_movement = nullptr;
    float shift = 1.0;
public:
    Peon(int profiles);

    virtual bool cuandoClick(const glm::vec3 &centro_wc);
};

#endif