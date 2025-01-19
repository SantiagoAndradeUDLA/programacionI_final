#include "funciones.h"
#include <stdlib.h>
#include <time.h>

int main() {
    Zona zonas[NUM_ZONAS];

    srand(time(NULL)); // Inicializa la semilla del generador de números aleatorios

    if (cargar_datos(zonas, NOMBRE_ARCHIVO) != 0) {
        printf("Error al cargar datos o archivo no existente. Se usarán datos por defecto.\n");
        simular_datos_historicos(zonas);
    }
    if(ingresar_datos_actuales(zonas)!=0){
        fprintf(stderr, "Error al ingresar los datos.\n");
        return 1;
    }

    mostrar_predicciones(zonas);

    if (guardar_datos(zonas, NOMBRE_ARCHIVO) != 0) {
        printf("Error al guardar los datos.\n");
        return 1;
    }

    return 0;
}
