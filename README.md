## Primera Implementacion: VRPB + BT
Codigo fuente para VRPB, escrito en C++.

Vicente Perelli - ROL: 201756594-2

### Ejecucion
Para la correcta ejecucion del programa, es necesario:
- Compilar el archivo utilizando `make`
- Ejecutar el archivo `./main` pasando como argumento la ruta de una instancia a ejecutar, por ejemplo:
  - `./main instancias/GA1.txt`

Para probar todas las instancias existentes en la carpeta `/instancias`, se ha creado un :
- `./runall.sh`

Para limpiar el directorio de archivos compilados:
- `make clean`



#### Consideraciones
Para ejecutar el codigo correctamente es necesario utilizar la version C++17, esta version de compilador esta especificada dentro del makefile.

El codigo compila, y ejecuta el algoritmo completamente para 41/62 instancias. Al ejecutar el codigo en terminal se muestra como va construyendo las rutas de manera iterativa, y ademas de los archivos formalmente solicitados como salida del programa, se ha creado una carpeta donde para cada instancia se imprimen todas las rutas generadas consideradas como factibles para el problema (posibles/instancia_posibles.out).


