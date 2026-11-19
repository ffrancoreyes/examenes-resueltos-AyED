#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

/*
================== PARTE 1 ================== 
*/

//ejercicio 1

//como vamos a usar un nodo con distintos tipos de datos, podemos usar un template. Aclaración, se puede hacer sin templates, pero el costo es de una estructura más.
template <typename T>
struct Nodo{
    T info;
    Nodo<T> *sgte;
};

struct Ventas{
    int numEntrada;
    char nombreComprador[30];
    float montoPagado;
};

struct Gasto{
    char concepto[30];
    float montoGasto;
};

struct Evento{
    Nodo<Gasto> *pilaGastos = NULL;
    Nodo<Ventas> *lstVentas = NULL;
};

struct Resumen{
    int numDia;
    float totalIngreso;
    float totalGasto;
    float gananciaDia;
};

//funciones que usamos pero no desarrollamos.
Gasto pop(Nodo<Gasto> *&pila);

template <typename T>
void insertarAlFinal(Nodo<T> *&lst, T dato);

//ejercicio 2:

Nodo<Resumen> *procesarEvento(Nodo<Evento> *lstDias){
    Nodo<Resumen> *lstResultante = NULL;
    Resumen datos;
    Nodo<Evento> *auxEv = lstDias;
    int contDias = 1;
    while(auxEv != NULL){
        datos.numDia = contDias;
        datos.totalIngreso = sumaIngresos(auxEv->info.lstVentas);
        datos.totalGasto = sumaGastos(auxEv->info.pilaGastos);
        datos.gananciaDia = datos.totalIngreso - datos.totalGasto;
        insertarAlFinal(lstResultante,datos);
        contDias++;
        auxEv = auxEv->sgte;
    }
    return lstResultante;
}

float sumaIngresos(Nodo<Ventas> *lstVentas){
    Nodo<Ventas> *aux = lstVentas;
    float totalVentas = 0;
    while(aux != NULL){
        totalVentas += aux->info.montoPagado;
        aux = aux->sgte;
    }
    return totalVentas;
}

float sumaGastos(Nodo<Gasto> *&pila){
    float totalGastos = 0;
    while(pila != NULL){
        Gasto info = pop(pila);
        totalGastos += info.montoGasto;
    }
    return totalGastos;
}


/*
================== PARTE 2 ================== 
*/

struct Ingrediente{
    char nombreIngrediente[30];
    float costoIngrediente;
    float cantNecesaria;
    char descripcionIngrediente[30];
};

struct Deposito{
    char nombreIngrediente[30];
    float cantDisponible;
};

/*
template <typename T>
struct Nodo{
    T info;
    Nodo<T> *sgte;
}

- Aclaración: Comenté el nodo porque en este programa ya lo tenemos definido más arriba. Pero en el parcial, lo correcto es redefinirlo porque pertenece a otro ejercicio (de igual modo, preguntar al docente si aplica reescribirlo o no).
*/

struct MateriaPrima{
    char nombreIngrediente[30];
    float cantTotalReq;
    float costoTotalIng;
};

//constantes que podemos usar
const int CANT_CACHORROS = 20;
const int CANT_ADULTOS = 40;


void calcularIngredientes(char rutaArchIngredientes[], Nodo<MateriaPrima> *& lstMateriaNecesaria){
    FILE *archIngredientes = fopen(rutaArchIngredientes,"rb");
    if(archIngredientes == NULL){
        cout << "EL archivo ingredientes no se pudo abrir" << endl;
        return;
    }

    MateriaPrima datos;
    Ingrediente regIngrediente;
    while(fread(&regIngrediente,sizeof(Ingrediente),1,archIngredientes)){
        strcpy(datos.nombreIngrediente,regIngrediente.nombreIngrediente);
        datos.cantTotalReq = regIngrediente.cantNecesaria * (CANT_ADULTOS + (CANT_CACHORROS * 0.5)); //los cachorros comen el 50% menos que un adulto, entonces representan el 50 real de los ingredientes.
        datos.costoTotalIng = regIngrediente.costoIngrediente * datos.cantTotalReq;
        insertarAlFinal(lstMateriaNecesaria,datos);
    }

    fclose(archIngredientes);

}

//como no es una función que nos dan desarrollada, la tenemos que hacer nosotros.
Nodo<Deposito> *buscarLista(Nodo<Deposito> *lstStock, char nombreIngrediente[]){
    Nodo<Deposito> *aux = lstStock;
    Nodo<Deposito> *ret = NULL;
    while(aux != NULL){
        if(strcmp(aux->info.nombreIngrediente, nombreIngrediente) == 0){
            ret = aux;
            break;
        }
        aux = aux->sgte;
    }
    return ret;
}

void mostrarFaltantes(Nodo<MateriaPrima> *lstMatPrimNecesaria, Nodo<Deposito> *lstStock){
    Nodo<MateriaPrima> *auxMatPrim = lstMatPrimNecesaria;
    while(auxMatPrim != NULL){
        //por cada elemento de la lista de materia prima que llegue a necesitar, voy a buscarlo en la lista de stock para saber si me alcanza, si me alcanza, no lo imprimo, si no me alcanza, lo imprimo.
        Nodo<Deposito> *datoStock = buscarLista(lstStock,auxMatPrim->info.nombreIngrediente);
        if(datoStock != NULL){
            //si es distinto de NULL es porque encontré el dato.
            float stockActual = datoStock->info.cantDisponible - auxMatPrim->info.cantTotalReq;
            if(stockActual < 0){
                cout << "Ingrediente: " <<  auxMatPrim->info.nombreIngrediente << " , cantidad necesaria en Grs. : " << (stockActual*-1) << endl;
            }
        }else{
            //si no lo encuentra, lo necesito
            cout << "Ingrediente: " <<  auxMatPrim->info.nombreIngrediente << " , cantidad necesaria en Grs. : " << auxMatPrim->info.cantTotalReq << endl;
        }
        auxMatPrim = auxMatPrim->sgte;
    }
}

/*
================== PARTE 3 ================== 

3) La función guarda los valores 20, 30 y 40 en el archivo notas.dat, pero solamente muestra 30 y 40.
4) La función imprime: "SEVEN".

*/


