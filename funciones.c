#include "funciones.h"
#include <stdlib.h>
#include <string.h>

float promedio_ponderado(const float *valores, int n) {
    if (valores == NULL || n <= 0) return 0; // Manejo de errores
    float suma_ponderada = 0;
    float suma_pesos = 0;
    for (int i = 0; i < n; i++) {
        float peso = (float)(n - i) / n;
        suma_ponderada += valores[i] * peso;
        suma_pesos += peso;
    }
    return suma_ponderada / suma_pesos;
}

int guardar_datos(const Zona *zonas, const char *nombre_archivo) {
    if (zonas == NULL || nombre_archivo == NULL) return 1; // Manejo de errores
    FILE *archivo = fopen(nombre_archivo, "wb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo para escritura");
        return 1;
    }
    size_t elementos_escritos = fwrite(zonas, sizeof(Zona), NUM_ZONAS, archivo);
    fclose(archivo);
    return (elementos_escritos != NUM_ZONAS);
}

int cargar_datos(Zona *zonas, const char *nombre_archivo) {
    if (zonas == NULL || nombre_archivo == NULL) return 1; // Manejo de errores
    FILE *archivo = fopen(nombre_archivo, "rb");
    if (archivo == NULL) {
        printf("Archivo no encontrado, se inicializarán los datos.\n");
        for (int i = 0; i < NUM_ZONAS; i++) {
            snprintf(zonas[i].nombre, 50, "Zona %d", i + 1);
        }
        return 0; // No es un error crítico
    }
    size_t elementos_leidos = fread(zonas, sizeof(Zona), NUM_ZONAS, archivo);
    fclose(archivo);
    if (elementos_leidos != NUM_ZONAS) {
        printf("Error al leer todos los datos del archivo. Datos posiblemente corruptos.\n");
        return 1;
    }
    return 0;
}

void simular_datos_historicos(Zona *zonas) {
    if (zonas == NULL) return; // Manejo de errores
    for (int i = 0; i < NUM_ZONAS; i++) {
        for (int d = 0; d < DIAS_HISTORICOS; d++) {
            for (int j = 0; j < NUM_CONTAMINANTES; j++) {
                zonas[i].historico[d].niveles[j] = (float)rand() / RAND_MAX * 100;
            }
        }
    }
}

void mostrar_predicciones(const Zona *zonas) {
    if (zonas == NULL) return; // Manejo de errores
    for (int i = 0; i < NUM_ZONAS; i++) {
        printf("Predicciones para %s en las próximas 24 horas:\n", zonas[i].nombre);
        for (int j = 0; j < NUM_CONTAMINANTES; j++) {
            float valores_historicos[DIAS_HISTORICOS];
            for (int k = 0; k < DIAS_HISTORICOS; k++) {
                valores_historicos[k] = zonas[i].historico[k].niveles[j];
            }
            float prediccion = promedio_ponderado(valores_historicos, DIAS_HISTORICOS);
            printf("Contaminante %d: %.2f\n", j + 1, prediccion);
        }
    }
}

int ingresar_datos_actuales(Zona *zonas){
        if (zonas == NULL) return 1;
    for (int i = 0; i < NUM_ZONAS; i++)
    {
        printf("Ingrese los datos actuales para %s:\n", zonas[i].nombre);
        for (int j = 0; j < NUM_CONTAMINANTES; j++)
        {
            printf("Nivel de contaminante %d: ", j+1);
            if (scanf("%f", &zonas[i].actual.niveles[j]) != 1) {
                fprintf(stderr, "Error al leer el nivel del contaminante %d para %s. Ingrese un valor numérico.\n", j + 1, zonas[i].nombre);
                return 1; //Indica un error.
            }
        }
        printf("Temperatura: ");
        if (scanf("%f", &zonas[i].actual.temperatura) != 1) {
            fprintf(stderr, "Error al leer la temperatura para %s. Ingrese un valor numérico.\n", zonas[i].nombre);
            return 1;
        }
        printf("Velocidad del viento: ");
        if (scanf("%f", &zonas[i].actual.velocidad_viento) != 1) {
            fprintf(stderr, "Error al leer la velocidad del viento para %s. Ingrese un valor numérico.\n", zonas[i].nombre);
            return 1;
        }
        printf("Humedad: ");
        if (scanf("%f", &zonas[i].actual.humedad) != 1) {
            fprintf(stderr, "Error al leer la humedad para %s. Ingrese un valor numérico.\n", zonas[i].nombre);
            return 1;
        }
    }
    return 0;
}
