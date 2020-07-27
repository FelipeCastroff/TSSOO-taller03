# TSSOO - taller 3

Felipe Guillermo Castro Aguilar - <felipe.castroa@alumnos.uv.cl>

>`Universidad de Valparaíso`

## Introducción
El presente documento esta sujeto y basado en el informe técnico presentado en este repositorio. 
Una vez analizado el proceso de diseño presentado en el informe mencionado, se trabajó a través del lenguaje de programación c++, con el cual manejaremos threads para cumplir con el objetivo del taller. Es importante destacar que el objetivo de este taller es implementar un programa que llene un arreglo de números enteros y luego sume los datos. Cada tarea se debe realizar de forma paralela, implementadas con OpenMP.

Además se agregó el código del taller dos el cual se solicito la creación de hilos para cumplir la misma tarea, el objetivo de incluir dicho código es solo para comparar los tiempos de ejecución y llegar a una conclusión luego de visualizar y analizar los tiempos.

Es destacable mencionar que para la ejecución del código se debe ejecutar con los parámetros necesarios para su correcta implementación. La manera de ejecutar el código es la siguiente:
> ./ sumArray -N **_nro_** -t  **_nro_** -l  **_nro_** -L  **_nro_** [-h]
>
>Los parámetros son los siguientes:
>
>-N : tamaño del arreglo.
>
>-t : número de threads.
>
>-l : limite inferior rango aleatorio.
>
>-L : límite superior rango aleatorio.
>
>[-h] : muestra la ayuda de uso y termina. 
>
Junto con lo anterior se agrego un parámetro no obligatorio al final de la forma de uso mostrada. El parámetro **-v** muestra información adicional la cual valida la suma, la captura de los datos entregados por parámetros y muestra los datos obtenidos en el taller dos los cuales se usaron para lograr las comparativas necesarias.

## Diseño de la solución

Dentro del problema planteado se nos solicitó crear dos módulos los cuales se encarguen de distintas tareas por lo tanto el diseño de solución se dividirá en dos sub-secciones, una encargada de almacenar los valores aleatorios en un arreglo base y la otra encargada de sumar aquellos datos de manera paralela.

Es destacable que se generaron dos apartados los cuales son el llenado de manera secuencial, la suma secuencial y la implementación manual de threads del taller dos, esto debido a que utilizaran para comparar los tiempos de ejecución y lograr una gráfica la cual demuestre y respalde el correcto uso de OpenMP y generar un análisis de los resultados. Además de la comparativa de tiempo de ejecución a realizar también se generó una sección en el código la cual esta destinada al calculo del desempeño generado por OpenMP contra las dos implementaciones con las cuales se compara. Dicha sección muestra cuantas veces es más eficiente la implementación de OpenMP vs. método secuencial vs. método thread(Implementación taller dos).

Cada uno de los módulos y apartados fueron medidos en termino de tiempo de ejecución mediante la función:
~~~
start = std::chrono::system_clock::now();

// Código a medir //

end = std::chrono::high_resolution_clock::now(); 
elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
auto "Nombre de la variable" = elapsed.count();
~~~
Esta función permite medir el tiempo de ejecución de un determinado código dentro de los parámetros establecidos(start - end).

### Módulo uno

En este módulo se implemento un arreglo cuya funcionalidad es almacenar los datos aleatorios generados por la función **std::random_device** para luego a través de OpenMP almacenarlos de manera paralela con una cantidad ***n*** de hilos, estos hilos dependen únicamente de los parámetros de entrada. La API se encarga de paralelizar el código encargado de llenar el arreglo base con los datos aleatorios, mediante la ejecución de un módelo fork-join().

### Módulo dos

En este segundo módulo se ocupó directamente la API OpenMP para sumar los valores almacenados en el arreglo base, para ello se ocupo la función:
~~~
reduction(+:"valor acumulable")
~~~
Esta función se encarga de realizar una operación de reducción sobre las variables que aparecen en la lista utilizando el operador/intrínseco especificado, que para este caso es **+**.
Una vez obtenido el resultado obtenido se procedió a comparar los resultados generados por OpenMP y los realizados de manera secuencial.

## Conclusión

En última instancia, se puede obtener los tiempos de ejecución de cada uno de los módulos y compararlos con la ejecución secuencial para verificar el correcto desarrollo. Además, luego de compararlos con la implementación generada en el taller dos se puede evidenciar que OpenMP es una API la cual cumple con la paralelisación de manera similar a la lograda pero mejorando en terminos de tiempo. Se desprende del desarrollo del código generado en este taller que existen herramientas que pueden ahorrar codificación y ganar en eficiencia, esto último hace referencia al desarrollo e implementación de thread en el taller dos. Cómo último agregado puedo decir que la utilización de OpenMP si bien es simple a primera vista, conlleva un cierto grado de dificutlad, ya que se deben entender los términos necesarios para poder entenderla. El análisis de los tiempos obtenidos luego de la ejecución del código se encuentran detallados en el informe técnico del mismo repositorio.