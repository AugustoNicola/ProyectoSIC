#include <iostream> //I/O basico
#include <conio.h> // _getch()
#include <climits> //INT_MIN, INT_MAX, etc
#include <vector> //arrays dinamicos
#include <optional> //valores optativos

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
		/// es numero
		n = atoi(str.c_str());
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		return (n >= min && n <= max) ? n : 0; //devuelve 0 si no esta en el rango y el numero si lo esta
	}
	else {
		///input no es numero
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		return 0;
	}
}

/**
 * @brief Muestra todas las cuentas del tipo determinado, en orden numerico. Pide elegir una
 *  y devuelve un puntero a ella.
 * 
 * @param  t: el valor del enum de la clase Cuenta que deben tener las clases para mostrarse
 * @param mensaje: el mensaje que se muestra acompaniando el menu de seleccion
 */
Cuenta* elegirCuenta(Cuenta::Tipo t, std::string mensaje)
{
	std::string opStr;
	int op;
	/* Bucle validacion */
	do
	{
		std::vector<Cuenta*> pos = {}; //vector que asocia cada posicion con 
		int cont = 1; //contador para el output
		
		system("CLS");

		/* Iteracion de cuentas */
		for(int i = 0; i < CUENTAS.size(); i++)
		{
			/* verifica si la cuenta actual es del tipo buscado*/
			if(t == Cuenta::AoP_OPER)
			{
				///mostrar Activos y Pasivos
				if (CUENTAS[i].tipo == Cuenta::A_OPER || CUENTAS[i].tipo == Cuenta::P_OPER)
				{
					std::cout << cont << ". " << CUENTAS[i].nombre << "\n"; //output
					pos.push_back((Cuenta*)&CUENTAS[i]); //guarda lugar de memoria de cuenta actual en vector
					cont++;
				}
			} else {
				///mostrar solo Activos o Pasivos (segun t)
				if (CUENTAS[i].tipo == t)
				{
					std::cout << cont << ". " << CUENTAS[i].nombre << "\n"; //output
					pos.push_back((Cuenta*)&CUENTAS[i]); //guarda lugar de memoria de cuenta actual en vector
					cont++;
				}
			}
		}

		/* Input */
		
		std::cout << "\n" << mensaje;
		std::cin >> opStr;

		/* Validacion/return */
		op = validarInt(opStr, 1, pos.size());
		if (op == 0)
		{
			/// valor no valido
			std::cout << "\nValor ingresado no valido, intentelo nuevamente.";
			_getch();
		} else {
			/// opcion elegida valida!
			Cuenta* resultado = pos[op - 1];
			return resultado;
			break;
		}
	} while (op == 0);
	return {};
}

// ############################################################################################
// ################################         FUNCIONES         #################################
// ############################################################################################

/**
 * .
 * 
 * \param DebeOHaber
 * \param t
 * \param mensaje
 * \param limite
 * \param mensajeLimite
 * \return 
 */
float aumentarPartida(bool DebeOHaber, Cuenta::Tipo t , std::string mensaje, std::optional<float> limite, std::optional<std::string> mensajeLimite)
{
	Cuenta* cuentaActual;
	std::string aumentoActualStr;
	float aumentoActual;
	float aumentoTotal = 0;

	if (limite)
	{
		/* ----- permitir aumentos hasta el limite ----- */
		while(aumentoTotal != limite)
		{
			system("CLS");
			
			/* seleccion de cuenta */
			cuentaActual = elegirCuenta(t, mensaje);

			/* seleccion de cantidad */
			std::cout << "\n\nTotal actual: $" << aumentoTotal;
			std::cout << "\n\nSeleccione la cantidad: $";
			std::cin >> aumentoActualStr;

			/* validacion de cantidad */
			aumentoActual = validarInt(aumentoActualStr, 1, (limite.value() - aumentoTotal)); //se asegura de que la cantidad sea valida
			
			if (aumentoActual != 0)
			{
				/// cantidad valida!
				aumentoTotal += aumentoTotal;
			} else {
				/// cantidad invalida
				std::cout << "\n\nCantidad invalida, presione cualquier tecla para intentarlo nuevamente";
				_getch();
			}
		}
	} else {
		/* ----- aumentos hasta que el usuario decida parar ----- */
		bool satisfecho = false;

		while (!satisfecho)
		{
			/* seleccion de cuenta */
			cuentaActual = elegirCuenta(t, mensaje);

			/* seleccion de cantidad */
			std::cout << "\n\nTotal actual: $" << aumentoTotal;
			std::cout << "\n\nSeleccione la cantidad: $";
			std::cin >> aumentoActualStr;

			/* validacion de cantidad */
			aumentoActual = validarInt(aumentoActualStr, 1, INT_MAX); //se asegura de que la cantidad sea valida
			if (aumentoActual != 0)
			{
				/// cantidad valida!
				aumentoActual += aumentoTotal;

				/* permitir finalizar */
				system("CLS");
				std::cout << "1. Continuar\n2.Finalizar\nElija una opcion: ";
				std::cin >> aumentoActualStr; //reusado de variables!!
				if (validarInt(aumentoActualStr, 1, 2) == 2) { satisfecho = true; }
			}
			else {
				/// cantidad invalida
				std::cout << "\n\nCantidad invalida, presione cualquier tecla para intentarlo nuevamente";
				_getch();
			}
		}
	}
	return aumentoTotal;
}


// ############################################################################################
// ################################         OPCIONES         ########$#########################
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
		int op = validarInt(opString, 1, OPCIONES.size());
		if (op != 0)
		{
			/// input valido!
			OPCIONES[op - 1].pFuncion();
			_getch();

			loop = false; //provisorio
		} else {
			///input invalido
			std::cout << "Valor no valido, presione cualquier tecla para volver a intentarlo: ";
			_getch();
		}

		system("CLS");
	} while (loop);
	return 0;
}