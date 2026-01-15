#include "citas.h"
#include "utils.h"
#include "medicos.h"
#include "pacientes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int existe_paciente(int codigo, Paciente pacientes[], int n) {
	return buscar_paciente_por_codigo(codigo, pacientes, n) != -1;
}

int existe_medico(const char *codigo, Medico medicos[], int n) {
	return buscar_medico_por_codigo(medicos, n, codigo) != -1;
}



/* ==================== Persistencia ==================== */

int cargar_citas(const char *ruta, Cita arr[], int max) {
	FILE *f = fopen(ruta, "r");
	if (!f) return 0;
	
	int n = 0;
	char linea[512];
	
	while (n < max && fgets(linea, sizeof(linea), f)) {
		size_t len = strlen(linea);
		if (len && linea[len-1] == '\n')
			linea[len-1] = '\0';
		
		sscanf(linea, "%d;%[^;];%[^;];%[^;];%d;%[^\n]",
			   &arr[n].codigo_enlace,
			   arr[n].codigo_medico,
			   arr[n].fecha,
			   arr[n].hora,
			   &arr[n].estado,
			   arr[n].reporte);
		
		{
			if (strchr(linea, ';') && !strchr(arr[n].reporte, '\0'))
				arr[n].reporte[0] = '\0';
			n++;
		}
	}
	
	fclose(f);
	return n;
}

int guardar_citas(const char *ruta, Cita arr[], int n) {
	FILE *f = fopen(ruta, "w");
	if (!f) return -1;
	
	for (int i = 0; i < n; i++) {
		fprintf(f, "%d;%s;%s;%s;%d;%s\n",
				arr[i].codigo_enlace,
				arr[i].codigo_medico,
				arr[i].fecha,
				arr[i].hora,
				arr[i].estado,
				arr[i].reporte);
		
	}
	
	fclose(f);
	return 0;
}

/* ==================== Registrar cita ==================== */

int registrar_cita(Cita citas[], int n,
				   Medico medicos[], int n_medicos,
				   Paciente pacientes[], int n_pacientes) {
	
	if (n >= MAX_CITAS) {
		printf("Limite de citas alcanzado.\n");
		pausar();
		return n;
	}
	
	Cita c;
	char buf[128];
	int codigo_paciente;
	
	while (1) {
		printf("Codigo del paciente: ");
		leer_linea(buf, sizeof(buf));
		
		codigo_paciente = atoi(buf);
		if (codigo_paciente <= 0) {
			printf("Codigo invalido.\n");
			pausar();
			continue;
		}
		
		if (!existe_paciente(codigo_paciente, pacientes, n_pacientes)) {
			printf("Paciente no existe.\n");
			pausar();
			continue;
		}
		
		c.codigo_enlace = codigo_paciente;
		break;
	}
	
	
	
	while (1) {
		printf("Codigo del medico: ");
		leer_linea(c.codigo_medico, sizeof(c.codigo_medico));
		
		if (!existe_medico(c.codigo_medico, medicos, n_medicos)) {
			printf("Medico no existe.\n");
			pausar();
			continue;
		}
		break;
	}
	
	
	while (1) {
		printf("Fecha (DD/MM/YYYY): ");
		leer_linea(buf, sizeof(buf));
		
		if (!es_fecha_valida(buf)) {
			printf("Formato de fecha invalido.\n");
			pausar();
			continue;
		}
		
		if (!fecha_no_pasada(buf)) {
			printf("La fecha no puede estar en el pasado.\n");
			pausar();
			continue;
		}
		if (medico_ocupado(citas, n, c.codigo_medico, c.fecha, c.hora)) {
			printf("El medico ya tiene una cita en esa fecha y hora.\n");
			pausar();
			continue;
		}
		
		strncpy(c.fecha, buf, sizeof(c.fecha) - 1);
		c.fecha[sizeof(c.fecha) - 1] = '\0';
		break;
	}
	
	
	while (1) {
		printf("Hora (HH:MM): ");
		leer_linea(buf, sizeof(buf));
		if (!es_hora_valida(buf)) {
			printf("Hora invalida.\n");
			pausar();
			continue;
		}
		strncpy(c.hora, buf, sizeof(c.hora) - 1);
		c.hora[sizeof(c.hora) - 1] = '\0';
		break;
		
	}
	
	c.reporte[0] = '\0';
	
	citas[n] = c;
	printf("Cita registrada correctamente.\n");
	pausar();
	return n + 1;
}

/* ==================== Reporte ==================== */

				   void registrar_reporte(Cita citas[], int n,
										  Paciente pacientes[], int n_pacientes,
										  Medico medicos[], int n_medicos)
				   {
					   char buf[64];
					   int codigo_paciente;
					   char codigo_medico[32];
					   
					   /* ===== Validar paciente ===== */
					   while (1) {
						   printf("Codigo del paciente: ");
						   leer_linea(buf, sizeof(buf));
						   codigo_paciente = atoi(buf);
						   
						   if (codigo_paciente <= 0) {
							   printf("Codigo invalido.\n");
							   pausar();
							   continue;
						   }
						   
						   if (!existe_paciente(codigo_paciente, pacientes, n_pacientes)) {
							   printf("Paciente no existe.\n");
							   pausar();
							   continue;
						   }
						   break;
					   }
					   
					   /* ===== Validar medico ===== */
					   while (1) {
						   printf("Codigo del medico: ");
						   leer_linea(codigo_medico, sizeof(codigo_medico));
						   
						   if (!existe_medico(codigo_medico, medicos, n_medicos)) {
							   printf("Medico no existe.\n");
							   pausar();
							   continue;
						   }
						   break;
					   }
					   
					   /* ===== Buscar cita ===== */
					   for (int i = 0; i < n; i++) {
						   if (citas[i].codigo_enlace == codigo_paciente &&
							   strcmp(citas[i].codigo_medico, codigo_medico) == 0)
						   {
							   printf("Ingrese reporte (max %d caracteres):\n", MAX_REPORTE - 1);
							   
							   leer_linea(citas[i].reporte, sizeof(citas[i].reporte));
							   citas[i].reporte[sizeof(citas[i].reporte) - 1] = '\0';
							   
							   printf("Reporte guardado correctamente.\n");
							   pausar();
							   return;
						   }
					   }
					   
					   printf("No se encontro una cita con esos datos.\n");
					   pausar();
				   }
				   
				   

/* ==================== Listar ==================== */

				   void listar_citas(Cita citas[], int n) {
					   int hay = 0;
					   
					   for (int i = 0; i < n; i++) {
						   
						   /* ===== Ignorar canceladas ===== */
						   if (citas[i].estado == ESTADO_CANCELADA)
							   continue;
						   
						   hay = 1;
						   
						   printf("\nPaciente (codigo enlace): %d\n", citas[i].codigo_enlace);
						   printf("Medico: %s\n", citas[i].codigo_medico);
						   printf("Fecha: %s  Hora: %s\n", citas[i].fecha, citas[i].hora);
						   
						   /* ===== Estado ===== */
						   printf("Estado: ");
						   switch (citas[i].estado) {
						   case ESTADO_PENDIENTE:
							   printf("Pendiente\n");
							   break;
						   case ESTADO_RETRASADA:
							   printf("Reprogramada\n");
							   break;
						   case ESTADO_HECHA:
							   printf("Atendida\n");
							   break;
						   }
						   
						   /* ===== Reporte ===== */
						   if (strlen(citas[i].reporte) == 0)
							   printf("Reporte: sin reporte\n");
						   else
							   printf("Reporte: %s\n", citas[i].reporte);
					   }
					   
					   if (!hay)
						   printf("No hay citas activas.\n");
					   
					   pausar();
				   }
				   

/* ==================== Menu ==================== */

void menu_citas(Cita citas[], int *n,
				Medico medicos[], int n_medicos,
				Paciente pacientes[], int n_pacientes) {
	char buf[16];
	int op;
	
	while (1) {
		limpiar_consola();
		printf("\n--- Modulo de Citas ---\n"
			   "1) Registrar cita\n"
			   "2) Registrar reporte\n"
			   "3) Listar citas\n"
			   "4) Pacientes con multiples citas\n"
			   "0) Volver\nSeleccione: ");
		
		leer_linea(buf, sizeof(buf));
		op = atoi(buf);
		
		switch (op) {
		case 1:
			*n = registrar_cita(citas, *n,
								medicos, n_medicos,
								pacientes, n_pacientes);
			break;
			
		case 2:
			registrar_reporte(citas, *n,
							  pacientes, n_pacientes,
							  medicos, n_medicos);
			break;
		case 3:
			listar_citas(citas, *n);
			break;
		case 4:
			listar_pacientes_con_multiples_citas(citas, *n);
			break;	
			
		case 0:
			return;
		default:
			printf("Opcion invalida.\n");
			pausar();
		}
	}
}
				int agendar_cita_paciente(
										  Cita citas[], int n,
										  Medico medicos[], int n_medicos,
										  Paciente pacientes[], int n_pacientes,
										  int codigo_paciente
										  ) {
					if (n >= MAX_CITAS) {
						printf("Limite de citas alcanzado.\n");
						pausar();
						return n;
					}
					
					if (!existe_paciente(codigo_paciente, pacientes, n_pacientes)) {
						printf("Paciente invalido.\n");
						pausar();
						return n;
					}
					
					Cita c;
					char buf[128];
					
					c.codigo_enlace = codigo_paciente;
					
					/* ===== Medico ===== */
					while (1) {
						printf("Codigo del medico: ");
						leer_linea(c.codigo_medico, sizeof(c.codigo_medico));
						
						if (!existe_medico(c.codigo_medico, medicos, n_medicos)) {
							printf("Medico no existe.\n");
							pausar();
							continue;
						}
						break;
					}
					
					/* ===== Fecha ===== */
					while (1) {
						printf("Fecha (DD/MM/YYYY): ");
						leer_linea(buf, sizeof(buf));
						
						if (!es_fecha_valida(buf)) {
							printf("Formato invalido.\n");
							pausar();
							continue;
						}
						
						if (!fecha_no_pasada(buf)) {
							printf("La fecha no puede estar en el pasado.\n");
							pausar();
							continue;
						}
						if (medico_ocupado(citas, n, c.codigo_medico, c.fecha, c.hora)) {
							printf("El medico ya tiene una cita en esa fecha y hora.\n");
							pausar();
							continue;
						}
						
						strncpy(c.fecha, buf, sizeof(c.fecha) - 1);
						c.fecha[sizeof(c.fecha) - 1] = '\0';
						break;
					}
					
					/* ===== Hora ===== */
					while (1) {
						printf("Hora (HH:MM): ");
						leer_linea(buf, sizeof(buf));
						
						if (!es_hora_valida(buf)) {
							printf("Hora invalida.\n");
							pausar();
							continue;
						}
						
						strncpy(c.hora, buf, sizeof(c.hora) - 1);
						c.hora[sizeof(c.hora) - 1] = '\0';
						break;
					}
					
					c.reporte[0] = '\0';
					
					citas[n] = c;
					printf("Cita agendada correctamente.\n");
					pausar();
					return n + 1;
				}
										  int medico_ocupado(
															 Cita citas[], int n,
															 const char *codigo_medico,
															 const char *fecha,
															 const char *hora
															 ) {
											  for (int i = 0; i < n; i++) {
												  if (citas[i].estado != ESTADO_CANCELADA &&
													  strcmp(citas[i].codigo_medico, codigo_medico) == 0 &&
													  strcmp(citas[i].fecha, fecha) == 0 &&
													  strcmp(citas[i].hora, hora) == 0)
													  return 1;
											  }
											  return 0;
										  }
															 
															 void cancelar_cita(Cita citas[], int n, int index) {
																 citas[index].estado = ESTADO_CANCELADA;
															 }
															 void reprogramar_cita(Cita *c, const char *nueva_fecha, const char *nueva_hora) {
																 strcpy(c->fecha, nueva_fecha);
																 strcpy(c->hora, nueva_hora);
																 c->estado = ESTADO_RETRASADA;
															 }
															 
															 void listar_pacientes_con_multiples_citas(Cita citas[], int n) {
																 int mostrados[MAX_CITAS];
																 int n_mostrados = 0;
																 
																 for (int i = 0; i < n; i++) {
																	 
																	 if (citas[i].estado == ESTADO_CANCELADA)
																		 continue;
																	 
																	 int codigo = citas[i].codigo_enlace;
																	 
																	 /* ===== evitar repetir paciente ===== */
																	 int ya = 0;
																	 for (int k = 0; k < n_mostrados; k++) {
																		 if (mostrados[k] == codigo) {
																			 ya = 1;
																			 break;
																		 }
																	 }
																	 if (ya) continue;
																	 
																	 /* ===== contar citas ===== */
																	 int contador = 0;
																	 for (int j = 0; j < n; j++) {
																		 if (citas[j].estado != ESTADO_CANCELADA &&
																			 citas[j].codigo_enlace == codigo) {
																			 contador++;
																		 }
																	 }
																	 
																	 if (contador >= 2) {
																		 mostrados[n_mostrados++] = codigo;
																		 
																		 printf("\nPaciente %d (%d citas)\n", codigo, contador);
																		 
																		 int idx = 1;
																		 for (int j = 0; j < n; j++) {
																			 if (citas[j].estado != ESTADO_CANCELADA &&
																				 citas[j].codigo_enlace == codigo) {
																				 
																				 printf("  %d) Medico: %s  Fecha: %s  Hora: %s\n",
																						idx++,
																						citas[j].codigo_medico,
																						citas[j].fecha,
																						citas[j].hora);
																			 }
																		 }
																	 }
																 }
																 
																 if (n_mostrados == 0)
																	 printf("No hay pacientes con multiples citas.\n");
																 
																 pausar();
															 }
															 
int contar_citas_activas_medico(
								Cita citas[], int n_citas,
								const char *codigo_medico
							) {
																 int count = 0;
																 
																 for (int i = 0; i < n_citas; i++) {
																	 if (citas[i].estado == ESTADO_PENDIENTE &&
																		 strcmp(citas[i].codigo_medico, codigo_medico) == 0) {
																		 count++;
																	 }
																 }
																 return count;
															 }
																							 
