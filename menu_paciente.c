
#include "menu_paciente.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

void menu_paciente(
				   Usuario usuario_logeado,
				   Paciente pacientes[], int n_pacientes,
				   Medico medicos[], int n_medicos,
				   Cita citas[], int *n_citas
				   ) {
	char buf[16];
	int op;
	
	limpiar_consola();
	
	printf("=== PACIENTE ===\n");
	printf("Codigo paciente: %d\n\n", usuario_logeado.codigo_paciente);
	
	while (1) {
		printf(
			   "1) Listar medicos\n"
			   "2) Agendar cita\n"
			   "0) Salir\n"
			   "Seleccione: "
			   );
		
		leer_linea(buf, sizeof(buf));
		op = atoi(buf);
		
		switch (op) {
		case 1:
			listar_medicos(medicos, n_medicos);
			break;
			
		case 2:
			*n_citas = agendar_cita_paciente(
											 citas, *n_citas,
											 medicos, n_medicos,
											 pacientes, n_pacientes,
											 usuario_logeado.codigo_paciente
											 );
			guardar_citas("citas.txt", citas, *n_citas);
			break;
			
		case 0:
			return;
			
		default:
			printf("Opcion invalida.\n");
			pausar();
		}
	}
}
