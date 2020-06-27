#include <iostream> //I/O basico
#include <conio.h> // _getch()
#include <climits> //INT_MIN, INT_MAX, etc
#include <vector> //arrays dinamicos

#include "clases.h" //estructuras de clases personalizadas
#include "presets.h" //vector CUENTAS
std::vector<std::vector<Operacion*>> DIAS = {}; //lista de todas las operaciones que deben exportarse, en orden cronologico
std::vector<Mercaderia*> MERCADERIAS = {}; //lista de todos los tipos de mercaderias usados
std::vector<MesIVA> IVA = {}; //registro mensual del IVA

// ############################################################################################
// ################################         UTILIDADES         ################################
// ############################################################################################

/**
 * @brief Verifica si un string es un integer.
 * 
 * @param str: string a comprobar
 * @param min/max: valor minimo y maximo (respectivamente e inclusive) que se permiten 
 * @return Si str es un integer, lo devuelve. Sino, devuelve 0
 */
int validarInt(std::string str, int min = INT_MIN, int max = INT_MAX)
{
	bool esNumero = true;
	int n;

	for (int i = 0; i < str.size(); i++)
	{
		if (!isdigit(str[i])) { esNumero = false; break; }
	}

	if (esNumero)
	{
		n = atoi(str.c_str());
		return (n >= min && n <= max) ? n : 0;
	}
	else {
		///input no es numero
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		return 0;
	}
}



// ############################################################################################
// ###############################         OPERACIONES         ################################
// ############################################################################################


const std::vector<Opcion> OPCIONES = {
	
};

// ############################################################################################
// ################################         EJECUCIÓN         #################################
// ############################################################################################

int main()
{
	bool loop = true; //Controla la ejecucion del programa
	std::string opString;
	int op;
	bool inputValido;
	
	// -------- LOOP PRINCIPAL --------
	std::cout << "=============== PROYECTO SIC ===============\n";
	do
	{
		/* Display de opciones y ingreso de input */
		std::cout << "Seleccione una opcion:\n";
		for (int i = 0; i < OPCIONES.size(); i++)
		{
			std::cout << i + 1 << ". " << OPCIONES[i].nombre << "\n";
		}
		std::cin >> opString;

		/* Validacion */
		inputValido = true;
		for (int i = 0; i < opString.size(); i++)
		{
			if (!isdigit(opString[i])) { inputValido = false; break; }
		}

		if (!inputValido)
		{
			///input no es numero
			inputValido = false;
			std::cin.clear();
			std::cin.ignore(1000, '\n');
			std::cout << "Valor no valido, presione cualquier tecla para volver a intentarlo: ";
			_getch();
		} else {
			///input es numero
			op = atoi(opString.c_str());
			if (!( (op <= OPCIONES.size()) && (op > 0)) )
			{
				///input no esta en el rango de opciones
				inputValido = false;
				std::cin.clear();
				std::cin.ignore(1000, '\n');
				std::cout << "Valor no valido, presione cualquier tecla para volver a intentarlo: ";
				_getch();
			} else {
				/// input valido!
				OPCIONES[op - 1].pFuncion();
				_getch();

				loop = false;
			}
		}
		system("CLS");
	} while (loop);
	
	return 0;
}