#ifndef ADMIN_H
#define ADMIN_H

#include "medicos.h"
#include "pacientes.h"
#include "citas.h"

void menu_admin(
				Medico medicos[], int *n_medicos,
				Paciente pacientes[], int *n_pacientes,
				Cita citas[], int *n_citas
				);

#endif
