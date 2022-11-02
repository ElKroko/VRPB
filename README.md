## Primera Implementacion: VRPB + BT
Codigo fuente para segundo informe VRPB, escrito en c++.

Vicente Perelli - ROL: 201756594-2

### Ejecucion
Para la correcta ejecucion del programa, es necesario:
- Compilar el archivo utilizando `make`
- Ejecutar el archivo `./main` pasando como argumento la ruta de una instancia a ejecutar, por ejemplo:
  - `./main instancias/GA1.txt`

Para limpiar el directorio de archivos compilados:
- `make clean`


#### Consideraciones
El codigo compila, pero no ejecuta el algoritmo completamente dado que tuve un problema con los calculos de distancias totales al momento de hacer backtracking, y por tiempo no alcance a terminarlo. 
De todas formas, al ejecutar el codigo en terminal se muestra como va construyendo las rutas de manera iterativa, haciendo backtracking en aquellos nodos donde no se encuentran mas nodos por instanciar.