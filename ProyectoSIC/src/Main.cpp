#include <iostream> //I/O basico
#include <fstream> //I/O de archivos .csv
#include <conio.h> // _getch()
#include <climits> //INT_MIN, INT_MAX
#include <cmath> //abs()
#include <regex> //regular expressions
#include <vector> //arrays dinamicos
#include <optional> //valores optativos

#include "clases.h" //estructuras de clases personalizadas
#include "presets.h" //vector CUENTAS

std::vector<DiaOperaciones> DIAS = {}; //lista de todas las operaciones que deben exportarse, en orden cronologico
std::vector<Mercaderia> MERCADERIAS = {}; //lista de todos los tipos de mercaderias usados
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
		if (CUENTAS[i].getNombre() == nombre)
		{
			return (Cuenta*)&CUENTAS[i];
		}
	}
	return nullptr;
}

/* Aniade comillas simples en los bordes del string */
std::string formatear(std::string texto)
{
	texto.insert(0, "'"); texto.append("'");
	return texto;
}

/* Imprime todos los valores de un vector en la respectiva columna del LibroMayor, dependiendo de su signo */
void imprimeValoresColumnaLibroMayor(std::ofstream &LibroMayor, std::vector<int> valores, unsigned int pos)
{

	for (unsigned int i = pos; i < valores.size(); i++)
	{
		if (valores[i] > 0)
		{
			LibroMayor << formatear(std::to_string(abs(valores[i]))) << ";''" << std::endl;
		} else {
			LibroMayor << "'';" << formatear(std::to_string(abs(valores[i]))) << std::endl;
		}
	}
	
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
 * @brief Crea un nuevo dia en el array con la fecha que se pide.
 * 
 * @param mensaje: String personalizable para mostrar al pedir el input.
 */
void pedirNuevaFecha(std::optional<std::string> mensaje = "Ingrese la nueva fecha")
{
	std::string fechaStr;
	do
	{
		system("CLS");
		std::cout << mensaje.value() << ": ";
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

/**
 * @brief Crea una Linea en la operacion actual y realiza la modificacion corresponiente en la Cuenta
 *  con la informacion recibida.
 * 
 * @param cuenta: Puntero a la cuenta que se esta modificando
 * @param modificacion: cantidad (positiva/negativa) de modificacion
 */
void modificarCuenta(Cuenta* cuenta, int modificacion)
{
	operacionActual->crearLinea(cuenta, modificacion); //agrega Linea a la Operacion actual
	cuenta->registrarModificacion(fecha, modificacion); //aumenta el valor de Cuenta
}

/**
 * @brief Funcion simple que pide un nombre para ponerle a la operacion ingresada.
 * 
 * @param op: Puntero con la operacion a modificar
 * @return Puntero con la operacion modificada
 */
Operacion* pedirNombreDocx(Operacion *op)
{
	std::string nombre;
	do
	{
		system("CLS");
		std::cout << "Ingrese el nombre del documento de esta operacion: ";
		std::getline(std::cin, nombre);
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		
		if (nombre.empty())
		{
			std::cout << "\n\nValor ingresado no valido, presione cualquier tecla para intentarlo nuevamente.";
			_getch();
		}
	} while (nombre.empty());
	op->getDocumento() = nombre;
	return op;
}

/* Pushea la operacion al ultimo dia */
void commitOperacion(Operacion* op)
{
	DIAS.back().crearOperacion(*operacionActual);
	oper = Operacion();
}

/**
 * @brief Muestra todas las cuentas del tipo/filtro determinado, en orden numerico. Pide elegir una
 *  y devuelve un puntero a ella.
 * 
 * @param t: el valor del enum de la clase Cuenta que deben tener las clases para mostrarse/filtro
 * @param tipoPart: el valor del enum del tipo de operacion que se busca
 * @param mensaje: el mensaje que se muestra acompaniando el menu de seleccion
 */
Cuenta* elegirCuenta(Cuenta::TipoCuenta t, tipoPartida tipoPart, std::string mensaje)
{
	std::string opStr;
	int op;

	/* ajuste de modificaciones */
	std::string mActivo, mPasivo, mResult;
	switch (tipoPart)
	{
	case Debe:
		mActivo = " (A+)"; mPasivo = " (P-)"; mResult = " (R-)";
		break;
	case Haber:
		mActivo = " (A-)"; mPasivo = " (P+)"; mResult = " ((R-)+)";
		break;
	case Apertura:
		mActivo = " (A+)"; mPasivo = " (P+)"; mResult = " (R-)";
		break;
	}

	/* Bucle validacion */
	do
	{
		std::vector<Cuenta*> pos = {}; //vector que asocia cada posicion con su cuenta correspondiente
		int cont = 1; //contador para el output
		
		system("CLS");
		/* Iteracion de cuentas */

		//verifica si se busca solo un tipo o un filtro
		if(t == Cuenta::TipoCuenta::F_OPER)
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

			for (int i = 0; i < ACTIVOS.size(); i++)
			{
				std::cout << "\n" << cont << ". " << ACTIVOS[i]->getNombre() << mActivo; //output
				pos.push_back(ACTIVOS[i]);
				cont++;
			}

			std::cout << "\n\n-------- PASIVOS --------";
			for (int i = 0; i < PASIVOS.size(); i++)
			{
				std::cout << "\n" << cont << ". " << PASIVOS[i]->getNombre() << mPasivo; //output
				pos.push_back(PASIVOS[i]);
				cont++;
			}

			std::cout << "\n\n-------- RESULTADOS --------";
			for (int i = 0; i < R_NEGS.size(); i++)
			{
				std::cout << "\n" << cont << ". " << R_NEGS[i]->getNombre() << mResult; //output
				pos.push_back(R_NEGS[i]);
				cont++;
			}
		}
		else {
			/// mostrar solo Activos, Pasivos o Resultados Negativos (segun t)

			/* set los modificadores */
			std::string titulo;
			std::string modificador;
			switch (t)
			{
			case Cuenta::TipoCuenta::ACTIVO_OPERATIVO:
				modificador = mActivo;
				titulo = "-------- ACTIVOS --------";
				break;
			case Cuenta::TipoCuenta::PASIVO_OPERATIVO:
				modificador = mPasivo;
				titulo = "-------- PASIVOS --------";
				break;
			case Cuenta::TipoCuenta::GASTO_OPERATIVO:
				modificador = mResult;
				titulo = "-------- RESULTADOS --------";
				break;
			}

			std::cout << titulo;
			/* recorre CUENTAS*/
			for (int i = 0; i < CUENTAS.size(); i++)
			{
				//solo muestra la cuenta si es del tipo buscado
				if (CUENTAS[i].getTipo() == t)
				{
					std::cout << "\n" << cont << ". " << CUENTAS[i].getNombre() << modificador; //output
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


struct operMercaderia
{
	int cantidad; int precioUnitario; int precioVenta;
};
/**
 * @brief Funcion utilizada para adquirir/vender mercaderia, eligiendo la mercaderia, su precio y su cantidad.
 *  Permite crear tanto mercaderias como precios de compra nuevos en el caso de una compra.
 * 
 * @param compra: Booleano: compra(true) / venta(false)
 * 
 * @return integer con el valor total de la venta/compra
 */
operMercaderia seleccionarMercaderia(bool compra)
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
				std::cout << "\n" << cont << ". " << MERCADERIAS[i].getNombre();
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
			hayOpcion = true;
		}
		
		// no hay opciones posibles
		if (!hayOpcion)
		{
			std::cout << "\nNo hay mercaderias disponibles!\n\nPresione cualquier tecla para cancelar";
			_getch();
			return {};
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
		std::cout << "=============== " << mercaElegida->getNombre() << ": PRECIOS UNITARIOS ===============\n";

		/* Iteracion de precios */
		for (int i = 0; i < mercaElegida->precios.size(); i++)
		{
			/* no es necesario verificar que al menos un precio tenga existencias, esta asegurado */

			// si es venta, comprueba que haya existencias para filtrar opciones
			if (compra || mercaElegida->precios[i].hayExistencias())
			{
				std::cout << "\n" << cont << ". " << mercaElegida->getNombre() << " ($" << mercaElegida->precios[i].precio << ")";
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
			std::cout << "=============== " << mercaElegida->getNombre() << ": NUEVO PRECIO UNITARIO ===============";
			std::cout << "\n\nIngrese el nuevo precio de compra: $";
			std::cin >> opStr;

			op = validarInt(opStr, {}, {}, 1);
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
	/* ----- Bucle validacion compra/venta ----- */
	int cantidad = 0; unsigned int precioVenta = 0;
	do
	{
		/* validar cantidad */

		system("CLS");
		std::cout << "=============== " << mercaElegida->getNombre() << " ($" << precioElegido->precio << " c/u) ===============";
		std::cout << "\n\nExistencias: " << ((precioElegido->hayExistencias()) ? precioElegido->dias.back().cantidad : 0) << " unidades"; //para compra, puede ser 0
		std::cout << "\n\nElija la cantidad de mercaderia que se " << ((compra) ? "compra" : "vende" ) << ": ";
		std::cin >> opStr;
		//si es compra, no hay maximo; si es venta, es la cantidad de existencias actuales (que no puede ser 0)
		op = validarInt(opStr, {}, {}, 1, ((!compra) ? precioElegido->dias.back().cantidad : INT_MAX));
		if (op == 0)
		{
			/// cantidad no valida
			std::cout << "\n\nValor ingresado no valido, presione cualquier tecla para intentarlo nuevamente";
			_getch();
			system("CLS");
		} else {
			///cantidad valida!
			
			cantidad = op; //set cantidad

			if (!compra)
			{
				/// necesidad de validar precio venta
				do
				{
					std::cout << "\n\nIngrese el precio unitario al cual se vende la mercaderia: $";
					std::cin >> opStr;
					op = validarInt(opStr, {}, {}, 1);
					if (op == 0)
					{
						/// precio venta no valido
						std::cout << "\n\nValor ingresado no valido, presione cualquier tecla para intentarlo nuevamente";
						_getch();
					} else {
						/// precio venta valido!
						
						precioVenta = op; // set precioVenta (solo en caso de venta)
					}
				} while (precioVenta == 0);
			}
		}
	} while (cantidad == 0);

	// carga la respuesta del struct
	operMercaderia resp;
	resp.cantidad = cantidad * ((compra) ? 1 : -1); resp.precioUnitario = precioElegido->precio; resp.precioVenta = precioVenta;

	precioElegido->nuevoDiaPrecioMerca(fecha, resp.cantidad); //efectua la modificacion de existencias

	return resp;
	
}

/**
 * @brief Le pide al usuario realizar operaciones con las cuentas permitidas en el sentido dictado hasta que
 *  se alcance un limite (de haberlo), o se decida parar.
 * 
 * @param t: enum del tipo de cuenta o filtro permitido para realizar operaciones
 * @param tipoPartida: enum representando si se estan sumando valores al debe, al haber, o si es el caso de una apertura
 * @param mensaje: string con el mensaje que acompania la seleccion de Cuenta
 * @param [limite]: int hasta el cual se deben hacer operaciones (siempre positivo)
 * 
 * @return int con la cantidad total que se sumo
 */

int aumentarPartida(Cuenta::TipoCuenta t, tipoPartida tipoPartida, std::string mensaje, std::optional<int> limite)
{
	Cuenta* cuentaActual;
	std::string aumentoActualStr;
	int aumentoActual;
	int aumentoTotal = 0;
	DiaMerca* mercaModif; //usado en caso de mercaderias

	if (limite)
	{
		/* ----- permitir aumentos hasta el limite ----- */
		while(abs(aumentoTotal) != limite)
		{
			system("CLS");
			
			/* seleccion de cuenta */
			cuentaActual = elegirCuenta(t, tipoPartida, mensaje);

			/* verificacion mercaderias*/
			if (cuentaActual == buscarCuenta("Mercaderias"))
			{
				/// hay mercaderias!
				
				operMercaderia operMerca = seleccionarMercaderia(true);
				aumentoActual = operMerca.cantidad * operMerca.precioUnitario;

				if (aumentoActual > 0)
				{
					aumentoTotal += aumentoActual;
					modificarCuenta(buscarCuenta("Mercaderias"), aumentoActual);	
				}

			} else {
				/// no hay mercaderias
				
				/* seleccion de cantidad */
				std::cout << "\n\nCuenta elegida: " << cuentaActual->getNombre() << " (valor: $" << cuentaActual->getSaldoActual() << ")";
				std::cout << "\n\nTotal actual: $" << abs(aumentoTotal);
				std::cout << "\nLimite: $" << limite.value();
				std::cout << "\n\nSeleccione la cantidad: $";
				std::cin >> aumentoActualStr;

				/* validacion de cantidad */
				aumentoActual = validarInt(aumentoActualStr, cuentaActual->getSaldoActual(), limite, 1, (limite.value() - abs(aumentoTotal))); //se asegura de que la cantidad sea valida

				if (aumentoActual != 0)
				{
					/// cantidad valida!

					/* ajusta el signo de la modificacion */
					if (tipoPartida == Apertura)
					{
						aumentoActual = aumentoActual * ((cuentaActual->getTipo() == Cuenta::TipoCuenta::PASIVO_OPERATIVO) ? -1 : 1);
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
			cuentaActual = elegirCuenta(t, tipoPartida, mensaje);

			/* verificacion mercaderias*/
			if (cuentaActual->getNombre() == "Mercaderias")
			{
				/// hay mercaderias!

				operMercaderia operMerca = seleccionarMercaderia(true);
				aumentoActual = operMerca.cantidad * operMerca.precioUnitario;

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
				aumentoActual = validarInt(aumentoActualStr, cuentaActual->getSaldoActual(), {}, 1); //se asegura de que la cantidad sea valida
				if (aumentoActual != 0)
				{
					/// cantidad valida!

					/* ajusta el signo de la modificacion */
					if (tipoPartida == Apertura)
					{
						aumentoActual = aumentoActual * ((cuentaActual->getTipo() == Cuenta::TipoCuenta::PASIVO_OPERATIVO) ? -1 : 1);
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

void NotaDC(bool credito)
{
	std::string opStr;
	int op;
	int modificacion;
	const Operacion* operacionModif = nullptr;
	const Linea* lineaModif = nullptr;
	tipoPartida tipo;

	/* elegir operacion valida */
	unsigned int cont;
	std::vector<const Operacion*> posOper;
	do
	{
		cont = 1;
		posOper = {};
		system("CLS");

		for (DiaOperaciones dia : DIAS) //recorre dias
		{
			for (const Operacion *operacion : dia.getOperaciones()) //recorre operaciones
			{
				std::cout << "\n" << cont << ". " << operacion->getDocumento() << " (" << dia.getFecha() << ")";
				posOper.push_back(operacion);
				cont++;
			}
		}
		std::cout << "\n\nElija la operacion a la que se refiere la nota: ";
		std::cin >> opStr;

		/* Validacion/return */
		op = validarInt(opStr, 1, posOper.size());
		if (op == 0)
		{
			/// valor no valido
			std::cout << "\nValor ingresado no valido, intentelo nuevamente.";
			_getch();
		}
		else {
			/// opcion elegida valida!
			operacionModif = posOper[op - 1];
		}
	} while (op == 0);

	/* verificacion mercaderias */
	if (operacionModif->contieneCuenta("Mercaderias"))
	{
		/// es compra-venta!
	}
	else {
		/// no es compra-venta!

		/* elegir linea de la modificacion */
		std::vector<const Linea*> posLinea;
		do
		{
			cont = 1;
			posLinea = {};
			system("CLS");

			for (const Linea* linea : operacionModif->getLineas()) //recorre lineas
			{
				std::cout << "\n" << cont << ". " << linea->cuenta->getNombre() << " (" << linea->delta << ")";
				posLinea.push_back(linea);
				cont++;
			}

			std::cout << "\n\nElija la cuenta que " << ((!credito) ? "aumenta" : "disminuye") <<  " en la nota: ";
			std::cin >> opStr;

			/* Validacion/return */
			op = validarInt(opStr, 1, posLinea.size());
			if (op == 0)
			{
				/// valor no valido
				std::cout << "\nValor ingresado no valido, intentelo nuevamente.";
				_getch();
			}
			else {
				/// opcion elegida valida!
				lineaModif = posLinea[op - 1];
			}
		} while (op == 0);

		/* ingresar modificacion */
		do
		{
			system("CLS");

			std::cout << "\n\n Ingrese el valor " << ((!credito) ? "del aumento" : "de la disminucion") << " de la cuenta: $";
			std::cin >> opStr;

			/* Validacion/return */
			op = validarInt(opStr, {}, {}, 1);
			if (op == 0)
			{
				/// valor no valido
				std::cout << "\nValor ingresado no valido, intentelo nuevamente.";
				_getch();
			}
			else {
				/// opcion elegida valida, realizar cambios!

				// ajusta signo modificacion y como se salda
				if (lineaModif->delta > 0)
				{
					/// La cuenta tenia una modificacion en el debe
					
					modificacion = op * ((!credito) ? 1 : -1); //si es debito la aumenta en el debe, sino en el haber
					tipo = (!credito) ? Haber : Debe; //contrrarestar acorde
				}
				else {
					/// La cuenta tenia una modificacion en el haber
					modificacion = op * ((!credito) ? -1 : 1); //si es debito la aumenta en el haber, sino en el debe
					tipo = (!credito) ? Debe : Haber; //contrarrestar acorde
				}

				modificarCuenta(lineaModif->cuenta, modificacion); //realiza modificacion

				/* realizar aumentoPartida para saciarlo */
				std::string mensaje = "Elija las cuentas que saldan el "; mensaje.append(((credito) ? "credito" : "debito"));
				aumentarPartida(Cuenta::TipoCuenta::F_OPER, tipo, mensaje, abs(modificacion));

				operacionActual = pedirNombreDocx(operacionActual);
				commitOperacion(operacionActual);
			}
		} while (op == 0);
	}

	/* cerrar operacion*/

}

/* Imprime las operaciones en un archivo LibroDiario.csv */
void EXP_LibroDiario()
{
	//inicializa archivo
	std::ofstream LibroDiario; LibroDiario.open("LibroDiario.csv");

	LibroDiario << "'';Cuenta;Modif;Debe;Haber" << std::endl << std::endl;

	DiaOperaciones* diaAct;
	const Operacion* opAct;
	const Linea* linAct;

	std::string nombreCuenta;
	std::string modif;
	int debe; int haber;

	///recorre dias
	for (DiaOperaciones dia : DIAS)
	{
		LibroDiario << "'" << dia.getFecha() << "'" << std::endl; // "'01/01'"

		/// recorre operaciones
		for (const Operacion* operacion : dia.getOperaciones())
		{

			///recorre lineas
			for (const Linea* linea : operacion->getLineas())
			{

				/* calcula valores */

				nombreCuenta = linea->cuenta->getNombre();
				
				//columnas debe y haber
				int delta = linea->delta;
				debe = ((delta > 0) ? delta : 0);
				haber = ((delta < 0) ? abs(delta) : 0);

				// modificador
				switch (linea->cuenta->getTipo())
				{
				case Cuenta::TipoCuenta::ACTIVO_OPERATIVO:
				case Cuenta::TipoCuenta::ACTIVO_NO_OPERATIVO:
					modif = ((debe != 0) ? "A+" : "A+");
					break;
				case Cuenta::TipoCuenta::PASIVO_OPERATIVO:
				case Cuenta::TipoCuenta::PASIVO_NO_OPERATIVO:
					modif = ((haber != 0) ? "P+" : "P-");
					break;
				case Cuenta::TipoCuenta::GASTO_OPERATIVO:
				case Cuenta::TipoCuenta::GASTO_NO_OPERATIVO:
				case Cuenta::TipoCuenta::GANANCIA:
					modif = ((haber != 0) ? "R+" : "R-");
					break;
				case Cuenta::TipoCuenta::PATRIMONIO_NETO:
					modif = ((haber != 0) ? "PN+" : "PN-");
					break;
				}

				/* imprime linea! */

				LibroDiario << "'';"
							<< (nombreCuenta) << ";"
							<< (modif) << ";" 
							<< formatear(std::to_string(debe)) << ";"
							<< formatear(std::to_string(haber)) << std::endl;

			} //lineas

			LibroDiario << "'';segun " << operacion->getDocumento() << std::endl << std::endl; //imprime documento
		} //operaciones

	} //dias
	LibroDiario.close();
}


/* Imprime el libro mayor de todas las cuentas usadas */
void EXP_LibroMayor()
{
	//inicializa archivo
	std::ofstream LibroMayor; LibroMayor.open("LibroMayor.csv");

	std::vector<int> debes; std::vector<int> haberes;
	int saldo;
	bool salir;

	for (Cuenta cuenta : CUENTAS)
	{
		debes.clear(); haberes.clear();

		if (cuenta.hayDias())
		{
			/// hay dias en la cuenta
			LibroMayor << cuenta.getNombre() << std::endl << "Debe;Haber" << std::endl;

			saldo = 0; salir = false;
			/* separa los deltas en positivos y negativos */
			for (DiaCuenta dia : cuenta.getDias())
			{
				if (dia.delta > 0)
				{
					debes.push_back(dia.delta);
				} else if (dia.delta < 0)
				{
					haberes.push_back(dia.delta);
				}

				saldo += dia.delta;
			}

			/* comienza a imprimir los valores */
			if (!debes.empty() && !haberes.empty())
			{
				for (unsigned int deb = 0; deb < debes.size(); deb++)
				{
					for (unsigned int hab = 0; hab < haberes.size(); hab++)
					{
						if (!salir)
						{
							/// imprime debe y haber en misma columna!
							LibroMayor << formatear(std::to_string(debes[deb])) << ";" << formatear(std::to_string(abs(haberes[hab]))) << std::endl;

							if (debes[deb] == debes.back() && haberes[hab] != haberes.back())
							{
								/// ultimo debe pero quedan haberes, imprime solo haberes
								imprimeValoresColumnaLibroMayor(LibroMayor, haberes, hab+1);
								salir = true;
							}
							else if (haberes[hab] == haberes.back() && debes[deb] != debes.back())
							{
								/// ultimo haber pero quedan debes, imprime solo debes
								imprimeValoresColumnaLibroMayor(LibroMayor, debes, deb+1);
								salir = true;
							}
						}
					}
				}
			}
			else if (debes.empty() || haberes.empty())
			{
				///imprime solo una de las dos columnas
				imprimeValoresColumnaLibroMayor(LibroMayor, ((!debes.empty()) ? debes : haberes ), 0);
			}

			if (saldo != 0)
			{
				LibroMayor << ((saldo > 0) ? "Saldo deudor: $" : "Saldo acreedor: $") << abs(saldo) << std::endl << std::endl;
			} else {
				LibroMayor << "Cuenta saldada: $0" << std::endl << std::endl;
			}
		}
	}

	LibroMayor.close();
}


/* Imprime el estado de resultados conformado por todas las cuentas de R */
void EXP_EstadoResultados()
{
	std::ofstream EstadoResultados; EstadoResultados.open("EstadoResultados.csv");
	int utilidad = (buscarCuenta("Ventas")->getSaldoActual() + buscarCuenta("CMV")->getSaldoActual()) * -1;

	/* Imprime Utilidad Bruta */
	EstadoResultados << "Ventas;" << buscarCuenta("Ventas")->getSaldoActual() * -1 << std::endl;
	EstadoResultados << "CMV;" << buscarCuenta("CMV")->getSaldoActual() * -1 << std::endl << std::endl;
	EstadoResultados << "Utilidad Bruta;" << utilidad << std::endl << std::endl;

	/* Imprime Utilidad Neto */

	for (Cuenta *cuentasGastos : R_NEGS)
	{
		if (cuentasGastos->getSaldoActual() != 0)
		{
			EstadoResultados << cuentasGastos->getNombre() << ";" << cuentasGastos->getSaldoActual() * -1 << std::endl;
			utilidad -= cuentasGastos->getSaldoActual();
		}
	}

	EstadoResultados << std::endl << "Utilidad Neto;$" << utilidad;
	EstadoResultados.close();
}



/// ############################################################################################
/// ################################         OPCIONES         ##################################
/// ############################################################################################

/* Llama a la funcion para crear una nueva fecha en el vector. */
void OP_NuevaFecha() { pedirNuevaFecha(); }

/**
 * @brief Operacion que le pide al usuario la primera fecha, y con ella inicializa la primera operacion.
 *  luego invoca a la funcion aumentarPartida sin limite para que el usuario pueda ingresar cuentas hasta
 *  que decida detenerse (las cuentas son A+ P+ y R-, excepcion).
 *	Por ultimo, crea una Linea para contrarrestar el aporte hecho con la cuenta Capital, y finaliza la operacion
 * 
 */
void OP_Capital()
{
	pedirNuevaFecha("Ingrese la fecha de apertura");

	//aumenta operaciones hasta que el usuario decida
	int totalAumentado = aumentarPartida(Cuenta::TipoCuenta::F_OPER, Apertura, "Elija la cuenta usada en el inicio de operaciones", {});

	/* iguala cuentas con Capital(PN+) */
	modificarCuenta(buscarCuenta("Capital"), totalAumentado * -1);

	/* finaliza operacion*/
	operacionActual->setDocumento("Apertura");
	commitOperacion(operacionActual);

	pedirNuevaFecha("Ingrese la primera fecha de operaciones"); //una vez finalizada apertura, crea nuevo dia
}

/* Pide valores en el Haber hasta que el usuario decida; luego pide valores en el debe hasta saciar el haber;
	por ultimo guarda la operacion */
void OP_Transaccion()
{
	/* ingreso de cuentas */
	int perdida = abs(aumentarPartida(Cuenta::TipoCuenta::F_OPER, tipoPartida::Haber, "Elija las cuentas del haber", {})); //le quita el signo negativo
	aumentarPartida(Cuenta::TipoCuenta::F_OPER, tipoPartida::Debe, "Elija las cuentas del debe", perdida);

	/* commit de operacion */
	operacionActual = pedirNombreDocx(operacionActual);
	commitOperacion(operacionActual);
}

/* Pide llevar a cabo una operacion de venta. Si es posible, llena las cuentas necesarias con los datos de la venta, y pide al usuario saciar los ingresos del debe;
	por ultimo guarda la operacion. */
void OP_VentaMercaderias()
{
	operMercaderia venta = seleccionarMercaderia(false);
	/* validacion posibilidad de venta */
	if (venta.cantidad != 0)
	{
		/// venta posible!
		
		int totalGanado = abs(venta.cantidad * venta.precioVenta); // valor absoluto para mayor comodidad
		modificarCuenta(buscarCuenta("Ventas"), totalGanado * -1); // Ventas (R+)

		aumentarPartida(Cuenta::TipoCuenta::F_OPER, Debe, "Elija las cuentas de ganancia de la venta", totalGanado); // iguala con debe

		/* anotar cmv y mercaderias */
		int totalPerdido = abs(venta.cantidad * venta.precioUnitario); // valor absoluto para mayor comodidad

		modificarCuenta(buscarCuenta("Mercaderias"), totalPerdido * -1); // Mercaderias (A-)
		modificarCuenta(buscarCuenta("CMV"), totalPerdido); // CMV (R-)

		/* guarda operacion */
		operacionActual = pedirNombreDocx(operacionActual);
		commitOperacion(operacionActual);
	}
}

/* Pide llevar a cabo una operacion de compra. LLena las cuentas necesarios con los datos de la compra, pide saciar las perdidas en el Haber.
	Por ultimo, guuarda la operacion*/
void OP_CompraMercaderias()
{
	/* compra de mercaderias */
	operMercaderia compra = seleccionarMercaderia(true);
	int totalPerdido = compra.cantidad * compra.precioUnitario; //siempre positivo

	/* amortizacion en Haber */
	modificarCuenta(buscarCuenta("Mercaderias"), totalPerdido); // Mercaderias (A+)
	aumentarPartida(Cuenta::TipoCuenta::F_OPER, Haber, "Elija las cuentas con las que se amortiza la compra", totalPerdido);

	/* guarda operacion */
	operacionActual = pedirNombreDocx(operacionActual);
	commitOperacion(operacionActual);
}

const std::vector<Opcion> OPCIONES = {
	Opcion("Nueva Fecha", []{ pedirNuevaFecha(); } ),
	Opcion("Transaccion de Cuentas", []{ OP_Transaccion(); }),
	Opcion("Venta de Mercaderias", &OP_VentaMercaderias),
	Opcion("Compra de Mercaderias", &OP_CompraMercaderias),
	Opcion("Nota de Credito", [] { NotaDC(true); } ),
	Opcion("Nota de Debito", [] { NotaDC(false); } ),
	Opcion("Exportar L. Diario", [] { EXP_LibroDiario(); }),
	Opcion("Exportar L. Mayor", [] { EXP_LibroMayor(); }),
	Opcion("Exportar Estado de Resultados", [] { EXP_EstadoResultados(); })
};


/// ############################################################################################
/// ################################         EJECUCIÓN         #################################
/// ############################################################################################

int main()
{
	/* division de vectores de Cuentas */
	for (Cuenta cuenta : CUENTAS)
	{
		if (cuenta.getTipo() == Cuenta::TipoCuenta::ACTIVO_OPERATIVO) { ACTIVOS.push_back((Cuenta*)&cuenta); }
		else if (cuenta.getTipo() == Cuenta::TipoCuenta::PASIVO_OPERATIVO) { PASIVOS.push_back((Cuenta*)&cuenta); }
		else if (cuenta.getTipo() == Cuenta::TipoCuenta::GASTO_OPERATIVO) { R_NEGS.push_back((Cuenta*)&cuenta); }
	}

	bool loop = true; //Controla la ejecucion del programa
	std::string opString;

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
			std::cout << i + 1 << ". " << OPCIONES[i].Nombre << "\n";
		}
		std::cin >> opString;

		/* Validacion */
		int op = validarInt(opString, 1, OPCIONES.size());
		if (op != 0)
		{
			/// input valido!
			OPCIONES[op - 1].Funcion();

			//loop = false; //provisorio
		}
		else {
			///input invalido
			std::cout << "Valor no valido, presione cualquier tecla para volver a intentarlo: ";
			_getch();
		}
	} while (loop);

	return 0;
}