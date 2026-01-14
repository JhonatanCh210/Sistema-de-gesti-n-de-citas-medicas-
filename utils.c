// ------------------------------------------------------------
// FILE: utils.c
// ------------------------------------------------------------
#include "utils.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

void leer_linea(char *buffer, int tam) {
	if (!fgets(buffer, tam, stdin)) return;
	size_t len = strlen(buffer);
	if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
}


bool es_cedula_valida(const char *cedula) {
	if (!cedula) return false;
	
	size_t len = strlen(cedula);
	
	// EXACTO 10 dígitos
	if (len != 10) {
		printf("Error: la cedula debe tener exactamente 10 digitos.\n");
		return false;
	}
	
	// Debe empezar con "17"
	if (!(cedula[0] == '1' && cedula[1] == '7')) {
		printf("Error: la cedula debe iniciar con 17.\n");
		return false;
	}
	
	// Todos los caracteres deben ser números
	for (size_t i = 0; i < len; ++i) {
		if (!isdigit((unsigned char)cedula[i])) {
			printf("Error: la cedula solo debe contener numeros.\n");
			return false;
		}
	}
	
	return true;
}

// Código: no vacío y sin espacios
bool es_codigo_valido(const char *codigo) {
	if (!codigo) return false;
	size_t len = strlen(codigo);
	if (len == 0) return false;
	for (size_t i = 0; i < len; ++i) if (isspace((unsigned char)codigo[i])) return false;
	return true;
}
void limpiar_consola(void) {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}
void pausar(void) {
	printf("\nPresione ENTER para continuar...");
	fflush(stdout);
	
	limpiar_buffer();   // elimina basura previa
	getchar();          // espera ENTER real
}
void limpiar_buffer(void) {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

bool es_hora_valida(const char *hora) {
	if (!hora || strlen(hora) != 5) return false;
	
	if (!isdigit(hora[0]) || !isdigit(hora[1]) ||
		hora[2] != ':' ||
		!isdigit(hora[3]) || !isdigit(hora[4]))
		return false;
	
	int h = (hora[0]-'0')*10 + (hora[1]-'0');
	int m = (hora[3]-'0')*10 + (hora[4]-'0');
	
	if (h < 0 || h > 23) return false;
	if (m < 0 || m > 59) return false;
	
	return true;
}

bool es_rango_horario_valido(const char *rango) {
	if (!rango) return false;
	
	// Formato esperado: HH:MM-HH:MM
	if (strlen(rango) != 11) return false;
	if (rango[5] != '-') return false;
	
	char inicio[6], fin[6];
	strncpy(inicio, rango, 5);
	inicio[5] = '\0';
	strncpy(fin, rango + 6, 5);
	fin[5] = '\0';
	
	if (!es_hora_valida(inicio) || !es_hora_valida(fin))
		return false;
	
	return true;
}
int generar_codigo_4digitos(void) {
	return rand() % 9000 + 1000; // 1000 - 9999
}
bool es_fecha_valida(const char *fecha) {
	if (!fecha) return false;
	
	// Formato exacto: DD/MM/YYYY ? 10 caracteres
	if (strlen(fecha) != 10) return false;
	
	// Validar separadores
	if (fecha[2] != '/' || fecha[5] != '/') return false;
	
	// Validar que todo lo demás sean dígitos
	for (int i = 0; i < 10; i++) {
		if (i == 2 || i == 5) continue;
		if (!isdigit((unsigned char)fecha[i]))
			return false;
	}
	
	int dia   = (fecha[0]-'0')*10 + (fecha[1]-'0');
	int mes   = (fecha[3]-'0')*10 + (fecha[4]-'0');
	int anio  = (fecha[6]-'0')*1000 +
		(fecha[7]-'0')*100 +
		(fecha[8]-'0')*10 +
		(fecha[9]-'0');
	
	if (anio < 1900 || anio > 2100) return false;
	if (mes < 1 || mes > 12) return false;
	
	int dias_mes[] = {31,28,31,30,31,30,31,31,30,31,30,31};
	
	// Año bisiesto
	if (mes == 2) {
		if ((anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0))
			dias_mes[1] = 29;
	}
	
	if (dia < 1 || dia > dias_mes[mes - 1]) return false;
	
	return true;
}
bool fecha_no_pasada(const char *fecha) {
	if (!es_fecha_valida(fecha)) return false;
	
	int d = (fecha[0]-'0')*10 + (fecha[1]-'0');
	int m = (fecha[3]-'0')*10 + (fecha[4]-'0');
	int y = (fecha[6]-'0')*1000 +
		(fecha[7]-'0')*100 +
		(fecha[8]-'0')*10 +
		(fecha[9]-'0');
	
	time_t t = time(NULL);
	struct tm hoy = *localtime(&t);
	
	int hoy_d = hoy.tm_mday;
	int hoy_m = hoy.tm_mon + 1;
	int hoy_y = hoy.tm_year + 1900;
	
	if (y < hoy_y) return false;
	if (y == hoy_y && m < hoy_m) return false;
	if (y == hoy_y && m == hoy_m && d < hoy_d) return false;
	
	return true;
}
