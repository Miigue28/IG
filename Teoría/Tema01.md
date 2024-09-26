
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

