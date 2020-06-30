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

std::vector<Cuenta*> ACTIVOS, PASIVOS, R_NEGS;

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
 * @brief Muestra todas las cuentas del tipo/filtro determinado, en orden numerico. Pide elegir una
 *  y devuelve un puntero a ella.
 * 
 * @param t: el valor del enum de la clase Cuenta que deben tener las clases para mostrarse/filtro
 * @param mensaje: el mensaje que se muestra acompaniando el menu de seleccion
 * @param DebeOHaber: usado para marcar si se realizara un aumento o disminucion a la cuenta (+/-)
 */
Cuenta* elegirCuenta(Cuenta::Tipo t, std::string mensaje, bool DebeOHaber)
{
	std::string opStr;
	int op;
	std::string modificador; //para mostrar (+/-)
	/* Bucle validacion */
	do
	{
		std::vector<Cuenta*> pos = {}; //vector que asocia cada posicion con su cuenta correspondiente
		int cont = 1; //contador para el output
		
		system("CLS");
		/* Iteracion de cuentas */

		//verifica si se busca solo un tipo o un filtro
		if(t == Cuenta::F_OPER)
		{
			/// filtro: mostrar Activos, Pasivos y Resultado Negativo

			std::cout << "-------- ACTIVOS --------";
			modificador = (DebeOHaber) ? " (A+)" : " (A-)" ;
			for (int i = 0; i < ACTIVOS.size(); i++)
			{
				std::cout << "\n" << cont << ". " << ACTIVOS[i]->nombre << modificador; //output
				pos.push_back(ACTIVOS[i]);
				cont++;
			}

			std::cout << "-------- PASIVOS --------";
			modificador = (DebeOHaber) ? " (P-)" : " (P+)";
			for (int i = 0; i < PASIVOS.size(); i++)
			{
				std::cout << "\n" << cont << ". " << PASIVOS[i]->nombre << modificador; //output
				pos.push_back(PASIVOS[i]);
				cont++;
			}

			std::cout << "-------- RESULTADOS --------";
			modificador = (DebeOHaber) ? " (R-)" : " ((R-)+)";
			for (int i = 0; i < R_NEGS.size(); i++)
			{
				std::cout << "\n" << cont << ". " << R_NEGS[i]->nombre << modificador; //output
				pos.push_back(R_NEGS[i]);
				cont++;
			}
		}
		else {
			/// mostrar solo Activos, Pasivos o Resultados Negativos (segun t)

			/* set los modificadores */
			std::string titulo;
			switch (t)
			{
			case Cuenta::ACTIVO:
				modificador = (DebeOHaber) ? " (A+)" : " (A-)";
				titulo = "-------- ACTIVOS --------";
				break;
			case Cuenta::PASIVO:
				modificador = (DebeOHaber) ? " (P-)" : " (P+)";
				titulo = "-------- PASIVOS --------";
				break;
			case Cuenta::R_NEG:
				modificador = (DebeOHaber) ? " (R-)" : " ((R-)+)";
				titulo = "-------- RESULTADOS --------";
				break;
			}

			std::cout << titulo;
			/* recorre CUENTAS*/
			for (int i = 0; i < CUENTAS.size(); i++)
			{
				//solo muestra la cuenta si es del tipo buscado
				if (CUENTAS[i].tipo == t)
				{
					std::cout << "\n" << cont << ". " << CUENTAS[i].nombre << modificador; //output
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
 * @brief Le pide al usuario realizar operaciones con las cuentas permitidas en el sentido dictado hasta que
 *  se alcance un limite (de haberlo), o se decida parar.
 * 
 * @param t enum del tipo de cuenta o filtro permitido para realizar operaciones
 * @param DebeOHaber booleano que define que columna esta satisfaciendo las cuentas
 * @param mensaje string con el mensaje que acompania la seleccion de Cuenta
 * @param [limite] float hasta el cual se deben hacer operaciones
 * 
 * @return float con la cantidad total que se sumo
 */
float aumentarPartida(Cuenta::Tipo t, bool DebeOHaber, std::string mensaje, std::optional<float> limite)
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
			cuentaActual = elegirCuenta(t, mensaje, DebeOHaber);

			/* seleccion de cantidad */
			std::cout << "\n\nTotal actual: $" << aumentoTotal;
			std::cout << "\n\nSeleccione la cantidad: $";
			std::cin >> aumentoActualStr;

			/* validacion de cantidad */
			aumentoActual = validarInt(aumentoActualStr, 1, (limite.value() - aumentoTotal)); //se asegura de que la cantidad sea valida
			
			if (aumentoActual != 0)
			{
				/// cantidad valida!
				aumentoTotal += aumentoActual;

				/* TODO: aumento de cuenta */
				
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
			cuentaActual = elegirCuenta(t, mensaje, DebeOHaber);

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
				satisfecho = (validarInt(aumentoActualStr, 1, 2) == 2) ? true : false;
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
	/* division de vectores de Cuentas */
	for (int i = 0; i < CUENTAS.size(); i++)
	{
		if(CUENTAS[i].tipo == Cuenta::ACTIVO) {ACTIVOS.push_back((Cuenta*)&CUENTAS[i]); }
		else if (CUENTAS[i].tipo == Cuenta::PASIVO) { PASIVOS.push_back((Cuenta*)&CUENTAS[i]); }
		else if (CUENTAS[i].tipo == Cuenta::R_NEG) { R_NEGS.push_back((Cuenta*)&CUENTAS[i]); }
	}

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
		}
		else {
			///input invalido
			std::cout << "Valor no valido, presione cualquier tecla para volver a intentarlo: ";
			_getch();
		}

		system("CLS");
	} while (loop);
	return 0;
}