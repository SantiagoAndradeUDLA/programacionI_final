#include "funciones.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

float promedio_ponderado(const float *valores, int n, int *error) {
    if (valores == NULL || n <= 0) {
        if (error) *error = 1;
        return NAN;
    }
    float suma_ponderada = 0;
    float suma_pesos = 0;
    for (int i = 0; i < n; i++) {
        float peso = (float)(n - i) / n;
        suma_ponderada += valores[i] * peso;
        suma_pesos += peso;
    }
    if (error) *error = 0;
    return suma_ponderada / suma_pesos;
}

int guardar_datos(const Zona *zonas, const char *nombre_archivo) {
    if (zonas == NULL || nombre_archivo == NULL) return 1;
    FILE *archivo = fopen(nombre_archivo, "wb");
    if (archivo == NULL) return 1;
    size_t elementos_escritos = fwrite(zonas, sizeof(Zona), NUM_ZONAS, archivo);
    fclose(archivo);
    return (elementos_escritos != NUM_ZONAS);
}

int cargar_datos(Zona *zonas, const char *nombre_archivo) {
    if (zonas == NULL || nombre_archivo == NULL) return 1;
    FILE *archivo = fopen(nombre_archivo, "rb");
    if (archivo == NULL) {
        for (int i = 0; i < NUM_ZONAS; i++) {
            snprintf(zonas[i].nombre, 50, "Zona %d", i + 1);
        }
        return 0;
    }
    size_t elementos_leidos = fread(zonas, sizeof(Zona), NUM_ZONAS, archivo);
    fclose(archivo);
    if (elementos_leidos != NUM_ZONAS && !feof(archivo)) return 1;
    return 0;
}

void simular_datos_historicos(Zona *zonas) {
    if (zonas == NULL) return;
    for (int i = 0; i < NUM_ZONAS; i++) {
        for (int d = 0; d < DIAS_HISTORICOS; d++) {
            for (int j = 0; j < NUM_CONTAMINANTES; j++) {
                zonas[i].historico[d].niveles[j] = (float)rand() / RAND_MAX * 100;
            }
        }
    }
}

int ingresar_datos_actuales(Zona *zonas) {
    if (zonas == NULL) return 1;
    for (int i = 0; i < NUM_ZONAS; i++) {
        printf("Ingrese los datos actuales para %s:\n", zonas[i].nombre);
        for (int j = 0; j < NUM_CONTAMINANTES; j++) {
            while (1) {
                printf("Nivel de contaminante %d: ", j + 1);
                if (scanf("%f", &zonas[i].actual.niveles[j]) != 1 || zonas[i].actual.niveles[j] < 0) {
                    fprintf(stderr, "Error al leer el nivel del contaminante %d para %s. Ingrese un valor valido (positivo).\n", j + 1, zonas[i].nombre);
                    while(getchar() != '\n');  // Limpiar buffer de entrada
                } else {
                    break;  // Salir del ciclo si el valor es válido
                }
            }
        }

        while (1) {
            printf("Temperatura: ");
            if (scanf("%f", &zonas[i].actual.temperatura) != 1) {
                fprintf(stderr, "Error al leer la temperatura para %s. Ingrese un valor valido.\n", zonas[i].nombre);
                while(getchar() != '\n');  // Limpiar buffer de entrada
            } else {
                break;  // Salir del ciclo si el valor es válido
            }
        }

        while (1) {
            printf("Velocidad del viento: ");
            if (scanf("%f", &zonas[i].actual.velocidad_viento) != 1 || zonas[i].actual.velocidad_viento < 0) {
                fprintf(stderr, "Error al leer la velocidad del viento para %s. Ingrese un valor valido (positivo).\n", zonas[i].nombre);
                while(getchar() != '\n');  // Limpiar buffer de entrada
            } else {
                break;  // Salir del ciclo si el valor es válido
            }
        }

        while (1) {
            printf("Humedad: ");
            if (scanf("%f", &zonas[i].actual.humedad) != 1 || zonas[i].actual.humedad < 0) {
                fprintf(stderr, "Error al leer la humedad para %s. Ingrese un valor valido (positivo).\n", zonas[i].nombre);
                while(getchar() != '\n');  // Limpiar buffer de entrada
            } else {
                break;  // Salir del ciclo si el valor es válido
            }
        }
    }
    return 0;
}



void mostrar_predicciones(const Zona *zonas) {
    if (zonas == NULL) return;
    for (int i = 0; i < NUM_ZONAS; i++) {
        printf("Predicciones para %s en las proximas 24 horas:\n", zonas[i].nombre);
        for (int j = 0; j < NUM_CONTAMINANTES; j++) {
            if (isnan(zonas[i].prediccion.niveles[j])) {
                printf("Contaminante %d: No se pudo calcular la prediccion (datos invalidos).\n", j + 1);
            } else {
                printf("Contaminante %d: %.2f\n", j + 1, zonas[i].prediccion.niveles[j]);
            }
        }
    }
}

void calcular_promedios_historicos(Zona *zonas) {
    for (int i = 0; i < NUM_ZONAS; i++) {
        for (int j = 0; j < NUM_CONTAMINANTES; j++) {
            float suma = 0;
            for (int k = 0; k < DIAS_HISTORICOS; k++) {
                suma += zonas[i].historico[k].niveles[j];
            }
            zonas[i].promedios_historicos[j] = suma / DIAS_HISTORICOS;
        }
    }
}

void mostrar_comparacion_oms(const Zona *zonas) {
    const float limites_oms[NUM_CONTAMINANTES] = LIMITES_OMS;
    printf("\nComparacion con limites de la OMS:\n");
    for (int i = 0; i < NUM_ZONAS; i++) {
        printf("Zona: %s\n", zonas[i].nombre);
        for (int j = 0; j < NUM_CONTAMINANTES; j++) {
            printf("Contaminante %d: Promedio Historico: %.2f, Limite OMS: %.2f ", j + 1, zonas[i].promedios_historicos[j], limites_oms[j]);
            if (zonas[i].promedios_historicos[j] > limites_oms[j]) {
                printf("(Excede el limite)\n");
            } else {
                printf("(Dentro del limite)\n");
            }
        }
    }
}

void calcular_predicciones(Zona *zonas) {
    for (int i = 0; i < NUM_ZONAS; i++) {
        for (int j = 0; j < NUM_CONTAMINANTES; j++) {
            float valores_historicos[DIAS_HISTORICOS];
            for (int k = 0; k < DIAS_HISTORICOS; k++) {
                valores_historicos[k] = zonas[i].historico[k].niveles[j];
            }
            int error = 0;
            float prediccion_base = promedio_ponderado(valores_historicos, DIAS_HISTORICOS, &error);
            if(error) {
                zonas[i].prediccion.niveles[j] = NAN;
                continue;
            }
            zonas[i].prediccion.niveles[j] = prediccion_base * (1 + (zonas[i].actual.temperatura - 20) * 0.01 + (zonas[i].actual.velocidad_viento * 0.005) - (zonas[i].actual.humedad - 50) * 0.002);
        }
    }
}

void generar_alertas_y_recomendaciones(const Zona *zonas) {
    const float limites_contaminantes[NUM_CONTAMINANTES] = LIMITES_CONTAMINANTES;
    printf("\nAlertas y Recomendaciones:\n");
    for (int i = 0; i < NUM_ZONAS; i++) {
        printf("Zona: %s\n", zonas[i].nombre);
        for (int j = 0; j < NUM_CONTAMINANTES; j++) {
            if (isnan(zonas[i].prediccion.niveles[j])) {
                printf("No se pueden generar alertas para el contaminante %d de %s por datos invalidos\n", j+1, zonas[i].nombre);
                continue;
            }
            if (zonas[i].prediccion.niveles[j] > limites_contaminantes[j]) {
                printf("ALERTA: Nivel de Contaminante %d (%.2f) en %s excede el limite (%.2f). ", j + 1, zonas[i].prediccion.niveles[j], zonas[i].nombre, limites_contaminantes[j]);
                switch (j) {
                case 0:
                    printf("RECOMENDACION: Reducir el trafico vehicular y promover el uso de transporte publico o bicicletas.\n");
                    break;
                case 1:
                    printf("RECOMENDACION: Las industrias deben reducir sus emisiones y la poblacion evitar la exposicion prolongada al aire libre.\n");
                    break;
                case 2:
                    printf("RECOMENDACION: Reducir la quema de combustibles fosiles y optimizar el uso de energia.\n");
                    break;
                case 3:
                    printf("RECOMENDACION: Usar mascarillas con filtro PM2.5 y evitar actividades fisicas intensas al aire libre, especialmente personas con problemas respiratorios.\n");
                    break;
                default:
                    printf("RECOMENDACION: Suspender actividades al aire libre.\n");
                    break;
                }
            }
        }
    }
}
