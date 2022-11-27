#include <iostream>
#include <vector>
#include <cstring>
#include <math.h>
#include <time.h>
#include <tuple>
#include <fstream>
// Debugging
#include <chrono>
#include <thread>

using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds


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
// struct variable {
//     auto nodo;
//     bool estaenrutaposible;
// };


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

void writeVehiculo (string nombreArchivo){}


void writeArchivo (float dist, vector<nodo> ruta, string nombreArchivo){

    // Eliminamos "/instancias" y ".txt"
    nombreArchivo.erase(0, 11);
    nombreArchivo.erase(nombreArchivo.size() - 4, 4);

    string extension = ".out";
    string pre = "out/";
    string file = pre + nombreArchivo + extension;
    cout << "Escribiendo output en: " << file << endl;
    ofstream myfile (file, ios::app);

    myfile << dist << " ";

    leerRuta(ruta);
    
    int i;
    for (i = 0; i < ruta.size()-1; i++)
    {
        myfile << ruta[i].id << "-";
    }
    myfile << ruta[i].id<< endl;
    myfile.close();

}

// Funcion para determinar si el nodo se encuentra en la ruta que queremos agregar
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

// Funcion para determinar si el nodo se encuentra en:
// Una ruta viable donde me quedo sin capacidad!

bool esta_en_ruta_posible (vector <tuple <float, vector<nodo>>> rutasPosibles, nodo actual, vector <nodo> rutaActual){
    // Esta funcion debe ver si la configuracion de la ruta que yo estoy elaborando con el nodo actual
    // ya esta en las rutas posibles

    int id = actual.id;
    int count = rutasPosibles.size();
    vector <nodo> ruta;

    vector <nodo> rutaOg = rutaActual;
    rutaOg.push_back(actual);               // Se crea copia para guardar la ruta que estamos buscando

    // Buscar que las rutas hagan match, tanto en la posicion como el ID en esa posicion.

    int largo_ruta = rutaOg.size();
    int pos_en_ruta = 0;

    for (int i = 0; i < count; i++)
    {
        ruta = get<1>(rutasPosibles[i]);
        int count2 = ruta.size();

        if (largo_ruta != count2)
        {
            // Los largos de las rutas son distintos, no es la misma config
            continue; // siguiente ruta de posibles
        }
        else{
            // buscamos que coincidan los id en cada posicion de la ruta
            int j = 0;
            for (j = 0; j < count2; j++)
            {
                if (ruta[j].id == rutaOg[j].id)
                {
                    continue;
                }
                else{
                    break;
                }
            }
            
            if (j == count2)
            {
                // Son la misma ruta!
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


vector<nodo> Backtracking(vector <nodo> lista, nodo deposito, Vehiculo& vehiculo, vector<nodo>& ruta_original, string nombreArchivo){

    // 
    // Este algoritmo debe buscar LA MEJOR DE TODAS LAS RUTAS para el conjunto determinado
    // Esta diseñado para considerar cualquier tipo de nodo: Linehaul o Backhaul
    // ruta_original contiene la ruta que lleva el vehiculo, la cual puede ser:
        // Linehaul: solo deposito
        // Backhaul: Deposito + nodos linehaul
    // considera que la demanda actual del vehiculo siempre parte en 0

    // Idea:
    // Nuevo input: MODO -> 0: linehaul 1: backhaul
        // Pensando en como "volver atras" cuando estoy en backhaul y evitar borrar el ultimo nodo de ruta_og?

    // IDEA NUEVA:
    // Hacer backtracking para obtener una ruta que sea factible y minimice las distancias entre ellos.
    // (1) guardar esa configuracion de backtracking en rutas posibles
    // volver un nodo atras y buscar todos los siguientes nuevamente
        // Revisar que la ruta que estoy haciendo y encuentro factible no esta en las rutas posibles ya guardadas
    // Si encuentro una que sea buena y cumpla con las condiciones (considerando que ya estoy casi full capacidad)
    // REPEAT (1)
    // de lo contrario (caso mas probable): se acabaron los nodos y no hay nada factible:
        // alto --, eliminar nodo actual, restar demanda y distancias
    // Desde el nuevo nodo, revisar instanciacion e instanciar el mejor que cumpla con la capacidad
    // al momento de revisar si es factible, REPEAT (1)
    // de lo contrario: no quedan nodos y nada factible:
        // alto--

    // Volver a hacer backtracking
    // 
    // Terminar cuando 

    float capacidad = vehiculo.capacidad;

    int L_actual = 0;
    int alto = 1;

    float min_demanda = 10000000.0;         // THRESHOLD PARA LA MINIMA DEMANDA DE LOS NODOS 
    int clientes_atendidos = 0;     

    vector<nodo> ruta = ruta_original;      // Copiamos la ruta original para "mover" los nodos en ella y no alterar la pasada x param

    vector <tuple <float, vector<nodo>>> rutasPosibles;     // Vector que almacena todas las rutas factibles encontradas
    float min_distancia = 1000000000.0;     // minima distancia recorrida 
    float distancia_actual;                 // Distancia entre nodoActual y Nodo Anterior       VARIABLE SIEMPRE
    int distancia_calculada;                // Distancia calculada en cada iteracion            VARIABLE SIEMPRE

    // Ojo! revisar que pasa en caso de backhaul
    float distancia_total = 0.0;            // distancia total recorrida en este viaje
    
    nodo nodoAnterior, nodoInstanciar;      // Nodos-variable

    int ultimo_sacado = deposito.id;

    int iteraciones = 0;
    // Buscaremos el nodo que minimice la distancia de todas las posibles instanciaciones en cada altura
    while(alto >= 1){

        
        sleep_for(500ms);
        iteraciones += 1;
        cout << iteraciones << endl;
        // cout << alto << "\n";

        // QUEDAN VARIABLES POR INSTANCIAR
        if (L_actual < lista.size())
        {
            nodo nodoActual = lista[L_actual];
            nodoAnterior = ruta.back();

            if (nodoActual.id == nodoAnterior.id) {
                cout << "Soy Yo! jeje " << nodoActual.id << endl;
                L_actual++;
                continue;
            }

            // NO SUPERA LA CAPACIDAD
            if (nodoActual.demanda + vehiculo.demanda < capacidad)
            {   
                // Condicion para probar si esta en la ruta actualmente
                if (esta_en_ruta(ruta, nodoActual))
                {   
                    cout << "Ya esta en ruta el: "<< nodoActual.id <<"\n";
                    L_actual++;
                    continue;
                }
                // Condicion para probar si esta en alguna ruta posible, en la "ruta de rutas" (rutas de otros vehiculos)
                if (esta_en_ruta_posible(rutasPosibles, nodoActual, ruta))
                {
                    cout << "Ya esta en ruta posible el: "<< nodoActual.id <<"\n";
                    L_actual++;
                    continue;
                }

                // NO ESTA EN NINGUNA RUTA, ES CONSISTENTE
                // buscamos la minima demanda posible para crear un threshold
                if (min_demanda > nodoActual.demanda)
                {
                    min_demanda = nodoActual.demanda;
                }
                
                // Se calcula la distancia entre el nodo anterior y el actual
                distancia_calculada = distancia(nodoAnterior, nodoActual);
                cout << "Nodo " << nodoActual.id << " distancia: " << distancia_calculada << endl;

                // si la distancia desde el nodo anterior a este nodo es menor a la conocida por otros nodos a instanciar, guardamos el id
                if (min_distancia > distancia_calculada)
                {   
                    if (nodoActual.id != ultimo_sacado)
                    {
                        nodoInstanciar = nodoActual;    // ALMACENO EL MEJOR NODO POSIBLE PARA METERLO A LA RUTA CUANDO NO QUEDEN POR INSTANCIAR
                        // cout << "Nodo a instanciar min distancia: " << nodoInstanciar.id << endl;
                        min_distancia = distancia_calculada;    // ALMACENO LA DISTANCIA MINIMA PARA SEGUIR COMPARANDO
                    }
                    else{
                        cout << "Me sacaron recien :c" << endl;
                    }
                }
                L_actual++;
                
            }
            // DEMANDA SUPERA CAPACIDAD
            else{
                // aumentamos el contador para llegar al final de la lista, a ver si existe alguno...
                L_actual++;
            }
            
        }

        // NO QUEDAN VARIABLES POR INSTANCIAR
        else {
            cout << "\nNo quedan nodos por instanciar..." << "\n";
            // Aun queda espacio en vehiculo???
            if (nodoInstanciar.demanda + vehiculo.demanda < capacidad)
            {   
                // En caso de que no este en ruta, ni ruta_posible
                if (!esta_en_ruta(ruta, nodoInstanciar) && !esta_en_ruta_posible(rutasPosibles, nodoInstanciar, ruta)){
                    if (nodoInstanciar.id == ultimo_sacado)
                    {   
                        cout << "caso raro!" << endl;
                        // Caso: Saque el nodo recien y aun asi entro aca pq no esta en la ruta actual y no es viable
                         alto --;                // Bajamos un nivel del arbol
                    
                        L_actual = 0;
                        min_distancia = 100000000.0;
                        // Nodo a sacar para backtrack:
                        ultimo_sacado = ruta.back().id;
                        cout<< "Nodo a sacar: " << ruta.back().id << endl;
                        vehiculo.demanda -= ruta.back().demanda;
                        nodoAnterior = ruta[ruta.size() - 2];       // penultimo elemento

                        cout << "distancia_total_antes : " << distancia_total << endl;
                        float distancia_restar = distancia(nodoAnterior, ruta.back());
                        distancia_total -= distancia_restar;        // 
                        cout << "distancia_restar : " << distancia_restar << endl;
                        cout << "distancia_total : " << distancia_total << endl;
                        ruta.pop_back();
                        leerRuta(ruta);
                        cout << '\n';
                        continue;
                    }
                    
                    // ES CONSISTENTE
                    // Procedemos a guardar el nodo en la ruta y aumentar un nivel el arbol, hasta que no queden mas!

                    cout << "...pero aun queda espacio!" << "\n";
                    cout << "Nodo_instanciar: " << nodoInstanciar.id << "\n";
                    cout << "Distancia entre nodos: "<< min_distancia << endl;
                    distancia_actual = min_distancia;           // La distancia actual (de la ruta) es la minima distancia de todos los hijos del arbol
                    vehiculo.demanda += nodoInstanciar.demanda; // sumamos la demanda del vehiculo para agregar el nodo a la ruta
                    distancia_total += min_distancia;           // Distancia total = Minima distancia
                    cout << "Distancia total: " << distancia_total << "\n";
                    cout << "Demanda Vehiculo: " << vehiculo.demanda << "\n";
                    ruta.push_back(nodoInstanciar);             // Agregamos el valor del nodo a laa ruta
                    leerRuta(ruta);
                    alto++;                                     // un nivel mas en el arbol!!!

                    // nodoInstanciar = deposito;
                    L_actual = 0;                               // Reiniciamos el contador de posicion de la lista
                    cout << "min_distancia: " << min_distancia << endl;
                    cout << "alto arbol de busqueda: " << alto << "\n\n";
                    cout << "min_dist reset!" << endl;
                    min_distancia = 100000000.0;  // Reiniciamos el contador de distancia minima para la siguiente instanciacion, en un nuevo nivel del arbol
                    
                    
                    // CREO QUE NO HACE FALTA REINICIAR LA MINIMA DEMANDA CUANDO AGREGO UN NODO A LA RUTA, ES UN TRESHOLD NO?
                    // min_demanda = 10000000.0;     // Se reinicia la minima demanda


                    continue;                       // Subimos de nivel!!!!!!!!
                }
                // Caso contrario: NODO EN LA RUTA
                else{
                    cout<< "El nodo esta en la ruta!!!!" << endl;
                    cout << "Nodo: " << nodoInstanciar.id << endl;
                    
                    // CASO: cuando ya no quedan nodos por instanciar, y el unico posible (mejor) es el mismo nodo origen, pero aun hay capacidad!
                    
                    leerRuta(ruta);

                    // No deberiamos agregarlo como ruta posible... ya que no entrega nada util!

                    // // Se almacena la solucion con su distancia total
                    // tuple <float, vector<nodo>> resultado;
                    // resultado = make_tuple(distancia_total, ruta);
                    // rutasPosibles.push_back(resultado);
                    // cout << "Distancia total: " << distancia_total << "\n";
                    // cout << "Demanda Vehiculo: " << vehiculo.demanda << "\n";
                    // cout << "Agregado a rutas posibles!" << endl;

                    // string xd;
                    // cin >> xd;

                    // Se reinician los parametros para volver a iniciar backtracking.
                    alto --;                // Bajamos un nivel del arbol
                    
                    L_actual = 0;
                    min_distancia = 100000000.0;
                    // Nodo a sacar para backtrack:
                    ultimo_sacado = ruta.back().id;
                    cout<< "Nodo a sacar: " << ruta.back().id << endl;
                    vehiculo.demanda -= ruta.back().demanda;
                    nodoAnterior = ruta[ruta.size() - 2];       // penultimo elemento

                    cout << "distancia_total_antes : " << distancia_total << endl;
                    float distancia_restar = distancia(nodoAnterior, ruta.back());
                    distancia_total -= distancia_restar;        // 
                    cout << "distancia_restar : " << distancia_restar << endl;
                    cout << "distancia_total : " << distancia_total << endl;
                    ruta.pop_back();
                    leerRuta(ruta);
                    cout << '\n';

                    // Que hacemos aca?!?!?!
                    continue;
                }
                
            }
            //Caso: No queda espacio en el vehiculo! ¿Deberia terminar la ruta?
            else{
                // Capacidad maxima, deberiamos terminar la ruta??
                // Backtracking?!

                cout << "No queda espacio en vehiculo! \n";
                cout << "Demanda: " << vehiculo.demanda << endl;
                cout << "capacidad: " << capacidad << endl;
                cout << "min_demanda: "<< min_demanda << endl;
                
                // Si no queda espacio y la diferencia es posible suplir por el nodo con menor demanda
                if (capacidad - vehiculo.demanda > min_demanda )
                {
                    cout << "Caso: queda espacio suplible por min_demanda" << endl;
                    /* ir atras dos nodo en la ruta y continuar */
                    alto --;
                    nodoAnterior = ruta[ruta.size() - 2];       // penultimo elemento

                    cout << "distancia_total_antes : " << distancia_total << endl;
                    float distancia_restar = distancia(nodoAnterior, ruta.back());
                    distancia_total -= distancia_restar;        // 
                    cout << "distancia_restar : " << distancia_restar << endl;
                    cout << "distancia_total : " << distancia_total << endl;
                    // Para el calculo de las distancias, restamos el ultimo de la ruta con el nodo anterior
                    
                    L_actual = 0;
                    cout << "min_dist reset!" << endl;
                    // Nodo a sacar para backtrack:
                    ultimo_sacado = ruta.back().id;
                    cout<< "Nodo a sacar: " << ruta.back().id << endl;
                    vehiculo.demanda -= ruta.back().demanda;
                    ruta.pop_back();
                    leerRuta(ruta);


                    // nodoAnterior = ruta[ruta.size() - 2];       // penultimo elemento

                    // cout << "distancia_total_antes : " << distancia_total << endl;
                    // distancia_restar = distancia(nodoAnterior, ruta.back());
                    // distancia_total -= distancia_restar;        // 
                    // cout << "distancia_restar : " << distancia_restar << endl;
                    // cout << "distancia_total : " << distancia_total << endl;
                    // // Para el calculo de las distancias, restamos el ultimo de la ruta con el nodo anterior
                    
                    // L_actual = 0;
                    // min_distancia = 100000000.0;
                    // // Nodo a sacar para backtrack:
                    // ultimo_sacado = ruta.back().id
                    // cout<< "Nodo a sacar: " << ruta.back().id << endl;
                    // vehiculo.demanda -= ruta.back().demanda;
                    // ruta.pop_back();
                    // leerRuta(ruta);


                    cout << '\n';

                    continue;       // Backtracking!

                }
                


                leerRuta(ruta);
                cout << "\n";
                // Se almacena la solucion con su distancia total
                tuple <float, vector<nodo>> resultado;
                resultado = make_tuple(distancia_total, ruta);

                writeArchivo(distancia_total, ruta, nombreArchivo);
                rutasPosibles.push_back(resultado);
                cout << "Distancia total: " << distancia_total << "\n";
                cout << "min_distancia: " << min_distancia << endl;
                cout << "Agregado a rutas posibles!" << endl;

                // string xd;
                // cin >> xd;

                // SE HACE BACKTRACKING, PARA BUSCAR UNA NUEVA RUTA POSIBLE, RECORDAR QUE NO SOLO BUSCAMOS FACTIBLE SINO QUE SEA LA MEJOR
                alto --;
                nodoAnterior = ruta[ruta.size() - 2];       // penultimo elemento

                cout << "distancia_total_antes : " << distancia_total << endl;
                float distancia_restar = distancia(nodoAnterior, ruta.back());
                distancia_total -= distancia_restar;        // 
                cout << "distancia_restar : " << distancia_restar << endl;
                cout << "distancia_total : " << distancia_total << endl;
                // Para el calculo de las distancias, restamos el ultimo de la ruta con el nodo anterior
                
                L_actual = 0;
                cout << "min_dist reset!" << endl;
                min_distancia = 100000000.0;
                //min_demanda = 10000000.0;
                // Nodo a sacar para backtrack:
                cout<< "Nodo a sacar: " << ruta.back().id << endl;
                vehiculo.demanda -= ruta.back().demanda;
                ruta.pop_back();
                leerRuta(ruta);
                cout << '\n';

                continue;       // Backtracking!
            }
            
            // no entiendo bien que hice aca, porque tendria que sacar 2 nodos al mismo tiempo?

            // Si la capacidad del vehiculo excede la demanda del vehiculo + minima demanda (aun queda espacio) despues de haber sacado 1
            if (capacidad - vehiculo.demanda > min_demanda)
            {   
                cout << "Nada por instanciar..." << "\n";
                alto --;
                cout << "alto: " << alto << endl;
                L_actual = 0;
                cout << "min_dist reset!" << endl;
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


    // ================================================================================ //
    // funcion para ejecutar backtracking Q veces (la cantidad de vehiculos)            //
    // para determinar la mejor ruta para cada uno, reduciendo el espacio de busqueda   //
    // ================================================================================ //

vector <vector <nodo>> Rutas_Vehiculos(int maxTiempo, vector <nodo> listaLinehaul, vector<nodo> listaBackhaul, nodo deposito, vector<Vehiculo> listaVehiculos, string nombreArchivo){

    // IDEAS:
    // 
    // Para el contador de tiempo, es necesario que el algoritmo determine cuando terminar.
    // Creo que es buena idea hacer que el backtracking tenga contador
    // cosa de que cuando el contador acabe, retorne todas las rutas posibles y luego se revise los 
    // la mejor ruta a elegir.

    // Otra opcion es que esta funcion use todo el tiempo, y divida la cantidad asignada en Q para darle ese tiempo de ejecucion al BT



    // Solucion final
    vector <vector <nodo>> rutas;
    // constantes para la iteracion
    
    int Q = listaVehiculos.size();
    Q = 1;
    int i = 0;
    unsigned t0, t1;
    t0 = clock();


    // Por cada vehiculo, buscamos una ruta factible para ingresar.
    while(i < Q){
        Vehiculo vehiculo = listaVehiculos[i];
        vector <nodo> ruta = {deposito}; 
        vector <nodo> rutaFinal;
        leerRuta(ruta);
        vector<nodo> rutaL = Backtracking(listaLinehaul, deposito, vehiculo, ruta, nombreArchivo);
        leerRuta(rutaL);

        string input;
        cin >> input;

        // eliminar los nodos de la ruta en linehaul

        for (int j = 0; j < rutaL.size(); j++)
        {
            sacar_nodo(listaLinehaul, rutaL[j]);
        }
        

         // cout << "Ruta: " << ruta << endl;
        if (!listaBackhaul.empty())
        {
            rutaFinal = Backtracking(listaBackhaul, deposito, vehiculo, ruta, nombreArchivo);
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
    
    string nombreArchivo = argv[1];

    vector <nodo> ruta = {nodo{
        1, 0, 0, 0
    }};
    writeArchivo(2.0, ruta, nombreArchivo);
    
    sleep_for(3000ms);
    
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

    vector <vector <nodo>> rutas = Rutas_Vehiculos(maxTiempo, listaLinehaul, listaBackhaul, deposito, listaVehiculos, nombreArchivo);

    return 0;
}