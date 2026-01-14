#ifndef MENU_MEDICO_H
#define MENU_MEDICO_H

#include "citas.h"
#include "auth.h"

void menu_medico(
				 Usuario usuario_logeado,
				 Cita citas[], int *n_citas
				 );

void listar_citas_medico(
						 Cita citas[], int n,
						 const char *codigo_medico
						 );

void registrar_reporte_medico(
							  Cita citas[], int n,
							  const char *codigo_medico
							  );

void cambiar_estado_cita(
						 Cita citas[], int n,
						 const char *codigo_medico
						 );

#endif
