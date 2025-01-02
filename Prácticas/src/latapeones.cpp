#include "latapeones.h"
#include "malla-revol.h"

LataPeones::LataPeones()
{
    using namespace glm;

    NodoGrafoEscena * lata = new NodoGrafoEscena();
    // Perfil de la parte central de la lata
    lata->agregar(new Material(new Textura("lata-coke.jpg"), 0.5, 0.4, 0.9, 40));
    lata->agregar(new MallaRevolPLY("lata-pcue.ply", 50));

    // Tapa superior metálica de la lata
    lata->agregar(new Material(0.5, 0.4, 0.9, 40));
    lata->agregar(new MallaRevolPLY("lata-psup.ply", 50));

    // Base inferior metálica de la lata
    lata->agregar(new Material(0.5, 0.4, 0.9, 40));
    lata->agregar(new MallaRevolPLY("lata-pinf.ply", 50));

    MallaRevolPLY *peon = new MallaRevolPLY("peon.ply", 50);

    // Peón de madera
    NodoGrafoEscena * peon_madera = new NodoGrafoEscena();
    peon_madera->agregar(new Material(new Textura("text-madera.jpg"), 0.5, 0.4, 0.9, 40));
    peon_madera->agregar(peon);

    // Peón blanco
    NodoGrafoEscena * peon_blanco = new NodoGrafoEscena();
    peon_blanco->agregar(translate(vec3(2.5, 0.0, 0.0)));
    peon_blanco->agregar(new Material(0.5, 0.2, 0.5, 5.0));
    peon_blanco->agregar(peon);

    // Peón negro
    NodoGrafoEscena * peon_negro = new NodoGrafoEscena();
    peon_negro->agregar(translate(vec3(5.0, 0.0, 0.0)));
    peon_negro->agregar(new Material(0.01, 0.2, 0.5, 50.0));
    peon_negro->agregar(peon);

    NodoGrafoEscena * peones = new NodoGrafoEscena();
    peones->agregar(peon_madera);
    peones->agregar(peon_blanco);
    peones->agregar(peon_negro);

    agregar(peones);
    agregar(translate(vec3(0.0, -1.0, 5.0)));
    agregar(scale(vec3(5.0, 5.0, 5.0)));
    agregar(lata);
}