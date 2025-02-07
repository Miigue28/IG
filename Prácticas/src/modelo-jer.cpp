/**
 * Nombre: Miguel Ángel Moreno Castro
 * Titulación: DGIIM
 * Email: miguelangelmc@correo.ugr.es
 * DNI: 20070272
**/

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
    calcularNormales();
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
    Material * brass_material = new Material(0.5, 0.75, 0.85, 5.0);
    Material * nickel_material = new Material(0.75, 0.35, 0.85, 10.0);
    Material * dark_nickel_material = new Material(0.65, 0.5, 0.85, 40.0);

    NodoGrafoEscena * joint = new NodoGrafoEscena();
    joint->agregar(translate(vec3(-10.0, -8.0, 0.0)));
    joint->agregar(scale(vec3(3.0, 1.0, 1.0)));
    joint->agregar(rotate(float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    joint->agregar(new Cilinder(50, 50));
    joint->agregar(brass_material);
    joint->ponerColor(vec3(218.0f / 255.0f, 165.0f / 255.0f, 32.0f / 255.0f));

    NodoGrafoEscena * upperdecorator = new NodoGrafoEscena();
    upperdecorator->agregar(translate(vec3(-15.0, 0.0, 0.0)));
    upperdecorator->agregar(scale(vec3(3.0, 1.1, 1.0)));
    upperdecorator->agregar(rotate(float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    upperdecorator->agregar(nickel_material);
    upperdecorator->agregar(new Cilinder(50, 50));
    upperdecorator->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    NodoGrafoEscena * lowerdecorator1 = new NodoGrafoEscena();
    lowerdecorator1->agregar(translate(vec3(-13.0, -8.0, 0.0)));
    lowerdecorator1->agregar(scale(vec3(5.0, 1.1, 1.0)));
    lowerdecorator1->agregar(rotate(float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    lowerdecorator1->agregar(nickel_material);
    lowerdecorator1->agregar(new Cilinder(50, 50));
    lowerdecorator1->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    NodoGrafoEscena * lowerdecorator2 = new NodoGrafoEscena();
    lowerdecorator2->agregar(translate(vec3(-7.0, -8.0, 0.0)));
    lowerdecorator2->agregar(scale(vec3(3.0, 1.1, 1.0)));
    lowerdecorator2->agregar(rotate(float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    lowerdecorator2->agregar(nickel_material);
    lowerdecorator2->agregar(new Cilinder(50, 50));
    lowerdecorator2->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    NodoGrafoEscena * beam1 = new NodoGrafoEscena();
    beam1->agregar(translate(vec3(-17.0, -7.0, 0.0)));
    beam1->agregar(scale(vec3(0.5, 6, 0.5)));
    beam1->agregar(nickel_material);
    beam1->agregar(new Cilinder(50, 50));
    beam1->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    NodoGrafoEscena * beam2 = new NodoGrafoEscena();
    beam2->agregar(translate(vec3(-9.0, -7.0, 0.0)));
    beam2->agregar(scale(vec3(0.5, 6, 0.5)));
    beam2->agregar(nickel_material);
    beam2->agregar(new Cilinder(50, 50));
    beam2->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    NodoGrafoEscena * screw = new NodoGrafoEscena();
    screw->agregar(translate(vec3(-6.0, -8.0, 0.0)));
    screw->agregar(scale(vec3(1.0, 1.3, 1.3)));
    screw->agregar(rotate(float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    screw->agregar(dark_nickel_material);
    screw->agregar(new Cilinder(50, 50));
    screw->ponerColor(vec3(114.0f / 255.0f, 116.0f / 255.0f, 114.0f / 255.0f));

    NodoGrafoEscena * tunning_slide = new NodoGrafoEscena();
    tunning_slide->agregar(brass_material);
    tunning_slide->agregar(new TunningSlide());

    NodoGrafoEscena * outer_bell = new NodoGrafoEscena();
    outer_bell->agregar(brass_material);
    outer_bell->agregar(new OuterBell(50, 50));

    agregar(screw);
    agregar(beam2);
    agregar(beam1);
    agregar(lowerdecorator2);
    agregar(lowerdecorator1);
    agregar(upperdecorator);
    agregar(joint);
    agregar(tunning_slide);
    agregar(outer_bell);
}

InnerSlide::InnerSlide()
{
    Material * silver_material = new Material(0.35, 0.85, 0.65, 10.0);
    Material * nickel_material = new Material(0.75, 0.35, 0.85, 10.0);

    NodoGrafoEscena * tube1 = new NodoGrafoEscena();
    tube1->agregar(translate(glm::vec3(+0.5, 0.0, 0.0)));
    tube1->agregar(scale(glm::vec3(0.1, 3.0, 0.1)));
    tube1->agregar(silver_material);
    tube1->agregar(new Cilinder(50, 50));
    tube1->ponerColor(vec3(227.0f / 255.0f, 228.0f / 255.0f, 229.0f / 255.0f));

    NodoGrafoEscena * tube2 = new NodoGrafoEscena();
    tube2->agregar(translate(vec3(-0.5, 0.0, 0.0)));
    tube2->agregar(scale(vec3(0.1, 3.0, 0.1)));
    tube2->agregar(silver_material);
    tube2->agregar(new Cilinder(50, 50));
    tube1->ponerColor(vec3(227.0f / 255.0f, 228.0f / 255.0f, 229.0f / 255.0f));

    NodoGrafoEscena * clip1 = new NodoGrafoEscena();
    clip1->agregar(translate(vec3(+0.5, 3.0, 0.0)));
    clip1->agregar(scale(vec3(0.15, 0.5, 0.15)));
    clip1->agregar(nickel_material);
    clip1->agregar(new Cilinder(50, 50));
    clip1->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    NodoGrafoEscena * clip2 = new NodoGrafoEscena();
    clip2->agregar(translate(vec3(-0.5, 3.0, 0.0)));
    clip2->agregar(scale(vec3(0.15, 0.5, 0.15)));
    clip2->agregar(nickel_material);
    clip2->agregar(new Cilinder(50, 50));
    clip2->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    NodoGrafoEscena * beamdecorator1 = new NodoGrafoEscena();
    beamdecorator1->agregar(translate(vec3(+0.4, 3.65, 0.0)));
    beamdecorator1->agregar(rotate(float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    beamdecorator1->agregar(scale(vec3(0.13, 0.15, 0.13)));
    beamdecorator1->agregar(nickel_material);
    beamdecorator1->agregar(new Cilinder(50, 50));
    beamdecorator1->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    NodoGrafoEscena * beamdecorator2 = new NodoGrafoEscena();
    beamdecorator2->agregar(translate(vec3(-0.25, 3.65, 0.0)));
    beamdecorator2->agregar(rotate(float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    beamdecorator2->agregar(scale(vec3(0.13, 0.15, 0.13)));
    beamdecorator2->agregar(nickel_material);
    beamdecorator2->agregar(new Cilinder(50, 50));
    beamdecorator2->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    NodoGrafoEscena * gripbeam = new NodoGrafoEscena();
    gripbeam->agregar(translate(vec3(+0.375, 3.65, 0.0)));
    gripbeam->agregar(rotate(float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    gripbeam->agregar(scale(vec3(0.125, 0.75, 0.125)));
    gripbeam->agregar(nickel_material);
    gripbeam->agregar(new Cilinder(50, 50));
    gripbeam->ponerColor(vec3(218.0f / 255.0f, 165.0f / 255.0f, 32.0f / 255.0f));

    NodoGrafoEscena * mouthpieceattach = new NodoGrafoEscena();
    mouthpieceattach->agregar(translate(vec3(+0.5, 3.5, 0.0)));
    mouthpieceattach->agregar(scale(vec3(0.125, 0.5, 0.125)));
    mouthpieceattach->agregar(nickel_material);
    mouthpieceattach->agregar(new Cilinder(50, 50));
    mouthpieceattach->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    NodoGrafoEscena * bellattach = new NodoGrafoEscena();
    bellattach->agregar(translate(vec3(-0.5, 3.5, 0.0)));
    bellattach->agregar(scale(vec3(0.125, 0.5, 0.125)));
    bellattach->agregar(nickel_material);
    bellattach->agregar(new Cilinder(50, 50));
    bellattach->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    agregar(tube1);
    agregar(tube2);
    agregar(clip1);
    agregar(clip2);
    agregar(beamdecorator1);
    agregar(beamdecorator2);
    agregar(gripbeam);
    agregar(mouthpieceattach);
    agregar(bellattach);
}

OuterSlide::OuterSlide()
{
    Material * brass_material = new Material(0.5, 0.75, 0.85, 5.0);
    Material * nickel_material = new Material(0.75, 0.35, 0.85, 10.0);

    NodoGrafoEscena * tube1 = new NodoGrafoEscena();
    tube1->agregar(translate(vec3(+0.5, 0.0, 0.0)));
    tube1->agregar(scale(vec3(0.125, 3.0, 0.125)));
    tube1->agregar(brass_material);
    tube1->agregar(new Cilinder(50, 50));
    tube1->ponerColor(vec3(218.0f / 255.0f, 165.0f / 255.0f, 32.0f / 255.0f));

    NodoGrafoEscena * tube2 = new NodoGrafoEscena();
    tube2->agregar(translate(vec3(-0.5, 0.0, 0.0)));
    tube2->agregar(scale(vec3(0.125, 3.0, 0.125)));
    tube2->agregar(brass_material);
    tube2->agregar(new Cilinder(50, 50));
    tube2->ponerColor(vec3(218.0f / 255.0f, 165.0f / 255.0f, 32.0f / 255.0f));

    NodoGrafoEscena * beamdecorator1 = new NodoGrafoEscena();
    beamdecorator1->agregar(translate(vec3(+0.4, 2.75, 0.0)));
    beamdecorator1->agregar(rotate(float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    beamdecorator1->agregar(scale(vec3(0.13, 0.15, 0.13)));
    beamdecorator1->agregar(brass_material);
    beamdecorator1->agregar(new Cilinder(50, 50));
    beamdecorator1->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    NodoGrafoEscena * beamdecorator2 = new NodoGrafoEscena();
    beamdecorator2->agregar(translate(vec3(-0.25, 2.75, 0.0)));
    beamdecorator2->agregar(rotate(float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    beamdecorator2->agregar(scale(vec3(0.13, 0.15, 0.13)));
    beamdecorator2->agregar(brass_material);
    beamdecorator2->agregar(new Cilinder(50, 50));
    beamdecorator2->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    NodoGrafoEscena * gripbeam = new NodoGrafoEscena();
    gripbeam->agregar(translate(vec3(+0.375, 2.75, 0.0)));
    gripbeam->agregar(rotate(float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    gripbeam->agregar(scale(vec3(0.125, 0.75, 0.125)));
    gripbeam->agregar(nickel_material);
    gripbeam->agregar(new Cilinder(50, 50));
    gripbeam->ponerColor(vec3(218.0f / 255.0f, 165.0f / 255.0f, 32.0f / 255.0f));

    NodoGrafoEscena * elbow = new NodoGrafoEscena();
    elbow->agregar(rotate(float(-M_PI/2.0), vec3(1.0, 0.0, 0.0)));
    elbow->agregar(scale(vec3(0.125, 0.125, 0.125)));
    elbow->agregar(nickel_material);
    elbow->agregar(new Torus(50, 50, float(M_PI), 4));
    elbow->ponerColor(vec3(181.0f / 255.0f, 182.0f / 255.0f, 181.0f / 255.0f));

    NodoGrafoEscena * groundholdtrunk = new NodoGrafoEscena();
    groundholdtrunk->agregar(translate(vec3(0.0, -0.73, 0.0)));
    groundholdtrunk->agregar(scale(vec3(0.1, 0.1, 0.1)));
    groundholdtrunk->agregar(new Cilinder(50, 50));
    groundholdtrunk->ponerColor(vec3(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f));

    NodoGrafoEscena * groundholdbase = new NodoGrafoEscena();
    groundholdbase->agregar(translate(vec3(0.0, -0.73, 0.0)));
    groundholdbase->agregar(scale(vec3(-0.1, -0.1, -0.1)));
    groundholdbase->agregar(new SemiSphere(50, 50));
    groundholdbase->ponerColor(vec3(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f));

    agregar(groundholdbase);
    agregar(groundholdtrunk);
    agregar(elbow);
    agregar(tube1);
    agregar(tube2);
    agregar(beamdecorator1);
    agregar(beamdecorator2);
    agregar(gripbeam);
}

Slide::Slide()
{
    NodoGrafoEscena * inner = new NodoGrafoEscena();
    inner->agregar(new InnerSlide());

    NodoGrafoEscena * outer = new NodoGrafoEscena();
    unsigned index = outer->agregar(translate(vec3(0.0, 0.0, 0.0)));
    outer->agregar(new OuterSlide());

    agregar(rotate(float(M_PI/2.0), vec3(1.0, 0.0, 0.0)));
    agregar(rotate(-float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    agregar(outer);
    agregar(inner);

    slidemovement = outer->leerPtrMatriz(index);
}

unsigned Slide::leerNumParametros() const
{
    // Un grado de libertad
    return 1;
}

void Slide::actualizarEstadoParametro(const unsigned iParam, const float t_sec)
{
    // Verificamos que el índice de parámetro sea válido
    assert(iParam < leerNumParametros());

    // Actualizar las matrices del nodo grafo de escena
    float min = 0, max = 3;
    float a = (min + max)/2, b = (max - min)/2;
    float n = 0.5;
    *slidemovement = translate(vec3(0.0, -(a + b*sin(2*M_PI*n*t_sec)), 0.0));
}

Mouthpiece::Mouthpiece()
{
    Material * silver_material = new Material(0.35, 0.85, 0.65, 10.0);

    NodoGrafoEscena * basecilinder = new NodoGrafoEscena();
    basecilinder->agregar(scale(vec3(0.5, 2.0, 0.5)));
    basecilinder->agregar(new Cilinder(50, 50));

    NodoGrafoEscena * basetorus = new NodoGrafoEscena();
    basetorus->agregar(translate(vec3(0.0, 2.25, 0.0)));
    basetorus->agregar(scale(vec3(0.25, 0.25, 0.25)));
    basetorus->agregar(new Torus(50, 50, float(2*M_PI), 2));

    NodoGrafoEscena * trunksemisphere = new NodoGrafoEscena();
    trunksemisphere->agregar(translate(vec3(0.0, 3.25, 0.0)));
    trunksemisphere->agregar(scale(vec3(-1.0, -1.0, -1.0)));
    trunksemisphere->agregar(new SemiSphere(50, 50));

    NodoGrafoEscena * trunkcilinder = new NodoGrafoEscena();
    trunkcilinder->agregar(translate(vec3(0.0, 3.25, 0.0)));
    trunkcilinder->agregar(scale(vec3(1.0, 0.5, 1.0)));
    trunkcilinder->agregar(new Cilinder(50, 50));

    NodoGrafoEscena * mouthtorus = new NodoGrafoEscena();
    mouthtorus->agregar(translate(vec3(0.0, 4.15, 0.0)));
    mouthtorus->agregar(scale(vec3(0.45, 0.45, 0.45)));
    mouthtorus->agregar(new Torus(50, 50, float(2*M_PI), 2.0));

    agregar(silver_material);
    agregar(mouthtorus);
    agregar(trunkcilinder);
    agregar(trunksemisphere);
    agregar(basetorus);
    agregar(basecilinder);
    ponerColor(vec3(227.0f / 255.0f, 228.0f / 255.0f, 229.0f / 255.0f));
}

Mute::Mute()
{
    Material * mute_texture = new Material(new Textura("plunger_mute.jpg"), 0.25, 0.85, 0.15, 10.0);

    NodoGrafoEscena * cup = new NodoGrafoEscena();
    cup->agregar(scale(vec3(1.0, 0.65, 1.0)));
    cup->agregar(new SemiSphere(50, 50));

    NodoGrafoEscena * base = new NodoGrafoEscena();
    base->agregar(translate(vec3(0.0, -0.2, 0.0)));
    base->agregar(scale(vec3(1.0, 0.2, 1.0)));
    base->agregar(new Cilinder(50, 50));

    NodoGrafoEscena * gripend = new NodoGrafoEscena();
    gripend->agregar(translate(vec3(0.0, 0.85, 0.0)));
    gripend->agregar(scale(vec3(0.1, 0.1, 0.25)));
    gripend->agregar(new Cubo24());

    NodoGrafoEscena * griptrunk = new NodoGrafoEscena();
    griptrunk->agregar(translate(vec3(0.0, 0.6, 0.0)));
    griptrunk->agregar(scale(vec3(0.1, 0.2, 0.1)));
    griptrunk->agregar(new Cilinder(50, 50));

    int index = agregar(rotate(0.0f, vec3(0.0, 0.0, 1.0)));
    agregar(mute_texture);
    agregar(griptrunk);
    agregar(gripend);
    agregar(base);
    agregar(cup);
    ponerColor(vec3(214.0f / 255.0f, 125.0f / 255.0f, 95.0f / 255.0f));

    mutemovement = leerPtrMatriz(index);
}

unsigned Mute::leerNumParametros() const
{
    // Un grado de libertad
    return 1;
}

void Mute::actualizarEstadoParametro(const unsigned iParam, const float t_sec)
{
    // Verificamos que el índice de parámetro sea válido
    assert(iParam < leerNumParametros());

    // Actualizar las matrices del nodo grafo de escena
    float min = 0, max = M_PI/6.0;
    float a = (min + max)/2, b = (max - min)/2;
    float n = 1;
    *mutemovement = rotate(-float(a + b*sin(2*M_PI*n*t_sec)), vec3(0.0, 0.0, 1.0));
}

Trombone::Trombone()
{
    unsigned slide_id = 1;
    unsigned bell_id = 2;
    unsigned mute_id = 3;
    unsigned mouthpiece_id = 4;

    NodoGrafoEscena * slide = new NodoGrafoEscena();
    slide_child = new Slide();
    slide->ponerNombre("Vara del Trombón");
    slide->ponerIdentificador(slide_id);
    slide->agregar(translate(vec3(6.0, 0.0, 1.0)));
    slide->agregar(rotate(float(M_PI), vec3(0.0, 1.0, 0.0)));
    slide->agregar(scale(vec3(2.0, 2.0, 2.0)));
    slide->agregar(slide_child);

    NodoGrafoEscena * bell = new NodoGrafoEscena();
    bell->ponerNombre("Campana del Trombón");
    bell->ponerIdentificador(bell_id);
    bell->agregar(translate(vec3(-0.5, 2.0, 0.0)));
    bell->agregar(scale(vec3(0.25, 0.25, 0.25)));
    bell->agregar(new Bell());

    NodoGrafoEscena * mouthpiece = new NodoGrafoEscena();
    mouthpiece->ponerNombre("Boquilla del Trombón");
    mouthpiece->ponerIdentificador(mouthpiece_id);
    mouthpiece->agregar(translate(vec3(-1.75, 0.0, 2.0)));
    mouthpiece->agregar(rotate(float(M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    mouthpiece->agregar(scale(vec3(0.35, 0.35, 0.35)));
    mouthpiece->agregar(new Mouthpiece());

    NodoGrafoEscena * mute = new NodoGrafoEscena();
    mute_child = new Mute();
    mute->ponerNombre("Sordina del Trombón");
    mute->ponerIdentificador(mute_id);
    mute->agregar(translate(vec3(0.85, 2.0, 0.0)));
    mute->agregar(rotate(float(-M_PI/2.0), vec3(0.0, 0.0, 1.0)));
    mute->agregar(mute_child);
    
    unsigned index = agregar(rotate(0.0f, vec3(0.0, 0.0, 1.0)));
    agregar(mute);
    agregar(mouthpiece);
    agregar(slide);
    agregar(bell);

    wavingmovement = leerPtrMatriz(index);
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

    float min = -M_PI/6.0, max = M_PI/4.0;
    float a = (min + max)/2, b = (max - min)/2;
    float n = 0.15;

    // Actualizar las matrices del nodo grafo de escena
    switch (iParam)
    {
        case 0:
            *wavingmovement = rotate(float(a + b*sin(2*M_PI*n*t_sec)), vec3(1.0, 1.0, 1.0));
        break;
        case 1:
            slide_child->actualizarEstadoParametro(0, t_sec);
        break;
        case 2:
            mute_child->actualizarEstadoParametro(0, t_sec);
        break;
    }
}
