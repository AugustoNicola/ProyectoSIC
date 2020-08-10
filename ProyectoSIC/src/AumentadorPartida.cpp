#include <vector>
#include <string>
#include <optional>
#include <iostream>

#include "clases.h"
#include "presets.h"
#include "Main.h"

#include "AumentadorPartida.h"

enum class ModoAumento
{
	Debe, Haber, Apertura
};

class AumentadorPartida
{
private:
	Cuenta::TipoCuenta filtroCuentas;
	ModoAumento modoAumento;
	std::string mensajeEleccionCuenta;
	int limite = 0;

	int aumentoTotal = 0;
	int aumentoActual = 0;
	Cuenta* cuentaOperacionActual;

public:
	AumentadorPartida(Cuenta::TipoCuenta _filtroCuentas, ModoAumento _modoAumento, std::string _mensajeEleccionCuenta, std::optional<int> _limite)
		: filtroCuentas(_filtroCuentas), modoAumento(_modoAumento), mensajeEleccionCuenta(_mensajeEleccionCuenta), limite(_limite.has_value() ? _limite.value() : 0)
	{
			while (condicionDeSalidaNoAlcanzada())
			{
				elegirCuentaOperacionActual();

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

	bool condicionDeSalidaNoAlcanzada();

	bool limiteNoAlcanzado();

	void elegirCuentaOperacionActual();

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