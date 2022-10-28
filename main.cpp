#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <time.h>

using namespace std;

// #include <bits/stdc++.h>

/*
=========== Formato de Entrada ==============
# Nodes
TipoDeNodo (0=depot, 1=linehaul, 2=backhaul) IDNodo CoordX CoordY
...
# Vehiculos Capacidad
IDNodo Demanda
...
=============================================

nombre_instancia.out (Si lee GA1.txt => GA1.out)
============= Formato de Salida =============
CalidadDeSolucion #ClientesAtendidos #Vehiculos TiempoDeEjecucion [s]
RutaVehiculo1 DistanciaRecorrida DemandaLinehaul DemandaBackhaul
RutaVehiculo2 DistanciaRecorrida DemandaLinehaul DemandaBackhaul
...
=============================================

Tecnicas completas deben mostrar las rutas que se tengan hasta el momento de parada del algoritmo.
Sea por evento por consola, o limite de tiempo definido.


*/
// struct para representar un nodo, con tipo, coordenadas y demanda.
struct nodo{
    int id;
    int tipo;
    float x;
    float y;
    float demanda;
    int visitado = 0;
};

// struct para representar un vehiculo.
struct Vehiculo {
    int id;
    float capacidad;
    vector <string> ruta;
    int clientesAtendidos;
    float distanciaRecorrida;
    float demandaLinehaul;
    float demandaBackhaul;

    Vehiculo(int a, float b){
        id = a;
        capacidad = b;
    }
};

// struct para hacer variable?!

// Funcion para calcular distancia entre 2 nodos de forma euclideana.
// recibe 2 nodos y retorna un float que corresponde a la distancia calculada

float distancia(nodo a, nodo b){
    float x = b.x - a.x;
    float y = b.y - a.y;
    float distancia;
    distancia = pow(x,2) + pow(y,2);
    distancia = sqrt(distancia);
    return distancia;
}

int lecturaArchivos(int vehiculos, float capacidad, char* argv[], nodo deposito, vector<nodo> listaLinehaul, vector<nodo> listaBackhaul) {
    // apertura de archivo y lectura
    FILE * instancia;
    int clientes;
    int cantL = 0, cantB = 0;

    instancia = fopen(argv[1], "r");

    fscanf(instancia, "%d", &clientes);

    int tempid, temptipo;
    float tempx, tempy, tempdemanda;

    

    fscanf(instancia, "%d %d %f %f",&temptipo, &tempid, &tempx, &tempy);

    deposito.id = tempid;
    deposito.tipo = temptipo;
    deposito.x = tempx;
    deposito.y = tempy;

    for (int i = 0; i < clientes-2; i++){
        fscanf(instancia, "%d %d %f %f",&temptipo, &tempid, &tempx, &tempy);
        
        switch (temptipo)
        {
        case 1:
            cantL++;
            listaLinehaul.push_back(
                nodo{
                    tempid, temptipo, tempx, tempy
            });
            break;
        case 2:
            cantB++;
            listaBackhaul.push_back(
                nodo{
                    tempid, temptipo, tempx, tempy
            });
            break;
        default:
            cout << "Invalid Tipo!";
            break;
        }

    }

    fscanf(instancia, "%d %f", &vehiculos, &capacidad);

    for (int j = 0; j < cantL-1; j++){
        fscanf(instancia, "%d %f",&tempid, &tempdemanda);
        listaLinehaul[j].demanda = tempdemanda;
    }

    
    for (int k = 0; k < cantB-1; k++){
        fscanf(instancia, "%d %f",&tempid, &tempdemanda);
        listaBackhaul[k].demanda = tempdemanda;
    }

    return 0;
}

int leerLista(vector <nodo> lista){
    int count = lista.size();
    for (int i = 0; i < count; i++)
    {
        cout << "id:" << lista[i].id << "\n";
        cout << "tipo:\t" << lista[i].tipo << "\n";
        cout << "x:\t" << lista[i].x << "\n";
        cout << "y:\t" << lista[i].y << "\n";
    }
    return 0;
}


int main(int arcg, char* argv[]) {
    cout << "Hola, estoy programando en c++!";
    
    vector <nodo> listaLinehaul;
    vector <nodo> listaBackhaul;
    nodo deposito;

    int clientes, vehiculos;
    int nVisitados = 0;
    float capacidad;
    
    cout << "llegue aqui!";
    
    // contador de tiempo!
    int maxTiempo = 180;
    unsigned t0, t1;
    t0 = clock();

    int res = lecturaArchivos(vehiculos, capacidad, argv, deposito, listaLinehaul, listaBackhaul);

    if (res==0) {
        res = leerLista(listaLinehaul);
        res = leerLista(listaBackhaul);
    }




    // while(nVisitados < clientes){

    //     // condicion de termino: tiempo maximo
    //     t1 = clock();
    //     double tiempo = (double(t1-t0)/CLOCKS_PER_SEC);
    //     if (tiempo > maxTiempo) {
    //         cout << "Tiempo limite!";
    //         break;
    //     }
    // }

    return 0;
}