
# 1. Introducción

> Autor: Miguel Ángel Moreno Castro
>
> Source: Carlos Ureña Almagro

El término **Informática Gráfica** designa, en un sentido amplio a el campo de la informática dedicado al estudio de los algoritmos, técnicas o metodologías destinados a la creación y manipulación de contenido visual digital.

Esencialmente nos centraremos en estudiar las técnicas para el diseño e implementación de programas interactivos para visualización 3D y animación de modelos de caras planas y jerárquicos.

Los elementos esenciales de una aplicación gráfica son:
- **Modelos digitales** de objetos reales, ficticios o de datos.
- **Imágenes o vídeos digitales** que se usan para visualizar dichos objetos.

> En las aplicaciones interactivas, los usuarios modifican los modelos y reciben retroalimentación inmediata:

![](./resources/img01.png)

# 2. Proceso de Visualización

## 2.1 Programas gráficos

Un **programa gráfico** es un programa que almacena una estructura de datos que constituye un **modelo** computacional de determinada información, además produce una salida constituida por una o varias imágenes.

Las imágenes típicas son **imágenes raster**, constituidas por un array de pixeles discretos, cada uno con un color RGB. Existen otro tipo de salidas gráficas, la más frecuentes son las **imágenes vectoriales** (`.svg`).

Hay dos tipos de programas gráficos:

- Un programa **interactivo** es un programa que visualiza en una ventana gráfica una imagen que constituye una representación visual del modelo. Procesa acciones del usuario, llamadas **eventos**, que se traducen en modificaciones del modelo.

> Cada vez que el modelo es modificado, se vuelve a visualizar de forma interactiva, es decir, desde que el usuario produce el evento hasta que puede observar la imagen actualizada pasan tiempos del orden de decenas de milisegundos como mucho.

- Un programa **no interactivo** es un programa que produce una o varias imágenes a partir del modelo, siendo la duración de la producción de estas desde unos segundos hasta varias horas. El usuario solo especifica el modelo y los parámetros de visualización, no interviniendo de ninguna otra forma durante el tiempo en el que se producen las imágenes.

## 2.2 El Proceso de Visualización

El proceso de visualización produce una imagen a partir de un **modelo de escena** y unos **parámetros**.

El **modelo de escena** es una estructura de datos en memoria que representa lo que se quiere ver, está formado por varias partes:
- **Modelo geométrico**: Conjunto de primitivas, típicamente polígonos planos, que definen la forma de los objetos a visualizar.
- **Modelo de aspecto**: Conjunto de parámetros que definen el aspecto de los objetos, entre ellos destacan el tipo de material, color, textura, fuentes de luz, etc.

Los **parámetros de visualización** son un conjunto amplio de valores que determinan como se visualiza la escena en la imagen. Algunos elementos esenciales son:
- **Cámara virtual**: Posición, orientación y ángulo de visión del observador ficticio que vería la escena como aparece en la imagen.
- **Viewport**: Resolución de la imagen, y si procede, posición de la misma en la ventana.

## 2.3 Rasterización y Ray-Tracing

### Visualización basada en rasterización

> Iniciar el color de todos los píxeles para cada primitiva $P$ del modelo a visualizar
> Encontrar el conjunto $S$ de píxeles cubiertos por $P$
> Para cada píxel $q$ de $S$:
	Calcular el color de $P$ en $q$
	Actualizar el color de $q$

La complejidad en tiempo es claramente del orden del número de primitivas ($n$) por el número de pixels ($p$), es decir, $O(n \cdot p)$.

### Visualización basada en Ray-Tracing

> Iniciar el color de todos los píxeles 
> Para cada píxel $q$ de la imagen a producir
> 	Calcular $T$, el conjunto de primitivas que cubren $q$
> 	Para cada primitiva $P$ del conjunto $T$
> 		Calcular el color de $P$ en $q$
> 		Actualizar el color de $q$

Se puede optimizar para lograr complejidad en número de pixels por el logaritmo del número de primitivas, es decir, $O(p \log n)$. Esto requiere el uso de **indexación espacial**, para el cálculo de $T$ en cada pixel.

El método de Ray-Tracing y sus variantes suele ser más lento, pero consigue resultados más realistas cuando se pretende reproducir ciertos efectos visuales. En los últimos años, han aparecido arquitecturas de GPUs con aceleración por hardware para Ray-Tracing, lo que está llevando a implementar algunos videojuegos usando esta técnica.

## 2.4 El Caucre Gráfico en Rasterización

El **cauce gráfico** es el conjunto de etapas de cálculo que permiten la síntesis de imágenes por rasterización.

Las entradas al cauce gráfico se denominan **primitivas**, formas visibles que no se pueden descomponer en otras más simples, siendo típicamente triángulos o segmentos de líneas/puntos. Un **vertice** es un punto del espacio, extremo de una arista de un triángulo, o de un segmente de recta, o donde se dibuja un punto. 

> Una o varias primitivas se especifican mediante una lista de coordenadas de vértices, más alguna información adicional.

El cauce escribe en el **framebuffer**, que es una zona de memoria donde se guardan uno o varios arrays con los colores RGB de los pixels de la imagen, además de alguna información adicional. Está conectado al monitor.

El cauce gráfico tiene las siguientes etapas:

- **Procesado de vértices**: Parte de una secuencia de vértices y produce una secuencia de primitivas. Tiene estas sub-etapas:
	- **Transformación**: Los vértices de cada primitiva son transformados en diversos pasos hasta encontrar su proyección en el plano de la imagen. Es realizado por el subprograma llamado **Vertex Shader**.
	- **Teselación y nivel de detalle**: Transformaciones adicionales avanzadas, realizadas por varios programas, entre ellos el **Geometry Shader**.
- **Post-procesado de vértices y montaje de primitivas**: Incluye varios cálculos como el recortado (_clipping_) y el cribado de caras (_face culling_).
- **Rasterización**: Cada primitiva es rasterizada, y se encuentran los pixels que cubre en la imagen de salida.
- **Sombreado**: En cada pixel cubierto se calcula el color que se le debe asignar. Se realiza por un programa llamado **Fragment Shader** o **Pixel Shader**.

![](./resources/img02.png)
## 3. OpenGL y GLFW

**OpenGL** es la especificación de un conjunto de funciones útil para visualización 2D/3D basada en rasterización. **GLSL** (_GL Shading Languaje_) es el lenguaje de programación de shaders que se usa con OpenGL.

> Para gestionar ventanas y eventos de entrada se deben usar librerías auxiliares, que pueden o no ser dependientes del entorno hardware/software, para la cuál utilizaremos **GLFW**.

## 3.1 Programación y eventos en GLFW

En las aplicaciones interactivas, un **evento** es la ocurrencia de un suceso relevante para la aplicación, hay varios tipos, entre los que destacamos:
- **Teclado**: Pulsación o levantado de una tecla, de tipo carácter o de otras teclas.
- **Ratón**: Pulsación o levantado de botones del ratón, movimiento del ratón, movimiento de la rueda del ratón para _scroll_.
- **Cambio de tamaño**: Cambio de tamaño de alguna ventana de la aplicación.

Las **funciones gestoras de eventos** (_event managers_, o _callbacks_) son funciones del programa que se invocan cuando ocurre un evento de un determinado tipo.

El programa establece que tipos de eventos se quieren gestionar y qué funciones lo harán. Tras invocar a una de estas funciones, se dice que el correspondiente evento ya ha sido **procesado** o **gestionado**.

```c++
void VisualizarFrame(){}
void FGE_CambioTamano(GLFWwindow* ventana, int nuevoAncho, int nuevoAlto){}
void FGE_PulsarLevantarTecla(GLFWwindow* ventana, int tecla, ...){}
void FGE_PulsarLevantarRaton(GLFWwindow* ventana, int boton, ...){}
void Inicializa_GLFW(int argc, char* argv[]){}
void Inicializa_OpenGL(){}
void BucleEventos_GLFW(){}
int main(int argc, char* argv[])
{
	Inicializa_GLFW(argc, argv); // Crea una ventana
	Inicializa_OpenGL(); // Inicializa estado del cauce
	BucleEventos_GLFW(); // Ejecuta el bucle
	glfwTerminate(); // Cerrar la ventana
}
```

Una aplicación OpenGL/GLFW ejecuta un **bucle principal** o **bucle de gestión de eventos** (en GLFW, el programador debe implementarlo explícitamente):
- GLFW mantiene una **cola de eventos**, con información de cada evento que ya ha ocurrido pero que no ha sido gestionado aún por la aplicación.
- En cada iteración se espera hasta que haya al menos un evento en la cola, entonces:
	- Se extrae el siguiente evento de la cola; si hay designada una función gestora para ese tipo de evento se ejecuta dicha función.
	- Si la ejecución de la función ha cambiado el modelo de escena o algún parámetro, se visualiza un cuadro nuevo.
- El bucle termina típicamente cuando alguna función gestora se ordena cerrarla.

```c++
void Inicializa_GLFW(int argc, char* argv[])
{
	// Intenta inicializar, termina si no puede
	if (!glfwInit())
	{
		cout << "Imposible inicializar GLFW\n\nSaliendo..." << end;
		exit(1);
	}
	// Especificar que función se llamará ante un error de GLFW
	glfwSetErrorCallback(ErrorGLFW);
	// Crear la ventana, activar el rendering context
	ventana_glfw = glfwCreateWindow(ventana_tam_x, ventana_tam_y, "Test", nullptr, nullptr);
	glfwMakeContextCurrent(ventana_glfw); // Necesario para OpenGL
	. . . 
}
```

Una vez creada la ventana, se deben especificar los nombres de las funciones de nuestro programa que deben ser llamadas cuando ocurre un evento:

```c++
void Inicializa_GLFW(int argc, char* argv[])
{
	. . . 
	// Definir cuales son las funciones gestoras de eventos
	glfwSetWindowSizeCallback(ventana_glfw, FGE_CambioTamano);
	glfwSetKeyCallback(ventana_glfw, FGE_PulsarLevantarTecla);
	glfwSetMouseButtonCallback(ventana_glfw, FGE_PulsarLevantarBotonRaton);
	glfwSetCursorPosCallback(ventala_glfw, FGE_MovimientoRaton);
	glfwSetScrollCallback(ventana_glfw, FGE_Scroll);
}
```

```c++
void BucleEventos_GLFW()
{
	redibujar_ventana = true; // Dibujar la ventana por primera vez
	terminar_programa = false; // Activar para terminar
	while (!terminar_programa)
	{
		if (redibujar_ventana)
		{
			VisualizarFrame(); // Dibujar escena
			redibujar_ventana = false; // Evitar que se dibuje continuamente
		}
		glfwWaitEvents(); // Esperar evento y llamar FGE
		terminar_programa = terminar_programam || glfwWindowShouldClose(ventala_glfw);
	}
}
```

> `redibujar_ventana` y `terminar_programa` son variables lógicas globales.

## 3.2 Tipos de Primitivas

En OpenGL, cada primitiva o conjunto de primitivas se especifica mediante una secuencia ordenada de coordenadas de **vertices**, esto es, un punto de un espacio afín 3D. Se representa en memoria mediante una tupla de coordenadas en algún marco de coordenadas de dicho espacio afín. Además, puede tener asociados otros valores, llamados **atributos**.

Existen tres tipos distintos de primitivas, así, una lista de $n$ coordenadas de vértices puede usarse para codificar:
- Si $n \geq 1$, $n$ **puntos** aislados (`GL_POINTS`)
- Si $n \geq 1$, uno o varios **segmentos** de recta, en concreto:
	- Si $n$ par, $\frac{n}{2}$ segmentos independientes (`GL_LINES`)
	- Si $n \geq 2$, $n-1$ segmentos formando una polilínea abierta (`GL_LINE_STRIP`)
	- Si $n \geq 3$, $n$ segmentos formando una polilínea cerrada (`GL_LINE_LOOP`)
- Si $n\geq 1 \land n \equiv 0 \mod 3$, $\frac{n}{3}$ **triángulos**, en concreto:
	- Si $n \geq 3$, $n-2$ triángulos compartinedo aristas, cada triángulo comparte dos vértices con el anterior (`GL_TRIANGLE_STRIP`)
	- Si $n \geq 3$, $n-1$ triángulos compartiendo un vértice (el primero de todos) y cada triángulo comparte dos vértices con el anterior (`GL_TRIANGLE_FAN`)

> Cada primitiva de tipo triángulo es clasificada por OpenGL como **delantera**, si su vértices se visualizan en pantalla en el sentido contrario de las agujas del reloj, o **trasera**, si sus vértices se visualizan en pantalla en el sentido de las agujas del reloj. Este comportamiento de OpenGL puede ser configurado para no visualizar las caras traseras o no visualizar las delanteras (_face culling_).

En el caso de las primitivas de tipo triángulos, OpenGL puede visualizarlos de varias formas, según el valor de un parámetro de configuración que se llama **modo de visualización de polígonos** y permite una de estas opciones:
- **Modo puntos**: Cada triángulo se visualiza como un punto en cada vértice (`GL_POINT`).
- **Modo líneas**: Cada triángulo se visualiza como una polilínea cerrada (`GL_LINE`).
- **Modo relleno**: Cada triángulo se visualiza relleno de color (`GL_FILL`).

> **Problema de vértices replicados**: Muchas veces necesitamos usar unas mismas coordenadas para varios vértices, lo que supone repetir vértices en la secuencia. Esto supone emplear más memoria y/o tiempo para visualizar del necesario.

Las APIs de rasterización permiten especificar una secuencia de vértices (con repeticiones) a partir de una secuencia de vértices únicos:
- Se parte de una secuencia $V_n$ de $n$ coordenadas arbitrarias de vértices $V_n = \{v_0, v_1, \dotsc, v_{n-1}\}$.
- Se usa una secuencia $I_m$ de $m$ índices $I_m = \{i_0, i_1, \dotsc, i_{m-1}\}$ donde cada valor $i_j$ es un entero entre $0$ y $n-1$ (ambos incluidos). Puede tener índices repetidos.
- La secuencia de vértices $V_n$ y la de índices determinan otra secuenci $S_m$ de $m$ vértices que tiene las mismas coordenadas de vértices $V_n$ pero en el orden especificado por los índices en $I_m$
$$S_m = \{v_{i_0}, v_{i_1}, \dotsc, v_{i_{m-1}}\}$$

## 3.3 Atributos de vértices

Las coordenadas de su posición se considera un **atributo** de los vértices, pero en rasterización se pueden opcionalmente usar otros atributos, por ejemplo:
- El **color** del vértice (una terna RGB con valores entre $0$ y $1$).
- Las **coordenadas del vector normal**, un vector unitario con tres coordenadas reales que determina la orientación de la superficie de un objeto en el punto donde está el vértice. Se usa para iluminación.
- Las **coordenadas de textura**, típicamente un par de valores reales que determinan que punto de la textura se fija al vértice.

> En OpenGL a cada vértice siempre se la asocia una tupla por cada atributo, es decir, todo vértice tiene siempre asociado una posición, un color, una normal y unas coordenadas de textura (u otros atributos). Según la configuración del cauce, algunos atributos serán usados o no, pudiendo definir un único valor de un atributo para todos los vértices de una primitiva, o bien especificar un valor para cada vértice.

> El valor de cada atributo está definido en cada pixel donde se proyecta la primitva. Estos valores se calculan durante la rasterización usando **interpolación**.


## 3.4 Almacenamiento de vértices y atributos

Cuando usamos arrays o tablas de coordenadas y atributos en memoria, tenemos dos opciones:
- **Array of Structures, AOS**: Se usa un array donde cada entrada contiene las coordenadas de un vértice y todos sus atributos.
- **Structure of Arrays, SOA**: Se usa una estructura con varios punteros a arrays. Uno de ellos contiene las coordenadas y los otros contienen cada uno una tabla de atributos.

```c++
struct
{
	float posicion[3],
		  color[3],
		  normal[3],
		  coord_text[2];
}
secuenciaAOS[num_vertices];
struct
{
	float posiciones[num_vertices*3], 
		  colores[num_vertices*3],
		  normales[num_vertices*3],
		  coord_text[num_vertices*2];
}
secuanciaSOA;
```

Usaremos la librería estándar de C++ junto con la librería **GLM** para gestionar el almacenamiento de las tablas de atributos e índices de una secuencia de vértices en la aplicación.

```c++
#include<glm>
std::vector<glm::vec3> posiciones; // Coordenadas de posición de los vértices
std::vector<glm::vec3> colores; // Colores de los vértices
std::vector<glm::vec3> normales; // Normales de los vértices
std::vector<glm::vec2> coord_text; // Coordenadas de textura de los vértices
std::vector<unsigned int> indices; // Indices
```

El almacenamiento de las tablas lo realizaremos en la GPU mediante los Vertex Buffer Objects

> Un **Vertex Buffer Object** es una secuencia de bytes contiguos en la memoria de la GPU. Dicho bloque contiene una o varias tablas con coordenadas, colores u otros atributosde vértices.

El uso de este bloque de memoria se hace exclusivamente a través de llamadas a OpenGL. Cada VBO tiene un valor entero único que denominamos **identificador** de VBO, de tipo `GLuint`. Un VBO puede tener atributos o puede tener índices, pero no ambos mezclados.




