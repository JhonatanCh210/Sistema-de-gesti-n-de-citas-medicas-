#include "medicos.h"
#include "utils.h"
#include "auth.h"
#include <string.h>
#include <stdlib.h>


// Formato CSV simple: codigo;nombre;especialidad;horario\n
int cargar_medicos(const char *ruta, Medico medicos[], int capacidad) {
	FILE *f = fopen(ruta, "r");
	if (!f) return 0; // archivo inexistente -> 0 registros
	
	char line[MAX_LINE];
	int i = 0;
	
	while (i < capacidad && fgets(line, sizeof(line), f)) {
		
		// eliminar \n
		size_t len = strlen(line);
		if (len > 0 && line[len-1] == '\n')
			line[len-1] = '\0';
		// tokenizar
		char *p = strtok(line, ";");
		if (!p) continue;
		strncpy(medicos[i].codigo, p, sizeof(medicos[i].codigo)-1);
		medicos[i].codigo[sizeof(medicos[i].codigo)-1] = '\0';
		
		p = strtok(NULL, ";");
		if (!p) continue;
		strncpy(medicos[i].nombre, p, sizeof(medicos[i].nombre)-1);
		medicos[i].nombre[sizeof(medicos[i].nombre)-1] = '\0';
		
		p = strtok(NULL, ";");
		if (!p) continue;
		medicos[i].tipo = (TipoMedico)atoi(p);
		
		p = strtok(NULL, ";");
		if (!p) p = "";
		strncpy(medicos[i].horario, p, sizeof(medicos[i].horario)-1);
		medicos[i].horario[sizeof(medicos[i].horario)-1] = '\0';
		
		i++;
	}
	
	fclose(f);
	return i;
}


int guardar_medicos(const char *ruta, Medico medicos[], int n) {
	FILE *f = fopen(ruta, "w");
	if (!f) return -1;
	for (int i = 0; i < n; ++i) {
		fprintf(f, "%s;%s;%d;%s\n",
				medicos[i].codigo,
				medicos[i].nombre,
				medicos[i].tipo,
				medicos[i].horario);
	}
	fclose(f);
	return 0;
}
bool codigo_unico(Medico medicos[], int n, const char *codigo) {
	return buscar_medico_por_codigo(medicos, n, codigo) == -1;
}


int buscar_medico_por_codigo(Medico medicos[], int n, const char *codigo) {
	for (int i = 0; i < n; ++i) {
		if (strcmp(medicos[i].codigo, codigo) == 0) return i;
	}
	return -1;
}


int registrar_medico(Medico medicos[], int n, char *codigo_creado) {
	if (n >= MAX_MEDICOS) {
		printf("Error: limite de medicos alcanzado.\n");
		return n;
	}
	
	Medico m;
	char buffer[MAX_LINE];
	
	/* ===== Codigo ===== */
	while (1) {
		printf("Ingrese codigo del medico (sin espacios): ");
		leer_linea(buffer, sizeof(buffer));
		
		if (!es_codigo_valido(buffer)) {
			printf("Codigo invalido.\n");
			continue;
		}
		
		if (!codigo_unico(medicos, n, buffer)) {
			printf("Codigo ya existe.\n");
			continue;
		}
		
		strncpy(m.codigo, buffer, sizeof(m.codigo));
		m.codigo[sizeof(m.codigo) - 1] = '\0';
		break;
	}
	
	/* ===== Nombre ===== */
	while (1) {
		printf("Ingrese nombre: ");
		leer_linea(buffer, sizeof(buffer));
		
		if (strlen(buffer) == 0) {
			printf("Nombre no puede estar vacio.\n");
			continue;
		}
		
		strncpy(m.nombre, buffer, sizeof(m.nombre));
		m.nombre[sizeof(m.nombre) - 1] = '\0';
		break;
	}
	
	/* ===== Tipo de medico (menu) ===== */
	m.tipo = seleccionar_tipo_medico();
	
	/* ===== Horario ===== */
	while (1) {
		printf("Ingrese horario (HH:MM-HH:MM) o deje vacio: ");
		leer_linea(buffer, sizeof(buffer));
		
		if (strlen(buffer) == 0) {
			m.horario[0] = '\0';
			break;
		}
		
		if (!es_rango_horario_valido(buffer)) {
			printf("Horario invalido.\n");
			continue;
		}
		
		strncpy(m.horario, buffer, sizeof(m.horario));
		m.horario[sizeof(m.horario) - 1] = '\0';
		break;
	}
	
	/* ===== Guardar medico ===== */
	medicos[n] = m;
	
	/* ===== Crear cuenta obligatoria para medico ===== */
	Usuario usuarios[MAX_USUARIOS];
	int n_users = cargar_usuarios(usuarios, MAX_USUARIOS);
	
	n_users = registrar_medico_usuario_directo(
											   usuarios,
											   n_users,
											   m.codigo
											   );
	
	guardar_usuarios(usuarios, n_users);
	
	printf("Medico y cuenta creados correctamente.\n");
	
	/* devolver codigo creado si se solicita */
	if (codigo_creado != NULL) {
		strcpy(codigo_creado, m.codigo);
	}
	
	return n + 1;
	
	
}

void listar_medicos(Medico medicos[], int n) {
	if (n == 0) { printf("No hay medicos registrados.\n"); return; }
	printf("\n--- Lista de medicos (%d) ---\n", n);
	for (int i = 0; i < n; ++i) {
		printf("%d) Codigo: %s\n Nombre: %s\n Tipo: %s\n Horario: %s\n\n",
			   i+1,
			   medicos[i].codigo,
			   medicos[i].nombre,
			   tipo_medico_a_texto(medicos[i].tipo),
			   medicos[i].horario[0] ? medicos[i].horario : "(no especificado)");
		
	}
}
int eliminar_medico(Medico medicos[], int n, const char *codigo) {
	int idx = buscar_medico_por_codigo(medicos, n, codigo);
	if (idx == -1) {
		printf("No existe un medico con ese codigo.\n");
		return n;
	}
	for (int i = idx; i < n - 1; ++i) medicos[i] = medicos[i+1];
	printf("Medico eliminado correctamente.\n");
	return n - 1;
}

void menu_medicos(Medico medicos[], int *n) {
	char buf[MAX_LINE];
	int opt;
	
	while (1) {
		limpiar_consola();
		printf("\n--- Modulo de Medicos ---\n"
			   "1) Registrar medico\n"
			   "2) Listar medicos\n"
			   "3) Eliminar medico por codigo\n"
			   "0) Volver\nSeleccione: ");
		
		leer_linea(buf, sizeof(buf));
		opt = atoi(buf);
		
		switch (opt) {
		case 1:
			*n = registrar_medico(medicos, *n, NULL);
			pausar();
			break;
			
		case 2:
			listar_medicos(medicos, *n);
			pausar();
			break;
		case 3: {
			char codigo[32];
			printf("Ingrese codigo del medico a eliminar: ");
			leer_linea(codigo, sizeof(codigo));
			*n = eliminar_medico(medicos, *n, codigo);
			pausar();
			break;
		}
		case 0:
			printf ("Volviendo...");
			pausar();
			return;
		default:
			printf("Opcion invalida.\n");
			pausar ();
		}
	}
}
const char* tipo_medico_a_texto(TipoMedico t) {
	switch (t) {
	case MEDICO_GENERAL: return "Medico General";
	case PEDIATRA:       return "Pediatra";
	case DERMATOLOGO:    return "Dermatologo";
	default:             return "Desconocido";
	}
}
TipoMedico seleccionar_tipo_medico(void) {
	char buf[16];
	int op;
	
	while (1) {
		printf("\nSeleccione tipo de medico:\n"
			   "1) Medico General\n"
			   "2) Pediatra\n"
			   "3) Dermatologo\n"
			   "Opcion: ");
		
		leer_linea(buf, sizeof(buf));
		op = atoi(buf);
		
		if (op >= 1 && op <= 3)
			return (TipoMedico)op;
		
		printf("Opcion invalida.\n");
		pausar();
	}
}

