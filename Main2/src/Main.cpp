#include "Main.h"

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
std::vector<Cuenta*> R_NEGS = {};

std::string fecha;
Operacion oper;
Operacion* operacionActual = &oper;

void OP_Capital()
{
	pedirNuevaFecha("Ingrese la fecha de apertura");

	//aumenta operaciones hasta que el usuario decida
	AumentadorPartida aumento(Cuenta::TipoCuenta::F_OPER, ModoAumento::Apertura, "Elija la cuenta usada en el inicio de operaciones", {});

	/* iguala cuentas con Capital(PN+) */
	modificarCuenta(buscarCuenta("Capital"), aumento.getAumentoTotal() * -1);

	/* finaliza operacion*/
	operacionActual->setDocumento("Apertura");
	commitOperacion(operacionActual);

	pedirNuevaFecha("Ingrese la primera fecha de operaciones"); //una vez finalizada apertura, crea nuevo dia
}
void OP_Transaccion()
{
	/* ingreso de cuentas */
	AumentadorPartida aumentoPerdida(Cuenta::TipoCuenta::F_OPER, ModoAumento::Haber, "Elija las cuentas del haber", {});
	int perdida = abs(aumentoPerdida.getAumentoTotal()); //le quita el signo negativo
	AumentadorPartida aumentoGanancia(Cuenta::TipoCuenta::F_OPER, ModoAumento::Debe, "Elija las cuentas del debe", perdida);

	/* commit de operacion */
	operacionActual = pedirNombreDocx(operacionActual);
	commitOperacion(operacionActual);
}
void NotaDC(bool credito)
{
	std::string opStr;
	int op;
	int modificacion;
	const Operacion* operacionModif = nullptr;
	const Linea* lineaModif = nullptr;
	ModoAumento tipo;

	/* elegir operacion valida */
	unsigned int cont;
	std::vector<const Operacion*> posOper;
	do
	{
		cont = 1;
		posOper = {};
		system("CLS");

		for (const DiaOperaciones& dia : DIAS) //recorre dias
		{
			for (const Operacion* operacion : dia.getOperaciones()) //recorre operaciones
			{
				std::cout << "\n" << cont << ". " << operacion->getDocumento() << " (" << dia.getFecha() << ")";
				posOper.push_back(operacion);
				cont++;
			}
		}
		std::cout << "\n\nElija la operacion a la que se refiere la nota: ";
		std::cin >> opStr;

		/* Validacion/return */
		op = validarInt(opStr, {}, {}, 1, posOper.size());
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

			std::cout << "\n\nElija la cuenta que " << ((!credito) ? "aumenta" : "disminuye") << " en la nota: ";
			std::cin >> opStr;

			/* Validacion/return */
			op = validarInt(opStr, {}, {}, 1, posLinea.size());
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
					tipo = (!credito) ? ModoAumento::Haber : ModoAumento::Debe; //contrrarestar acorde
				}
				else {
					/// La cuenta tenia una modificacion en el haber
					modificacion = op * ((!credito) ? -1 : 1); //si es debito la aumenta en el haber, sino en el debe
					tipo = (!credito) ? ModoAumento::Debe : ModoAumento::Haber; //contrarrestar acorde
				}

				modificarCuenta(lineaModif->cuenta, modificacion); //realiza modificacion

				/* realizar aumentoPartida para saciarlo */
				std::string mensaje = "Elija las cuentas que saldan el "; mensaje.append(((credito) ? "credito" : "debito"));
				AumentadorPartida aum(Cuenta::TipoCuenta::F_OPER, tipo, mensaje, modificacion * -1);

				operacionActual = pedirNombreDocx(operacionActual);
				commitOperacion(operacionActual);
			}
		} while (op == 0);
	}

	/* cerrar operacion*/

}

void OP_VentaMercaderias()
{
	SeleccionadorDeMercaderias venta(false);
	if (venta.getCantidad() != 0)
	{
		/// venta realizada!

		int totalGanado = venta.getTotalGanadoVenta();
		modificarCuenta(buscarCuenta("Ventas"), totalGanado * -1);

		AumentadorPartida igualacionEnDebe(Cuenta::TipoCuenta::F_OPER, ModoAumento::Debe, "Elija las cuentas de ganancia de la venta", totalGanado);

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
	SeleccionadorDeMercaderias compra(true);
	if (compra.getCantidad() != 0)
	{
		/// compra realizada!
		int totalCompraMercaderias = compra.getTotalGastadoCompra();

		modificarCuenta(buscarCuenta("Mercaderias"), totalCompraMercaderias);
		AumentadorPartida igualacionEnHaber(Cuenta::TipoCuenta::F_OPER, ModoAumento::Haber, "Elija las cuentas con las que se amortiza la compra", totalCompraMercaderias * -1);

		/* guarda operacion */
		operacionActual = pedirNombreDocx(operacionActual);
		commitOperacion(operacionActual);
	}
}

void EXP_LibroDiario()
{
	//inicializa archivo
	std::ofstream LibroDiario; LibroDiario.open("LibroDiario.csv");

	LibroDiario << "'';Cuenta;Modif;Debe;Haber" << std::endl << std::endl;

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
					modif = ((debe != 0) ? "A+" : "A-");
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

void imprimeValoresColumnaLibroMayor(std::ofstream& LibroMayor, std::vector<int> valores, unsigned int pos)
{

	for (unsigned int i = pos; i < valores.size(); i++)
	{
		if (valores[i] > 0)
		{
			LibroMayor << formatear(std::to_string(abs(valores[i]))) << ";''" << std::endl;
		}
		else {
			LibroMayor << "'';" << formatear(std::to_string(abs(valores[i]))) << std::endl;
		}
	}

}
std::string formatear(std::string texto)
{
	texto.insert(0, "'"); texto.append("'");
	return texto;
}

void initVectores() {
	for (unsigned int i = 0; i < CUENTAS.size(); i++)
	{
		if (CUENTAS[i].getTipo() == Cuenta::TipoCuenta::ACTIVO_OPERATIVO) { ACTIVOS.push_back((Cuenta*)&CUENTAS[i]); }
		else if (CUENTAS[i].getTipo() == Cuenta::TipoCuenta::PASIVO_OPERATIVO) { PASIVOS.push_back((Cuenta*)&CUENTAS[i]); }
		else if (CUENTAS[i].getTipo() == Cuenta::TipoCuenta::GASTO_OPERATIVO) { R_NEGS.push_back((Cuenta*)&CUENTAS[i]); }
	}
}

const std::vector<Opcion> OPCIONES = {
	Opcion("Nueva Fecha", [] { pedirNuevaFecha(); }),
	Opcion("Transaccion de Cuentas", [] { OP_Transaccion(); }),
	Opcion("Venta de Mercaderias", &OP_VentaMercaderias),
	Opcion("Compra de Mercaderias", &OP_CompraMercaderias),
	Opcion("Nota de Credito", [] { NotaDC(true); }),
	Opcion("Nota de Debito", [] { NotaDC(false); }),
	Opcion("Exportar L. Diario", [] { EXP_LibroDiario(); }),
	Opcion("Exportar L. Mayor", [] { EXP_LibroMayor(); }),
	Opcion("Exportar Estado de Resultados", [] { EXP_EstadoResultados(); })
};

int main()
{
	initVectores();

	bool loop = true; //Controla la ejecucion del programa
	std::string opString;

	std::cout << "=============== PROYECTO SIC ===============";
	std::cout << "\n\nIniciar con apertura?\n1. Si\n2. No\n";
	std::cin >> opString;
	if (validarInt(opString, {}, {}, 1) == 1)
	{
		OP_Capital();
	}
	// -------- LOOP PRINCIPAL --------
	do
	{
		/* Display de opciones y ingreso de input */
		system("CLS");
		std::cout << "Seleccione una opcion:\n";
		for (unsigned int i = 0; i < OPCIONES.size(); i++)
		{
			std::cout << i + 1 << ". " << OPCIONES[i].Nombre << "\n";
		}
		std::cin >> opString;

		/* Validacion */
		int op = validarInt(opString, {}, {}, 1, OPCIONES.size());
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