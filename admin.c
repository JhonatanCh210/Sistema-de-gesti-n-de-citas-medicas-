	
#include "admin.h"
#include "citas.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
	
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
				   "4) Listar medicos con mas demanda\n"
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
				
			case 4:
				listar_medicos_con_mas_demanda(
											   medicos, *n_medicos,
											   citas, *n_citas
											   );
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
					
					void listar_medicos_con_mas_demanda(
														Medico medicos[], int n_medicos,
														Cita citas[], int n_citas
														) {
						if (n_medicos == 0 || n_citas == 0) {
							printf("No hay datos suficientes.\n");
							pausar();
							return;
						}
						
						int usados[MAX_MEDICOS] = {0};
						
						printf("\n--- Top 3 Medicos con mas citas activas ---\n");
						
						for (int rank = 1; rank <= 3; rank++) {
							int max = 0;
							int idx = -1;
							
							for (int i = 0; i < n_medicos; i++) {
								if (usados[i]) continue;
								
								int count = 0;
								for (int j = 0; j < n_citas; j++) {
									if (citas[j].estado == ESTADO_PENDIENTE &&
										strcmp(citas[j].codigo_medico, medicos[i].codigo) == 0) {
										count++;
									}
								}
								
								if (count > max) {
									max = count;
									idx = i;
								}
							}
							
							if (idx == -1 || max == 0)
								break;
							
							printf("\n%d) Codigo: %s\n", rank, medicos[idx].codigo);
							printf("   Nombre: %s\n", medicos[idx].nombre);
							printf("   Tipo: %s\n", tipo_medico_a_texto(medicos[idx].tipo));
							printf("   Citas activas: %d\n", max);
							
							usados[idx] = 1;
						}
						
						pausar();
					}
														
