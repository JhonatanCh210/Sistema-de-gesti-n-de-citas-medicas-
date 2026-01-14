#include "auth.h"
#include "medicos.h"
#include "pacientes.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ARCHIVO_PACIENTES "pacientes.txt"

int cargar_usuarios(Usuario usuarios[], int max) {
	FILE *f = fopen(ARCHIVO_USUARIOS, "r");
	if (!f) return 0;
	
	char line[128];
	int i = 0;
	
	while (i < max && fgets(line, sizeof(line), f)) {
		line[strcspn(line, "\n")] = '\0';
		
		Usuario u;
		memset(&u, 0, sizeof(Usuario));
		
		char *p = strtok(line, ";");
		if (!p) continue;
		strncpy(u.usuario, p, sizeof(u.usuario) - 1);
		
		p = strtok(NULL, ";");
		if (!p) continue;
		strncpy(u.password, p, sizeof(u.password) - 1);
		
		p = strtok(NULL, ";");
		if (!p) continue;
		u.rol = atoi(p);
		
		/* VALIDACIÓN DEL ROL */
		if (u.rol != ROL_ADMIN &&
			u.rol != ROL_MEDICO &&
			u.rol != ROL_PACIENTE) {
			continue;   // ? DESCARTA REGISTRO CORRUPTO
		}
		
		p = strtok(NULL, ";");
		if (p && strcmp(p, "0") != 0)
			strncpy(u.codigo_medico, p, sizeof(u.codigo_medico) - 1);
		else
			u.codigo_medico[0] = '\0';
		
		
		p = strtok(NULL, ";");
		if (p)
			u.codigo_paciente = atoi(p);
		else
			u.codigo_paciente = -1;
		
		if (u.rol == ROL_PACIENTE && u.codigo_paciente <= 0) continue;
		if (u.rol == ROL_MEDICO   && u.codigo_medico[0] == '\0') continue;
		
		
		
		usuarios[i++] = u;
	}
	
	fclose(f);
	return i;
}


int guardar_usuarios(Usuario usuarios[], int n) {
	FILE *f = fopen(ARCHIVO_USUARIOS, "w");
	if (!f) return -1;
	
	for (int i = 0; i < n; i++) {
		fprintf(
				f,
				"%s;%s;%d;%s;%d\n",
				usuarios[i].usuario,
				usuarios[i].password,
				usuarios[i].rol,
				usuarios[i].codigo_medico[0] ? usuarios[i].codigo_medico : "0",
				usuarios[i].codigo_paciente
				);
	}
	
	fclose(f);
	return 0;
}



static bool existe_admin(Usuario usuarios[], int n) {
	for (int i = 0; i < n; i++) {
		if (usuarios[i].rol == ROL_ADMIN)
			return true;
	}
	return false;
}

int registrar_admin_inicial(Usuario usuarios[], int n) {
	Usuario u;
	memset(&u, 0, sizeof(Usuario));
	char buffer[64];
	
	printf("\n=== Registro obligatorio de ADMINISTRADOR ===\n");
	
	/* Usuario */
	while (1) {
		printf("Usuario admin: ");
		leer_linea(buffer, sizeof(buffer));
		
		if (!es_codigo_valido(buffer)) {
			printf("Usuario invalido.\n");
			continue;
		}
		
		strncpy(u.usuario, buffer, sizeof(u.usuario));
		u.usuario[sizeof(u.usuario) - 1] = '\0';
		break;
	}
	
	/* Password */
	while (1) {
		printf("Password: ");
		leer_linea(buffer, sizeof(buffer));
		
		if (strlen(buffer) < 4) {
			printf("Password muy corto.\n");
			continue;
		}
		
		strncpy(u.password, buffer, sizeof(u.password));
		u.password[sizeof(u.password) - 1] = '\0';
		break;
	}
	
	u.rol = ROL_ADMIN;
	u.codigo_medico[0] = '\0';
	u.codigo_paciente = -1;   
	
	usuarios[n] = u;
	
	printf("Administrador registrado correctamente.\n");
	return n + 1;
}


bool login(Usuario usuarios[], int n, Usuario *usuario_logeado) {
	char user[32], pass[32];
	
	printf("\n=== Login ===\n");
	printf("Usuario: ");
	leer_linea(user, sizeof(user));
	printf("Password: ");
	leer_linea(pass, sizeof(pass));
	
	for (int i = 0; i < n; i++) {
		if (strcmp(usuarios[i].usuario, user) == 0 &&
			strcmp(usuarios[i].password, pass) == 0) {
			
			*usuario_logeado = usuarios[i];
			return true;
		}
	}
	
	printf("Credenciales incorrectas.\n");
	return false;
}

bool menu_auth(Usuario *usuario_logeado,
			   Paciente pacientes[], int *n_pacientes)
{
	Usuario usuarios[MAX_USUARIOS];
	int n = cargar_usuarios(usuarios, MAX_USUARIOS);
	
	/* CARGAR PACIENTES EXISTENTES */
	*n_pacientes = cargar_pacientes(
									ARCHIVO_PACIENTES,
									pacientes,
									MAX_PACIENTES
									);
	
	/* Admin obligatorio */
	if (!existe_admin(usuarios, n)) {
		n = registrar_admin_inicial(usuarios, n);
		guardar_usuarios(usuarios, n);
	}
	
	/* ===== Login directo ===== */
	if (login(usuarios, n, usuario_logeado)) {
		return true;
	}
	
	/* ===== Fallo ? registro SOLO paciente ===== */
	printf("\nNo existe una cuenta con esas credenciales.\n");
	printf("Debe registrarse como PACIENTE.\n");
	pausar();
	
	Usuario nuevo;
	memset(&nuevo, 0, sizeof(Usuario));
	char buffer[64];
	
	/* Usuario */
	while (1) {
		printf("Usuario: ");
		leer_linea(buffer, sizeof(buffer));
		
		if (!es_codigo_valido(buffer)) {
			printf("Usuario invalido.\n");
			continue;
		}
		
		bool repetido = false;
		for (int i = 0; i < n; i++) {
			if (strcmp(usuarios[i].usuario, buffer) == 0) {
				repetido = true;
				break;
			}
		}
		
		if (repetido) {
			printf("Usuario ya existe.\n");
			continue;
		}
		
		strncpy(nuevo.usuario, buffer, sizeof(nuevo.usuario) - 1);
		break;
	}
	
	/* Password */
	while (1) {
		printf("Password: ");
		leer_linea(buffer, sizeof(buffer));
		
		if (strlen(buffer) < 4) {
			printf("Password muy corto.\n");
			continue;
		}
		
		strncpy(nuevo.password, buffer, sizeof(nuevo.password) - 1);
		break;
	}
	
	/* Registrar PACIENTE real */
	int codigo = registrar_paciente(pacientes, *n_pacientes);
	(*n_pacientes)++;
	
	guardar_pacientes(ARCHIVO_PACIENTES, pacientes, *n_pacientes);
	
	nuevo.rol = ROL_PACIENTE;
	nuevo.codigo_paciente = codigo;
	nuevo.codigo_medico[0] = '\0';
	
	if (codigo == -1) {
		printf("Error registrando paciente.\n");
		return false;
	}
	
	usuarios[n++] = nuevo;
	guardar_usuarios(usuarios, n);
	
	*usuario_logeado = nuevo;
	
	printf("Registro y login exitosos.\n");
	pausar();
	return true;
}

int registrar_medico_usuario_directo(Usuario usuarios[], int n,
									 const char *codigo_medico) {
	Usuario u;
	memset(&u, 0, sizeof(Usuario));
	char buffer[64];
	
	printf("\n=== Creacion de cuenta para medico %s ===\n", codigo_medico);
	
	strncpy(u.codigo_medico, codigo_medico, sizeof(u.codigo_medico) - 1);
	
	/* Usuario */
	while (1) {
		printf("Usuario: ");
		leer_linea(buffer, sizeof(buffer));
		
		if (!es_codigo_valido(buffer)) {
			printf("Usuario invalido.\n");
			continue;
		}
		
		bool repetido = false;
		for (int i = 0; i < n; i++) {
			if (strcmp(usuarios[i].usuario, buffer) == 0) {
				repetido = true;
				break;
			}
		}
		
		if (repetido) {
			printf("Usuario ya existe.\n");
			continue;
		}
		
		strncpy(u.usuario, buffer, sizeof(u.usuario) - 1);
		break;
	}
	
	/* Password */
	while (1) {
		printf("Password: ");
		leer_linea(buffer, sizeof(buffer));
		
		if (strlen(buffer) < 4) {
			printf("Password muy corto.\n");
			continue;
		}
		
		strncpy(u.password, buffer, sizeof(u.password) - 1);
		break;
	}
	
	u.rol = ROL_MEDICO;
	u.codigo_paciente = -1;
	
	usuarios[n] = u;
	printf("Cuenta de medico creada correctamente.\n");
	return n + 1;
}
									 
