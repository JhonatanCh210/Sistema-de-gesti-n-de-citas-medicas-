#include <stdio.h>
#include <stdlib.h>

#include "auth.h"
#include "admin.h"
#include "medicos.h"
#include "pacientes.h"
#include "citas.h"
#include "menu_paciente.h"
#include "menu_medico.h"

#define ARCHIVO_MEDICOS "medicos.txt"
#define ARCHIVO_PACIENTES "pacientes.txt"

int main(void) {
	srand(time(NULL)); //numero aleatorio para codigo_paciente
	Medico medicos[MAX_MEDICOS];
	int n_medicos = cargar_medicos(ARCHIVO_MEDICOS, medicos, MAX_MEDICOS);
	
	Paciente pacientes[MAX_PACIENTES];
	int n_pacientes = cargar_pacientes(ARCHIVO_PACIENTES, pacientes, MAX_PACIENTES);
	
	Cita citas[MAX_CITAS];
	int n_citas = cargar_citas("citas.txt", citas, MAX_CITAS);
	
	Usuario usuario_logeado;
	
	/* AUTENTICACION (incluye registro obligatorio de admin la primera vez) */
	if (!menu_auth(&usuario_logeado,
				   pacientes, &n_pacientes)) {
		printf("Saliendo del sistema.\n");
		return 0;
	}
	
	
	
	
	/* CONTROL DE ACCESO */
	switch (usuario_logeado.rol) {
	case ROL_ADMIN:
		menu_admin(medicos, &n_medicos,
				   pacientes, &n_pacientes,
				   citas, &n_citas);
		break;
		
	case ROL_MEDICO:
		menu_medico(
					usuario_logeado,
					citas, &n_citas
					);
		break;
		
		
	case ROL_PACIENTE:
		menu_paciente(
					  usuario_logeado,
					  pacientes, n_pacientes,
					  medicos, n_medicos,
					  citas, &n_citas
					  );
		break;
		
		
	default:
		printf("Rol desconocido.\n");
	}
	
	printf("Programa finalizado.\n");
	return 0;
}



