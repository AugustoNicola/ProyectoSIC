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
		if (limite != 0)
		{
			while (limiteNoAlcanzado())
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
			}
		}
		else {
			/* ----- aumentos hasta que el usuario decida parar ----- */
			bool satisfecho = false;

			while (!satisfecho)
			{
				system("CLS");

				/* seleccion de cuenta */
				cuentaOperacionActual = elegirCuenta(t, tipoPartida, mensaje);

				/* verificacion mercaderias*/
				if (cuentaOperacionActual->getNombre() == "Mercaderias")
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

				}
				else {

					/// no hay mercaderias

					/* seleccion de cantidad */
					std::cout << "\n\nTotal actual: $" << abs(aumentoTotal);
					std::cout << "\n\nSeleccione la cantidad: $";
					std::cin >> aumentoActualStr;

					/* validacion de cantidad */
					aumentoActual = validarInt(aumentoActualStr, cuentaOperacionActual->getSaldoActual(), {}, 1); //se asegura de que la cantidad sea valida
					if (aumentoActual != 0)
					{
						/// cantidad valida!

						/* ajusta el signo de la modificacion */
						if (tipoPartida == Apertura)
						{
							aumentoActual = aumentoActual * ((cuentaOperacionActual->getTipo() == Cuenta::TipoCuenta::PASIVO_OPERATIVO) ? -1 : 1);
						}
						else {
							aumentoActual = aumentoActual * ((tipoPartida == Debe) ? 1 : -1);
						}

						aumentoTotal += aumentoActual;
						modificarCuenta(cuentaOperacionActual, aumentoActual);

						/* permitir finalizar */
						system("CLS");
						std::cout << "1. Continuar\n2.Finalizar\nElija una opcion: ";
						std::cin >> aumentoActualStr; //reusado de variables!!
						satisfecho = (validarInt(aumentoActualStr, 1, 2) == 2) ? true : false;

					}
					else {
						/// cantidad invalida
						std::cout << "\n\nCantidad invalida, presione cualquier tecla para intentarlo nuevamente";
						_getch();
					}
				}
			}
		}
	}


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


};