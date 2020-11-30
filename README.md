# Trabajo Práctico TLA - Lenguaje U3D

### Instrucciones de uso:
> - Descargar e instalar la JVM (Java Virtual Machine)
>- Descargar e instalar Lex y Yacc con el comando  **sudo apt-get install bison flex**
>- Descargar la última versión de Processing  https://processing.org/download/
> - Clonar el proyecto Git.
> - Setear la variable de entorno U3DRE_PATH con el path hacia la carpeta descargada de processing (por ejemplo, la carpeta se puede haber descargado con el nombre “processing-3.5.4”).
>>  - Para setear la variable de entorno, en Linux se puede usar el comando:
**export U3DRE_PATH=\<path>**
> - Estando en la carpeta root del proyecto, compilar el programa compilador con:
>> - **make**: compila el proyecto de la manera estándar y genera el ejecutable u3dc.
>> - **make -o \<carpeta-de-output>** hace lo mismo pero permite elegir el nombre de la carpeta de output.
>> - **make -w** compila el proyecto para el sistema operativo Windows.
>> - **make -l** compila el proyecto para el sistema operativo Linux (opción por default).
>> - **make -p** solamente genera el output de código Processing.
> - Correr el compilador con **./u3dc <path_hacia_algún_test>**. Si el programa test es correcto, se generará una carpeta U3Dout. Allí dentro, se encuentra el ejecutable llamado U3Dout que al correr llamará a Processing para que muestre visualmente el resultado.



### Programas de ejemplo
> Para ver el funcionamiento de U3D, en la carpeta /test se encuentran programas de ejemplo. Su propósito es mostrar la capacidad que tiene nuestro lenguaje de generar dibujos creativos y dinámicos en 3D.