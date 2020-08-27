#include "SeleccionadorDeCuentas.h"

Cuenta::TipoCuenta SeleccionadorDeCuentas::FiltroCuentas;
ModoAumento SeleccionadorDeCuentas::modoAumento;
std::string SeleccionadorDeCuentas::MsgEleccionCuenta;

std::vector<Cuenta*> SeleccionadorDeCuentas::cuentasSeleccionables;
std::string SeleccionadorDeCuentas::leyendaActivo;
std::string SeleccionadorDeCuentas::leyendaPasivo;
std::string SeleccionadorDeCuentas::leyendaGasto;

Cuenta* SeleccionadorDeCuentas::elegirCuenta(Cuenta::TipoCuenta _filtroCuentas, ModoAumento _modoAumento, std::string _msgEleccionCuenta)
{
	FiltroCuentas = _filtroCuentas;
	modoAumento = _modoAumento;
	MsgEleccionCuenta = _msgEleccionCuenta;

	configurarModoAumento();
	configurarFiltroCuentas();

	std::string strOpcionElegida;
	int opcionElegida;
	do
	{
		system("CLS");
		mostrarCuentas();
		std::cout << "\n\n" << MsgEleccionCuenta << ": ";
		std::cin >> strOpcionElegida;

		opcionElegida = validarInt(strOpcionElegida, {}, {}, 1, cuentasSeleccionables.size());
		if (opcionElegida == 0)
		{
			std::cout << "\nValor ingresado no valido, intentelo nuevamente.";
			_getch();
		}
		else {
			break;
		}
	} while (opcionElegida == 0);

	return cuentasSeleccionables[opcionElegida - 1];


}
void SeleccionadorDeCuentas::configurarModoAumento()
{
	switch (modoAumento)
	{
	case ModoAumento::Debe:
		leyendaActivo = " (A+)"; leyendaPasivo = " (P-)"; leyendaGasto = " (R-)";
		break;
	case ModoAumento::Haber:
		leyendaActivo = " (A-)"; leyendaPasivo = " (P+)"; leyendaGasto = " ((R-)+)";
		break;
	case ModoAumento::Apertura:
		leyendaActivo = " (A+)"; leyendaPasivo = " (P+)"; leyendaGasto = " (R-)";
		break;
	}
}
void SeleccionadorDeCuentas::configurarFiltroCuentas()
{
	cuentasSeleccionables = {};
	if (FiltroCuentas == Cuenta::TipoCuenta::F_OPER || FiltroCuentas == Cuenta::TipoCuenta::ACTIVO_OPERATIVO)
	{
		if (modoAumento != ModoAumento::Haber) { cuentasSeleccionables.push_back(buscarCuenta("Mercaderias")); }
		cuentasSeleccionables.insert(cuentasSeleccionables.end(), ACTIVOS.begin(), ACTIVOS.end());
	}
	if (FiltroCuentas == Cuenta::TipoCuenta::F_OPER || FiltroCuentas == Cuenta::TipoCuenta::PASIVO_OPERATIVO)
	{
		cuentasSeleccionables.insert(cuentasSeleccionables.end(), PASIVOS.begin(), PASIVOS.end());
	}
	if (FiltroCuentas == Cuenta::TipoCuenta::F_OPER || FiltroCuentas == Cuenta::TipoCuenta::GASTO_OPERATIVO)
	{
		cuentasSeleccionables.insert(cuentasSeleccionables.end(), GASTOS.begin(), GASTOS.end());
	}
}

void SeleccionadorDeCuentas::mostrarCuentas()
{
	int contadorCuentasRecorridas = 1;
	std::string leyendaActual;
	for (Cuenta* cuenta : cuentasSeleccionables)
	{
		switch (cuenta->getTipo())
		{
		case Cuenta::TipoCuenta::ACTIVO_OPERATIVO:
			leyendaActual = leyendaActivo;
			break;
		case Cuenta::TipoCuenta::PASIVO_OPERATIVO:
			leyendaActual = leyendaPasivo;
			break;
		case Cuenta::TipoCuenta::GASTO_OPERATIVO:
			leyendaActual = leyendaGasto;
			break;
		}
		std::cout << "\n" << contadorCuentasRecorridas << ". " << cuenta->getNombre() << leyendaActual;
		contadorCuentasRecorridas++;
	}
}