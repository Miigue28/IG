#include "modelo-jer.h"
#include "malla-revol.h"
#include "aplicacion-ig.h"


Torus::Torus(const int num_verts_per, const unsigned nperfiles, const float angle)
{
    std::vector<glm::vec3> perfil;
    ponerNombre(std::string("Toro de Revolución"));
    // El perfil es una circunferencia centrada en el (2, 0) cuyo radio es 1
    for (int i = 0; i <= num_verts_per; i++)
    {
       float angle = (2*M_PI * i) / float(num_verts_per-1);
       perfil.push_back({2 + cos(angle), sin(angle), 0.0});
    }
    inicializar(perfil, nperfiles, angle);
}

Bell::Bell(const int num_verts_per, const unsigned nperfiles)
{
    std::vector<glm::vec3> perfil;
    ponerNombre(std::string("Campana de Revolución"));

    // (b-a)/n
    const float subintervals = 10.0/float(num_verts_per);

    // El perfil es la imagen de la función 1+e^(x/4) en el intervalo [-5, 5]
    for (int i = 0; i <= num_verts_per; i++)
    {
        // a + i(b-a)/n
       double x = -5 + double(i)*subintervals;
       perfil.push_back({x, 1+exp(x/3.5), 0.0});
    }
    inicializarX(perfil, nperfiles);
}

Slide::Slide()
{
    NodoGrafoEscena * tube = new NodoGrafoEscena();
    tube->agregar(glm::scale(glm::vec3(0.1, 3.0, 0.1)));
    // TODO: Rotarlo
    tube->agregar(new Cilindro(50, 50));

    agregar(glm::translate(glm::vec3(+1.0, 0.0, 0.0)));
    agregar(tube);
    agregar(glm::translate(glm::vec3(-1.0, 0.0, 0.0)));
    agregar(tube);
}
