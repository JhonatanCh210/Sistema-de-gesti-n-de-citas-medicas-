#ifndef MENU_PACIENTE_H
#define MENU_PACIENTE_H

#include "auth.h"
#include "pacientes.h"
#include "medicos.h"
#include "citas.h"

void menu_paciente(
				   Usuario usuario_logeado,
				   Paciente pacientes[], int n_pacientes,
				   Medico medicos[], int n_medicos,
				   Cita citas[], int *n_citas
				   );

#endif
