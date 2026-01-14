// ------------------------------------------------------------
// FILE: utils.h
// ------------------------------------------------------------
#ifndef UTILS_H
#define UTILS_H


#include <stdio.h>
#include <stdbool.h>


#define MAX_LINE 256


// Lee una línea desde stdin y elimina el '\n'
void leer_linea(char *buffer, int tam);


// Validaciones simples
bool es_cedula_valida(const char *cedula); // validación básica de formato
bool es_codigo_valido(const char *codigo); // validación básica de código

void limpiar_consola(void);
void limpiar_buffer(void);
void pausar(void);
int generar_codigo_4digitos(void);
bool es_hora_valida(const char *hora);
bool es_rango_horario_valido(const char *rango);
bool es_fecha_valida(const char *fecha);
bool fecha_no_pasada(const char *fecha);

#endif // UTILS_H
