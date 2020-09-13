#include "Main.h"

using namespace conmanip;

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

	int aumentoTotal = AumentadorPartida::realizarAumento(true, Cuenta::TipoCuenta::F_OPER, ModoAumento::Apertura,
		"Elija la cuenta usada en el inicio de operaciones", {});
	
	if (aumentoTotal == 0) { return false; }
	
	modificarCuenta(buscarCuenta("Capital"), aumentoTotal * -1);
	
	operacionActual->setDocumento("Apertura");
	commitOperacion(operacionActual);

	return true;
	
}
void OP_Transaccion()
{
	int aumentoPerdida = AumentadorPartida::realizarAumento(true, Cuenta::TipoCuenta::F_OPER, ModoAumento::Haber,
		"Elija las cuentas del haber", {});
	
	if (aumentoPerdida != 0)
	{
		int aumentoGanancia = AumentadorPartida::realizarAumento(false, Cuenta::TipoCuenta::F_OPER, ModoAumento::Debe,
			"Elija las cuentas del debe", abs(aumentoPerdida));

		operacionActual = pedirNombreDocx(operacionActual);
		commitOperacion(operacionActual);
	}
	
}

void OP_CompraMercaderias()
{
	SeleccionadorDeMercaderias compra(true, SeleccionadorDeMercaderias::TipoOperacion::COMPRA);
	if (compra.getCantidad() != 0)
	{
		int totalCompraMercaderias = compra.getTotalGastadoCompra();

		modificarCuenta(buscarCuenta("Mercaderias"), totalCompraMercaderias);
		int igualacionEnHaber = AumentadorPartida::realizarAumento(false, Cuenta::TipoCuenta::F_OPER, ModoAumento::Haber, "Elija las cuentas con las que se amortiza la compra", totalCompraMercaderias * -1);

		operacionActual = pedirNombreDocx(operacionActual);
		commitOperacion(operacionActual);
	}
}
void OP_VentaMercaderias()
{
	SeleccionadorDeMercaderias venta(true, SeleccionadorDeMercaderias::TipoOperacion::VENTA);
	if (venta.getCantidad() != 0)
	{
		int totalGanado = venta.getTotalGanadoVenta();
		modificarCuenta(buscarCuenta("Ventas"), totalGanado * -1);

		int igualacionEnDebe = AumentadorPartida::realizarAumento(false ,Cuenta::TipoCuenta::F_OPER, ModoAumento::Debe,
			"Elija las cuentas de ganancia de la venta", totalGanado);

		int totalPerdido = venta.getTotalPerdidoVenta();
		modificarCuenta(buscarCuenta("Mercaderias"), totalPerdido * -1);
		modificarCuenta(buscarCuenta("CMV"), totalPerdido);

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

		if (lineaEsMercaderia())
		{
			operacionMercaderia();
		} else {
			operacionConvencional();
		}
	}
}
bool OP_Nota::intentarElegirOperacion()
{
	unsigned int cont;
	std::string strOperacion;
	bool hayOperaciones = false;

	do
	{
		hayOperaciones = false;
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
			std::cout 
				<< settextcolor(colorError) << "\nNo hay dias con operaciones validas!"
				<< settextcolor(colorBase) << "\n\nPresione cualquier tecla para volver...";
			_getch();
			return false;
		}

		std::cout << settextcolor(colorError) << "\n" << cont << ". Cancelar";

		std::cout << settextcolor(colorBase) << "\n\nElija la operacion a la que se refiere la nota: ";
		std::cin >> settextcolor(colorInput) >> strOperacion;

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
		error();
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
				std::cout
					<< settextcolor(colorBase) << "\n" << cont << ". " << linea->cuenta->getNombre()
					<< settextcolor(colorDatos) << " (" << formatearDinero(linea->delta) << ")";
				lineasDisponibles.push_back(linea);
				cont++;
			}
		}

		std::cout << settextcolor(colorBase)
			<< "\n\nElija la cuenta que " << ((!esCredito) ? "aumenta" : "disminuye") << " en la nota: ";
		std::cin >> settextcolor(colorInput) >> strLinea;

	} while (!validarLinea(strLinea));
	
	lineaModificada = lineasDisponibles[posLinea - 1];
}
bool OP_Nota::validarLinea(std::string strLinea)
{
	posLinea = validarInt(strLinea, 1, lineasDisponibles.size());
	if (posLinea == 0)
	{
		error();
		return false;
	}
	return true;
}
bool OP_Nota::lineaEsMercaderia()
{
	return lineaModificada->cuenta->getNombre() == "Mercaderias";
}
void OP_Nota::operacionMercaderia()
{
	if (esCredito && lineaModificada->delta > 0)
	{
		/// credito en compra de mercaderias
		SeleccionadorDeMercaderias devolucion(true, SeleccionadorDeMercaderias::TipoOperacion::DEVOLUCION);
		if (devolucion.getCantidad() != 0)
		{
			int totalGanadoDevolucion = devolucion.getTotalGanadoDevolucion();

			modificarCuenta(buscarCuenta("Mercaderias"), -totalGanadoDevolucion);
			int igualacionEnHaber = AumentadorPartida::realizarAumento(false, Cuenta::TipoCuenta::F_OPER, ModoAumento::Debe,
				"Elija las cuentas con las que se amortiza la devolucion", totalGanadoDevolucion);

			operacionActual = pedirNombreDocx(operacionActual);
			commitOperacion(operacionActual);
		}
		
	} else if (esCredito && lineaModificada->delta < 0)
	{
		/// credito en venta de mercaderias
		SeleccionadorDeMercaderias reintegro(true, SeleccionadorDeMercaderias::TipoOperacion::REINTEGRO);
		if (reintegro.getCantidad() != 0)
		{
			int totalReembolsadoPorVenta = reintegro.getTotalGanadoVenta();
			int totalReembolsadoPorMercaderias = reintegro.getTotalGastadoCompra();

			modificarCuenta(buscarCuenta("Ventas"), totalReembolsadoPorVenta);
			modificarCuenta(buscarCuenta("CMV"), -totalReembolsadoPorMercaderias);
			modificarCuenta(buscarCuenta("Mercaderias"), totalReembolsadoPorMercaderias);
			int igualacionEnHaber = AumentadorPartida::realizarAumento(false, Cuenta::TipoCuenta::F_OPER, ModoAumento::Haber,
				"Elija las cuentas con las que se amortiza el reintegro", -totalReembolsadoPorVenta);

			// guarda operacion
			operacionActual = pedirNombreDocx(operacionActual);
			commitOperacion(operacionActual);
		}
		
	} else if (!esCredito && lineaModificada->delta > 0)
	{
		/// debito de compra de mercaderias
		OP_CompraMercaderias();
	} else if (!esCredito && lineaModificada->delta < 0)
	{
		/// debito de venta de mercaderias
		OP_VentaMercaderias();
	}
}
void OP_Nota::operacionConvencional()
{
	std::string strValorAumento;

	do
	{
		header(lineaModificada->cuenta->getNombre() += (!esCredito ? ": AUMENTO" : ": DISMINUCION"));

		std::cout << "\n\n Ingrese el valor " << ((!esCredito) ? "del aumento" : "de la disminucion") << " de la cuenta: $";
		std::cin >> settextcolor(colorInput) >> strValorAumento;

	} while (!validarModificacion(strValorAumento));

	ajustarSignoModificacion();
	modificarCuenta(lineaModificada->cuenta, modificacion);

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
		error();
		return false;
	}
	return true;
}
void OP_Nota::ajustarSignoModificacion()
{
	modificacion *= (lineaModificada->delta > 0 ?
		(esCredito ? -1 : 1) :
		(esCredito ? 1 : -1) );
	tipo = (lineaModificada->delta > 0 ? 
		(esCredito ? ModoAumento::Debe : ModoAumento::Haber) :
		(esCredito ? ModoAumento::Haber : ModoAumento::Debe) );
}

void OP_mostrarLibroDiario()
{
	// medidas de columnas: 50/12/12
	std::string separador = "--------------------------------------------------+------------+------------";
	std::string lineaVacia = "|            |            ";

	std::string nombreCuenta;
	std::string nombreCuentaCompleto;
	std::string espacios;
	std::string modif;
	console_text_colors colorModif;
	bool esDebe;

	header("LIBRO DIARIO", 3);
	std::cout
		<< settextcolor(colorEnfoque) << "Cuenta                                            "
		<< settextcolor(colorBase) << "|"
		<< settextcolor(colorEnfoque) << "    Debe    "
		<< settextcolor(colorBase) << "|"
		<< settextcolor(colorEnfoque) << "   Haber  " << std::endl
		<< settextcolor(colorBase) << separador << std::endl;

	for (DiaOperaciones& dia : DIAS)
	{
		std::cout 
			<< settextcolor(colorInput) << formatearColumnaCuenta(dia.getFecha())
			<< settextcolor(colorBase) << lineaVacia << std::endl
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
					modif = esDebe ? " (A+) " : " (A-) ";
					colorModif = console_text_colors::light_green;
					break;
				case Cuenta::TipoCuenta::PASIVO_OPERATIVO:
				case Cuenta::TipoCuenta::PASIVO_NO_OPERATIVO:
					modif = esDebe ? " (P-) " : " (P+) ";
					colorModif = console_text_colors::light_red;
					break;
				case Cuenta::TipoCuenta::GASTO_OPERATIVO:
				case Cuenta::TipoCuenta::GASTO_NO_OPERATIVO:
				case Cuenta::TipoCuenta::GANANCIA:
					modif = esDebe ? " (R-) " : " (R+) ";
					colorModif = console_text_colors::light_cyan;
					break;
				case Cuenta::TipoCuenta::PATRIMONIO_NETO:
					modif = esDebe ? " (PN-) " : " (PN+) ";
					colorModif = console_text_colors::light_magenta;
					break;
				}

				nombreCuentaCompleto = nombreCuenta; nombreCuentaCompleto += modif;
				espacios = calcularEspacios(nombreCuentaCompleto);

				std::cout
					<< settextcolor(colorBase) << nombreCuenta
					<< settextcolor(colorModif) << modif << espacios
					<< settextcolor(colorBase) << "|"
					<< formatearColumnaNumero(esDebe ? linea->delta : 0) << "|"
					<< formatearColumnaNumero(!esDebe ? abs(linea->delta) : 0) << std::endl;

			} //lineas

			std::string nombreDocumento = "s/"; nombreDocumento += operacion->getDocumento();
			std::cout
				<< settextcolor(colorEnfoque) << formatearColumnaCuenta(nombreDocumento)
				<< settextcolor(colorBase) << lineaVacia << std::endl
				<< settextcolor(colorBase) << separador << std::endl;
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
std::string calcularEspacios(std::string str)
{
	std::string espacios;
	unsigned int longitudActual = str.size();

	if (longitudActual < 50)
	{
		while (longitudActual != 50)
		{
			espacios += " ";
			longitudActual++;
		}
	}
	return espacios;
}

void OP_mostrarLibroMayor()
{
	//medidas de columnas: 12/12
	std::vector<int> debes; std::vector<int> haberes;
	int saldo;
	bool salir;
	
	header("LIBRO MAYOR", 3);
	for (Cuenta& cuenta : CUENTAS)
	{
		debes.clear(); haberes.clear();
		if (cuenta.hayDias())
		{
			std::string nombreCuenta = cuenta.getNombre();
			nombreCuenta.insert(0, " "); nombreCuenta.append(" ");

			std::cout
				<< settextcolor(colorInput) << formatearCentrado(nombreCuenta, 25, "=") << std::endl 
				<< settextcolor(colorEnfoque) << "    Debe    "
				<< settextcolor(colorBase) <<  "|"
				<< settextcolor(colorEnfoque) << "   Haber  " << std::endl;

			saldo = 0; salir = false;

			for (DiaCuenta& dia : cuenta.getDias())
			{
				(dia.delta > 0 ? debes : haberes).push_back(abs(dia.delta));
				saldo += dia.delta;
			}

			unsigned int lineaActual = 0;
			while (lineaActual < debes.size() || lineaActual < haberes.size())
			{
				std::cout << settextcolor(colorBase)
					<< (lineaActual < debes.size() ? formatearColumnaNumero(debes[lineaActual]) : "            ") << "|"
					<< (lineaActual < haberes.size() ? formatearColumnaNumero(haberes[lineaActual]) : "            ") << std::endl;
				lineaActual++;
			}
			std::cout << "------------+------------" << std::endl;

			if (saldo != 0)
			{
				formatearSaldo(saldo);
			}
			else {
				std::cout << settextcolor(colorDatos) << "   Cuenta saldada: $0    ";
			}
			std::cout << std::endl << std::endl << std::endl << std::endl;
		}
	} //cuentas

	std::cout << settextcolor(colorBase) << std::endl << std::endl << "Presione cualquier tecla para volver...";
	_getch();
}
void formatearSaldo(int saldo)
{
	std::cout
		<< settextcolor(saldo > 0 ? console_text_colors::light_green : console_text_colors::light_red)
		<< (saldo > 0 ? "Saldo deudor: " : "Saldo acreedor: ")
		<< formatearDinero(saldo);
}

void OP_mostrarEstadoResultados()
{
	header("ESTADO DE RESULTADOS", 3);

	int utilidad = -(buscarCuenta("Ventas")->getSaldoActual() + buscarCuenta("CMV")->getSaldoActual());

	std::cout
		<< "Ventas: " << settextcolor(console_text_colors::light_green) << formatearDinero(abs(buscarCuenta("Ventas")->getSaldoActual())) << std::endl
		<< "CMV: " << settextcolor(console_text_colors::light_red) << formatearDinero(abs(buscarCuenta("CMV")->getSaldoActual())) << std::endl << std::endl
		<< "Utilidad Bruta: " << settextcolor(console_text_colors::light_cyan) << formatearDinero(utilidad) << std::endl << std::endl;

	for (const Cuenta* cuentaGastos : GASTOS)
	{
		if (cuentaGastos->getSaldoActual() != 0)
		{
			std::cout << settextcolor(console_text_colors::light_red) << cuentaGastos->getNombre() << ": " << formatearDinero(cuentaGastos->getSaldoActual() * -1) << std::endl;
			utilidad -= cuentaGastos->getSaldoActual();
		}
	}
	std::cout << settextcolor(console_text_colors::light_magenta) << std::endl << "Utilidad Neto: " << formatearDinero(utilidad);

	std::cout << settextcolor(colorBase) << std::endl << std::endl << "Presione cualquier tecla para volver...";
	_getch();
}

void OP_mostrarFichaStock()
{
	//medidas de columnas: 12/38(12/12/12)/38(idem)/38(idem)
	std::string grupoVacio = "            |            |            ";
	std::string separadorDia = "------------||------------+------------+------------||------------+------------+------------||------------+------------+------------||";

	std::string nombreMercaderia;
	bool primerRegistro;
	std::vector<ExistenciasPrecioMercaderia> Existencias;
	unsigned int iter;

	header("FICHA DE STOCK", 2);
	for (const Mercaderia& mercaderia : MERCADERIAS)
	{
		Existencias.clear();
		nombreMercaderia = "  "; nombreMercaderia.insert(1, mercaderia.getNombre());

		std::cout
			<< settextcolor(console_text_colors::light_magenta) << formatearCentrado(nombreMercaderia, 134, "=") << std::endl
			<< settextcolor(colorEnfoque) << "   Fecha    " << settextcolor(colorBase) << "||"
			<< settextcolor(colorEnfoque) << "               Ingresos               " << settextcolor(colorBase) << "||"
			<< settextcolor(colorEnfoque) << "                Egresos               " << settextcolor(colorBase) << "||"
			<< settextcolor(colorEnfoque) << "              Existencias             " << settextcolor(colorBase) << "||"
			<< std::endl
			<< settextcolor(colorEnfoque) << "            " << settextcolor(colorBase) << "||"
			<< settextcolor(colorEnfoque) << "    unid.   " << settextcolor(colorBase) << "|"
			<< settextcolor(colorEnfoque) << "   costo    " << settextcolor(colorBase) << "|"
			<< settextcolor(colorEnfoque) << " costo tot. " << settextcolor(colorBase) << "||"
			<< settextcolor(colorEnfoque) << "    unid.   " << settextcolor(colorBase) << "|"
			<< settextcolor(colorEnfoque) << "   costo    " << settextcolor(colorBase) << "|"
			<< settextcolor(colorEnfoque) << " costo tot. " << settextcolor(colorBase) << "||"
			<< settextcolor(colorEnfoque) << "    unid.   " << settextcolor(colorBase) << "|"
			<< settextcolor(colorEnfoque) << "   costo    " << settextcolor(colorBase) << "|"
			<< settextcolor(colorEnfoque) << " costo tot. " << settextcolor(colorBase) << "||"
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
				std::cout
					<< settextcolor(colorInput) << formatearCentrado( (primerRegistro ? dia->Fecha : ""), 12, " " ) //fecha
					<< settextcolor(colorBase) << "||"
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

void terminarPrograma(int signal)
{
	std::filesystem::create_directory("Libros");
	EXP_LibroDiario();
	EXP_LibroMayor();
	EXP_EstadoResultados();
	EXP_FichaStock();

	if (signal == -1)
	{
		header("PROGRAMA FINALIZADO", 2);
		std::cout << settextcolor(console_text_colors::light_green) << "Archivos .csv guardados correctamente en carpeta"
			<< settextcolor(colorEnfoque) << " 'Libros' " << settextcolor(console_text_colors::light_green) << "!"
			<< settextcolor(colorDatos) << "\nIngreselos en Microsoft Excel como archivo de texto delimitado por "
			<< settextcolor(colorEnfoque) << "';'" << settextcolor(colorDatos) << " para leerlos correctamente.";

		std::cout << settextcolor(colorBase) << "\n\nPresione cualquier tecla para cerrar la consola...";
		_getch();
		loop = false;
	}
	else {
		exit(signal);
	}
}
void EXP_LibroDiario()
{
	std::string nombreCuenta;
	std::string modif;
	bool esDebe;
	
	std::ofstream LibroDiario; LibroDiario.open("Libros/LibroDiario.csv");
	LibroDiario << "'';Cuenta;Modif;Debe;Haber" << std::endl << std::endl;
	for (DiaOperaciones &dia : DIAS)
	{
		LibroDiario << formatearParaArchivo(dia.getFecha()) << std::endl;

		for (const Operacion* operacion : dia.getOperaciones())
		{
			for (const Linea* linea : operacion->getLineas())
			{
				nombreCuenta = linea->cuenta->getNombre();
				esDebe = (linea->delta > 0) ? true : false;

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

				LibroDiario 
					<< "'';"
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
	std::vector<int> debes; std::vector<int> haberes;
	int saldo;
	bool salir;
	unsigned int lineaActual;
	
	std::ofstream LibroMayor; LibroMayor.open("Libros/LibroMayor.csv");
	for (Cuenta &cuenta : CUENTAS)
	{
		debes.clear(); haberes.clear();
		if (cuenta.hayDias())
		{
			saldo = 0; salir = false;

			LibroMayor << cuenta.getNombre() << std::endl 
				<< "Debe;Haber" << std::endl;
			for (DiaCuenta &dia : cuenta.getDias())
			{
				(dia.delta > 0 ? debes : haberes).push_back(abs(dia.delta));
				saldo += dia.delta;
			}

			lineaActual = 0;
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
	} //cuentas

	LibroMayor.close();
}
void EXP_EstadoResultados()
{
	std::ofstream EstadoResultados; EstadoResultados.open("Libros/EstadoResultados.csv");
	int utilidad = -(buscarCuenta("Ventas")->getSaldoActual() + buscarCuenta("CMV")->getSaldoActual());

	EstadoResultados << "Ventas;" << formatearDinero(abs(buscarCuenta("Ventas")->getSaldoActual())) << std::endl;
	EstadoResultados << "CMV;" << formatearDinero(abs(buscarCuenta("CMV")->getSaldoActual())) << std::endl << std::endl;
	EstadoResultados << "Utilidad Bruta;" << formatearDinero(utilidad) << std::endl << std::endl;

	for (const Cuenta *cuentaGastos : GASTOS)
	{
		if (cuentaGastos->getSaldoActual() != 0)
		{
			EstadoResultados << cuentaGastos->getNombre() << ";" << formatearDinero(abs(cuentaGastos->getSaldoActual())) << std::endl;
			utilidad -= cuentaGastos->getSaldoActual();
		}
	}
	EstadoResultados << std::endl << "Utilidad Neto;" << formatearDinero(utilidad);

	EstadoResultados.close();
}
void EXP_FichaStock()
{
	bool primerRegistro;
	std::vector<ExistenciasPrecioMercaderia> Existencias;
	unsigned int iter;

	std::ofstream FichaStock; FichaStock.open("Libros/FichaStock.csv");
	for (const Mercaderia& mercaderia : MERCADERIAS)
	{
		Existencias.clear();

		FichaStock 
			<< ";;;;" << mercaderia.getNombre() << std::endl
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
				FichaStock 
					<< (primerRegistro ? formatearParaArchivo(dia->Fecha) : "") << ";" //fecha
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
	return formatearParaArchivo(std::to_string(num));
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

void initVectores()
{
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
	Opcion("Compra de Mercaderias", &OP_CompraMercaderias),
	Opcion("Venta de Mercaderias", &OP_VentaMercaderias),
	Opcion("Nota de Credito", [] { OP_Nota::efectuarNota(true); }),
	Opcion("Nota de Debito", [] { OP_Nota::efectuarNota(false); }),
	Opcion("Ver Libro Diario", [] { OP_mostrarLibroDiario(); }),
	Opcion("Ver Libro Mayor", [] { OP_mostrarLibroMayor(); }),
	Opcion("Ver Estado de Resultados", [] {OP_mostrarEstadoResultados(); }),
	Opcion("Ver Ficha de Stock", [] {OP_mostrarFichaStock(); }),
	Opcion("Salir del Programa", [] { terminarPrograma(-1); })
};


int main()
{
	signal(SIGINT, terminarPrograma);
	console_out_context ctxout;
	console<console_type::out> conout(ctxout);
	conout.settitle("Proyecto SIC v1.0");
	::SetConsoleDisplayMode(ctxout.handle, (DWORD)CONSOLE_FULLSCREEN_MODE, nullptr);
	::SetConsoleScreenBufferSize(ctxout.handle, {conout.getsize().X, 100});
	initVectores();

	std::string opString;
	bool pedirFecha = true;

	header("PROYECTO SIC", 2);
	std::cout
		<< "Iniciar con apertura?"
		<< settextcolor(console_text_colors::light_green) << "\n\n1. Si"
		<< settextcolor(console_text_colors::light_red) << "\n2. No"
		<< settextcolor(colorBase) << "\n\nElija una opcion: ";
	std::cin >> settextcolor(colorInput) >> opString;
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
		std::cin >> settextcolor(console_text_colors::light_magenta) >> opString;

		
		int op = validarInt(opString, 1, OPCIONES.size());
		if (op != 0)
		{
			OPCIONES[op - 1].Funcion();
		}
		else {
			error();
		}
	} while (loop);

	return 0;
}