# TSSOO - taller 3

Felipe Guillermo Castro Aguilar - <felipe.castroa@alumnos.uv.cl>

>`Universidad de Valparaíso`

## Introducción
El presente documento esta sujeto y basado en el informe técnico presentado en este repositorio. 
Una vez analizado el proceso de diseño presentado en el informe mencionado, se trabajó a través del lenguaje de programación c++, con el cual manejaremos threads para cumplir con el objetivo del taller. Es importante destacar que el objetivo de este taller es implementar un programa que llene un arreglo de números enteros y luego sume los datos. Cada tarea se debe realizar de forma paralela, implementadas con OpenMP.

Ademas se agregó el código del taller dos el cual se solicito la creación de hilos para cumplir la misma tarea, el objetivo de incluir dicho código es solo para comparar los tiempos de ejecución y llegar a una conclución luego de visualizar y analisar los tiempos.

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

Dentro del problema planteado se nos solicitó crear dos módulos los cuales se encarguen de distintas tareas por lo tanto el diseño de solución se dividirá en dos sub-secciones.

Antes de entrar al diseño y desarrollo es destacable que la lógica del código enfocado a cumplir con el objetivo del taller inicia en la linea 158 y finaliza en la linea 209. Obviamente presenta vectores, variables y arreglos compartidos con los otras secciones. El código fuera de ese rango corresponde al desarrollo del taller dos el cual nos ayuda a comparar los tiempos de ejecución para llegar a una conclución consolidada.

