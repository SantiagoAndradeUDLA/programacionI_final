#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>
#include <math.h> // Para NAN

#define NUM_ZONAS 5
#define DIAS_HISTORICOS 30
#define NUM_CONTAMINANTES 4 // CO2, SO2, NO2, PM2.5
#define NOMBRE_ARCHIVO "contaminacion.dat"
#define LIMITES_OMS {75, 20, 40, 15} // Ejemplo: Límites OMS para CO2, SO2, NO2, PM2.5 (Ajustar según valores reales)
#define LIMITES_CONTAMINANTES {100, 30, 60, 25} // Ejemplo: Límites para generar alertas (Ajustar según valores reales)

// Estructuras
typedef struct {
    float niveles[NUM_CONTAMINANTES];
    float temperatura;
    float velocidad_viento;
    float humedad;
} DatosContaminacion;

typedef struct {
    char nombre[50];
    DatosContaminacion historico[DIAS_HISTORICOS];
    DatosContaminacion actual;
    DatosContaminacion prediccion;
    float promedios_historicos[NUM_CONTAMINANTES];
} Zona;

// Prototipos de funciones
float promedio_ponderado(const float *valores, int n, int *error);
int guardar_datos(const Zona *zonas, const char *nombre_archivo);
int cargar_datos(Zona *zonas, const char *nombre_archivo);
void simular_datos_historicos(Zona *zonas);
void mostrar_predicciones(const Zona *zonas);
int ingresar_datos_actuales(Zona *zonas);
void calcular_promedios_historicos(Zona *zonas);
void mostrar_comparacion_oms(const Zona *zonas);
void calcular_predicciones(Zona *zonas);
void generar_alertas_y_recomendaciones(const Zona *zonas);

#endif
