	
#include "admin.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
	
#define ARCHIVO_MEDICOS "medicos.txt"
#define ARCHIVO_PACIENTES "pacientes.txt"
#define ARCHIVO_CITAS "citas.txt"
	
	void menu_admin(Medico medicos[], int *n_medicos,
					Paciente pacientes[], int *n_pacientes,
					Cita citas[], int *n_citas) {
		
		char buf[MAX_LINE];
		int opcion;
		
		while (1) {
			limpiar_consola();
			printf("\n==== Sistema de Citas - Menu Administrador ====\n"
				   "1) Modulo de Medicos\n"
				   "2) Modulo de Pacientes\n"
				   "3) Modulo de Citas\n"
				   "0) Cerrar sesion\n"
				   "Seleccione una opcion: ");
			
			leer_linea(buf, sizeof(buf));
			opcion = atoi(buf);
			
			switch (opcion) {
				
			case 1:
				menu_medicos(medicos, n_medicos);
				guardar_medicos(ARCHIVO_MEDICOS, medicos, *n_medicos);
				break;
				
			case 2:
				menu_pacientes(pacientes, n_pacientes);
				guardar_pacientes(ARCHIVO_PACIENTES, pacientes, *n_pacientes);
				break;
				
			case 3:
				menu_citas(citas, n_citas,
						   medicos, *n_medicos,
						   pacientes, *n_pacientes);
				
				guardar_citas(ARCHIVO_CITAS, citas, *n_citas);
				break;
				
			case 0:
				printf("Cerrando sesion...\n");
				pausar();
				return;
				
			default:
				printf("Opcion invalida.\n");
				pausar();
			}
		}
	}
					
				
