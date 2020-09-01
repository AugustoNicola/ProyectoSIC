#include "SeleccionadorDeCuentas.h"

bool SeleccionadorDeCuentas::PermitirCancelar;
Cuenta::TipoCuenta SeleccionadorDeCuentas::FiltroCuentas;
ModoAumento SeleccionadorDeCuentas::modoAumento;
std::string SeleccionadorDeCuentas::MsgEleccionCuenta;

std::vector<Cuenta*> SeleccionadorDeCuentas::cuentasSeleccionables;
std::string SeleccionadorDeCuentas::leyendaActivo;
std::string SeleccionadorDeCuentas::leyendaPasivo;
std::string SeleccionadorDeCuentas::leyendaGasto;

Cuenta* SeleccionadorDeCuentas::elegirCuenta(bool _permitirCancelar, Cuenta::TipoCuenta _filtroCuentas, ModoAumento _modoAumento, std::string _msgEleccionCuenta)
{
	PermitirCancelar = _permitirCancelar;
	FiltroCuentas = _filtroCuentas;
	modoAumento = _modoAumento;
	MsgEleccionCuenta = _msgEleccionCuenta;

	configurarModoAumento();
	configurarFiltroCuentas();

	std::string strOpcionElegida;
	int opcionElegida;
	do
	{
		mostrarCuentas();
		std::cout << "\n\n" << MsgEleccionCuenta << ": ";
		std::cin >> strOpcionElegida;

		opcionElegida = validarInt(strOpcionElegida, 1, cuentasSeleccionables.size() + (PermitirCancelar ? 1 : 0));
		if (opcionElegida == 0)
		{
			std::cout << "\n\nValor ingresado no valido, intentelo nuevamente.";
			_getch();
		}
		else {
			break;
		}
	} while (opcionElegida == 0);

	return ( (opcionElegida == cuentasSeleccionables.size() + 1) ? 
		nullptr : cuentasSeleccionables[opcionElegida - 1]);
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
	switch (modoAumento)
	{
	case ModoAumento::Debe:
		header("AUMENTO EN EL DEBE");
		break;
	case ModoAumento::Haber:
		header("AUMENTO EN EL HABER");
		break;
	case ModoAumento::Apertura:
		header("AUMENTO DE APERTURA");
		break;
	default:
		break;
	}

	int contadorCuentasRecorridas = 1;
	std::string leyendaActual;

	separadorDeTipo(cuentasSeleccionables[0]->getTipo());
	for (unsigned int i = 0; i < cuentasSeleccionables.size(); i++)
	{
		switch (cuentasSeleccionables[i]->getTipo())
		{
		case Cuenta::TipoCuenta::ACTIVO_OPERATIVO:
		case Cuenta::TipoCuenta::ACTIVO_NO_OPERATIVO:
			leyendaActual = leyendaActivo;
			break;
		case Cuenta::TipoCuenta::PASIVO_OPERATIVO:
			leyendaActual = leyendaPasivo;
			break;
		case Cuenta::TipoCuenta::GASTO_OPERATIVO:
			leyendaActual = leyendaGasto;
			break;
		}
		std::cout << "\n" << contadorCuentasRecorridas << ". " << cuentasSeleccionables[i]->getNombre() << leyendaActual;
		contadorCuentasRecorridas++;

		if (i != cuentasSeleccionables.size() - 1 && cuentasSeleccionables[i]->getNombre() != "Mercaderias")
		{
			if (cuentasSeleccionables[i]->getTipo() != cuentasSeleccionables[i + 1]->getTipo())
			{
				separadorDeTipo(cuentasSeleccionables[i + 1]->getTipo());
			}
		}
	}
	if (PermitirCancelar)
	{
		std::cout << "\n=========================\n" << 
			contadorCuentasRecorridas << ". " << "Cancelar";
	}
	
}
void SeleccionadorDeCuentas::separadorDeTipo(Cuenta::TipoCuenta tipo)
{
	switch (tipo)
	{
	case Cuenta::TipoCuenta::ACTIVO_OPERATIVO:
	case Cuenta::TipoCuenta::ACTIVO_NO_OPERATIVO:
		std::cout << "\n\n-------- ACTIVOS --------";
		break;
	case Cuenta::TipoCuenta::PASIVO_OPERATIVO:
		std::cout << "\n\n-------- PASIVOS --------";
		break;
	case Cuenta::TipoCuenta::GASTO_OPERATIVO:
		std::cout << "\n\n-------- RESULTADOS --------";
		break;
	}
}