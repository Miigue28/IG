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
    
    const float angle = 2*glm::pi<float>()/static_cast<float>(n);

    const glm::mat4 M = glm::rotate(angle, glm::vec3(0.0, 0.0, 1.0));

    vertices.push_back({1.0, 0.0, 0.0}); // 0
    vertices.push_back({1.0, 0.0, 1.0}); // 1
    vertices.push_back({1.5, 1.0, 0.0}); // 2
    vertices.push_back({1.5, 0.0, 1.0}); // 3
    vertices.push_back({2.0, 1.0, 0.0}); // 4
    vertices.push_back({2.0, 0.0, 1.0}); // 5
    vertices.push_back({2.5, 1.0, 0.0}); // 6
    vertices.push_back({2.5, 0.0, 1.0}); // 7
    vertices.push_back({3.0, 1.0, 0.0}); // 8
    vertices.push_back({3.0, 0.0, 1.0}); // 9
    for (unsigned i = 0; i < n - 1; ++i)
    {
        for (size_t j = 0; j < 10; ++j)
        {
            glm::vec4 tmp = glm::vec4(vertices[10*i + j], 1.0);
            vertices.push_back(M*tmp);
        }
    }
    vertices.push_back({0.0, 0.0, 0.0});
    vertices.push_back({0.0, 0.0, 1.0});
    for (unsigned i = 0; i < n; ++i)
    {
        triangulos.push_back({vertices.size() - 2, vertices.size() - 1, (10*i) % (10*n)});
        triangulos.push_back({vertices.size() - 1, 10*i % (10*n), (10*i + 1) % (10*n)});
        for (unsigned j = 0; j < 4; ++j)
        {
            triangulos.push_back({10*i + 2*j, 10*i + (2*j + 1), 10*i + (2*j + 2)});
            triangulos.push_back({10*i + (2*j + 1), 10*i + (2*j + 2), 10*i + (2*j + 3)});
        }
    }

    // Tabla de colores
    for (unsigned i = 0; i < n; ++i)
    {
        col_ver.push_back({0.5, 0.5, 0.5}); // 0
        col_ver.push_back({0.5, 0.5, 0.5}); // 1
        col_ver.push_back({1.0, 1.0, 1.0}); // 2
        col_ver.push_back({1.0, 1.0, 1.0}); // 3
        col_ver.push_back({1.0, 1.0, 1.0}); // 4
        col_ver.push_back({1.0, 1.0, 1.0}); // 5
        col_ver.push_back({1.0, 1.0, 1.0}); // 6
        col_ver.push_back({1.0, 1.0, 1.0}); // 7
        col_ver.push_back({1.0, 1.0, 1.0}); // 8
        col_ver.push_back({1.0, 1.0, 1.0}); // 9
    }
    col_ver.push_back({0.5, 0.5, 0.5});
    col_ver.push_back({0.5, 0.5, 0.5});
};

Ejercicio03::Ejercicio03(float w, float h)
{
    assert(w > 1 && h < 0.3);

    this->width = w;
    this->height = h;

    Cubo *cubo = new Cubo();

    NodoGrafoEscena *tablero = new NodoGrafoEscena();
    unsigned ind_escalado = tablero->agregar(glm::scale(glm::vec3(1.0, 1.0, 1.0)));
    tablero->agregar(glm::scale(glm::vec3(w, h, w)));
    tablero->agregar(cubo);

    cubescaling = tablero->leerPtrMatriz(ind_escalado);

    NodoGrafoEscena *cuborotatorio = new NodoGrafoEscena();
    unsigned ind_rotacion = cuborotatorio->agregar(glm::rotate(0.0f, glm::vec3(0.0, 1.0, 0.0)));
    cuborotatorio->agregar(glm::translate(glm::vec3(0.0, 2*h, w-h)));
    cuborotatorio->agregar(glm::scale(glm::vec3(h, h, h)));
    cuborotatorio->agregar(cubo);

    cuberotation = cuborotatorio->leerPtrMatriz(ind_rotacion);

    agregar(tablero);
    unsigned ind_traslacion = agregar(glm::translate(glm::vec3(0.0, 0.0, 0.0)));
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
        *inducedtranslation = translate(vec3(0.0, static_cast<float>((a + b*sin(2*M_PI*n*t_sec))*height), 0.0));
    break;
    }
}