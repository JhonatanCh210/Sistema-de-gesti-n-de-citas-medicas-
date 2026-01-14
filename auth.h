#ifndef AUTH_H
#define AUTH_H

#include <stdbool.h>
#include "medicos.h"
#include "pacientes.h"

#define MAX_USUARIOS 100
#define ARCHIVO_USUARIOS "usuarios.txt"
typedef struct Usuario {
	char usuario[32];
	char password[32];
	int  rol;
	int  codigo_paciente;
	char codigo_medico[16];
} Usuario;

typedef enum {
	ROL_ADMIN = 1,
		ROL_MEDICO,
		ROL_PACIENTE
} Rol;

/* Funciones */
int cargar_usuarios(Usuario usuarios[], int max);
int guardar_usuarios(Usuario usuarios[], int n);

int registrar_admin_inicial(Usuario usuarios[], int n);
bool login(Usuario usuarios[], int n, Usuario *usuario_logeado);

bool menu_auth(
			   Usuario *usuario_logeado,
			   Paciente *pacientes, int *n_pacientes
			   );

#endif

