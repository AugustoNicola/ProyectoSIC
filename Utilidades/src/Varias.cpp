#include "Varias.h"
#include "Globales.h"

Cuenta* buscarCuenta(std::string nombre)
{
	for (int i = 0; i < CUENTAS.size(); i++)
	{
		if (CUENTAS[i].getNombre() == nombre)
		{
			return (Cuenta*)&CUENTAS[i];
		}
	}
	return nullptr;
}

void modificarCuenta(Cuenta* cuenta, int modificacion)
{
	operacionActual->crearLinea(cuenta, modificacion);
	cuenta->registrarModificacion(fecha, modificacion);
}

int validarInt(std::string str, std::optional<int> valorC = {}, std::optional<int> valorT = {}, std::optional<int> min = INT_MIN, std::optional<int> max = INT_MAX)
{
	std::cin.clear();
	std::cin.ignore(1000, '\n');

	/* uso de regex */

	std::basic_regex reg("^[ ]*(-)?([0-9]+)(%[c|t])?[ ]*$"); // cubrir posibilidad de valor relativo
	std::smatch smatch;
	bool match = std::regex_match(str, smatch, reg);


	if (smatch[2].matched)
	{
		/// es numero!

		int n = std::stoi(smatch[2].str());

		/* verifica valor relativo o absoluto */
		if (smatch[3].matched && 100 >= n > 0)
		{
			/// usar valor relativo

			/* verificar si el tipo usado esta disponible */
			if (smatch[3] == "%c" && valorC)
			{
				/// usar valorC
				return valorC.value() * n / 100;
			}
			else if (smatch[3] == "%t" && valorT)
			{
				/// usar valorT
				return valorT.value() * n / 100;
			}
			else {
				/// valor relativo imposible
				return 0;
			}
		}
		else {
			/// valor absoluto

			//verifica negativo
			n *= ((smatch[1].matched) ? -1 : 1);

			return (max >= n >= min) ? n : 0;
		}

	}
	else {
		/// no es numero
		return 0;
	}
}
