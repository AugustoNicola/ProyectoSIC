#include <vector>
#include <string>
#include <optional>
#include <iostream>
#include <conio.h>

#include "clases.h"
#include "Vectores.h"
#include "Varias.h"

enum class ModoAumento
{
	Debe, Haber, Apertura
};

class SeleccionadorDeCuentas
{
private:
	Cuenta::TipoCuenta FiltroCuentas;
	ModoAumento modoAumento;
	std::string MsgEleccionCuenta;

	std::vector<Cuenta*> cuentasSeleccionables;
	std::string leyendaActivo;
	std::string leyendaPasivo;
	std::string leyendaGasto;

public:
	SeleccionadorDeCuentas(Cuenta::TipoCuenta _filtroCuentas, ModoAumento _modoAumento, std::string _msgEleccionCuenta)
	{
		FiltroCuentas = _filtroCuentas;
		modoAumento = _modoAumento;
		MsgEleccionCuenta = _msgEleccionCuenta;

		configurarModoAumento();
		configurarFiltroCuentas();
	}
	void configurarModoAumento()
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
	void configurarFiltroCuentas()
	{
		if (FiltroCuentas == Cuenta::TipoCuenta::F_OPER || FiltroCuentas == Cuenta::TipoCuenta::ACTIVO_OPERATIVO)
		{
			cuentasSeleccionables.push_back(buscarCuenta("Mercaderias"));
			cuentasSeleccionables.insert(cuentasSeleccionables.end(), ACTIVOS.begin(), ACTIVOS.end());
		}
		if (FiltroCuentas == Cuenta::TipoCuenta::F_OPER || FiltroCuentas == Cuenta::TipoCuenta::PASIVO_OPERATIVO)
		{
			cuentasSeleccionables.insert(cuentasSeleccionables.end(), PASIVOS.begin(), PASIVOS.end());
		}
		if (FiltroCuentas == Cuenta::TipoCuenta::F_OPER || FiltroCuentas == Cuenta::TipoCuenta::GASTO_OPERATIVO)
		{
			cuentasSeleccionables.insert(cuentasSeleccionables.end(), R_NEGS.begin(), R_NEGS.end());
		}
	}

	Cuenta* elegirCuenta()
	{
		std::string strOpcionElegida;
		int opcionElegida;
		do
		{
			system("CLS");
			mostrarCuentas();
			std::cout << "\n\n" << MsgEleccionCuenta << ": ";
			std::cin >> strOpcionElegida;

			opcionElegida = validarInt(strOpcionElegida, 1, cuentasSeleccionables.size(), {}, {});
			if (opcionElegida == 0)
			{
				std::cout << "\nValor ingresado no valido, intentelo nuevamente.";
				_getch();
			}
			else {
				return cuentasSeleccionables[opcionElegida - 1];
			}
		} while (opcionElegida == 0);
	}
	void mostrarCuentas()
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
};


class AumentadorPartida
{
private:
	Cuenta::TipoCuenta filtroCuentas;
	ModoAumento modoAumento;
	std::string mensajeEleccionCuenta;
	int limite = 0;
	bool salir = false;

	int aumentoTotal = 0;
	int aumentoActual = 0;
	Cuenta* cuentaOperacionActual;

public:
	AumentadorPartida(Cuenta::TipoCuenta _filtroCuentas, ModoAumento _modoAumento, std::string _mensajeEleccionCuenta, std::optional<int> _limite)
		: filtroCuentas(_filtroCuentas), modoAumento(_modoAumento), mensajeEleccionCuenta(_mensajeEleccionCuenta), limite(_limite.has_value() ? _limite.value() : 0)
	{
		SeleccionadorDeCuentas seleccionador(filtroCuentas, modoAumento, mensajeEleccionCuenta);

		while (!condicionDeSalidaAlcanzada())
		{
			seleccionador.elegirCuenta();

			if (cuentaOperacionActualEsMercaderia())
			{
				/// hay mercaderias!
				/*
				operMercaderia operMerca = seleccionarMercaderia(true);
				aumentoActual = operMerca.cantidad * operMerca.precioUnitario;

				if (aumentoActual > 0)
				{
					aumentoTotal += aumentoActual;
					modificarCuenta(buscarCuenta("Mercaderias"), aumentoActual);
				}
				*/
			}
			else {
				elegirAumentoActual();
			}

			efectuarAumento();

			if (noHayLimite())
			{
				permitirFinalizar();
			}
		}
	}

	bool condicionDeSalidaAlcanzada()
	{
		if (limite > 0)
		{
			return limite > aumentoTotal;
		} else if (limite < 0) {
			return limite < aumentoTotal;
		} else {
			return salir;
		}
	}

	bool cuentaOperacionActualEsMercaderia()
	{
		return cuentaOperacionActual->getNombre() == "Mercaderias";
	}

	void elegirAumentoActual()
	{
		std::string strAumentoActual;
		do
		{
			system("CLS");
			mostrarInformacion();
			std::cout << "\n\nSeleccione la cantidad: $";
			std::cin >> strAumentoActual;
		} while (!validarAumentoActual(strAumentoActual));
	}

	void mostrarInformacion()
	{
		std::cout << "\n\nCuenta elegida: " << cuentaOperacionActual->getNombre() << " (valor: $" << cuentaOperacionActual->getSaldoActual() << ")";
		std::cout << "\n\nTotal actual: $" << abs(aumentoTotal);
		std::cout << "\nLimite: $" << abs(limite);
	}

	bool validarAumentoActual(std::string strCantidad)
	{
		aumentoActual = validarInt(strCantidad, cuentaOperacionActual->getSaldoActual(), limite, 1, (limite - abs(aumentoTotal)));
		if (aumentoActual != 0)
		{
			ajustarSignoAumentoActual();
			return true;
		} else {
			std::cout << "\nValor ingresado no valido, intentelo nuevamente.";
			_getch();
			return false;
		}
	}

	void ajustarSignoAumentoActual()
	{
		if (modoAumento == ModoAumento::Apertura)
		{
			aumentoActual = aumentoActual * ((cuentaOperacionActual->getTipo() == Cuenta::TipoCuenta::PASIVO_OPERATIVO) ? -1 : 1);
		}
		else {
			aumentoActual = aumentoActual * ((modoAumento == ModoAumento::Debe) ? 1 : -1);
		}
	}

	void efectuarAumento()
	{
		aumentoTotal += aumentoActual;
		modificarCuenta(cuentaOperacionActual, aumentoActual);
	}

	bool noHayLimite()
	{
		return limite == 0;
	}

	void permitirFinalizar()
	{
		std::string strFinalizar;
		system("CLS");
		std::cout << "1. Continuar\n2.Finalizar\nElija una opcion: ";
		std::cin >> strFinalizar; //reusado de variables!!
		salir = (validarInt(strFinalizar, 1, 2, {}, {}) == 2) ? true : false;
	}
};