#ifndef CITAS_H
#define CITAS_H

#include "medicos.h"
#include "pacientes.h"
#define MAX_CITAS 1000
#define MAX_REPORTE 301
#define MAX_FECHA 11   // DD/MM/YYYY
#define MAX_HORA 6     // HH:MM

#define ESTADO_CANCELADA 0
#define ESTADO_PENDIENTE 1
#define ESTADO_RETRASADA 2
#define ESTADO_HECHA     3

typedef struct {
	int  codigo_enlace;          // paciente
	char codigo_medico[32];
	char fecha[11];              // DD/MM/YYYY
	char hora[6];                // HH:MM
	char reporte[MAX_REPORTE];
	int  estado;                 // NUEVO
} Cita;
/* Persistencia */
int cargar_citas(const char *ruta, Cita arr[], int max);
int guardar_citas(const char *ruta, Cita arr[], int n);

/* Operaciones */
int registrar_cita(Cita citas[], int n,
				   Medico medicos[], int n_medicos,
				   Paciente pacientes[], int n_pacientes);

void registrar_reporte(Cita citas[], int n,
					   Paciente pacientes[], int n_pacientes,
					   Medico medicos[], int n_medicos);
	
void listar_citas(Cita citas[], int n);
int existe_medico(const char *codigo, Medico medicos[], int n);
int existe_paciente(int codigo, Paciente pacientes[], int n);

/* Menu */
void menu_citas(Cita citas[], int *n,
				Medico medicos[], int n_medicos,
				Paciente pacientes[], int n_pacientes);

int medico_ocupado(
				   Cita citas[], int n,
				   const char *codigo_medico,
				   const char *fecha,
				   const char *hora
				   );
#endif
