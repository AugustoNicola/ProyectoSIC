#pragma once

#include <string>

#pragma once
/**
 * @brief Estructura de cada instancia de mes del registro del iva.
 * 
 * @param mes: mes del registro
 * @param valor: registro del saldo del iva (positivo para A Favor, negativo para A Pagar)
 */
struct MesIVA
{
	std::string mes;
	int valor;
};

