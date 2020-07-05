#pragma once

#include <string>
#include <vector>
#include "cuenta.h"

/**
 * @brief Elemento con la informacion de una linea del registro
 * 
 * @param cuenta: puntero al tipo de cuenta que es utilizada
 * @param modificacion: numero que se anota en la partida doble; positivo para debe, negativo para haber
 */
struct Linea
{
	const Cuenta *cuenta;
	float modificacion;
	Linea(Cuenta *c, float modif) : cuenta(c), modificacion(modif) {};
};

/**
 * @brief Contiene cada una de las lineas que conforman a la operacion, seguida de su documento correspondiente
 *
 * @param documento: string con el nombre del documento
 * @param lineas: vector que contiene a todas las lineas de una operacion
 */
class Operacion
{
public:
	std::string documento;
	std::vector<Linea> lineas = {};

	/**
	 * @brief Agrega una linea a la operacion, con su cuenta y modificacion.
	 * 
	 * @param cuenta: cuenta que se anota en la linea
	 * @param modificacion: valor que se debita (positivo) o acredita (negativo)
	*/
	void nuevaLinea(Cuenta *cuenta, float modificacion)
	{
		lineas.push_back(Linea(cuenta, modificacion));
	}

	Operacion(std::string doc) : documento(doc) { };
	Operacion() {};
};

/**
 * @brief Estructura para relacionar todo el conjunto de operaciones de un dia con su fecha.
 * 
 * @param fecha: fecha del dia (DD/MM)
 */
struct DiaOperaciones
{
	std::string fecha;
	std::vector<Operacion> operaciones;

	/**
	 * @brief Agrega una operacion al dia.
	 *
	 * @param oper: operacion que se anota en el dia
	 */
	void nuevaOperacion(Operacion oper) { operaciones.push_back(oper); }

	DiaOperaciones(std::string f) : fecha(f) {};
};