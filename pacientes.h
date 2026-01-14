#ifndef PACIENTES_H
#define PACIENTES_H

#define MAX_PACIENTES 500
#define MAX_STR 100

typedef struct {
	char nombre[MAX_STR];
	char cedula[16];
	int  edad;
	char telefono[MAX_STR];
	char correo[MAX_STR];
	int  codigo_enlace;   
} Paciente;
//Funciones principales del modulo
int cargar_pacientes(const char *ruta, Paciente arr[], int max);
int guardar_pacientes(const char *ruta, Paciente arr[], int n);

int registrar_paciente(Paciente arr[], int n);
void listar_pacientes(Paciente arr[], int n);
int buscar_paciente_por_codigo(int codigo, Paciente pacientes[], int n);




void menu_pacientes(Paciente pacientes[], int *n);
#endif
