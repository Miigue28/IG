#include "modelo-jer.h"
#include "malla-revol.h"
#include "aplicacion-ig.h"

using namespace glm;

SemiSphere::SemiSphere(const int num_verts_per, const unsigned nperfiles)
{
    std::vector<glm::vec3> perfil;
    const float subintervals = (M_PI/2.0)/float(num_verts_per); 
    ponerNombre(std::string("Semiesfera de Revolución"));
    // La semiesfera tiene el centro en el origen, el radio es 1
    for (int i = 0; i <= num_verts_per; i++)
    {
       float angle = float(i)*subintervals;
       perfil.push_back({cos(angle), sin(angle), 0.0});
    }
    inicializar(perfil, nperfiles);
}

Torus::Torus(const int num_verts_per, const unsigned nperfiles, const float angle, const float center)
{
    std::vector<glm::vec3> perfil;
    ponerNombre(std::string("Toro de Revolución"));
    // El perfil es una circunferencia centrada en el (2, 0) cuyo radio es 1
    for (int i = 0; i <= num_verts_per; i++)
    {
       float angle = (2*M_PI * i) / float(num_verts_per-1);
       perfil.push_back({center + cos(angle), sin(angle), 0.0});
    }
    inicializar(perfil, nperfiles, angle);
}

OuterBell::OuterBell(const int num_verts_per, const unsigned nperfiles)
{
    std::vector<glm::vec3> perfil;
    ponerNombre(std::string("Campana de Revolución"));

    // (b-a)/n
    const float subintervals = 20.0/float(num_verts_per);

    // El perfil es la imagen de la función 1+e^(x/4) en el intervalo [-15, 5]
    for (int i = 0; i <= num_verts_per; i++)
    {
        // a + i(b-a)/n
       double x = -15 + double(i)*subintervals;
       perfil.push_back({x, 1+exp(x/3.5), 0.0});
    }
    inicializarX(perfil, nperfiles);
    ponerColor(vec3(218.0f / 255.0f, 165.0f / 255.0f, 32.0f / 255.0f));
}

TunningSlide::TunningSlide()
{
    agregar(rotate(float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    agregar(rotate(float(M_PI/2.0), vec3(1.0, 0.0, 0.0)));
    agregar(translate(vec3(-4.0, 0.0, -18.0)));
    agregar(new Torus(50, 50, float(M_PI), 4));
    ponerColor(vec3(218.0f / 255.0f, 165.0f / 255.0f, 32.0f / 255.0f));
}

Bell::Bell()
{
    NodoGrafoEscena * joint = new NodoGrafoEscena();
    joint->agregar(translate(vec3(-10.0, -8.0, 0.0)));
    joint->agregar(scale(vec3(3.0, 1.0, 1.0)));
    joint->agregar(rotate(float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    joint->agregar(new Cilindro(50, 50));
    joint->ponerColor(vec3(218.0f / 255.0f, 165.0f / 255.0f, 32.0f / 255.0f));

    NodoGrafoEscena * upperdecorator = new NodoGrafoEscena();
    upperdecorator->agregar(translate(vec3(-15.0, 0.0, 0.0)));
    upperdecorator->agregar(scale(vec3(3.0, 1.1, 1.0)));
    upperdecorator->agregar(rotate(float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    upperdecorator->agregar(new Cilindro(50, 50));
    upperdecorator->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    NodoGrafoEscena * lowerdecorator1 = new NodoGrafoEscena();
    lowerdecorator1->agregar(translate(vec3(-13.0, -8.0, 0.0)));
    lowerdecorator1->agregar(scale(vec3(5.0, 1.1, 1.0)));
    lowerdecorator1->agregar(rotate(float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    lowerdecorator1->agregar(new Cilindro(50, 50));
    lowerdecorator1->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    NodoGrafoEscena * lowerdecorator2 = new NodoGrafoEscena();
    lowerdecorator2->agregar(translate(vec3(-7.0, -8.0, 0.0)));
    lowerdecorator2->agregar(scale(vec3(3.0, 1.1, 1.0)));
    lowerdecorator2->agregar(rotate(float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    lowerdecorator2->agregar(new Cilindro(50, 50));
    lowerdecorator2->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    NodoGrafoEscena * hang1 = new NodoGrafoEscena();
    hang1->agregar(translate(vec3(-17.0, -7.0, 0.0)));
    hang1->agregar(scale(vec3(0.5, 6, 0.5)));
    hang1->agregar(new Cilindro(50, 50));
    hang1->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    NodoGrafoEscena * hang2 = new NodoGrafoEscena();
    hang2->agregar(translate(vec3(-9.0, -7.0, 0.0)));
    hang2->agregar(scale(vec3(0.5, 6, 0.5)));
    hang2->agregar(new Cilindro(50, 50));
    hang2->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    NodoGrafoEscena * screw = new NodoGrafoEscena();
    screw->agregar(translate(vec3(-6.0, -8.0, 0.0)));
    screw->agregar(scale(vec3(1.0, 1.3, 1.3)));
    screw->agregar(rotate(float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    screw->agregar(new Cilindro(50, 50));
    screw->ponerColor(vec3(114.0f / 255.0f, 116.0f / 255.0f, 114.0f / 255.0f));

    agregar(screw);
    agregar(hang2);
    agregar(hang1);
    agregar(lowerdecorator2);
    agregar(lowerdecorator1);
    agregar(upperdecorator);
    agregar(joint);
    agregar(new TunningSlide());
    agregar(new OuterBell(50, 50));
}

InnerSlide::InnerSlide()
{
    NodoGrafoEscena * tube1 = new NodoGrafoEscena();
    tube1->agregar(glm::translate(glm::vec3(+0.5, 0.0, 0.0)));
    tube1->agregar(glm::scale(glm::vec3(0.1, 3.0, 0.1)));
    tube1->agregar(new Cilindro(50, 50));

    NodoGrafoEscena * tube2 = new NodoGrafoEscena();
    tube2->agregar(glm::translate(glm::vec3(-0.5, 0.0, 0.0)));
    tube2->agregar(glm::scale(glm::vec3(0.1, 3.0, 0.1)));
    tube2->agregar(new Cilindro(50, 50));

    NodoGrafoEscena * clip1 = new NodoGrafoEscena();
    clip1->agregar(translate(vec3(+0.5, 3.0, 0.0)));
    clip1->agregar(scale(vec3(0.15, 0.5, 0.15)));
    clip1->agregar(new Cilindro(50, 50));
    clip1->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    NodoGrafoEscena * clip2 = new NodoGrafoEscena();
    clip2->agregar(translate(vec3(-0.5, 3.0, 0.0)));
    clip2->agregar(scale(vec3(0.15, 0.5, 0.15)));
    clip2->agregar(new Cilindro(50, 50));
    clip2->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    NodoGrafoEscena * hangdecorator1 = new NodoGrafoEscena();
    hangdecorator1->agregar(translate(vec3(+0.4, 3.65, 0.0)));
    hangdecorator1->agregar(rotate(float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    hangdecorator1->agregar(scale(vec3(0.13, 0.15, 0.13)));
    hangdecorator1->agregar(new Cilindro(50, 50));
    hangdecorator1->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    NodoGrafoEscena * hangdecorator2 = new NodoGrafoEscena();
    hangdecorator2->agregar(translate(vec3(-0.25, 3.65, 0.0)));
    hangdecorator2->agregar(rotate(float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    hangdecorator2->agregar(scale(vec3(0.13, 0.15, 0.13)));
    hangdecorator2->agregar(new Cilindro(50, 50));
    hangdecorator2->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    NodoGrafoEscena * hang = new NodoGrafoEscena();
    hang->agregar(translate(vec3(+0.375, 3.65, 0.0)));
    hang->agregar(rotate(float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    hang->agregar(scale(vec3(0.125, 0.75, 0.125)));
    hang->agregar(new Cilindro(50, 50));
    hang->ponerColor(vec3(218.0f / 255.0f, 165.0f / 255.0f, 32.0f / 255.0f));

    NodoGrafoEscena * mouthpieceattach = new NodoGrafoEscena();
    mouthpieceattach->agregar(translate(vec3(+0.5, 3.5, 0.0)));
    mouthpieceattach->agregar(scale(vec3(0.125, 0.5, 0.125)));
    mouthpieceattach->agregar(new Cilindro(50, 50));
    mouthpieceattach->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    NodoGrafoEscena * bellattach = new NodoGrafoEscena();
    bellattach->agregar(translate(vec3(-0.5, 3.5, 0.0)));
    bellattach->agregar(scale(vec3(0.125, 0.5, 0.125)));
    bellattach->agregar(new Cilindro(50, 50));
    bellattach->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    agregar(rotate(float(M_PI/2.0), vec3(1.0, 0.0, 0.0)));
    agregar(rotate(-float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    agregar(tube1);
    agregar(tube2);
    agregar(clip1);
    agregar(clip2);
    agregar(hangdecorator1);
    agregar(hangdecorator2);
    agregar(hang);
    agregar(mouthpieceattach);
    agregar(bellattach);
}

OuterSlide::OuterSlide()
{
    NodoGrafoEscena * tube1 = new NodoGrafoEscena();
    tube1->agregar(glm::translate(glm::vec3(+0.5, 0.0, 0.0)));
    tube1->agregar(glm::scale(glm::vec3(0.125, 3.0, 0.125)));
    tube1->agregar(new Cilindro(50, 50));
    tube1->ponerColor(vec3(218.0f / 255.0f, 165.0f / 255.0f, 32.0f / 255.0f));

    NodoGrafoEscena * tube2 = new NodoGrafoEscena();
    tube2->agregar(glm::translate(glm::vec3(-0.5, 0.0, 0.0)));
    tube2->agregar(glm::scale(glm::vec3(0.125, 3.0, 0.125)));
    tube2->agregar(new Cilindro(50, 50));
    tube2->ponerColor(vec3(218.0f / 255.0f, 165.0f / 255.0f, 32.0f / 255.0f));

    NodoGrafoEscena * hangdecorator1 = new NodoGrafoEscena();
    hangdecorator1->agregar(translate(vec3(+0.4, 2.75, 0.0)));
    hangdecorator1->agregar(rotate(float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    hangdecorator1->agregar(scale(vec3(0.13, 0.15, 0.13)));
    hangdecorator1->agregar(new Cilindro(50, 50));
    hangdecorator1->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    NodoGrafoEscena * hangdecorator2 = new NodoGrafoEscena();
    hangdecorator2->agregar(translate(vec3(-0.25, 2.75, 0.0)));
    hangdecorator2->agregar(rotate(float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    hangdecorator2->agregar(scale(vec3(0.13, 0.15, 0.13)));
    hangdecorator2->agregar(new Cilindro(50, 50));
    hangdecorator2->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    NodoGrafoEscena * hang = new NodoGrafoEscena();
    hang->agregar(translate(vec3(+0.375, 2.75, 0.0)));
    hang->agregar(rotate(float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    hang->agregar(scale(vec3(0.125, 0.75, 0.125)));
    hang->agregar(new Cilindro(50, 50));
    hang->ponerColor(vec3(218.0f / 255.0f, 165.0f / 255.0f, 32.0f / 255.0f));

    NodoGrafoEscena * elbow = new NodoGrafoEscena();
    elbow->agregar(rotate(float(M_PI/2.0), vec3(0.0, 1.0, 0.0)));
    elbow->agregar(scale(vec3(0.125, 0.125, 0.125)));
    elbow->agregar(new Torus(50, 50, float(M_PI), 4));
    elbow->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    agregar(elbow);
    agregar(rotate(float(M_PI/2.0), vec3(1.0, 0.0, 0.0)));
    agregar(rotate(-float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    agregar(tube1);
    agregar(tube2);
    agregar(hangdecorator1);
    agregar(hangdecorator2);
    agregar(hang);
}

Slide::Slide()
{
    agregar(new InnerSlide());
    agregar(new OuterSlide());
}

Mouthpiece::Mouthpiece()
{
    NodoGrafoEscena * base = new NodoGrafoEscena();
    base->agregar(scale(vec3(0.5, 2.0, 0.5)));
    base->agregar(new Cilindro(50, 50));

    NodoGrafoEscena * basetorus = new NodoGrafoEscena();
    basetorus->agregar(translate(vec3(0.0, 2.25, 0.0)));
    basetorus->agregar(scale(vec3(0.25, 0.25, 0.25)));
    basetorus->agregar(new Torus(50, 50, float(2*M_PI), 2));

    NodoGrafoEscena * semisphere = new NodoGrafoEscena();
    semisphere->agregar(translate(vec3(0.0, 3.25, 0.0)));
    semisphere->agregar(scale(vec3(-1.0, -1.0, -1.0)));
    semisphere->agregar(new SemiSphere(50, 50));

    NodoGrafoEscena * trunk = new NodoGrafoEscena();
    trunk->agregar(translate(vec3(0.0, 3.25, 0.0)));
    trunk->agregar(scale(vec3(1.0, 0.5, 1.0)));
    trunk->agregar(new Cilindro(50, 50));

    NodoGrafoEscena * mouth = new NodoGrafoEscena();
    mouth->agregar(translate(vec3(0.0, 4.15, 0.0)));
    mouth->agregar(scale(vec3(0.45, 0.45, 0.45)));
    mouth->agregar(new Torus(50, 50, float(2*M_PI), 2.0));

    agregar(translate(vec3(0.0, -3.0, 0.0)));
    agregar(mouth);
    agregar(trunk);
    agregar(semisphere);
    agregar(basetorus);
    agregar(base);
    ponerColor(vec3(240.0f / 255.0f, 240.0f / 255.0f, 240.0f / 255.0f));
}

Trombone::Trombone()
{
    NodoGrafoEscena * slide = new NodoGrafoEscena();
    slide->agregar(translate(vec3(6.0, 0.0, 1.0)));
    slide->agregar(rotate(float(M_PI), vec3(0.0, 1.0, 0.0)));
    slide->agregar(scale(vec3(2.0, 2.0, 2.0)));
    slide->agregar(new Slide());

    NodoGrafoEscena * bell = new NodoGrafoEscena();
    bell->agregar(translate(vec3(-0.5, 2.0, 0.0)));
    bell->agregar(scale(vec3(0.25, 0.25, 0.25)));
    bell->agregar(new Bell());

    NodoGrafoEscena * mouthpiece = new NodoGrafoEscena();
    mouthpiece->agregar(translate(vec3(-2.75, 0.0, 2.0)));
    mouthpiece->agregar(rotate(float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    mouthpiece->agregar(scale(vec3(0.35, 0.35, 0.35)));
    mouthpiece->agregar(new Mouthpiece());
    
    agregar(mouthpiece);
    agregar(slide);
    agregar(bell);
}

unsigned Trombone::leerNumParametros() const
{
    // Tres grados de libertad
    return 3;
}

void Trombone::actualizarEstadoParametro(const unsigned iParam, const float t_sec)
{
    // Verificamos que el índice de parámetro sea válido
    assert(iParam < leerNumParametros());

    // Actualizar las matrices del nodo grafo de escena
}
