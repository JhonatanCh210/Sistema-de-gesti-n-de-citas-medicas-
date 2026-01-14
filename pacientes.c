#include "pacientes.h"
#include "medicos.h"
#include "citas.h"
#include "auth.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int buscar_paciente_por_codigo(int codigo, Paciente pacientes[], int n) {
	for (int i = 0; i < n; i++) {
		if (pacientes[i].codigo_enlace == codigo)
			return i;
	}
	return -1;
}



	int cedulaExiste(const char *cedulaBuscada, Paciente arr[], int n) {
		for (int i = 0; i < n; i++) {
			if (strcmp(arr[i].cedula, cedulaBuscada) == 0)
				return 1;
		}
		return 0;
	}
	
	int registrar_paciente(Paciente arr[], int n) {
		Paciente p;
		char buf[MAX_STR];
		
		/* ===== Nombre ===== */
		while (1) {
			printf("Nombre del paciente: ");
			leer_linea(p.nombre, sizeof(p.nombre));
			if (strlen(p.nombre) == 0) {
				printf("El nombre no puede estar vacío.\n");
				continue;
			}
			break;
		}
		
		char tmp[MAX_STR];
		
		while (1) {
			printf("Cedula: ");
			leer_linea(tmp, sizeof(tmp));
			
			if (strlen(tmp) == 0) {
				printf("La cedula no puede estar vacia.\n");
				continue;
			}
			
			if (!es_cedula_valida(tmp)) {
				continue;
			}
			
			if (cedulaExiste(tmp, arr, n)) {
				printf("La cedula ya existe, ingrese otra.\n");
				continue;
			}
			
			strncpy(p.cedula, tmp, sizeof(p.cedula) - 1);
			p.cedula[sizeof(p.cedula) - 1] = '\0';
			break;
		}
		
		/* ===== Edad ===== */
		while (1) {
			printf("Edad: ");
			leer_linea(buf, sizeof(buf));
			
			if (strlen(buf) == 0) {
				printf("La edad no puede estar vacia.\n");
				continue;
			}
			
			char *endptr;
			long valor = strtol(buf, &endptr, 10);
			
			if (*endptr != '\0') {
				printf("Edad invalida. Ingrese solo digitos.\n");
				continue;
			}
			
			if (valor < 0 || valor > 120) {
				printf("Edad fuera de rango (0 - 120).\n");
				continue;
			}
			
			p.edad = (int)valor;
			break;
		}
		
		
		/* ===== Telefono ===== */
		while (1) {
			printf("Telefono: ");
			leer_linea(p.telefono, sizeof(p.telefono));
			
			if (strlen(p.telefono) == 0) {
				printf("El telefono no puede estar vacio.\n");
				continue;
			}
			break;
		}
		
		
		/* ===== Correo ===== */
		while (1) {
			printf("Correo: ");
			leer_linea(p.correo, sizeof(p.correo));
			
			if (strlen(p.correo) == 0) {
				printf("El correo no puede estar vacio.\n");
				continue;
			}
			break;
		}
		
		/* ===== Codigo de enlace ===== */
		p.codigo_enlace = generar_codigo_4digitos();
		printf("Codigo de enlace asignado al paciente: %d\n", p.codigo_enlace);
		
		/* ===== Guardar ===== */
		arr[n] = p;
		printf("Paciente registrado correctamente.\n");
		
		return p.codigo_enlace;
	}
	
	void listar_pacientes(Paciente arr[], int n) {
		printf("\n--- Lista de Pacientes ---\n");
		for (int i = 0; i < n; i++) {
			printf("\nNombre: %s\n", arr[i].nombre);
			printf("Cedula: %s\n", arr[i].cedula);
			printf("Edad: %d\n", arr[i].edad);
			printf("Telefono: %s\n", arr[i].telefono);
			printf("Correo: %s\n", arr[i].correo);
			printf("Codigo de enlace: %d\n", arr[i].codigo_enlace);
		}
	}
	int cargar_pacientes(const char *ruta, Paciente arr[], int max) {
		FILE *fp = fopen(ruta, "r");
		if (!fp) return 0;
		
		int count = 0;
		while (count < max) {
			char linea[256];
			if (!fgets(linea, sizeof(linea), fp))
				break;
			
			// quitar salto de linea
			size_t len = strlen(linea);
			if (len > 0 && linea[len - 1] == '\n')
				linea[len - 1] = '\0';
			
			if (sscanf(linea, "%[^;];%[^;];%d;%[^;];%[^;];%d",
					   arr[count].nombre,
					   arr[count].cedula,
					   &arr[count].edad,
					   arr[count].telefono,
					   arr[count].correo,
					   &arr[count].codigo_enlace) == 6)
			{
				count++;
			}
			
		}
		fclose(fp);
		return count;
	}
	int guardar_pacientes(const char *ruta, Paciente arr[], int n) {
		FILE *fp = fopen(ruta, "w");
		if (!fp) return -1;
		
		for (int i = 0; i < n; i++) {
			fprintf(fp, "%s;%s;%d;%s;%s;%d\n",
					arr[i].nombre,
					arr[i].cedula,
					arr[i].edad,
					arr[i].telefono,
					arr[i].correo,
					arr[i].codigo_enlace);
			
		}
		fclose(fp);
		return 0;
	}
	
	void menu_pacientes(Paciente pacientes[], int *n) {
		char buf[MAX_LINE];
		int opt;
		
		while (1) {
			printf("\n--- Modulo de Pacientes ---\n"
				   "1) Registrar paciente\n"
				   "2) Listar pacientes\n"
				   "0) Volver\nSeleccione: ");
			
			leer_linea(buf, sizeof(buf));
			opt = atoi(buf);
			
			switch (opt) {
			case 1:
				*n = registrar_paciente(pacientes, *n);
				break;
			case 2:
				listar_pacientes(pacientes, *n);
				break;
			case 0:
				return;
			default:
				printf("Opción inválida.\n");
			}
		}
	}
