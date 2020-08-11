#include <vector>
#include <string>
#include <optional>
#include <iostream>
#include <conio.h>

#include "clases.h"
#include "presets.h"
#include "Main.h"

#include "AumentadorPartida.h"

enum class ModoAumento
{
	Debe, Haber, Apertura
};



class SeleccionadorDeCuentas
{
private:
	static Cuenta::TipoCuenta FiltroCuentas;
	static ModoAumento modoAumento;
	static std::string MsgEleccionCuenta;

	static std::vector<Cuenta*> cuentasSeleccionables;
	static std::string mActivo;
	static std::string mPasivo;
	static std::string mResult;

public:
	static void Init(Cuenta::TipoCuenta _filtroCuentas, ModoAumento _modoAumento, std::string _msgEleccionCuenta)
	{
		FiltroCuentas = _filtroCuentas;
		modoAumento = _modoAumento;
		MsgEleccionCuenta = _msgEleccionCuenta;

		configurarModoAumento();
		configurarFiltroCuentas();
	}

	static void configurarModoAumento()
	{
		switch (modoAumento)
		{
		case ModoAumento::Debe:
			mActivo = " (A+)"; mPasivo = " (P-)"; mResult = " (R-)";
			break;
		case ModoAumento::Haber:
			mActivo = " (A-)"; mPasivo = " (P+)"; mResult = " ((R-)+)";
			break;
		case ModoAumento::Apertura:
			mActivo = " (A+)"; mPasivo = " (P+)"; mResult = " (R-)";
			break;
		}
	}
	static void configurarFiltroCuentas()
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


	static Cuenta* elegirCuenta()
	{
		std::string strOpcionElegida;
		int opcionElegida;
		do
		{
			int contadorCuentasRecorridas = 1;

			system("CLS");

			for (Cuenta* cuenta : cuentasSeleccionables)
			{
				std::cout << "\n" << contadorCuentasRecorridas << ". " << cuenta->getNombre() << mActivo;
				contadorCuentasRecorridas++;
			}

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
		SeleccionadorDeCuentas::Init(filtroCuentas, modoAumento, mensajeEleccionCuenta);

		while (!condicionDeSalidaAlcanzada())
		{
			SeleccionadorDeCuentas::elegirCuenta();

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
				elegirCantidad();
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




	bool cuentaOperacionActualEsMercaderia();

	void elegirCantidad()
	{
		std::string strCantidad;
		do
		{
			mostrarInformacion();
			std::cout << "\n\nSeleccione la cantidad: $";
			std::cin >> strCantidad;
		} while (!validarCantidad(strCantidad));
	}

	void mostrarInformacion()
	{
		std::cout << "\n\nCuenta elegida: " << cuentaOperacionActual->getNombre() << " (valor: $" << cuentaOperacionActual->getSaldoActual() << ")";
		std::cout << "\n\nTotal actual: $" << abs(aumentoTotal);
		std::cout << "\nLimite: $" << limite;
	}

	bool validarCantidad(std::string strCantidad)
	{
		/* validacion de cantidad */
		aumentoActual = validarInt(strCantidad, cuentaOperacionActual->getSaldoActual(), limite, 1, (limite - abs(aumentoTotal))); //se asegura de que la cantidad sea valida
		if (aumentoActual != 0)
		{
			ajustarSignoAumentoActual();
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

	bool noHayLimite();

	void permitirFinalizar();
};