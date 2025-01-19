#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>

#define NUM_ZONAS 5
#define DIAS_HISTORICOS 30
#define NUM_CONTAMINANTES 4 // CO2, SO2, NO2, PM2.5
#define NOMBRE_ARCHIVO "contaminacion.dat"

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
} Zona;

// Prototipos de funciones
float promedio_ponderado(const float *valores, int n);
int guardar_datos(const Zona *zonas, const char *nombre_archivo);
int cargar_datos(Zona *zonas, const char *nombre_archivo);
void simular_datos_historicos(Zona *zonas);
void mostrar_predicciones(const Zona *zonas);
int ingresar_datos_actuales(Zona *zonas);


#endif
