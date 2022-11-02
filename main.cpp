#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <time.h>
#include <tuple>

using namespace std;

int THRESHOLD = 100;

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
    float demanda = 0.0;
    int visitado = 0;
};

// struct para representar un vehiculo.
struct Vehiculo {
    int id;
    float capacidad;
    vector <int> ruta;
    int clientesAtendidos;
    float distanciaRecorrida;
    float demanda = 0.0;
    float demandaLinehaul = 0.0;
    float demandaBackhaul = 0.0;

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

// Funcion para leer el archivo e inicializar los structs necesarios para representar el problema.
// Retorna un nodo deposito.

nodo lecturaArchivos(char* argv[], vector<nodo>& listaLinehaul, vector<nodo>& listaBackhaul, vector<Vehiculo>& listaVehiculos) {
    // apertura de archivo y lectura
    FILE * instancia;
    int clientes;
    int cantL = 0, cantB = 0;

    int vehiculos;
    float capacidad;

    instancia = fopen(argv[1], "r");

    fscanf(instancia, "%d", &clientes);

    int tempid, temptipo;
    float tempx, tempy, tempdemanda;
    nodo deposito;

    

    fscanf(instancia, "%d %d %f %f",&temptipo, &tempid, &tempx, &tempy);

    deposito.id = tempid;
    deposito.tipo = temptipo;
    deposito.x = tempx;
    deposito.y = tempy;

    for (int i = 0; i < clientes-1; i++){
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
            cout << "Invalid Tipo!\n";
            break;
        }

    }

    fscanf(instancia, "%d %f", &vehiculos, &capacidad);

    for (int i = 0; i < vehiculos; i++){
        listaVehiculos.push_back(
            Vehiculo(i, capacidad)
        );
    }



    for (int j = 0; j < cantL; j++){
        fscanf(instancia, "%d %f",&tempid, &tempdemanda);
        listaLinehaul[j].demanda = tempdemanda;
    }

    
    for (int k = 0; k < cantB; k++){
        fscanf(instancia, "%d %f",&tempid, &tempdemanda);
        listaBackhaul[k].demanda = tempdemanda;
    }


    return deposito;
}

void leerLista(vector <nodo> lista){
    int count = lista.size();
    for (int i = 0; i < count; i++)
    {  
        cout << "id:\t" << lista[i].id << "\n";
        cout << "tipo:\t" << lista[i].tipo << "\n";
        cout << "x:\t" << lista[i].x << "\n";
        cout << "y:\t" << lista[i].y << "\n";
        cout << "demanda:" << lista[i].demanda << "\n\n";
    }
}

void leerNodo (nodo Node) {
    cout << "id:\t" << Node.id << "\n";
    cout << "tipo:\t" << Node.tipo << "\n";
    cout << "x:\t" << Node.x << "\n";
    cout << "y:\t" << Node.y << "\n";
    cout << "demanda:" << Node.demanda << "\n\n";
}

void leerRuta (vector <nodo> ruta){
    int count = ruta.size();
    cout << "Leer Ruta: ";
    int i = 0;
    for (i = 0; i < count-1; i++)
    {
        cout << ruta[i].id << "-";
    }
    cout << ruta[i].id << "\n";
    
}


bool esta_en_ruta(vector <nodo> ruta, nodo actual){
    int id = actual.id;
    int count = ruta.size();

    for (int i = 0; i < count; i++)
    {
        if (id == ruta[i].id)
        {
            return true;
        }
        
    }
    return false;
    
}

bool esta_en_ruta_posible (vector <tuple <float, vector<nodo>>> rutasPosibles, nodo actual){
    int id = actual.id;
    int count = rutasPosibles.size();
    vector <nodo> ruta;

    for (int i = 0; i < count; i++)
    {
        ruta = get<1>(rutasPosibles[i]);
        int count2 = ruta.size();
        for (int j = 0; j < count2; j++)
        {
            if (id == ruta[j].id)
            {
                return true;
            }
        }
        
    }
    return false;
}

void sacar_nodo(vector <nodo>& lista, nodo nodo_a_sacar){
    int id = nodo_a_sacar.id;
    int count = lista.size();

    for (int i = 0; i < count; i++)
    {
        if (id == lista[i].id)
        {
            lista.erase(lista.begin() + i);
        }
        
    }

}


vector<nodo> Backtracking(vector <nodo> lista, nodo deposito, Vehiculo& vehiculo, vector<nodo>& ruta_original){

    float capacidad = vehiculo.capacidad;

    int L_actual = 0;
    int alto = 1;

    float min_demanda = 10000000.0;
    int clientes_atendidos = 0;

    vector<nodo> ruta = ruta_original;

    vector <tuple <float, vector<nodo>>> rutasPosibles;

    float min_distancia = 1000000000.0, distancia_actual;
    int id_min;

    int distancia_calculada;
    float distancia_total = 0.0;
    nodo nodoAnterior, nodoInstanciar;

    while(alto >= 1){
        // cout << alto << "\n";
        if (L_actual < lista.size())
        {
            nodo nodoActual = lista[L_actual];
            nodoAnterior = ruta.back();

            if (esta_en_ruta(ruta, nodoActual))
            {
                // cout << "Ya esta en ruta el: "<< nodoActual.id <<"\n";
                L_actual++;
                continue;
            }

            if (esta_en_ruta_posible(rutasPosibles, nodoActual))
            {
                // cout << "Ya esta en ruta posible el: "<< nodoActual.id <<"\n";
                L_actual++;
                continue;
            }
            

            // cout << "entre aqui!";
            // Aun quedan nodos por instanciar
            if (nodoActual.demanda + vehiculo.demanda < capacidad)
            {   
                // cout << "Nodo " << nodoActual.id << " entro! \n";
                // buscamos la minima demanda posible para crear un threshold
                if (min_demanda > nodoActual.demanda)
                {
                    min_demanda = nodoActual.demanda;
                }
                
                // Se calcula la distancia entre el nodo anterior y el actual
                distancia_calculada = distancia(nodoAnterior, nodoActual);

                // si la distancia desde el nodo anterior a este nodo es menor a la conocida por otros nodos a instanciar, guardamos el id
                if (min_distancia > distancia_calculada)
                {   
                    if (!esta_en_ruta(ruta, nodoActual))
                    {
                        nodoInstanciar = nodoActual;
                        // cout << "Nodo a instanciar min distancia: " << nodoInstanciar.id << endl;
                        min_distancia = distancia_calculada;
                    }
                }
                L_actual++;
                
            }else{
                // como manejo el caso en que no entre al if anterior porque la ruta este llena?
                L_actual++;
            }
            
        }
        else {
            cout << "\nNo quedan nodos por instanciar..." << "\n";
            // No quedan nodos por instanciar
            // Aun queda espacio en vehiculo
            

            if (nodoInstanciar.demanda + vehiculo.demanda < capacidad)
            {   
                if (!esta_en_ruta(ruta, nodoInstanciar) && !esta_en_ruta_posible(rutasPosibles, nodoInstanciar)){
                    // string input;
                    // cin >> input;
                    cout << "...pero aun queda espacio!" << "\n";
                    cout << "Nodo_instanciar: " << nodoInstanciar.id << "\n";
                    cout << "Distancia entre nodos: "<< min_distancia << endl;
                    distancia_actual = min_distancia;
                    vehiculo.demanda += nodoInstanciar.demanda;
                    distancia_total += min_distancia;
                    cout << "Distancia total: " << distancia_total << "\n";
                    cout << "Demanda Vehiculo: " << vehiculo.demanda << "\n";
                    ruta.push_back(nodoInstanciar);
                    alto++;

                    // nodoInstanciar = deposito;
                    L_actual = 0;    // Comenzamos la siguiente instanciacion desde el siguiente valor al instanciado en la lista!
                    cout << "alto arbol de busqueda: " << alto << "\n\n";
                    min_distancia = 100000000.0;  // Reiniciamos el contador de distancia minima para la siguiente instanciacion, en un nuevo nivel del arbol
                    min_demanda = 10000000.0;
                    continue;
                }
                else{
                    cout<< "El nodo esta en la ruta!!!!" << endl;

                    leerRuta(ruta);
                    // Se almacena la solucion con su distancia total
                    tuple <float, vector<nodo>> resultado;
                    resultado = make_tuple(distancia_total, ruta);
                    rutasPosibles.push_back(resultado);
                    cout << "Distancia total: " << distancia_total << "\n";
                    cout << "Demanda Vehiculo: " << vehiculo.demanda << "\n";
                    cout << "Agregado a rutas posibles!" << endl;

                    // string xd;
                    // cin >> xd;

                    // Se reinician los parametros para volver a iniciar backtracking.
                    alto --;
                    distancia_total -= distancia_actual;
                    L_actual = 0;
                    min_distancia = 100000000.0;
                    min_demanda = 10000000.0;
                    // Nodo a sacar para backtrack:
                    cout<< "Nodo a sacar: " << ruta.back().id << endl;
                    vehiculo.demanda -= ruta.back().demanda;
                    ruta.pop_back();
                    leerRuta(ruta);
                    cout << '\n';
                    
                }
                
            }
            //Caso: No queda espacio en el vehiculo!
            else{
                cout << "No queda espacio en vehiculo! \n";
                cout << "Demanda: " << vehiculo.demanda << endl;
                cout << "capacidad: " << capacidad << endl;


                leerRuta(ruta);
                cout << "\n";
                // Se almacena la solucion con su distancia total
                tuple <float, vector<nodo>> resultado;
                resultado = make_tuple(distancia_total, ruta);
                rutasPosibles.push_back(resultado);
                cout << "Distancia total: " << distancia_total << "\n";
                cout << "Agregado a rutas posibles!" << endl;

                // string xd;
                // cin >> xd;

                // Se reinician los parametros para volver a iniciar backtracking.
                alto --;
                distancia_total -= distancia_actual;
                L_actual = 0;
                min_distancia = 100000000.0;
                min_demanda = 10000000.0;
                // Nodo a sacar para backtrack:
                cout<< "Nodo a sacar: " << ruta.back().id << endl;
                vehiculo.demanda -= ruta.back().demanda;
                ruta.pop_back();
                leerRuta(ruta);
                cout << '\n';

                continue;
            }
            
            
            if (capacidad - vehiculo.demanda > min_demanda)
            {   
                cout << "Nada por instanciar..." << "\n";
                alto --;
                cout << "alto: " << alto << endl;
                L_actual = 0;
                min_demanda = 10000000.0;
                min_distancia = 100000000.0;
                distancia_total -= distancia_actual;
                vehiculo.demanda -= ruta.back().demanda;
                ruta.pop_back();
                vehiculo.demanda -= ruta.back().demanda;
                ruta.pop_back();
            }
        }
        
    }
    cout << "Distancia total recorrida: " << distancia_total << endl;
    vehiculo.demanda = 0.0;

    int j = 0;
    float min_distancia_final = 100000000000.0;
    vector <nodo> rutaFinal;
    for (j = 0; j < rutasPosibles.size(); j++)
    {
        if (get<0>(rutasPosibles[j]) < min_distancia_final){
            rutaFinal = get<1>(rutasPosibles[j]);
        }
    }
    

    return rutaFinal;
}


vector <vector <nodo>> Rutas_Vehiculos(int maxTiempo, vector <nodo> listaLinehaul, vector<nodo> listaBackhaul, nodo deposito, vector<Vehiculo> listaVehiculos){



    // Solucion final
    vector <vector <nodo>> rutas;
    // constantes para la iteracion
    
    int Q = listaVehiculos.size();
    // int Q = 1;
    int i = 0;
    unsigned t0, t1;
    t0 = clock();


    // Por cada vehiculo, buscamos una ruta factible para ingresar.
    while(i < Q){
        Vehiculo vehiculo = listaVehiculos[i];
        vector <nodo> ruta = {deposito}; 
        vector <nodo> rutaFinal;
        leerRuta(ruta);
        vector<nodo> rutaL = Backtracking(listaLinehaul, deposito, vehiculo, ruta);
        leerRuta(rutaL);
        // eliminar los nodos de la ruta en linehaul

        for (int j = 0; j < rutaL.size(); j++)
        {
            sacar_nodo(listaLinehaul, rutaL[j]);
        }
        

         // cout << "Ruta: " << ruta << endl;
        if (!listaBackhaul.empty())
        {
            rutaFinal = Backtracking(listaBackhaul, deposito, vehiculo, ruta);
            for (int k = 0; k < rutaFinal.size(); k++)
            {
                sacar_nodo(listaLinehaul, rutaL[k]);
            }
        }else{
            rutaFinal = rutaL;
        }
        rutaFinal.push_back(deposito);
        // Eliminar los nodos de la ruta en backhaul
        leerRuta(rutaFinal);
        
        rutas.push_back(rutaFinal);
        i++;
    

    }
    return rutas;
}



int main(int arcg, char* argv[]) {

    if (arcg!=2){
        cout << "Ingrese un archivo!\n";
        exit(1);
    }
    
    vector <nodo> listaLinehaul;
    vector <nodo> listaBackhaul;
    vector <Vehiculo> listaVehiculos;

    int clientes;
    int nVisitados = 0;
    
    
    
    // contador de tiempo!
    int maxTiempo = 180;

    nodo deposito = lecturaArchivos(argv, listaLinehaul, listaBackhaul, listaVehiculos);

    
    cout << "=========== Deposito ===========\n";
    leerNodo(deposito);

    cout << "Linehauls: \t" << listaLinehaul.size() << "\n";
    cout << "Backhauls: \t" << listaBackhaul.size() << "\n\n";

    cout << "Vehiculos: \t" << listaVehiculos.size() << "\n";
    cout << "Capacidad: \t" << listaVehiculos[1].capacidad << "\n\n";

    cout << "=========== Rutas ===========\n\n";

    // leerLista(listaLinehaul);
    // leerLista(listaBackhaul);

    vector <vector <nodo>> rutas = Rutas_Vehiculos(maxTiempo, listaLinehaul, listaBackhaul, deposito, listaVehiculos);

    return 0;
}