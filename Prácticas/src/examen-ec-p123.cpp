/**
 * Nombre: Miguel Ángel Moreno Castro
 * Titulación: DGIIM
 * Email: miguelangelmc@correo.ugr.es
 * DNI: 20070272
**/

#include "examen-ec-p123.h"

Ejercicio01::Ejercicio01()
{
    vertices = 
    {
        {0.0, 0.0, 0.0}, // 0 
        {1.0, 0.0, 0.0}, // 1
        {1.5, 1.0, 0.0}, // 2
        {2.0, 1.0, 0.0}, // 3
        {2.5, 1.0, 0.0}, // 4
        {3.0, 1.0, 0.0}, // 5
        {0.0, 0.0, 1.0}, // 6
        {1.0, 0.0, 1.0}, // 7
        {1.5, 0.0, 1.0}, // 8
        {2.0, 0.0, 1.0}, // 9
        {2.5, 0.0, 1.0}, // 10
        {3.0, 0.0, 1.0}  // 11
    };
    triangulos =
    {
        {0, 1, 6},
        {1, 6, 7},
        {1, 2, 7},
        {2, 7, 8},
        {2, 3, 8},
        {3, 8, 9},
        {3, 4, 9},
        {4, 9, 10},
        {4, 5, 10},
        {5, 10, 11}
    };
    col_ver =
    {
        {0.5, 0.5, 0.5},
        {0.5, 0.5, 0.5},
        {1.0, 1.0, 1.0},
        {1.0, 1.0, 1.0},
        {1.0, 1.0, 1.0},
        {1.0, 1.0, 1.0},
        {0.5, 0.5, 0.5},
        {0.5, 0.5, 0.5},
        {1.0, 1.0, 1.0},
        {1.0, 1.0, 1.0},
        {1.0, 1.0, 1.0}, 
        {1.0, 1.0, 1.0}  
    };
}

Ejercicio02::Ejercicio02(unsigned n)
{
    assert(n > 0);
    
    const float angle = 2*M_PI/static_cast<float>(n);

    // TODO: Rotarlo bien porque está haciendo unas rotaciones raras

    // Tabla de vértices
    for (unsigned i = 0; i < n; ++i)
    {
        vertices.push_back({0.0 + cos(i*angle), 0.0 + sin(i*angle), 0.0}); // 1
        vertices.push_back({0.5 + cos(i*angle), 0.0 + sin(i*angle), 0.0}); // 2
        vertices.push_back({1.0 + cos(i*angle), 1.0 + sin(i*angle), 0.0}); // 3
        vertices.push_back({1.5 + cos(i*angle), 1.0 + sin(i*angle), 0.0}); // 4
        vertices.push_back({2.0 + cos(i*angle), 1.0 + sin(i*angle), 0.0}); // 5
        vertices.push_back({0.0 + cos(i*angle), 0.0 + sin(i*angle), 1.0}); // 7
        vertices.push_back({0.5 + cos(i*angle), 0.0 + sin(i*angle), 1.0}); // 8
        vertices.push_back({1.0 + cos(i*angle), 0.0 + sin(i*angle), 1.0}); // 9
        vertices.push_back({1.5 + cos(i*angle), 0.0 + sin(i*angle), 1.0}); // 10
        vertices.push_back({2.0 + cos(i*angle), 0.0 + sin(i*angle), 1.0}); // 11
    }
    vertices.push_back({0.0, 0.0, 0.0});
    vertices.push_back({0.0, 0.0, 1.0});

    // Tabla de triángulos
    for (unsigned i = 0; i < n; ++i)
    {
        triangulos.push_back({10*n, 12*i, 10*n+1});
        triangulos.push_back({12*i, 10*n+1, 12*i + 5});
        for (unsigned j = 0; j < 4; ++j)
        {
            triangulos.push_back({12*i + j, 12*i + j + 1, 12*i + j + 6});
            triangulos.push_back({12*i + j + 1, 12*i + j + 6, 12*i + j + 1 + 6});
        }
    }

    // Tabla de colores
    for (unsigned i = 0; i < n; ++i)
    {
        col_ver.push_back({0.5, 0.5, 0.5});
        col_ver.push_back({0.5, 0.5, 0.5});
        col_ver.push_back({1.0, 1.0, 1.0});
        col_ver.push_back({1.0, 1.0, 1.0});
        col_ver.push_back({1.0, 1.0, 1.0});
        col_ver.push_back({1.0, 1.0, 1.0});
        col_ver.push_back({0.5, 0.5, 0.5});
        col_ver.push_back({0.5, 0.5, 0.5});
        col_ver.push_back({1.0, 1.0, 1.0});
        col_ver.push_back({1.0, 1.0, 1.0});
        col_ver.push_back({1.0, 1.0, 1.0}); 
        col_ver.push_back({1.0, 1.0, 1.0}); 
    }
};

Ejercicio03::Ejercicio03(float w, float h)
{
    assert(w > 1 && h < 0.3);

    using namespace glm;

    NodoGrafoEscena *tablero = new NodoGrafoEscena();
    unsigned ind_escalado = tablero->agregar(scale(vec3(1.0, 1.0, 1.0)));
    tablero->agregar(scale(vec3(w, h, w)));
    tablero->agregar(new Cubo());

    cubescaling = tablero->leerPtrMatriz(ind_escalado);

    NodoGrafoEscena *cuborotatorio = new NodoGrafoEscena();
    unsigned ind_rotacion = cuborotatorio->agregar(rotate(0.0f, vec3(0.0, 1.0, 0.0)));
    cuborotatorio->agregar(translate(vec3(0.0, 2*h, w-0.2)));
    cuborotatorio->agregar(scale(vec3(0.2, 0.2, 0.2)));
    cuborotatorio->agregar(new Cubo());

    cuberotation = cuborotatorio->leerPtrMatriz(ind_rotacion);

    agregar(tablero);
    unsigned ind_traslacion = agregar(translate(vec3(0.0, 0.0, 0.0)));
    agregar(cuborotatorio);

    inducedtranslation = leerPtrMatriz(ind_traslacion);
}

unsigned Ejercicio03::leerNumParametros() const
{
   // Dos grados de libertad
   return 2;
}

void Ejercicio03::actualizarEstadoParametro(const unsigned iParam, const float t_sec)
{
    using namespace glm;

    // Verificamos que el índice de parámetro sea válido
    assert(iParam < leerNumParametros());  

    float min = 0.5f, max = 2.0f;
    float a = (min + max)/2, b = (max - min)/2; 
    float n = 0.25;

    // Actualizar las matrices del nodo grafo de escena
    switch (iParam)
    {
    case 0:
       *cuberotation = rotate(static_cast<float>(2*M_PI*0.5*t_sec), vec3(0.0, 1.0, 0.0));
    break;
    case 1:
        // TODO: Coordinar estos dos movimientos
        *cubescaling = scale(vec3(1.0, -static_cast<float>(a + b*sin(2*M_PI*n*t_sec)), 1.0));
        *inducedtranslation = translate(vec3(0.0, static_cast<float>((a + b*sin(2*M_PI*n*t_sec))/10.0f), 0.0));
    break;
    }
}