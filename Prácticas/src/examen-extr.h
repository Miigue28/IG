/**
 * Nombre: Miguel Ángel Moreno Castro
 * Titulación: DGIIM
 * Email: miguelangelmc@correo.ugr.es
 * DNI: 20070272
**/

#ifndef EXAMEN_EXTR_HPP
#define EXAMEN_EXTR_HPP

#include "malla-ind.h"
#include "grafo-escena.h"
#include "materiales-luces.h"

class EjercicioExtr01 : public MallaInd
{
public:
    EjercicioExtr01();
};

class EjercicioExtr02 : public MallaInd
{
public:
    EjercicioExtr02();
};

class KeyCap : public NodoGrafoEscena
{
protected:
    glm::mat4 *traslacion_base = nullptr;
    glm::mat4 *traslacion_tecla = nullptr;
    glm::mat4 *escalado_base = nullptr;

    bool up = true;

    int i = 0, j = 0;
public:
    KeyCap(const int i, const int j);

    virtual bool cuandoClick(const glm::vec3 &centro_wc);

    virtual unsigned leerNumParametros() const;
   
    virtual void actualizarEstadoParametro(const unsigned iParam, const float t_sec);
};

class EjercicioExtr03 : public NodoGrafoEscena
{
protected:
    std::vector<KeyCap*> keys;
    bool alternate_key = true;
public:
    EjercicioExtr03(const int n, const int m);

   virtual unsigned leerNumParametros() const;
   
   virtual void actualizarEstadoParametro(const unsigned iParam, const float t_sec);
};

class EjercicioExtr04 : public NodoGrafoEscena
{
protected:
    bool change_texture_orientation = true;
public:
    EjercicioExtr04(const int n, const int m);
};

class EjercicioExtr05 : public NodoGrafoEscena
{
public:
    EjercicioExtr05(const int n, const int m);
};

#endif