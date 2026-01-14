#ifndef MEDICOS_H
#define MEDICOS_H


#include <stdio.h>
#include <stdbool.h>


#define MAX_MEDICOS 1024
#define MAX_NOMBRE 100
#define MAX_ESPECIALIDAD 100
#define MAX_HORARIO 64

typedef enum {
	MEDICO_GENERAL = 1,
		PEDIATRA,
		DERMATOLOGO
} TipoMedico;

typedef struct {
	char codigo[16];
	char nombre[64];
	TipoMedico tipo;     
	char horario[32];
} Medico;




// Carga y guarda
int cargar_medicos(const char *ruta, Medico medicos[], int capacidad);
int guardar_medicos(const char *ruta, Medico medicos[], int n);


// Operaciones
int eliminar_medico(Medico medicos[], int n, const char *codigo);
bool codigo_unico(Medico medicos[], int n, const char *codigo);
int registrar_medico(Medico medicos[], int n, char *codigo_creado);
void listar_medicos(Medico medicos[], int n);
int eliminar_medico(Medico medicos[], int n, const char *codigo);
int buscar_medico_por_codigo(Medico medicos[], int n, const char *codigo);

/* MENÚ DEL MÓDULO */
void menu_medicos(Medico medicos[], int *n);
// Conversión de tipo a texto
const char* tipo_medico_a_texto(TipoMedico t);

// Menú de selección de tipo
TipoMedico seleccionar_tipo_medico(void);


#endif // MEDICOS_H
