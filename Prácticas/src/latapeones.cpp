#include "latapeones.h"
#include "malla-revol.h"

Peon::Peon(int profiles)
{
    unsigned index = agregar(glm::translate(glm::vec3(0.0, 0.0, 0.0)));
    agregar(new MallaRevolPLY("peon.ply", profiles));

    traslation_movement = leerPtrMatriz(index);
}

bool Peon::cuandoClick(const glm::vec3 & centro_wc)
{
    *traslation_movement += glm::translate(glm::vec3(0.0, 0.0, 1.0));
}

Lata::Lata(const std::string & filename)
{
    // Perfil de la parte central de la lata
    NodoGrafoEscena * trunk = new NodoGrafoEscena();
    trunk->agregar(new Material(new Textura(filename), 0.75, 0.65, 0.25, 50.0));
    trunk->agregar(new MallaRevolPLY("lata-pcue.ply", 35));

    // Tapa superior metálica de la lata
    NodoGrafoEscena * sup = new NodoGrafoEscena();
    sup->agregar(new Material(0.35, 0.45, 0.25, 50.0));
    sup->agregar(new MallaRevolPLY("lata-psup.ply", 35));

    // Base inferior metálica de la lata
    NodoGrafoEscena * inf = new NodoGrafoEscena();
    inf->agregar(new Material(0.35, 0.45, 0.25, 50.0));
    inf->agregar(new MallaRevolPLY("lata-pinf.ply", 35));

    NodoGrafoEscena * lata = new NodoGrafoEscena();
    lata->agregar(inf);
    lata-agregar(trunk);
    lata->agregar(sup);

    agregar(lata);
}

LataPeones::LataPeones()
{
    using namespace glm;

    // Peón de madera
    NodoGrafoEscena * peon_madera = new NodoGrafoEscena();
    unsigned ident_peon_madera = 1;
    peon_madera->ponerNombre("Peón de madera");
    peon_madera->ponerIdentificador(ident_peon_madera);
    peon_madera->agregar(new Material(new Textura("text-madera.jpg"), 0.5, 0.6, 0.5, 50.0));
    peon_madera->agregar(new Peon(35));

    // Peón blanco
    NodoGrafoEscena * peon_blanco = new NodoGrafoEscena();
    unsigned ident_peon_blanco = 2;
    peon_blanco->ponerNombre("Peón blanco");
    peon_blanco->ponerIdentificador(ident_peon_blanco);
    peon_blanco->agregar(translate(vec3(2.5, 0.0, 0.0)));
    peon_blanco->agregar(new Material(0.5, 0.2, 0.5, 5.0));
    peon_blanco->agregar(new Peon(35));

    // Peón negro
    NodoGrafoEscena * peon_negro = new NodoGrafoEscena();
    unsigned ident_peon_negro = 3;
    peon_negro->ponerNombre("Peón negro");
    peon_negro->ponerIdentificador(ident_peon_negro);
    peon_negro->agregar(translate(vec3(5.0, 0.0, 0.0)));
    peon_negro->agregar(new Material(0.01, 0.2, 0.5, 50.0));
    peon_negro->agregar(new Peon(35));

    NodoGrafoEscena * peones = new NodoGrafoEscena();
    peones->agregar(translate(vec3(0.0, 0.4, 0.7)));
    peones->agregar(scale(vec3(0.25, 0.25, 0.25)));
    peones->agregar(peon_madera);
    peones->agregar(peon_blanco);
    peones->agregar(peon_negro);

    NodoGrafoEscena * lata = new NodoGrafoEscena;
    unsigned ident_lata = 4;
    lata->ponerNombre("Lata de Coca-Cola");
    lata->ponerIdentificador(ident_lata);
    lata->agregar(new Lata("lata-coke.jpg"));

    agregar(peones);
    agregar(lata);
}

VariasLatasPeones::VariasLatasPeones()
{
    using namespace glm;

    NodoGrafoEscena * lata_pepsi = new NodoGrafoEscena();
    unsigned ident_lata_pepsi = 5;
    lata_pepsi->ponerNombre("Lata de Pepsi");
    lata_pepsi->ponerIdentificador(ident_lata_pepsi);
    lata_pepsi->agregar(translate(vec3(1.0, 0.0, 0.0)));
    lata_pepsi->agregar(new Lata("lata-pepsi.jpg"));

    NodoGrafoEscena * lata_ugr = new NodoGrafoEscena;
    unsigned ident_lata_ugr = 6;
    lata_ugr->ponerNombre("Lata de la UGR");
    lata_ugr->ponerIdentificador(ident_lata_ugr);
    lata_ugr->agregar(translate(vec3(2.0, 0.0, 0.0)));
    lata_ugr->agregar(new Lata("window-icon.jpg"));

    agregar(lata_pepsi);
    agregar(lata_ugr);
    agregar(new LataPeones());
}