#include "menu_medico.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void listar_citas_medico(Cita citas[], int n, const char *codigo_medico) {
	int hay = 0;
	
	for (int i = 0; i < n; i++) {
		if (strcmp(citas[i].codigo_medico, codigo_medico) != 0)
			continue;
		
		hay = 1;
		
		printf("\nPaciente: %d\n", citas[i].codigo_enlace);
		printf("Fecha: %s Hora: %s\n", citas[i].fecha, citas[i].hora);
		
		printf("Estado: ");
		switch (citas[i].estado) {
		case ESTADO_PENDIENTE: printf("Pendiente\n"); break;
		case ESTADO_RETRASADA: printf("Retrasada\n"); break;
		case ESTADO_HECHA:     printf("Hecha\n"); break;
		case ESTADO_CANCELADA: printf("Cancelada\n"); break;
		}
		
		if (strlen(citas[i].reporte) == 0)
			printf("Reporte: (sin reporte)\n");
		else
			printf("Reporte: %s\n", citas[i].reporte);
	}
	
	if (!hay)
		printf("No hay citas asignadas.\n");
	
	pausar();
}
void registrar_reporte_medico(Cita citas[], int n, const char *codigo_medico) {
	char buf[MAX_REPORTE];
	
	for (int i = 0; i < n; i++) {
		if (strcmp(citas[i].codigo_medico, codigo_medico) != 0)
			continue;
		
		if (citas[i].estado == ESTADO_CANCELADA) {
			continue;
		}
		
		printf("Cita %s %s\n", citas[i].fecha, citas[i].hora);
		printf("Ingrese reporte:\n");
		leer_linea(buf, sizeof(buf));
		
		strncpy(citas[i].reporte, buf, sizeof(citas[i].reporte) - 1);
		citas[i].reporte[sizeof(citas[i].reporte) - 1] = '\0';
		
		printf("Reporte guardado.\n");
		pausar();
		return;
	}
	
	printf("No se encontro cita valida.\n");
	pausar();
}
void cambiar_estado_cita(Cita citas[], int n, const char *codigo_medico) {
	char buf[16];
	int nuevo_estado;
	
	for (int i = 0; i < n; i++) {
		if (strcmp(citas[i].codigo_medico, codigo_medico) != 0)
			continue;
		
		if (citas[i].estado == ESTADO_CANCELADA ||
			citas[i].estado == ESTADO_HECHA)
			continue;
		
		printf("Cita %s %s\n", citas[i].fecha, citas[i].hora);
		printf("Estado actual: %d\n", citas[i].estado);
		
		if (citas[i].estado == ESTADO_PENDIENTE)
			printf("Nuevo estado (2=Retrasada 3=Hecha 0=Cancelada): ");
		else if (citas[i].estado == ESTADO_RETRASADA)
			printf("Nuevo estado (0=Cancelada): ");
		
		leer_linea(buf, sizeof(buf));
		nuevo_estado = atoi(buf);
		
		if (citas[i].estado == ESTADO_RETRASADA &&
			nuevo_estado == ESTADO_HECHA) {
			printf("Operacion no permitida.\n");
			pausar();
			return;
		}
		
		citas[i].estado = nuevo_estado;
		printf("Estado actualizado.\n");
		pausar();
		return;
	}
	
	printf("No hay citas modificables.\n");
	pausar();
}
void menu_medico(
				 Usuario usuario_logeado,
				 Cita citas[], int *n_citas
				 ) {
	char buf[16];
	int op;
	
	while (1) {
		limpiar_consola();
		printf("=== MEDICO ===\n");
		printf("Codigo: %s\n\n", usuario_logeado.codigo_medico);
		
		printf(
			   "1) Registrar / editar reporte\n"
			   "2) Cambiar estado de cita\n"
			   "3) Listar mis citas\n"
			   "0) Cerrar sesion\n"
			   "Seleccione: "
			   );
		
		leer_linea(buf, sizeof(buf));
		op = atoi(buf);
		
		switch (op) {
		case 1:
			registrar_reporte_medico(
									 citas, *n_citas,
									 usuario_logeado.codigo_medico
									 );
			guardar_citas("citas.txt", citas, *n_citas);
			break;
			
		case 2:
			cambiar_estado_cita(
								citas, *n_citas,
								usuario_logeado.codigo_medico
								);
			guardar_citas("citas.txt", citas, *n_citas);
			break;
			
		case 3:
			listar_citas_medico(
								citas, *n_citas,
								usuario_logeado.codigo_medico
								);
			break;
			
		case 0:
			return;
			
		default:
			printf("Opcion invalida.\n");
			pausar();
		}
	}
}
				 
