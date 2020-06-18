#include <iostream>

/**
 * @brief Estructura que contiene las caracteristicas de las opciones (o templates) para enlazar a las funciones
 * 
 * @param nombre: Nombre que debe aparecer en el menu de opciones.
 * @param pFuncion: puntero a la funcion correspondiente
 */
struct Opcion
{
	std::string nombre;
	void (*pFuncion);

	Opcion(std::string n, void (*pF)()) : nombre(n), pFuncion(pF) {};
};