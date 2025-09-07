/**
 * Nombre: Miguel Ángel Moreno Castro
 * Titulación: DGIIM
 * Email: miguelangelmc@correo.ugr.es
 * DNI: 20070272
**/

#include "examen-extr.h"

EjercicioExtr01::EjercicioExtr01()
{
    vertices = 
    {
        // Cara frontal
        {-1.0, 0.0, 1.0}, {1.0, 0.0, 1.0}, {0.5, 1.0, 0.5}, {-0.5, 1.0, 0.5},
        // Cara lateral derecha
        {1.0, 0.0, 1.0}, {1.0, 0.0, -1.0}, {0.5, 1.0, -0.5}, {0.5, 1.0, 0.5},
        // Cara trasera
        {1.0, 0.0, -1.0}, {0.5, 1.0, -0.5}, {-0.5, 1.0, -0.5}, {-1.0, 0.0, -1.0},
        // Cara lateral izquierda
        {-1.0, 0.0, -1.0}, {-0.5, 1.0, -0.5}, {-0.5, 1.0, 0.5}, {-1.0, 0.0, 1.0},
        // Base
        {-1.0, 0.0, 1.0}, {1.0, 0.0, 1.0}, {1.0, 0.0, -1.0}, {-1.0, 0.0, -1.0},
        // Tapa
        {-0.5, 1.0, 0.5}, {0.5, 1.0, 0.5}, {0.5, 1.0, -0.5}, {-0.5, 1.0, -0.5}
    };
    triangulos =
    {
        // Cara frontal
        {0, 1, 2}, {0, 2, 3},
        // Cara lateral derecha
        {4, 5, 6}, {4, 6, 7},
        // Cara trasera
        {8, 9, 10}, {8, 10, 11},
        // Cara lateral izquierda
        {12, 13, 14}, {12, 14, 15},
        // Base
        {16, 17, 18}, {16, 18, 19},
        // Tapa
        {20, 21, 22}, {20, 22, 23}
    };
    calcularNormales();
}

EjercicioExtr02::EjercicioExtr02()
{
    const size_t num_faces = 6;
    const size_t num_figures = 4;
    glm::mat4 R = glm::rotate(glm::pi<float>()/2.0f, glm::vec3(0.0, 0.0, 1.0));
    glm::mat4 S = glm::scale(glm::vec3(-1.0, -1.0, 1.0));
    glm::mat4 T = glm::translate(glm::vec3(0.0, 1.0, 0.0));

    const std::vector<glm::vec3> tmp = 
    {
        // Cara frontal
        {-1.0, 0.0, 1.0}, {1.0, 0.0, 1.0}, {0.5, 1.0, 0.5}, {-0.5, 1.0, 0.5},
        // Cara lateral derecha
        {1.0, 0.0, 1.0}, {1.0, 0.0, -1.0}, {0.5, 1.0, -0.5}, {0.5, 1.0, 0.5},
        // Cara trasera
        {1.0, 0.0, -1.0}, {0.5, 1.0, -0.5}, {-0.5, 1.0, -0.5}, {-1.0, 0.0, -1.0},
        // Cara lateral izquierda
        {-1.0, 0.0, -1.0}, {-0.5, 1.0, -0.5}, {-0.5, 1.0, 0.5}, {-1.0, 0.0, 1.0},
        // Base
        {-1.0, 0.0, 1.0}, {1.0, 0.0, 1.0}, {1.0, 0.0, -1.0}, {-1.0, 0.0, -1.0},
        // Tapa
        {-0.5, 1.0, 0.5}, {0.5, 1.0, 0.5}, {0.5, 1.0, -0.5}, {-0.5, 1.0, -0.5}
    };

    // Tecla superior
    for (size_t i = 0; i < tmp.size(); ++i)
    {
        glm::vec4 v = glm::vec4(tmp[i], 1.0f);
        vertices.push_back(T*v);
    }
    // Tecla inferior
    for (size_t i = 0; i < tmp.size(); ++i)
    {
        glm::vec4 v = glm::vec4(tmp[i], 1.0f);
        vertices.push_back(S*(T*v));
    }
    // Tecla lateral izquierda
    for (size_t i = 0; i < tmp.size(); ++i)
    {
        glm::vec4 v = glm::vec4(tmp[i], 1.0f);
        vertices.push_back(R*(T*v));
    }
    // Tecla lateral derecha
    for (size_t i = 0; i < tmp.size(); ++i)
    {
        glm::vec4 v = glm::vec4(tmp[i], 1.0f);
        vertices.push_back(S*(R*(T*v)));
    }

    // Tabla de triangulos
    for (size_t j = 0; j < num_figures*num_faces; ++j)
    {
        triangulos.push_back({4*j, 4*j + 1, 4*j + 2});
        triangulos.push_back({4*j, 4*j + 2, 4*j + 3});
    }
    calcularNormales();
}

KeyCap::KeyCap(const int i, const int j)
{
    this->i = i;
    this->j = j;

    NodoGrafoEscena *base = new NodoGrafoEscena();
    unsigned int id_traslacion_base = base->agregar(glm::translate(glm::vec3(0.0, 0.0, 0.0)));
    unsigned int id_escalado_base = base->agregar(glm::scale(glm::vec3(1.0, 1.0, 1.0)));
    base->agregar(new Cubo24());

    escalado_base = base->leerPtrMatriz(id_escalado_base);
    traslacion_base = base->leerPtrMatriz(id_traslacion_base);

    NodoGrafoEscena *tecla = new NodoGrafoEscena();
    unsigned int id_traslacion_tecla = tecla->agregar(glm::translate(glm::vec3(0.0, 0.0, 0.0)));
    tecla->agregar(new EjercicioExtr01());

    traslacion_tecla = tecla->leerPtrMatriz(id_traslacion_tecla);

    agregar(base);
    agregar(glm::translate(glm::vec3(0.0, 1.0, 0.0)));
    agregar(tecla);
}

unsigned KeyCap::leerNumParametros() const
{
    return 1;
}

void KeyCap::actualizarEstadoParametro(const unsigned iParam, const float t_sec)
{
    // Verificamos que el índice de parámetro sea válido
    assert(iParam < leerNumParametros());  

    float min = 0.5f, max = 2.0f;
    float a = (min + max)/2, b = (max - min)/2; 
    float n = 0.25;
    float y = 0.0f;

    // Actualizar las matrices del nodo grafo de escena
    switch (iParam)
    {
    case 0:
        y = static_cast<float>(a + b*sin(2*M_PI*n*t_sec));
        *escalado_base = glm::scale(glm::vec3(1.0, y, 1.0));
        *traslacion_base = glm::translate(glm::vec3(0.0, y/2.0f, 0.0));
        *traslacion_tecla = glm::translate(glm::vec3(0.0, y, 0.0));
    break;
    }
}

EjercicioExtr03::EjercicioExtr03(const int n, const int m)
{
    for (int i = 0; i < n; ++i)
    {
        NodoGrafoEscena *fila_teclas = new NodoGrafoEscena();
        for (int j = 0; j < m; ++j)
        {
            keys.push_back(new KeyCap(i, j));
            fila_teclas->agregar(glm::translate(glm::vec3(2.0, 0.0, 0.0)));
            fila_teclas->agregar(keys.back());
        }
        agregar(fila_teclas);
        agregar(glm::translate(glm::vec3(0.0, 0.0, 2.0)));
    }
}

unsigned EjercicioExtr03::leerNumParametros() const
{
    // Un grado de libertad
    return 1;
}

void EjercicioExtr03::actualizarEstadoParametro(const unsigned iParam, const float t_sec)
{
    // Verificamos que el índice de parámetro sea válido
    assert(iParam < leerNumParametros());  

    float min = 0.5f, max = 2.0f;
    float a = (min + max)/2, b = (max - min)/2; 
    float n = 0.25;

    // Actualizar las matrices del nodo grafo de escena
    switch (iParam)
    {
    case 0:
        for (auto & key : keys)
        {
            //if (alternate_key)
            //    key->actualizarEstadoParametro(iParam, t_sec);
            //else
            //    key->actualizarEstadoParametro(iParam, t_sec/2.0f);
            //alternate_key = !alternate_key;

            key->actualizarEstadoParametro(iParam, t_sec);
        }
    break;
    }
}

EjercicioExtr04::EjercicioExtr04(const int n, const int m)
{
    NodoGrafoEscena * key_1 = new NodoGrafoEscena();
    key_1->agregar(new Cubo24());
    key_1->agregar(glm::translate(glm::vec3(0.0, 1.0, 0.0)));
    key_1->agregar(new Material(new TexturaXZ("window-icon.jpg"), 0.1, 0.9, 0.5, 100.0));
    key_1->agregar(new EjercicioExtr01());

    NodoGrafoEscena * key_2 = new NodoGrafoEscena();
    key_2->agregar(new Cubo24());
    key_2->agregar(glm::translate(glm::vec3(0.0, 1.0, 0.0)));
    key_2->agregar(new Material(new TexturaXZ("window-icon.jpg"), 0.1, 0.9, 0.5, 100.0));
    key_2->agregar(new EjercicioExtr01());
    key_2->agregar(glm::rotate(glm::pi<float>()/2.0f, glm::vec3(0.0, 0.0, 1.0)));

    for (int i = 0; i < n; ++i)
    {
        NodoGrafoEscena *fila_teclas = new NodoGrafoEscena();
        for (int j = 0; j < m; ++j)
        {
            if (change_texture_orientation)
                fila_teclas->agregar(key_1);
            else
                fila_teclas->agregar(key_1);
            change_texture_orientation = !change_texture_orientation;
            fila_teclas->agregar(glm::translate(glm::vec3(2.0, 0.0, 0.0)));
        }
        agregar(fila_teclas);
        agregar(glm::translate(glm::vec3(0.0, 0.0, 2.0)));
    }
}

EjercicioExtr05::EjercicioExtr05(const int n, const int m)
{
    for (int i = 0; i < n; ++i)
    {
        NodoGrafoEscena *fila_teclas = new NodoGrafoEscena();
        for (int j = 0; j < m; ++j)
        {
            KeyCap * key = new KeyCap(i, j);
            key->ponerIdentificador((i+1)*m + (j+1));
            fila_teclas->agregar(glm::translate(glm::vec3(2.0, 0.0, 0.0)));
            fila_teclas->agregar(key);
        }
        agregar(fila_teclas);
        agregar(glm::translate(glm::vec3(0.0, 0.0, 2.0)));
    }
}

bool KeyCap::cuandoClick(const glm::vec3 &centro_wc)
{
    if (up)
    {
        actualizarEstadoParametro(0, 1.0);
        std::cout << "La tecla en la fila " << i << ", columna " << j << ", se ha levantado" << std::endl;
    }
    else
    {
        actualizarEstadoParametro(0, 0.0);
        std::cout << "La tecla en la fila " << i << ", columna " << j << ", se ha hundido" << std::endl;
    }
    up = !up;

    return true;
}