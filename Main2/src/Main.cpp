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

bool OP_Nota::esCredito;

const Operacion* OP_Nota::operacionModificada;
std::vector<const Operacion*> OP_Nota::operacionesDisponibles;
int OP_Nota::posOperacion;

const Linea* OP_Nota::lineaModificada;
std::vector<const Linea*> OP_Nota::lineasDisponibles;
int OP_Nota::posLinea;

ModoAumento OP_Nota::tipo;
int OP_Nota::modificacion;

void OP_Nota::efectuarNota(bool _esCredito)
{
	esCredito = _esCredito;

	if (intentarElegirOperacion())
	{
		elegirLinea();

		if (lineaModificada->cuenta->getNombre() == "Mercaderias")
		{
			/// operacion respecto a mercaderias
		}
		else if (lineaModificada->cuenta->getNombre() == "Venta")
		{
			/// operacion respecto a venta
		}
		else {
			operacionConvencional();
		}
	}
}
bool OP_Nota::intentarElegirOperacion()
{
	unsigned int cont;
	std::string strOperacion;

	do
	{
		bool hayOperaciones = false;
		cont = 1;
		operacionesDisponibles.clear();
		header(std::string(esCredito ? "NOTA DE CREDITO" : "NOTA DE DEBITO") += ": LISTADO DE OPERACIONES", 1);

		for (const DiaOperaciones& dia : DIAS)
		{
			for (const Operacion* operacion : dia.getOperaciones())
			{
				std::cout << "\n" << cont << ". " << operacion->getDocumento() << " (" << dia.getFecha() << ")";
				operacionesDisponibles.push_back(operacion);
				cont++;

				hayOperaciones = true;
			}
		}

		if (!hayOperaciones)
		{
			std::cout << "\nNo hay dias con operaciones validas!\n\nPresione cualquier tecla para volver...";
			_getch();
			return false;
		}

		std::cout << "\n" << cont << ". Cancelar";

		std::cout << "\n\nElija la operacion a la que se refiere la nota: ";
		std::cin >> strOperacion;

	} while (!validarOperacion(strOperacion));

	if (posOperacion == operacionesDisponibles.size() + 1) {
		return false; // cancelar!
	} else {
		/// opcion elegida valida!
		operacionModificada = operacionesDisponibles[posOperacion - 1];
		return true;
	}
}
bool OP_Nota::validarOperacion(std::string strOperacion)
{
	posOperacion = validarInt(strOperacion, 1, operacionesDisponibles.size() + 1);
	if (posOperacion == 0)
	{
		/// valor no valido
		std::cout << "\n\nValor ingresado no valido, intentelo nuevamente.";
		_getch();
		return false;
	}
	return true;
}

void OP_Nota::elegirLinea()
{
	unsigned int cont;
	std::string strLinea;

	do
	{
		cont = 1;
		lineasDisponibles.clear();
		header(operacionModificada->getDocumento() += ": LISTADO DE CUENTAS", 1);

		for (const Linea* linea : operacionModificada->getLineas())
		{
			if (linea->cuenta->getNombre() != "CMV")
			{
				std::cout << "\n" << cont << ". " << linea->cuenta->getNombre() << " (" << linea->delta << ")";
				lineasDisponibles.push_back(linea);
				cont++;
			}
		}

		std::cout << "\n\nElija la cuenta que " << ((!esCredito) ? "aumenta" : "disminuye") << " en la nota: ";
		std::cin >> strLinea;

	} while (!validarLinea(strLinea));
	
	lineaModificada = lineasDisponibles[posLinea - 1];
}
bool OP_Nota::validarLinea(std::string strLinea)
{
	posLinea = validarInt(strLinea, 1, lineasDisponibles.size());
	if (posLinea == 0)
	{
		/// valor no valido
		std::cout << "\n\nValor ingresado no valido, intentelo nuevamente.";
		_getch();
		return false;
	}
	return true;
}

void OP_Nota::operacionConvencional()
{
	std::string strValorAumento;

	do
	{
		header(lineaModificada->cuenta->getNombre() += (!esCredito ? ": AUMENTO" : ": DISMINUCION"));

		std::cout << "\n\n Ingrese el valor " << ((!esCredito) ? "del aumento" : "de la disminucion") << " de la cuenta: $";
		std::cin >> strValorAumento;

	} while (!validarModificacion(strValorAumento));

	ajustarSignoModificacion();

	modificarCuenta(lineaModificada->cuenta, modificacion); //realiza modificacion

	// realizar aumentoPartida para saciarlo
	std::string mensaje = std::string("Elija las cuentas que saldan el ") += (esCredito ? "credito" : "debito");
	int aumento = AumentadorPartida::realizarAumento(false, Cuenta::TipoCuenta::F_OPER, tipo, mensaje, modificacion * -1);

	operacionActual = pedirNombreDocx(operacionActual);
	commitOperacion(operacionActual);
		
}
bool OP_Nota::validarModificacion(std::string strValorAumento)
{
	modificacion = validarInt(strValorAumento, 1);
	if (modificacion == 0)
	{
		/// valor no valido
		std::cout << "\n\nValor ingresado no valido, intentelo nuevamente.";
		_getch();
		return false;
	}
	return true;
}
void OP_Nota::ajustarSignoModificacion()
{
	if (lineaModificada->delta > 0)
	{
		/// La cuenta tenia una modificacion en el debe
		modificacion = modificacion * ((!esCredito) ? 1 : -1); //si es debito la aumenta en el debe, sino en el haber
		tipo = (!esCredito) ? ModoAumento::Haber : ModoAumento::Debe; //contrrarestar acorde
	} else {
		/// La cuenta tenia una modificacion en el haber
		modificacion = modificacion * ((!esCredito) ? -1 : 1); //si es debito la aumenta en el haber, sino en el debe
		tipo = (!esCredito) ? ModoAumento::Debe : ModoAumento::Haber; //contrarrestar acorde
	}
}

void OP_mostrarLibroDiario()
{
	header("LIBRO DIARIO", 3);

	std::string separador = "--------------------------------------------------+------------+------------";
	std::string lineaVacia = "|            |            ";

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
				nombreCuenta += " ("; nombreCuenta += modif; nombreCuenta += ")";

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
			str.insert(0, " ");
		}
	}
	return str;
}

void OP_mostrarLibroMayor()
{
	header("LIBRO MAYOR", 3);

	std::vector<int> debes; std::vector<int> haberes;
	int saldo;
	bool salir;

	for (Cuenta& cuenta : CUENTAS)
	{
		debes.clear(); haberes.clear();

		if (cuenta.hayDias())
		{
			std::string nombreCuenta = cuenta.getNombre();
			nombreCuenta.insert(0, " "); nombreCuenta.append(" ");

			//           12     |    12
			std::cout << formatearCentrado(nombreCuenta, 25, "=") << std::endl 
				<< "    Debe    |   Haber  " << std::endl;

			saldo = 0; salir = false;

			for (DiaCuenta& dia : cuenta.getDias())
			{
				if (dia.delta > 0)
				{
					debes.push_back(dia.delta);
				}
				else if (dia.delta < 0)
				{
					haberes.push_back(abs(dia.delta));
				}

				saldo += dia.delta;
			}

			unsigned int lineaActual = 0;
			while (lineaActual < debes.size() || lineaActual < haberes.size())
			{
				std::cout
					<< (lineaActual < debes.size() ? formatearColumnaNumero(debes[lineaActual]) : "            ") << "|"
					<< (lineaActual < haberes.size() ? formatearColumnaNumero(haberes[lineaActual]) : "            ") << std::endl;
				lineaActual++;
			}
			std::cout << "------------+------------" << std::endl;

			if (saldo != 0)
			{
				std::cout << formatearSaldo(saldo);
			}
			else {
				std::cout << "   Cuenta saldada: $0    ";
			}
			std::cout << std::endl << std::endl << std::endl << std::endl;
		}
	}

	std::cout << std::endl << std::endl << "Presione cualquier tecla para volver...";
	_getch();
}
std::string formatearSaldo(int saldo)
{
	std::string str = std::to_string(abs(saldo));
	str.insert(0, (saldo > 0 ? "Saldo deudor: $" : "Saldo acreedor: $"));
	return formatearCentrado(str, 25, " ");
}

void OP_mostrarEstadoResultados()
{
	header("ESTADO DE RESULTADOS", 3);

	int utilidad = (buscarCuenta("Ventas")->getSaldoActual() + buscarCuenta("CMV")->getSaldoActual()) * -1;

	std::cout << "Ventas: " << formatearResultado(buscarCuenta("Ventas")->getSaldoActual() * -1) << std::endl;
	std::cout << "CMV: " << formatearResultado(buscarCuenta("CMV")->getSaldoActual() * -1) << std::endl << std::endl;

	std::cout << "Utilidad Bruta: " << formatearResultado(utilidad) << std::endl << std::endl;

	for (const Cuenta* cuentaGastos : GASTOS)
	{
		if (cuentaGastos->getSaldoActual() != 0)
		{
			std::cout << cuentaGastos->getNombre() << ": " << formatearResultado(cuentaGastos->getSaldoActual() * -1) << std::endl;
			utilidad -= cuentaGastos->getSaldoActual();
		}
	}
	std::cout << std::endl << "Utilidad Neto: " << formatearResultado(utilidad);

	std::cout << std::endl << std::endl << "Presione cualquier tecla para volver...";
	_getch();
}
std::string formatearResultado(int num)
{
	std::string str = std::to_string(abs(num));
	str.insert(0, (num >= 0 ? "$" : "-$"));
	return str;
}

void OP_mostrarFichaStock()
{
	std::string grupoVacio = "            |            |            ";
	std::string separadorDia = "------------||------------+------------+------------||------------+------------+------------||------------+------------+------------||";

	bool primerRegistro;
	std::vector<ExistenciasPrecioMercaderia> Existencias;
	unsigned int iter;

	header("FICHA DE STOCK", 2);

	for (const Mercaderia& mercaderia : MERCADERIAS)
	{
		Existencias.clear();

		std::cout << formatearNombreMercaderia(mercaderia.getNombre()) << std::endl
			//     12                          38
			<< "   Fecha    ||               Ingresos               ||                Egresos               ||              Existencias             ||" << std::endl
			<< "            ||    unid.   |   costo    | costo tot. ||    unid.   |   costo    | costo tot. ||    unid.   |   costo    | costo tot. ||"
			<< std::endl;

		for (const DiaMercaderia* dia : mercaderia.getDias())
		{
			//actualiza Existencias para todo el dia
			for (const RegistroPrecio& registro : dia->Registros)
			{
				unsigned int pos = buscarOCrearExistencia(Existencias, registro.precio);
				Existencias[pos].modificar(registro.delta);
				if (Existencias[pos].existencias == 0) { Existencias.erase(Existencias.begin() + pos); }
			}

			iter = 0;
			primerRegistro = true;
			//sigue imprimiendo hasta que no queden ni registros ni existencias
			while (iter < dia->Registros.size() || iter < Existencias.size())
			{
				std::cout << formatearCentrado( (primerRegistro ? dia->Fecha : ""), 12, " " ) << "||" //fecha
					<< (iter < dia->Registros.size() ? (dia->Registros[iter].delta > 0 ? mostrarRegistro(dia->Registros[iter]) : grupoVacio) : grupoVacio) << "||" //ingresos
					<< (iter < dia->Registros.size() ? (dia->Registros[iter].delta < 0 ? mostrarRegistro(dia->Registros[iter]) : grupoVacio) : grupoVacio) << "||" //egresos
					<< (iter < Existencias.size() ? mostrarExistencia(Existencias[iter]) : grupoVacio) << "||" //existencias
					<< std::endl;

				primerRegistro = false;
				iter++;
			}
			std::cout << separadorDia << std::endl;
		} //dia
		std::cout << std::endl << std::endl << std::endl;
	} //mercaderia
	std::cout << std::endl << std::endl << "Presione cualquier tecla para volver...";
	_getch();
}
std::string formatearNombreMercaderia(std::string nombre)
{
	nombre.insert(0, " "); nombre.append(" ");
	return formatearCentrado(nombre, 134, "=");
}
std::string mostrarRegistro(const RegistroPrecio& registro)
{
	std::string impresion;
	impresion.append(formatearColumnaNumero(abs(registro.delta))); impresion.append("|");
	impresion.append(formatearColumnaNumero(registro.precio)); impresion.append("|");
	impresion.append(formatearColumnaNumero(abs(registro.delta) * registro.precio));
	return impresion;
}
std::string mostrarExistencia(const ExistenciasPrecioMercaderia& existencia)
{
	std::string impresion;
	impresion.append(formatearColumnaNumero(existencia.existencias)); impresion.append("|");
	impresion.append(formatearColumnaNumero(existencia.precio)); impresion.append("|");
	impresion.append(formatearColumnaNumero(existencia.existencias * existencia.precio));
	return impresion;
}

std::string formatearCentrado(std::string str, unsigned int longitud, std::string caracter)
{
	if (str.size() < longitud)
	{
		bool alternar = true;
		while (str.size() < longitud)
		{
			str.insert((alternar ? str.size() : 0), caracter);
			alternar = !alternar;
		}
	}
	return str;
}
std::string formatearCentrado(int num, unsigned int longitud, std::string caracter)
{
	return formatearCentrado(std::to_string(num), longitud, caracter);
}

void OP_salir()
{
	EXP_LibroDiario();
	EXP_LibroMayor();
	EXP_EstadoResultados();
	EXP_FichaStock();

	header("PROGRAMA FINALIZADO", 2);
	std::cout << "Archivos .csv guardados correctamente en directorio actual!";
	std::cout << "\nIngreselos en Microsoft Excel como archivo de texto delimitado por ';' para leerlos correctamente ";
	std::cout << "\n\nPresione cualquier tecla para cerrar la consola...";
	_getch();

	loop = false;
}

void EXP_LibroDiario()
{
	//inicializa archivo
	std::ofstream LibroDiario; LibroDiario.open("LibroDiario.csv");

	LibroDiario << "'';Cuenta;Modif;Debe;Haber" << std::endl << std::endl;

	std::string nombreCuenta;
	std::string modif;
	bool esDebe;

	for (DiaOperaciones &dia : DIAS)
	{
		LibroDiario << formatearParaArchivo(dia.getFecha()) << std::endl; // "'01/01'"

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
							<< formatearParaArchivo(esDebe ? linea->delta : 0) << ";"
							<< formatearParaArchivo(!esDebe ? abs(linea->delta) : 0) << std::endl;

			} //lineas

			LibroDiario << "'';segun " << operacion->getDocumento() << std::endl << std::endl;
		} //operaciones

	} //dias
	LibroDiario.close();
}
void EXP_LibroMayor()
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
					<< (lineaActual < debes.size() ? formatearParaArchivo(debes[lineaActual]) : "") << ";"
					<< (lineaActual < haberes.size() ? formatearParaArchivo(haberes[lineaActual]) : "") << std::endl;
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
}
void EXP_EstadoResultados()
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
}
void EXP_FichaStock()
{
	std::ofstream FichaStock; FichaStock.open("FichaStock.csv");

	bool primerRegistro;
	std::vector<ExistenciasPrecioMercaderia> Existencias;
	unsigned int iter;

	for (const Mercaderia& mercaderia : MERCADERIAS)
	{
		Existencias.clear();

		FichaStock << ";;;;" << mercaderia.getNombre() << std::endl
			<< "Fecha;;Ingresos;;;Egresos;;;Existencias;" << std::endl
			<< ";Unidades; C.Unitario; C.Total; Unidades; C.Unitario; C.Total; Unidades; C.Unitario; C.Total"
			<< std::endl << std::endl;

		for (const DiaMercaderia* dia : mercaderia.getDias())
		{
			//actualiza Existencias para todo el dia
			for (const RegistroPrecio& registro : dia->Registros)
			{
				unsigned int pos = buscarOCrearExistencia(Existencias, registro.precio);
				Existencias[pos].modificar(registro.delta);
				if (Existencias[pos].existencias == 0) { Existencias.erase(Existencias.begin() + pos); }
			}

			iter = 0;
			primerRegistro = true;
			//sigue imprimiendo hasta que no queden ni registros ni existencias
			while(iter < dia->Registros.size() || iter < Existencias.size())
			{
				FichaStock << (primerRegistro ? formatearParaArchivo(dia->Fecha) : "") << ";" //fecha
					<< (iter < dia->Registros.size() ? (dia->Registros[iter].delta > 0 ? imprimirRegistro(dia->Registros[iter]) : ";;;") : ";;;") //ingresos
					<< (iter < dia->Registros.size() ? (dia->Registros[iter].delta < 0 ? imprimirRegistro(dia->Registros[iter]) : ";;;") : ";;;") //egresos
					<< (iter < Existencias.size() ? imprimirExistencia(Existencias[iter]) : "") //existencias
					<< std::endl;

				primerRegistro = false;
				iter++;
			}
			FichaStock << std::endl;
		} //dia
		FichaStock << std::endl << std::endl;
	} //mercaderia

	FichaStock.close();
}

std::string formatearParaArchivo(std::string str)
{
	str.insert(0, "'"); str.append("'");
	return str;
}
std::string formatearParaArchivo(int num)
{
	std::string str = std::to_string(num);
	str.insert(0, "'"); str.append("'");
	return str;
}
unsigned int buscarOCrearExistencia(std::vector<ExistenciasPrecioMercaderia>& Existencias, unsigned int precio)
{
	for (unsigned int i = 0; i < Existencias.size(); i++)
	{
		if (precio == Existencias[i].precio) 
		{
			return i;
		}
	}
	Existencias.push_back(ExistenciasPrecioMercaderia(precio));
	return Existencias.size() - 1;
}
std::string imprimirRegistro(const RegistroPrecio& registro)
{
	std::string impresion;
	impresion.append(std::to_string(abs(registro.delta))); impresion.append(";");
	impresion.append(std::to_string(registro.precio)); impresion.append(";");
	impresion.append(std::to_string(abs(registro.delta) * registro.precio)); impresion.append(";");
	return impresion;
}
std::string imprimirExistencia(const ExistenciasPrecioMercaderia& existencia)
{
	std::string impresion;
	impresion.append(std::to_string(existencia.existencias)); impresion.append(";");
	impresion.append(std::to_string(existencia.precio)); impresion.append(";");
	impresion.append(std::to_string(existencia.existencias * existencia.precio)); impresion.append(";");
	return impresion;
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
	Opcion("Nota de Credito", [] { OP_Nota::efectuarNota(true); }),
	Opcion("Nota de Debito", [] { OP_Nota::efectuarNota(false); }),
	Opcion("Ver L. Diario", [] { OP_mostrarLibroDiario(); }),
	Opcion("Ver L. Mayor", [] { OP_mostrarLibroMayor(); }),
	Opcion("Ver Estado de Resultados", [] {OP_mostrarEstadoResultados(); }),
	Opcion("Ver Ficha de Stock", [] {OP_mostrarFichaStock(); }),
	Opcion("Salir del Programa", [] { OP_salir(); })
};

int main()
{
	initVectores();

	std::string opString;
	bool pedirFecha = true;

	header("PROYECTO SIC", 2);
	std::cout << "Iniciar con apertura?\n1. Si\n2. No\n";
	std::cin >> opString;
	if (validarInt(opString, 1) == 1)
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

		
		int op = validarInt(opString, 1, OPCIONES.size());
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