#include "Main.h"

bool loop = true; //Controla la ejecucion del programa
std::vector<DiaOperaciones> DIAS = {};
std::vector<Mercaderia> MERCADERIAS = {};
std::vector<Cuenta> CUENTAS = {
	/* ----- activo ----- */
	Cuenta("Caja", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Fondo Fijo", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Bco. Cuenta Corriente", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Bco. Caja de Ahorro", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Moneda Extranjera", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Muebles y Utiles", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Instalaciones", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Equipos de Computacion", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Maquinarias", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Rodados", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Compras", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Acciones", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Patentes", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Derechos de Autor", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Marcas de Fabrica", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Formulas", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Valores a Depositar", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Documento a Cobrar", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Dividendo a Cobrar", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),

	Cuenta("Deudores por Ventas", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Deudores Varios", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Deudores Morosos", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Deudores en Litigio", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Socio Cuenta Aporte", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Gastos Pagados por Adelantado", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),

	Cuenta("Acciones con Caracter Transitorio", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Deposito a Plazo Fijo", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Inmueble destinado a la Renta", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),

	Cuenta("Mercaderias", Cuenta::TipoCuenta::ACTIVO_NO_OPERATIVO),

	/* ----- pasivo ----- */
	Cuenta("Proovedores", Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Acreedores Varios", Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Acreedores Prendarios", Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Acreedores Hipotecarios", Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Cheque Diferido a Pagar", Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Documento a Pagar", Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Dividendo a Pagar", Cuenta::TipoCuenta::PASIVO_OPERATIVO),

	Cuenta("Sueldo a Pagar", Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Cargas Sociales a Pagar", Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Indemnizaciones a Pagar", Cuenta::TipoCuenta::PASIVO_OPERATIVO),

	Cuenta("Adelanto en Cuenta Corriente", Cuenta::TipoCuenta::PASIVO_OPERATIVO),

	Cuenta("Prevision para Despido", Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Prevision para Accidentes de Trabajo", Cuenta::TipoCuenta::PASIVO_OPERATIVO),

	/* ----- patrimonio neto ----- */
	Cuenta("Capital", Cuenta::TipoCuenta::PATRIMONIO_NETO),

	/* ----- resultados ----- */
	Cuenta("Ventas", Cuenta::TipoCuenta::GANANCIA),
	Cuenta("CMV", Cuenta::TipoCuenta::GASTO_NO_OPERATIVO),

	Cuenta("Gastos", Cuenta::TipoCuenta::GASTO_OPERATIVO),
	Cuenta("Gastos de Publicidad", Cuenta::TipoCuenta::GASTO_OPERATIVO),
	Cuenta("Sueldos", Cuenta::TipoCuenta::GASTO_OPERATIVO),
	Cuenta("Cargas Sociales", Cuenta::TipoCuenta::GASTO_OPERATIVO),

	Cuenta("Depreciacion de Bienes de Uso", Cuenta::TipoCuenta::GASTO_OPERATIVO),
	Cuenta("Intereses Perdidos", Cuenta::TipoCuenta::GASTO_OPERATIVO),
	Cuenta("Faltante de Caja", Cuenta::TipoCuenta::GASTO_OPERATIVO),
	Cuenta("Deudores Incobrables", Cuenta::TipoCuenta::GASTO_OPERATIVO),

	Cuenta("Descuentos Otorgados", Cuenta::TipoCuenta::GASTO_OPERATIVO),
};
std::vector<Cuenta*> ACTIVOS = {};
std::vector<Cuenta*> PASIVOS = {};
std::vector<Cuenta*> GASTOS = {};

std::string fecha;
Operacion oper;
Operacion* operacionActual = &oper;

bool OP_Apertura()
{
	pedirNuevaFecha("Ingrese la fecha de apertura", "APERTURA");

	int aumentoTotal = AumentadorPartida::realizarAumento(true, Cuenta::TipoCuenta::F_OPER, ModoAumento::Apertura, "Elija la cuenta usada en el inicio de operaciones", {});
	
	if (aumentoTotal == 0) { return false; }
	
	modificarCuenta(buscarCuenta("Capital"), aumentoTotal * -1);
	
	operacionActual->setDocumento("Apertura");
	commitOperacion(operacionActual);

	return true;
	
}
void OP_Transaccion()
{
	int aumentoPerdida = AumentadorPartida::realizarAumento(true, Cuenta::TipoCuenta::F_OPER, ModoAumento::Haber, "Elija las cuentas del haber", {});
	if (aumentoPerdida == 0) { return; }
	
	int aumentoGanancia = AumentadorPartida::realizarAumento(false, Cuenta::TipoCuenta::F_OPER, ModoAumento::Debe, "Elija las cuentas del debe", abs(aumentoPerdida));

	operacionActual = pedirNombreDocx(operacionActual);
	commitOperacion(operacionActual);
}

void OP_VentaMercaderias()
{
	SeleccionadorDeMercaderias venta(true, false);
	if (venta.getCantidad() != 0)
	{
		/// venta realizada!

		int totalGanado = venta.getTotalGanadoVenta();
		modificarCuenta(buscarCuenta("Ventas"), totalGanado * -1);

		int igualacionEnDebe = AumentadorPartida::realizarAumento(false ,Cuenta::TipoCuenta::F_OPER, ModoAumento::Debe, "Elija las cuentas de ganancia de la venta", totalGanado);

		/* anotar cmv y mercaderias */
		int totalPerdido = venta.getTotalPerdidoVenta();

		modificarCuenta(buscarCuenta("Mercaderias"), totalPerdido * -1);
		modificarCuenta(buscarCuenta("CMV"), totalPerdido);

		/* guarda operacion */
		operacionActual = pedirNombreDocx(operacionActual);
		commitOperacion(operacionActual);
	}
}
void OP_CompraMercaderias()
{
	SeleccionadorDeMercaderias compra(true, true);
	if (compra.getCantidad() != 0)
	{
		/// compra realizada!
		int totalCompraMercaderias = compra.getTotalGastadoCompra();

		modificarCuenta(buscarCuenta("Mercaderias"), totalCompraMercaderias);
		int igualacionEnHaber = AumentadorPartida::realizarAumento(false, Cuenta::TipoCuenta::F_OPER, ModoAumento::Haber, "Elija las cuentas con las que se amortiza la compra", totalCompraMercaderias * -1);

		/* guarda operacion */
		operacionActual = pedirNombreDocx(operacionActual);
		commitOperacion(operacionActual);
	}
}

void NotaDC(bool credito)
{
	std::string opStr;
	int op;
	int modificacion;
	const Operacion* operacionModif = nullptr;
	const Linea* lineaModif = nullptr;
	ModoAumento tipo;

	// elegir operacion valida
	unsigned int cont;
	std::vector<const Operacion*> posOper;
	do
	{
		cont = 1;
		posOper = {};
		header( std::string(credito ? "NOTA DE CREDITO" : "NOTA DE DEBITO") += ": LISTADO DE OPERACIONES", 1);

		for (const DiaOperaciones& dia : DIAS)
		{
			for (const Operacion* operacion : dia.getOperaciones())
			{
				std::cout << "\n" << cont << ". " << operacion->getDocumento() << " (" << dia.getFecha() << ")";
				posOper.push_back(operacion);
				cont++;
			}
		}
		std::cout << "\n" << cont << ". Cancelar";

		std::cout << "\n\nElija la operacion a la que se refiere la nota: ";
		std::cin >> opStr;

		// Validacion/return
		op = validarInt(opStr, {}, {}, 1, posOper.size() + 1);
		if (op == 0)
		{
			/// valor no valido
			std::cout << "\n\nValor ingresado no valido, intentelo nuevamente.";
			_getch();
		}
		else if (op == posOper.size() + 1) {
			return; // cancelar!
		} else {
			/// opcion elegida valida!
			operacionModif = posOper[op - 1];
		}
	} while (op == 0);

	/* verificacion mercaderias */
	if (operacionModif->contieneCuenta("Mercaderias"))
	{
		std::cout << "\n\nFuncionalidad no admitida: mercaderia interviniente!";
		std::cout << "\n\nPresione cualquier tecla para volver...";
		_getch();
		return;
	}
	else {
		/// no es compra-venta!

		// elegir linea de la modificacion
		std::vector<const Linea*> posLinea;
		do
		{
			cont = 1;
			posLinea = {};
			header(operacionModif->getDocumento() += ": LISTADO DE CUENTAS", 1);

			for (const Linea* linea : operacionModif->getLineas())
			{
				std::cout << "\n" << cont << ". " << linea->cuenta->getNombre() << " (" << linea->delta << ")";
				posLinea.push_back(linea);
				cont++;
			}

			std::cout << "\n\nElija la cuenta que " << ((!credito) ? "aumenta" : "disminuye") << " en la nota: ";
			std::cin >> opStr;

			// Validacion/return
			op = validarInt(opStr, {}, {}, 1, posLinea.size());
			if (op == 0)
			{
				/// valor no valido
				std::cout << "\n\nValor ingresado no valido, intentelo nuevamente.";
				_getch();
			}
			else {
				/// opcion elegida valida!
				lineaModif = posLinea[op - 1];
			}
		} while (op == 0);

		// ingresar modificacion
		do
		{
			header(lineaModif->cuenta->getNombre() += (!credito ? ": AUMENTO" : ": DISMINUCION"));

			std::cout << "\n\n Ingrese el valor " << ((!credito) ? "del aumento" : "de la disminucion") << " de la cuenta: $";
			std::cin >> opStr;

			/* Validacion/return */
			op = validarInt(opStr, {}, {}, 1);
			if (op == 0)
			{
				/// valor no valido
				std::cout << "\n\nValor ingresado no valido, intentelo nuevamente.";
				_getch();
			}
			else {
				/// opcion elegida valida, realizar cambios!

				// ajusta signo modificacion y como se salda
				if (lineaModif->delta > 0)
				{
					/// La cuenta tenia una modificacion en el debe

					modificacion = op * ((!credito) ? 1 : -1); //si es debito la aumenta en el debe, sino en el haber
					tipo = (!credito) ? ModoAumento::Haber : ModoAumento::Debe; //contrrarestar acorde
				}
				else {
					/// La cuenta tenia una modificacion en el haber
					modificacion = op * ((!credito) ? -1 : 1); //si es debito la aumenta en el haber, sino en el debe
					tipo = (!credito) ? ModoAumento::Debe : ModoAumento::Haber; //contrarrestar acorde
				}

				modificarCuenta(lineaModif->cuenta, modificacion); //realiza modificacion

				// realizar aumentoPartida para saciarlo
				std::string mensaje = std::string("Elija las cuentas que saldan el ") += (credito ? "credito" : "debito");
				int aumento = AumentadorPartida::realizarAumento(false, Cuenta::TipoCuenta::F_OPER, tipo, mensaje, modificacion * -1);

				operacionActual = pedirNombreDocx(operacionActual);
				commitOperacion(operacionActual);
			}
		} while (op == 0);
	}
}

void OP_mostrarLibroDiario()
{
	std::string separador = "--------------------------------------------------+------------+------------";
	std::string lineaVacia = "|            |            ";

	header("LIBRO DIARIO", 3);

	//                 50                  |     12     |     12     
	std::cout << "Cuenta                                            |    Debe    |   Haber  " << std::endl
		      << separador << std::endl;

	std::string nombreCuenta;
	std::string modif;
	bool esDebe;

	for (DiaOperaciones& dia : DIAS)
	{
		std::cout << formatearColumnaCuenta(dia.getFecha()) << lineaVacia << std::endl
			<< formatearColumnaCuenta("") << lineaVacia << std::endl;

		for (const Operacion* operacion : dia.getOperaciones())
		{
			for (const Linea* linea : operacion->getLineas())
			{
				nombreCuenta = linea->cuenta->getNombre();
				esDebe = (linea->delta > 0) ? true : false;

				// modificador
				switch (linea->cuenta->getTipo())
				{
				case Cuenta::TipoCuenta::ACTIVO_OPERATIVO:
				case Cuenta::TipoCuenta::ACTIVO_NO_OPERATIVO:
					modif = esDebe ? "A+" : "A-";
					break;
				case Cuenta::TipoCuenta::PASIVO_OPERATIVO:
				case Cuenta::TipoCuenta::PASIVO_NO_OPERATIVO:
					modif = esDebe ? "P-" : "P+";
					break;
				case Cuenta::TipoCuenta::GASTO_OPERATIVO:
				case Cuenta::TipoCuenta::GASTO_NO_OPERATIVO:
				case Cuenta::TipoCuenta::GANANCIA:
					modif = esDebe ? "R-" : "R+";
					break;
				case Cuenta::TipoCuenta::PATRIMONIO_NETO:
					modif = esDebe ? "PN-" : "PN+";
					break;
				}
				nombreCuenta += " "; nombreCuenta += modif;

				std::cout << formatearColumnaCuenta(nombreCuenta) << "|" 
					<< formatearColumnaNumero(esDebe ? linea->delta : 0) << "|" 
					<< formatearColumnaNumero(!esDebe ? abs(linea->delta) : 0) << std::endl;

			} //lineas

			std::string nombreDocumento = "segun "; nombreDocumento += operacion->getDocumento();
			std::cout << formatearColumnaCuenta(nombreDocumento)
				<< lineaVacia << std::endl 
				<< separador << std::endl;
		} //operaciones

	} //dias
	std::cout << std::endl << std::endl << "Presione cualquier tecla para volver...";
	_getch();
}

std::string formatearColumnaCuenta(std::string str)
{
	if (str.size() > 50)
	{
		str = str.substr(0, 50);
	} else if (str.size() < 50)
	{
		while (str.size() != 50)
		{
			str += " ";
		}
	}
	return str;
}
std::string formatearColumnaNumero(int num)
{
	std::string str = std::to_string(num);
	if (str.size() < 12)
	{
		while (str.size() < 12)
		{
			str.insert(0," ");
		}
	}
	return str;
}

void EXP_LibroDiario(bool mostrarMensaje)
{
	//inicializa archivo
	std::ofstream LibroDiario; LibroDiario.open("LibroDiario.csv");

	LibroDiario << "'';Cuenta;Modif;Debe;Haber" << std::endl << std::endl;

	std::string nombreCuenta;
	std::string modif;
	bool esDebe;

	for (DiaOperaciones &dia : DIAS)
	{
		LibroDiario << formatear(dia.getFecha()) << std::endl; // "'01/01'"

		for (const Operacion* operacion : dia.getOperaciones())
		{
			for (const Linea* linea : operacion->getLineas())
			{
				nombreCuenta = linea->cuenta->getNombre();
				esDebe = (linea->delta > 0) ? true : false;

				// modificador
				switch (linea->cuenta->getTipo())
				{
				case Cuenta::TipoCuenta::ACTIVO_OPERATIVO:
				case Cuenta::TipoCuenta::ACTIVO_NO_OPERATIVO:
					modif = esDebe ? "A+" : "A-";
					break;
				case Cuenta::TipoCuenta::PASIVO_OPERATIVO:
				case Cuenta::TipoCuenta::PASIVO_NO_OPERATIVO:
					modif = esDebe ? "P-" : "P+";
					break;
				case Cuenta::TipoCuenta::GASTO_OPERATIVO:
				case Cuenta::TipoCuenta::GASTO_NO_OPERATIVO:
				case Cuenta::TipoCuenta::GANANCIA:
					modif = esDebe ? "R-" : "R+";
					break;
				case Cuenta::TipoCuenta::PATRIMONIO_NETO:
					modif = esDebe ? "PN-" : "PN+";
					break;
				}


				LibroDiario << "'';"
							<< nombreCuenta << ";"
							<< modif << ";" 
							<< formatear(esDebe ? linea->delta : 0) << ";"
							<< formatear(!esDebe ? abs(linea->delta) : 0) << std::endl;

			} //lineas

			LibroDiario << "'';segun " << operacion->getDocumento() << std::endl << std::endl;
		} //operaciones

	} //dias
	LibroDiario.close();

	if (mostrarMensaje)
	{
		header("EXPORTAR LIBRO DIARIO", 2);
		std::cout << "Libro diario exportado como LibroDiario.csv en el directorio actual!";
		std::cout << "\n\nPresione cualquier tecla para continuar...";
		_getch();
	}
}
void EXP_LibroMayor(bool mostrarMensaje)
{
	std::ofstream LibroMayor; LibroMayor.open("LibroMayor.csv");

	std::vector<int> debes; std::vector<int> haberes;
	int saldo;
	bool salir;

	for (Cuenta &cuenta : CUENTAS)
	{
		debes.clear(); haberes.clear();

		if (cuenta.hayDias())
		{
			LibroMayor << cuenta.getNombre() << std::endl << "Debe;Haber" << std::endl;

			saldo = 0; salir = false;

			for (DiaCuenta &dia : cuenta.getDias())
			{
				if (dia.delta > 0)
				{
					debes.push_back(dia.delta);
				} else if (dia.delta < 0)
				{
					haberes.push_back(abs(dia.delta));
				}

				saldo += dia.delta;
			}

			unsigned int lineaActual = 0;
			while (lineaActual < debes.size() || lineaActual < haberes.size())
			{
				LibroMayor
					<< (lineaActual < debes.size() ? formatear(debes[lineaActual]) : "") << ";"
					<< (lineaActual < haberes.size() ? formatear(haberes[lineaActual]) : "") << std::endl;
				lineaActual++;
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

	if (mostrarMensaje)
	{
	header("EXPORTAR LIBRO MAYOR", 2);
	std::cout << "Libro mayor exportado como LibroMayor.csv en el directorio actual!";
	std::cout << "\n\nPresione cualquier tecla para continuar...";
	_getch();
	}
}
void EXP_EstadoResultados(bool mostrarMensaje)
{
	std::ofstream EstadoResultados; EstadoResultados.open("EstadoResultados.csv");
	int utilidad = (buscarCuenta("Ventas")->getSaldoActual() + buscarCuenta("CMV")->getSaldoActual()) * -1;

	EstadoResultados << "Ventas;$" << buscarCuenta("Ventas")->getSaldoActual() * -1 << std::endl;
	EstadoResultados << "CMV;$" << buscarCuenta("CMV")->getSaldoActual() * -1 << std::endl << std::endl;
	EstadoResultados << "Utilidad Bruta;$" << utilidad << std::endl << std::endl;

	for (const Cuenta *cuentaGastos : GASTOS)
	{
		if (cuentaGastos->getSaldoActual() != 0)
		{
			EstadoResultados << cuentaGastos->getNombre() << ";$" << cuentaGastos->getSaldoActual() * -1 << std::endl;
			utilidad -= cuentaGastos->getSaldoActual();
		}
	}
	EstadoResultados << std::endl << "Utilidad Neto;$" << utilidad;

	EstadoResultados.close();

	if (mostrarMensaje)
	{
		header("EXPORTAR ESTADO DE RESULTADOS", 2);
		std::cout << "Estado de Resultados exportado como EstadoResultados.csv en el directorio actual!";
		std::cout << "\n\nPresione cualquier tecla para continuar...";
		_getch();
	}
}
std::string formatear(std::string texto)
{
	texto.insert(0, "'"); texto.append("'");
	return texto;
}
std::string formatear(int texto)
{
	std::string strTexto = std::to_string(texto);
	strTexto.insert(0, "'"); strTexto.append("'");
	return strTexto;
}

void initVectores() {
	for (Cuenta& cuenta : CUENTAS)
	{
		if (cuenta.getTipo() == Cuenta::TipoCuenta::ACTIVO_OPERATIVO) { ACTIVOS.push_back(&cuenta); }
		else if (cuenta.getTipo() == Cuenta::TipoCuenta::PASIVO_OPERATIVO) { PASIVOS.push_back(&cuenta); }
		else if (cuenta.getTipo() == Cuenta::TipoCuenta::GASTO_OPERATIVO) { GASTOS.push_back(&cuenta); }
	}
}

const std::vector<Opcion> OPCIONES = {
	Opcion("Nueva Fecha", [] { pedirNuevaFecha("Ingrese la nueva fecha", "NUEVA FECHA"); }),
	Opcion("Transaccion de Cuentas", [] { OP_Transaccion(); }),
	Opcion("Venta de Mercaderias", &OP_VentaMercaderias),
	Opcion("Compra de Mercaderias", &OP_CompraMercaderias),
	Opcion("Nota de Credito", [] { NotaDC(true); }),
	Opcion("Nota de Debito", [] { NotaDC(false); }),
	Opcion("Ver L. Diario", [] { OP_mostrarLibroDiario(); }),
	Opcion("Exportar L. Diario", [] { EXP_LibroDiario(true); }),
	Opcion("Exportar L. Mayor", [] { EXP_LibroMayor(true); }),
	Opcion("Exportar Estado de Resultados", [] { EXP_EstadoResultados(true); }),
	Opcion("Salir del Programa", []{ loop = false; EXP_LibroDiario(false); EXP_LibroMayor(false); EXP_EstadoResultados(false); })
};

int main()
{
	initVectores();

	std::string opString;
	bool pedirFecha = true;

	header("PROYECTO SIC", 2);
	std::cout << "Iniciar con apertura?\n1. Si\n2. No\n";
	std::cin >> opString;
	if (validarInt(opString, {}, {}, 1) == 1)
	{
		pedirFecha = OP_Apertura();
	}
	if (pedirFecha) { pedirNuevaFecha("Ingrese la primera fecha de operaciones", "COMIENZO DE OPERACIONES"); }
	
	// -------- LOOP PRINCIPAL --------
	do
	{
		header("PROYECTO SIC", 1);
		for (unsigned int i = 0; i < OPCIONES.size(); i++)
		{
			std::cout << "\n" << i + 1 << ". " << OPCIONES[i].Nombre;
		}

		std::cout << "\n\nSeleccione una opcion: ";
		std::cin >> opString;

		
		int op = validarInt(opString, {}, {}, 1, OPCIONES.size());
		if (op != 0)
		{
			/// input valido!
			OPCIONES[op - 1].Funcion();
		}
		else {
			///input invalido
			std::cout << "\n\nValor no valido, presione cualquier tecla para volver a intentarlo: ";
			_getch();
		}
	} while (loop);

	return 0;
}