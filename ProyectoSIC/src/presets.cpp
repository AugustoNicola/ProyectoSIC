#include <iostream>
#include <string>
#include <vector>
#include "clases.h"


void opConvencional()
{
	std::cout << "OPCONVENCIONAL" << std::endl;
}

extern const std::vector<Opcion> OPCIONES = {
	Opcion("Operacion Convencional", &opConvencional)
};

extern const std::vector<Cuenta> CUENTAS = {
	Cuenta("Caja", true)
};