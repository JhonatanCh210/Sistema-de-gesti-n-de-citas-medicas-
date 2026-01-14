Sistema de Gestión de Citas Médicas

Descripción
Este proyecto es una aplicación de consola desarrollada en lenguaje C que permite administrar el flujo de trabajo de un centro de salud. El sistema gestiona el registro de pacientes, médicos y la asignación de citas, asegurando la persistencia de datos mediante archivos de texto.
Objetivo
Desarrollar un sistema funcional aplicando los principios de programación estructurada, modularización y Clean Code, permitiendo la gestión eficiente de turnos médicos y la generación de reportes operativos.
Integrantes
* Jhonatan Chango
* Eidan Clavijo

Instrucciones para compilar
Para compilar el proyecto correctamente con todos sus módulos, utiliza el siguiente comando en la terminal:

```bash
gcc main.c admin.c auth.c citas.c medicos.c menu_medico.c menu_paciente.c pacientes.c utils.c -o sistema_citas
