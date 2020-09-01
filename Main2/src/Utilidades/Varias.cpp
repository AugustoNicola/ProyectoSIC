#include "Varias.h"


void pedirNuevaFecha(std::string mensaje, std::optional<std::string> strHeader)
{
	std::string fechaStr;
	do
	{
		system("CLS");
		if (strHeader) { header(strHeader.value(), 2); }
		std::cout << mensaje << ": ";
		std::cin >> fechaStr;
		if (validarFecha(fechaStr))
		{
			break;
		}
		else {
			std::cout << "\n\nValor no valido, presione cualquier tecla para intentarlo nuevamente.";
			_getch();
		}
	} while (true);

	DIAS.push_back(DiaOperaciones(fecha)); //crea el nuevo dia con la fecha ingresada
}

void header(std::string texto, unsigned int espacios)
{
	std::string header = "===============  ===============";
	header.insert(16, texto);

	system("CLS");
	std::cout << header;

	for (unsigned int i = 0; i < espacios; i++)
	{
		std::cout << "\n";
	}
	
}

bool validarStr(std::string str)
{
	std::cin.clear();
	std::cin.ignore(1000, '\n');

	if (str.empty())
	{
		std::cout << "\n\nValor ingresado no valido, intentelo nuevamente.";
		_getch();
		return false;
	}
	return true;
}

bool validarFecha(std::string str)
{
	std::cin.clear();
	std::cin.ignore(1000, '\n');

	/* uso de regex */

	std::basic_regex reg("^[ ]*([0-9]{1,2})\/([0-9]{1,2})[ ]*$");
	std::smatch smatch;
	bool match = std::regex_match(str, smatch, reg);

	if (match)
	{
		///contiene ambos campos!
		int numFecha[2] = { std::stoi(smatch[1].str()), std::stoi(smatch[2].str()) }; // extrae partes numericas

		/* verificacion campos validos */
		if ((31 >= numFecha[0] && numFecha[0] >= 1) && (12 >= numFecha[1] && numFecha[1] >= 1))
		{
			///ambos campos son validos
			std::string strFecha[2] = { std::to_string(numFecha[0]), std::to_string(numFecha[1]) };

			/* verificacion formato DD/MM */
			if (numFecha[0] <= 9) { strFecha[0].insert(0, "0"); }
			if (numFecha[1] <= 9) { strFecha[1].insert(0, "0"); }

			//construccion de string
			std::string nuevaFecha = strFecha[0] + "/" + strFecha[1];

			/* verificacion repeticion fecha */
			if (nuevaFecha != fecha)
			{
				/// fechas diferentes, permitir cambio
				fecha = nuevaFecha; //actualiza la nueva fecha
				return true;
			}
			else {
				/// misma fecha, invalidar
				return false;
			}
		}
		else {
			/// campos no validos
			return false;
		}
	}
	else {
		/// no contiene ambos campos
		return false;
	}
}

int validarInt(std::string str, std::optional<int> min, std::optional<int> max, std::optional<int> valorC, std::optional<int> valorT)
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
		if (smatch[3].matched && 100 >= n && n > 0)
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

			n *= ((smatch[1].matched) ? -1 : 1); //verifica negativo

			return (max >= n && n >= min) ? n : 0;
		}
	}
	else {
		/// no es numero
		return 0;
	}
}

void modificarCuenta(Cuenta* cuenta, int modificacion)
{
	operacionActual->crearLinea(cuenta, modificacion);
	cuenta->registrarModificacion(fecha, modificacion);
}

Cuenta* buscarCuenta(std::string nombre)
{
	for (Cuenta &cuenta : CUENTAS)
	{
		if (cuenta.getNombre() == nombre)
		{
			return (Cuenta*)&cuenta;
		}
	}
	return nullptr;
}

void commitOperacion(Operacion* op)
{
	DIAS.back().crearOperacion(*op);
	oper = Operacion();
}

Operacion* pedirNombreDocx(Operacion* op)
{
	std::string nombre;
	do
	{
		header("OPERACION FINALIZADA", 2);
		std::cout << "Ingrese el nombre del documento de esta operacion: ";
		std::getline(std::cin, nombre);
	} while (!validarStr(nombre));

	op->setDocumento(nombre);
	return op;
}



