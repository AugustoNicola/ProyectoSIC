#include <iostream> //I/O basico
#include <conio.h> // _getch()
#include <climits> //INT_MIN, INT_MAX
#include <cfloat> //FLT_MIN, FLT_MAX
#include <cmath> //abs()
#include <regex> //regular expressions
#include <vector> //arrays dinamicos
#include <optional> //valores optativos

#include "clases.h" //estructuras de clases personalizadas
#include "presets.h" //vector CUENTAS

std::vector<DiaOperaciones> DIAS = {}; //lista de todas las operaciones que deben exportarse, en orden cronologico
std::vector<Mercaderia> MERCADERIAS = {}; //lista de todos los tipos de mercaderias usados
std::vector<MesIVA> IVA = {}; //registro mensual del IVA
std::vector<Cuenta*> ACTIVOS, PASIVOS, R_NEGS;

/* enum usado para definir si las operaciones de una funcion son en el Debe, en el Haber,
   o un caso especial como Apertura.*/
enum tipoPartida
{
	Debe, Haber, Apertura
};

std::string fecha; //global con la fecha actual
Operacion oper;
Operacion *operacionActual = &oper; //puntero global con la operacion actual

/// ############################################################################################
/// ################################         UTILIDADES         ################################
/// ############################################################################################

/**
 * @brief Busca la cuenta por nombre.
 * 
 * @param nombre: String con el nombre de la cuenta a buscar
 * @return puntero a la cuenta si la encuentra, sino nullptr
 */
Cuenta* buscarCuenta(std::string nombre)
{
	for (int i = 0; i < CUENTAS.size(); i++)
	{
		if (CUENTAS[i].nombre == nombre)
		{
			return (Cuenta*)&CUENTAS[i];
		}
	}
	return nullptr;
}

/**
 * @brief Verifica si un string es un integer, y esta dentro del rango permitido. Ademas, acepta porcentajes de los valores especificados
 * 
 * @param str: string a comprobar
 * @param valorC/valorT: valores de los cuales sacar porcentajes relativos (de querer permitir)
 * @param min/max: valor minimo y maximo (respectivamente e inclusive) que se permiten 
 * @return Si str es un integer, lo devuelve. Sino, devuelve 0
 */
int validarInt(std::string str, std::optional<int> valorC = {}, std::optional<int> valorT = {}, std::optional<int> min = INT_MIN, std::optional<int> max = INT_MAX)
{
	std::cin.clear();
	std::cin.ignore(1000, '\n');

	/* uso de regex */

	std::basic_regex reg("^[ ]*(-)?([0-9]+)(%[c|t])?[ ]*$"); // cubrir posibilidad de valor relativo
	std::smatch smatch;
	bool match = std::regex_match(str, smatch, reg);


	if (match)
	{
		/// es numero!

		int n = std::stoi(smatch[2].str());

		/* verifica valor relativo o absoluto */
		if (smatch[3].matched && !smatch[1].matched && 100 >= n > 0)
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
			n *= (smatch[1].matched) ? -1 : 1;

			return (max >= n >= min) ? n : 0;
		}

	}
	else {
		/// no es numero
		return 0;
	}
}

/**
 * @brief Verifica si un string es un float, y esta dentro del rango permitido. Ademas, acepta porcentajes de los valores especificados
 *
 * @param str: string a comprobar
 * @param valorC/valorT: valores de los cuales sacar porcentajes relativos (de querer permitir)
 * @param min/max: valor minimo y maximo (respectivamente e inclusive) que se permiten
 * @return Si str es un float, lo devuelve. Sino, devuelve 0.0f
 */
float validarFloat(std::string str, std::optional<float> valorC = {}, std::optional<float> valorT = {}, std::optional<float> min = FLT_MIN, std::optional<float> max = FLT_MAX)
{
	std::cin.clear();
	std::cin.ignore(1000, '\n');

	/* uso de regex */

	std::basic_regex reg("^[ ]*(-)?([0-9]+)(?:\.?([0-9]+))?(%[c|t])?[ ]*$"); // cubrir posibilidad de valor relativo
	std::smatch smatch;
	bool match = std::regex_match(str, smatch, reg);


	if (match)
	{
		/// es numero!
		
		int numero[2];
		numero[0] = std::stoi(smatch[2].str());
		numero[1] = (smatch[3].matched) ? std::stoi(smatch[3].str()) : 0;

		/* crea un string con las partes del numero y lo lee como float */
		std::string numStr;
		numStr.append(std::to_string(numero[0]));
		numStr.append(".");
		numStr.append(std::to_string(numero[1]));
		float n = std::stof(numStr);

		/* verifica valor relativo o absoluto */
		if (smatch[4].matched && !smatch[1].matched && 100 >= n > 0)
		{
			/// usar valor relativo
			
			/* verificar si el tipo usado esta disponible */
			if (smatch[4] == "%c" && valorC)
			{
				/// usar valorC
				return valorC.value() * n / 100;
			}
			else if (smatch[4] == "%t" && valorT)
			{
				/// usar valorT
				return valorT.value() * n / 100;
			} else {
				/// valor relativo imposible
				return 0.0f;
			}
		} else {
			/// valor absoluto
			
			//verifica negativo
			n *= (smatch[1].matched) ? -1 : 1;

			return (max >= n >= min) ? n : 0.0f;
		}

	}
	else {
		/// no es numero
		return 0.0f;
	}
}

/**
 * @brief Verifica si un string es valido como fecha. Si lo es, lo settea como nueva fecha global
 * 
 * @param str: string a comprobar
 * @return Booleano segun si es un valor apto y fue setteado o no
 */
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
		if ((31 >= numFecha[0] >= 1) && (12 >= numFecha[1] >= 1))
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
			} else {
				/// misma fecha, invalidar
				return false;
			}
		} else {
			/// campos no validos
			return false;
		}
	} else {
		/// no contiene ambos campos
		return false;
	}
}

/**
 * @brief Crea una Linea en la operacion actual y realiza la modificacion corresponiente en la Cuenta
 *  con la informacion recibida.
 * 
 * @param cuenta: Puntero a la cuenta que se esta modificando
 * @param modificacion: cantidad (positiva/negativa) de modificacion
 */
void modificarCuenta(Cuenta* cuenta, float modificacion)
{
	operacionActual->nuevaLinea(cuenta, modificacion); //agrega Linea a la Operacion actual
	cuenta->modifDiaCuenta(fecha, modificacion); //aumenta el valor de Cuenta
}

/**
 * @brief Muestra todas las cuentas del tipo/filtro determinado, en orden numerico. Pide elegir una
 *  y devuelve un puntero a ella.
 * 
 * @param t: el valor del enum de la clase Cuenta que deben tener las clases para mostrarse/filtro
 * @param mensaje: el mensaje que se muestra acompaniando el menu de seleccion
 * @param DebeOHaber: usado para marcar si se realizara un aumento o disminucion a la cuenta (+/-)
 */
Cuenta* elegirCuenta(Cuenta::Tipo t, std::string mensaje, tipoPartida tipoPart)
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
			/* agrega mercaderia */
			if (tipoPart == Apertura)
			{
				std::cout << "\n" << cont << ". " << "Mercaderias (A+)"; //output
				pos.push_back(buscarCuenta("Mercaderias"));
				cont++;
			}

			modificador = (tipoPart == Haber) ? " (A-)" : " (A+)";
			for (int i = 0; i < ACTIVOS.size(); i++)
			{
				std::cout << "\n" << cont << ". " << ACTIVOS[i]->nombre << modificador; //output
				pos.push_back(ACTIVOS[i]);
				cont++;
			}

			std::cout << "\n\n-------- PASIVOS --------";
			modificador = (tipoPart == Debe) ? " (P-)" : " (P+)";
			for (int i = 0; i < PASIVOS.size(); i++)
			{
				std::cout << "\n" << cont << ". " << PASIVOS[i]->nombre << modificador; //output
				pos.push_back(PASIVOS[i]);
				cont++;
			}

			std::cout << "\n\n-------- RESULTADOS --------";
			modificador = (tipoPart == Haber) ? " ((R-)+)" : " (R-)";
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
				modificador = (tipoPart == Haber) ? " (A-)" : " (A+)";
				titulo = "-------- ACTIVOS --------";
				break;
			case Cuenta::PASIVO:
				modificador = (tipoPart == Debe) ? " (P-)" : " (P+)";
				titulo = "-------- PASIVOS --------";
				break;
			case Cuenta::R_NEG:
				modificador = (tipoPart == Haber) ? " ((R-)+)" : "(R-)";
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
		
		std::cout << "\n\n" << mensaje << ": ";
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


/**
 * @brief Funcion utilizada para adquirir/vender mercaderia, eligiendo la mercaderia, su precio y su cantidad.
 *  Permite crear tanto mercaderias como precios de compra nuevos en el caso de una compra.
 * 
 * @param compra: Booleano: compra(true) / venta(false)
 * 
 * @return puntero al dia de la mercaderia adquirida/vendida
 */
float seleccionarMercaderia(bool compra)
{
	std::string opStr;
	int op;
	std::vector<Mercaderia*> posMerc;
	unsigned int offSet; // offset para diferenciar valores validos en caso de compra o venta
	bool hayOpcion; //booleano para determinar si una operacion de venta es valida
	int cont; //contador para el output

	/* ----- Bucle validacion mercaderia ----- */
	do
	{
		cont = 1; //contador para el output
		offSet = 0;
		hayOpcion = false;
		posMerc = {}; //vector que asocia cada posicion con su mercaderia

		system("CLS");

		/* Iteracion de mercaderias */
		std::cout << "=============== " << ((compra) ? "COMPRA" : "VENTA") << " DE MERCADERIAS ===============\n";
		for (int i = 0; i < MERCADERIAS.size(); i++)
		{
			// si es venta, comprueba que haya existencias de al menos un precio
			if (compra || MERCADERIAS[i].hayExistencias())
			{
				std::cout << "\n" << cont << ". " << MERCADERIAS[i].nombre;
				posMerc.push_back((Mercaderia*)&MERCADERIAS[i]); //guarda lugar de memoria de mercaderia actual en vector
				cont++;
				hayOpcion = true;
			}
		}
		//opcion crear mercaderia
		if(compra)
		{
			std::cout << "\n" << cont << ". " << "Nueva mercaderia";
			offSet = 1;
		}
		
		// no hay opciones posibles
		if (!hayOpcion)
		{
			std::cout << "\nNo hay mercaderias disponibles!\n\nPresione cualquier tecla para cancelar";
			_getch();
			return 0;
		}

		/* Input */

		std::cout << "\n\n" << "Elija una opcion: ";
		std::cin >> opStr;

		/* Validacion/return */
		op = validarInt(opStr, {}, {}, 1, posMerc.size() + offSet);
		if (op == 0)
		{
			/// valor no valido
			std::cout << "\n\nValor ingresado no valido, intentelo nuevamente.";
			_getch();
		}
	} while (op == 0);
	
	/* crear/encontrar mercaderia */
	system("CLS");
	Mercaderia* mercaElegida;
	if (op == posMerc.size() + 1)
	{
		/// crear mercaderia
		std::cout << "=============== NUEVA MERCADERIA ===============";
		std::cout << "\n\nIngrese el nombre de la nueva mercaderia: ";
		std::cin >> opStr;

		MERCADERIAS.push_back(Mercaderia(opStr)); //crea la nueva mercaderia
		mercaElegida = &(MERCADERIAS.back());
	}
	else {
		/// encontrar mercaderia
		mercaElegida = posMerc[op - 1];
	}

	/* ----- Bucle validacion precio ----- */
	std::vector<PrecioMerca*> posPrecio;
	do 
	{
		posPrecio = {}; //vector que asocia cada posicion con su precio-mercaderia
		cont = 1; //contador para el output
		offSet = 0;

		system("CLS");
		std::cout << "=============== " << mercaElegida->nombre << ": PRECIOS UNITARIOS ===============\n";

		/* Iteracion de precios */
		for (int i = 0; i < mercaElegida->precios.size(); i++)
		{
			/* no es necesario verificar que al menos un precio tenga existencias, esta asegurado */

			// si es venta, comprueba que haya existencias para filtrar opciones
			if (compra || mercaElegida->precios[i].hayExistencias())
			{
				std::cout << "\n" << cont << ". " << mercaElegida->nombre << " ($" << mercaElegida->precios[i].precio << ")";
				posPrecio.push_back((PrecioMerca*)&mercaElegida->precios[i]); //guarda lugar de memoria del precio-mercaderia actual en vector
				cont++;
			}
		}

		if (compra)
		{
			std::cout << "\n" << cont << ". " << "Nuevo precio unitario";
			offSet = 1;
		}

		/* Input */

		std::cout << "\n\n" << "Elija una opcion: ";
		std::cin >> opStr;

		/* Validacion/return */
		op = validarInt(opStr, {}, {}, 1, posPrecio.size() + offSet);
		if (op == 0)
		{
			/// valor no valido
			std::cout << "\n\nValor ingresado no valido, intentelo nuevamente.";
			_getch();
		}
	} while (op == 0);

	/* crear/encontrar mercaderia */
	system("CLS");
	PrecioMerca* precioElegido;
	if (op == posPrecio.size() + 1)
	{
		/// crear precio-mercaderia
		do
		{
			std::cout << "=============== " << mercaElegida->nombre << ": NUEVO PRECIO UNITARIO ===============";
			std::cout << "\n\nIngrese el nuevo precio de compra: $";
			std::cin >> opStr;

			op = validarFloat(opStr, {}, {}, 1);
			if (op == 0)
			{
				std::cout << "\n\nValor ingresado no valido, presione cualquier tecla para intentarlo nuevamente";
				_getch();
				system("CLS");
			}
		} while (op == 0);

		mercaElegida->precios.push_back(PrecioMerca(op)); //crea el nuevo precio-mercaderia
		precioElegido = &(mercaElegida->precios.back()); //guarda el puntero al precio-mercaderia
	}
	else {
		/// encontrar mercaderia
		precioElegido = &(mercaElegida->precios[op - 1]);
	}

	/* en este punto ya tenemos una mercaElegida y un precioElegido!! */
	/* ----- Bucle validacion compra ----- */
	do
	{
		system("CLS");
		std::cout << "=============== " << mercaElegida->nombre << " ($" << precioElegido->precio << " c/u) ===============";
		std::cout << "\n\nExistencias: " << ((precioElegido->hayExistencias()) ? precioElegido->dias.back().cantidad : 0) << " unidades"; //para compra, puede ser 0
		std::cout << "\n\nElija la cantidad de mercaderia que se " << ((compra) ? "compra" : "vende" ) << ": ";
		std::cin >> opStr;
		//si es compra, no hay maximo; si es venta, es la cantidad de existencias actuales (que no puede ser 0)
		op = validarInt(opStr, {}, {}, 1, ((!compra) ? precioElegido->dias.back().cantidad : INT_MAX));
		if (op == 0)
		{
			std::cout << "\n\nValor ingresado no valido, presione cualquier tecla para intentarlo nuevamente";
			_getch();
			system("CLS");
		}
	} while (op == 0);

	precioElegido->nuevoDiaPrecioMerca(fecha, ( op * ((compra) ? 1 : -1 ) )); //efectua la compra/venta
	return precioElegido->precio * op; //devuelve el valor de esta (siempre positivo)
	
}

/**
 * @brief Le pide al usuario realizar operaciones con las cuentas permitidas en el sentido dictado hasta que
 *  se alcance un limite (de haberlo), o se decida parar.
 * 
 * @param t: enum del tipo de cuenta o filtro permitido para realizar operaciones
 * @param tipoPartida: enum representando si se estan sumando valores al debe, al haber, o si es el caso de una apertura
 * @param mensaje: string con el mensaje que acompania la seleccion de Cuenta
 * @param [limite]: float hasta el cual se deben hacer operaciones (siempre positivo)
 * 
 * @return float con la cantidad total que se sumo
 */

float aumentarPartida(Cuenta::Tipo t, tipoPartida tipoPartida, std::string mensaje, std::optional<float> limite)
{
	Cuenta* cuentaActual;
	std::string aumentoActualStr;
	float aumentoActual;
	float aumentoTotal = 0;
	DiaMerca* mercaModif; //usado en caso de mercaderias

	if (limite)
	{
		/* ----- permitir aumentos hasta el limite ----- */
		while(abs(aumentoTotal) != limite)
		{
			system("CLS");
			
			/* seleccion de cuenta */
			cuentaActual = elegirCuenta(t, mensaje, tipoPartida);

			/* verificacion mercaderias*/
			if (cuentaActual != buscarCuenta("Mercaderias"))
			{
				/// hay mercaderias!
				
				aumentoActual = seleccionarMercaderia(true);

				if (aumentoActual > 0)
				{
					aumentoTotal += aumentoActual;
					modificarCuenta(buscarCuenta("Mercaderias"), aumentoActual);	
				}

			} else {
				/// no hay mercaderias
				
				/* seleccion de cantidad */
				std::cout << "\n\nCuenta elegida: " << cuentaActual->nombre << " (valor: $" << cuentaActual->valorActual() << ")";
				std::cout << "\n\nTotal actual: $" << abs(aumentoTotal);
				std::cout << "\nLimite: $" << limite.value();
				std::cout << "\n\nSeleccione la cantidad: $";
				std::cin >> aumentoActualStr;

				/* validacion de cantidad */
				aumentoActual = validarFloat(aumentoActualStr, cuentaActual->valorActual(), limite, 1, (limite.value() - abs(aumentoTotal))); //se asegura de que la cantidad sea valida

				if (aumentoActual != 0)
				{
					/// cantidad valida!

					/* ajusta el signo de la modificacion */
					if (tipoPartida == Apertura)
					{
						aumentoActual = aumentoActual * ((cuentaActual->tipo == Cuenta::PASIVO) ? -1 : 1);
					}
					else {
						aumentoActual = aumentoActual * ((tipoPartida == Debe) ? 1 : -1);
					}

					aumentoTotal += aumentoActual;
					modificarCuenta(cuentaActual, aumentoActual);

				}
				else {
					/// cantidad invalida
					std::cout << "\n\nCantidad invalida, presione cualquier tecla para intentarlo nuevamente";
					_getch();
				}
			}
			
		}
	} else {
		/* ----- aumentos hasta que el usuario decida parar ----- */
		bool satisfecho = false;

		while (!satisfecho)
		{
			system("CLS");

			/* seleccion de cuenta */
			cuentaActual = elegirCuenta(t, mensaje, tipoPartida);

			/* verificacion mercaderias*/
			if (cuentaActual->nombre == "Mercaderias")
			{
				/// hay mercaderias!

				aumentoActual = seleccionarMercaderia(true);

				if (aumentoActual > 0)
				{
					aumentoTotal += aumentoActual;
					modificarCuenta(buscarCuenta("Mercaderias"), aumentoActual);

					/* permitir finalizar */
					system("CLS");
					std::cout << "1. Continuar\n2.Finalizar\nElija una opcion: ";
					std::cin >> aumentoActualStr; //reusado de variables!!
					satisfecho = (validarInt(aumentoActualStr, 1, 2) == 2) ? true : false;	
				}

			} else {

				/// no hay mercaderias
				
				/* seleccion de cantidad */
				std::cout << "\n\nTotal actual: $" << abs(aumentoTotal);
				std::cout << "\n\nSeleccione la cantidad: $";
				std::cin >> aumentoActualStr;

				/* validacion de cantidad */
				aumentoActual = validarFloat(aumentoActualStr, cuentaActual->valorActual(), {}, 1); //se asegura de que la cantidad sea valida
				if (aumentoActual != 0)
				{
					/// cantidad valida!

					/* ajusta el signo de la modificacion */
					if (tipoPartida == Apertura)
					{
						aumentoActual = aumentoActual * ((cuentaActual->tipo == Cuenta::PASIVO) ? -1 : 1);
					}
					else {
						aumentoActual = aumentoActual * ((tipoPartida == Debe) ? 1 : -1);
					}

					aumentoTotal += aumentoActual;
					modificarCuenta(cuentaActual, aumentoActual);

					/* permitir finalizar */
					system("CLS");
					std::cout << "1. Continuar\n2.Finalizar\nElija una opcion: ";
					std::cin >> aumentoActualStr; //reusado de variables!!
					satisfecho = (validarInt(aumentoActualStr, 1, 2) == 2) ? true : false;

				} else {
					/// cantidad invalida
					std::cout << "\n\nCantidad invalida, presione cualquier tecla para intentarlo nuevamente";
					_getch();
				}
			}
		}
	}
	return aumentoTotal;
}


/// ############################################################################################
/// ################################         OPCIONES         ########$#########################
/// ############################################################################################

/**
 * @brief Operacion que le pide al usuario la primera fecha, y con ella inicializa la primera operacion.
 *  luego invoca a la funcion aumentarPartida sin limite para que el usuario pueda ingresar cuentas hasta
 *  que decida detenerse (las cuentas son A+ P+ y R-, excepcion).
 *	Por ultimo, crea una Linea para contrarrestar el aporte hecho con la cuenta Capital, y finaliza la operacion
 * 
 */
void OP_Capital()
{
	/* ingreso fecha */
	std::string fechaStr;
	do
	{
		system("CLS");
		std::cout << "Ingrese la fecha de apertura: ";
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

	//aumenta operaciones hasta que el usuario decida
	float totalAumentado = aumentarPartida(Cuenta::F_OPER, Apertura, "Elija la cuenta usada en el inicio de operaciones", {});

	/* iguala cuentas con Capital(PN+) */
	modificarCuenta(buscarCuenta("Capital"), totalAumentado * -1);

	/* finaliza operacion*/
	operacionActual->documento = "Apertura";
	DIAS.back().nuevaOperacion(*operacionActual);
	oper = Operacion();
}


const std::vector<Opcion> OPCIONES = {
	
};


/// ############################################################################################
/// ################################         EJECUCIÓN         #################################
/// ############################################################################################

int main()
{
	MERCADERIAS.push_back(Mercaderia("Producto A"));
	MERCADERIAS.back().precios.push_back(PrecioMerca(15));
	MERCADERIAS.back().precios.back().nuevoDiaPrecioMerca("1/1", 3);

	MERCADERIAS.back().precios.push_back(PrecioMerca(28));
	MERCADERIAS.back().precios.back().nuevoDiaPrecioMerca("1/1", 5);
	MERCADERIAS.back().precios.back().nuevoDiaPrecioMerca("1/1", 1);


	MERCADERIAS.push_back(Mercaderia("Producto B"));
	MERCADERIAS.back().precios.push_back(PrecioMerca(365));
	MERCADERIAS.back().precios.back().nuevoDiaPrecioMerca("2/2", 15);

	MERCADERIAS.back().precios.push_back(PrecioMerca(446));


	MERCADERIAS.push_back(Mercaderia("Producto C"));
	/* division de vectores de Cuentas */
	for (int i = 0; i < CUENTAS.size(); i++)
	{
		if(CUENTAS[i].tipo == Cuenta::ACTIVO) {ACTIVOS.push_back((Cuenta*)&CUENTAS[i]); }
		else if (CUENTAS[i].tipo == Cuenta::PASIVO) { PASIVOS.push_back((Cuenta*)&CUENTAS[i]); }
		else if (CUENTAS[i].tipo == Cuenta::R_NEG) { R_NEGS.push_back((Cuenta*)&CUENTAS[i]); }
	}

	bool loop = true; //Controla la ejecucion del programa
	std::string opString;

	// -------- APERTURA --------
	std::cout << "=============== PROYECTO SIC ===============";
	std::cout << "\n\nIniciar con apertura?\n1. Si\n2. No\n";
	std::cin >> opString;
	if (validarInt(opString) == 1)
	{
		OP_Capital();
	}
	// -------- LOOP PRINCIPAL --------
	do
	{
		/* Display de opciones y ingreso de input */
		system("CLS");
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
	} while (loop);

	return 0;
}