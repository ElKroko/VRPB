#include <bits/stdc++.h>
using namespace std;

//Struct para representar un nodo
struct nodo{
    int id;
    char tipo;
    float longitud;
    float latitud;
    int visitado= 0;
};

//struct para representar un vehiculo
//Capacidad del tanque=60 y consumo es de 0.2 galones por milla
struct vehiculo{
    int id;
    float speed;
    float tiempo=0; 
    float maxDistance;
    float maxTime;
    bool completo=false;
    vector <string> ruta;
    float distanciarecorrida;
    float distanciaRestante;
    vehiculo(int a,float b,float c, float d){
        id=a;
        speed=b;
        distanciaRestante=c;
        maxTime=d;
        maxDistance=d;
    }
};
//struct para representar las variables
struct variable{
    int valor;
    bool dominio0=true;
    bool dominio1=true;
};

//Funcion para calcular la distancia entre 2 nodos con la formula de haversine
//recibe como parametro 2 nodos y retorna un float que corresponde a la distancia calculada
float distancia(nodo a, nodo b){
    //diferencias con los valores pasados a radianes
    float diflat=b.latitud/57.29578-a.latitud/57.29578;
    float diflong=b.longitud/57.29578-a.longitud/57.29578;
    float a1=pow(sin(diflat/2),2)+cos(a.latitud/57.29578)*cos(b.latitud/57.29578)*pow(sin(diflong/2),2);
    float c=2*atan2(sqrt(a1),sqrt(1-a1));
    return c*4182.44949;   
}

bool movimientovalidoNodos(vehiculo *autoactual, vector <nodo> &listaNodos,int posinicial, int posfinal, int ServiceTime, int RefuelTime){
    //Calculo de distancia recorrida, tiempo y combustible    
    float distanciarecorrida=distancia(listaNodos[posinicial],listaNodos[posfinal]);
    //verifica si el combustible es suficiente para llegar al nodo de cliente
    if (autoactual->distanciaRestante<distanciarecorrida){
        return false;
    }
    //verifica si al llegar al nodo no se supera el tiempo
    //reminder, poner aqui tambien el refueltime
    if(listaNodos[posfinal].tipo=='c'){
        if (autoactual->tiempo+distanciarecorrida/autoactual->speed + ServiceTime> autoactual->maxTime ){
            return false;
        }
    }else{
        if (autoactual->tiempo+distanciarecorrida/autoactual->speed + RefuelTime> autoactual->maxTime ){
            return false;
        }
    }
    
    return true;
}

bool propagacion(vehiculo *autoactual, vector<vector<variable>> &variables, vector<nodo> &listaNodos,int nodoactual, int ServiceTime,int RefuelTime, float distanciarecorrida, unsigned t0, int tamaniolistanodos){
    
    unsigned t1 = clock();
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    if (time>180){
        return true;
    }
    bool ready;
    //Se revisan todos los movimientos desde el punto actual y si no son válidos, se elimina del dominio de esas variables el 1
    //if para asegurarse de que no vuelva al inicio después del primer movimiento
    for (int i=1; i<tamaniolistanodos;i++){
        if (movimientovalidoNodos(autoactual,listaNodos,nodoactual,i,ServiceTime,RefuelTime)==false and nodoactual!= i){
            variables[nodoactual][i].dominio1=false;
        }
    }
    //Se instancia en orden la primera variable que si tenga el 1 en su dominio
    for (int i=1;i<tamaniolistanodos;i++){
        //si la variable a consultar no tiene el 1 en el dominio o ya a sido visitada o si se compara sobre si misma, se omite
        if(variables[nodoactual][i].dominio1==false or listaNodos[i].visitado==1 or nodoactual==i){
            continue;
        }
        //Se sigue a la siguiente variable, actualizando los valores del vehiculo
        string inicialnodo;
        inicialnodo.push_back(listaNodos[nodoactual].tipo);
        string numeronodo=to_string(nodoactual);
        autoactual->ruta.push_back(inicialnodo+numeronodo);
        autoactual->distanciarecorrida+=distanciarecorrida;
        if (listaNodos[nodoactual].tipo=='c'){
            autoactual->tiempo+=distanciarecorrida/autoactual->speed+ ServiceTime;
            autoactual->distanciaRestante-=distanciarecorrida;        
        }else{
            autoactual->tiempo+=distanciarecorrida/autoactual->speed+ RefuelTime;
            autoactual->distanciarecorrida=autoactual->maxDistance;        
        }        
        float nuevadistancia=distancia(listaNodos[nodoactual],listaNodos[i]);
        //En caso de volver al deposito, se termina la recursión, indicando que se logró hacer una ruta
        if(listaNodos[i].tipo=='d'){
            return true;
        }
        //Se marca el nodo como Visitados
        listaNodos[nodoactual].visitado=1;
        variables[nodoactual][i].valor=1;     
        //Se continua a la siguiente variable               
        ready=propagacion(autoactual,variables,listaNodos,i,ServiceTime,RefuelTime,nuevadistancia,t0,tamaniolistanodos);
        if(ready==true){
            return true;
        }else{
            //reestablecimiento de modificaciones
            autoactual->ruta.pop_back();
            autoactual->tiempo-=distanciarecorrida/autoactual->speed;
            autoactual->distanciaRestante+=distanciarecorrida;
            autoactual->distanciarecorrida-=distanciarecorrida;
            listaNodos[nodoactual].visitado=0; 
            variables[nodoactual][i].valor=0;        
        }       
    }
    //Se comprueba factibilidad de vuelta al origen
    bool vueltaInicio=false;
    if (movimientovalidoNodos(autoactual,listaNodos,nodoactual,0,ServiceTime,RefuelTime)==true and nodoactual!= 0){
        vueltaInicio=true;
    }
    //Si se puede volver al inicio, se vuelve
    if (vueltaInicio){
        //Se sigue a la siguiente variable, actualizando los valores del vehiculo
        string inicialnodo;
        inicialnodo.push_back(listaNodos[nodoactual].tipo);
        string numeronodo=to_string(nodoactual);
        autoactual->ruta.push_back(inicialnodo+numeronodo);
        numeronodo=to_string(0);
        inicialnodo="d";
        autoactual->ruta.push_back(inicialnodo+numeronodo);
        autoactual->distanciarecorrida-=distanciarecorrida;
        variables[nodoactual][0].valor=1;
        if (listaNodos[nodoactual].tipo=='c'){
            autoactual->tiempo+=distanciarecorrida/autoactual->speed+ ServiceTime;
            autoactual->distanciaRestante-=distanciarecorrida;        
        }else{
            autoactual->tiempo+=distanciarecorrida/autoactual->speed+ RefuelTime;
            autoactual->distanciaRestante=autoactual->maxDistance;                    
        }        
        //En caso de volver al deposito, se termina la recursión, indicando que se logró hacer una ruta
        return true;
    }    

    //Se reestablecen los dominios modificados
    for (int i=1; i<tamaniolistanodos;i++){
        if (movimientovalidoNodos(autoactual,listaNodos,nodoactual,i,ServiceTime,RefuelTime)==false and nodoactual!= i){
            variables[nodoactual][i].dominio1=true;
        }
    }    
    return false;
       
}


int main(int argc, char*argv[]) {
    ios_base::sync_with_stdio(false); 
    cin.tie(NULL);
    cout.setf(ios::fixed);
    cout.precision(4);
    //Se asegura de que se ingreso un archivo al ejecutar el programa
    if (argc!=2){
        cout << "Ingrese un archivo\n";
        exit(1);
    }

    
    //vector para almacenar los clientes
    vector <nodo> listaClientes;

    //vector para almacenar las Estaciones
    vector <nodo> listaEstaciones;

    
    //Parametros
    char name[20];
    int customers, stations, MaxTime, MaxDistance, ServiceTime, RefuelTime;
    float Speed;
    
    //apertura de archivo y lectura
    FILE * instancia;

    instancia = fopen(argv[1], "r");

    fscanf(instancia, "%s %d %d %d %d %f %d %d",name,&customers,&stations, &MaxTime, &MaxDistance, &Speed, &ServiceTime, &RefuelTime );
    
    int tempid;
    char tempTipo;
    float tempLong, tempLat;

    //Nodo que representa el deposito (origen)
    nodo deposito;
    fscanf(instancia, "%d %c %f %f",&tempid,&tempTipo,&tempLong,&tempLat);
    deposito.id= tempid;
    deposito.tipo= tempTipo;
    deposito.longitud=tempLong;
    deposito.latitud=tempLat;
    //se leen los nodos de estacion de recarga
    for(int i=0; i<stations; i++){
        fscanf(instancia, "%d %c %f %f",&tempid,&tempTipo,&tempLong,&tempLat);
        listaEstaciones.push_back(nodo{tempid,tempTipo, tempLong,tempLat});
    }
    //se leen los nodos de clientes
    for(int i=0; i<customers; i++){
        fscanf(instancia, "%d %c %f %f",&tempid,&tempTipo,&tempLong,&tempLat);
        listaClientes.push_back(nodo{tempid,tempTipo, tempLong,tempLat});
    }
    fclose(instancia);
    //Esta copia se crea para podet tener 2 veces cada nodo de recarga
    vector <nodo> copialistaEstaciones;
    copialistaEstaciones=listaEstaciones;
    //se combinan todos los vectores en uno sólo para comodidad de trabajo
    vector <nodo> listaNodos;
    listaNodos.push_back(deposito);
    listaNodos.insert(listaNodos.end(),listaClientes.begin(),listaClientes.end());
    listaNodos.insert(listaNodos.end(),listaEstaciones.begin(),listaEstaciones.end());
    listaNodos.insert(listaNodos.end(),copialistaEstaciones.begin(),copialistaEstaciones.end());
    
    //matriz de variables
    vector < vector<variable> > variables(customers+stations+stations+1, vector<variable> (customers+stations+stations+1,variable()));
    
    //comienzo del FC
    //Variables para medir el tiempo
    unsigned t0, t1;
    t0=clock();
    int nVisitados=0;
    int autoactual=1;
    int porVisitar=1;
    vector <vehiculo> listaVehiculos;
    while (nVisitados<customers){
        t1 = clock();        
        double time = (double(t1-t0)/CLOCKS_PER_SEC);
        if (time>180){
            break;
        }
        vehiculo autotemp(autoactual,Speed,MaxTime,MaxDistance); 
        float distanciarecorrida=distancia(listaNodos[0],listaNodos[porVisitar]);          
        //Eleccion de punto de partida válido desde el origen        
        if(movimientovalidoNodos(&autotemp,listaNodos,0,porVisitar, ServiceTime,RefuelTime) and variables[0][porVisitar].valor!=1){
            //Se instancia el primer movimiento y se actualiza el vehiculo
            variables[0][porVisitar].valor=1;
            string inicialnodo;
            inicialnodo.push_back(listaNodos[0].tipo);
            string numeronodo=to_string(0);
            autotemp.ruta.push_back(inicialnodo+numeronodo); 
            //Se hace FC            
            bool exito = propagacion(&autotemp,variables,listaNodos,porVisitar,ServiceTime,RefuelTime,distanciarecorrida,t0,listaNodos.size());
            //Si se logra una ruta con exito, se almacena el vehiculo en una lista de vehiculos
            if (exito==true){
                listaVehiculos.push_back(autotemp);
                for (int i=1; i<customers+1;i++){
                    if(listaNodos[i].visitado==1){
                        nVisitados++;
                    }
                }
                porVisitar++;         
            }else{
                //Se deshace la instanciacion y se revierte la actualizacion del vehiculos
                autotemp.ruta.pop_back(); 
                variables[0][porVisitar].valor=0;
                porVisitar++;
            }
        }else{            
            porVisitar++;
        }
        if(porVisitar==customers){
            porVisitar=1;
        }       
    }
    t1 = clock();
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    if (time>180){
        time=180;
    }

    //Escritura de output
    strcat(name, ".out");
    FILE * resultados = fopen(name,"w");
    float calidadSolucion=0;
    if(listaVehiculos.size()>0){
        for (long unsigned int i=0; i<listaVehiculos.size(); i++){
            calidadSolucion+=listaVehiculos[i].distanciarecorrida;
        }
        fprintf(resultados,"%f %d %ld %f\n",calidadSolucion,nVisitados,listaVehiculos.size(),time );        
        for (long unsigned int j=0; j<listaVehiculos.size();j++){
            for (long unsigned int i=0; i<listaVehiculos[j].ruta.size();i++){ 
                string temp =listaVehiculos[j].ruta[i];
                fwrite(temp.c_str(), 1,temp.size(),resultados);
                fprintf(resultados," ");                               
            }
            string temp = to_string(listaVehiculos[j].distanciarecorrida);
            fwrite(temp.c_str(), 1,temp.size(),resultados);
            fprintf(resultados," ");
            temp= to_string(listaVehiculos[j].tiempo);
            fwrite(temp.c_str(), 1,temp.size(),resultados);
            fprintf(resultados,"\n");
        }          
    }        
    fclose(resultados);

    return 0;
}
