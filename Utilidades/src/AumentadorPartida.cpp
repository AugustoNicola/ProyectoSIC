#include "AumentadorPartida.h"

	SeleccionadorDeCuentas::SeleccionadorDeCuentas(std::fstream& _data, Cuenta::TipoCuenta _filtroCuentas, ModoAumento _modoAumento, std::string _msgEleccionCuenta)
		: data(_data), FiltroCuentas(_filtroCuentas), modoAumento(_modoAumento), MsgEleccionCuenta(_msgEleccionCuenta)
	{
		configurarModoAumento();
		configurarFiltroCuentas();
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

	Cuenta* SeleccionadorDeCuentas::elegirCuenta()
	{
		std::string strOpcionElegida;
		int opcionElegida;
		do
		{
			system("CLS");
			mostrarCuentas();
			std::cout << "\n\n" << MsgEleccionCuenta << ": ";
			std::getline(data, strOpcionElegida);

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

	AumentadorPartida::AumentadorPartida(std::fstream& _data, Cuenta::TipoCuenta _filtroCuentas, ModoAumento _modoAumento, std::string _mensajeEleccionCuenta, std::optional<int> _limite)
		: data(_data), filtroCuentas(_filtroCuentas), modoAumento(_modoAumento), mensajeEleccionCuenta(_mensajeEleccionCuenta), limite(_limite.has_value() ? _limite.value() : 0)
	{
		SeleccionadorDeCuentas seleccionador(data, filtroCuentas, modoAumento, mensajeEleccionCuenta);

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

	bool AumentadorPartida::condicionDeSalidaAlcanzada()
	{
		if (limite > 0)
		{
			return limite > aumentoTotal;
		}
		else if (limite < 0) {
			return limite < aumentoTotal;
		}
		else {
			return salir;
		}
	}

	bool AumentadorPartida::cuentaOperacionActualEsMercaderia()
	{
		return cuentaOperacionActual->getNombre() == "Mercaderias";
	}

	void AumentadorPartida::elegirAumentoActual()
	{
		std::string strAumentoActual;
		do
		{
			system("CLS");
			mostrarInformacion();
			std::cout << "\n\nSeleccione la cantidad: $";
			std::getline(data, strAumentoActual);
		} while (!validarAumentoActual(strAumentoActual));
	}

	void AumentadorPartida::mostrarInformacion()
	{
		std::cout << "\n\nCuenta elegida: " << cuentaOperacionActual->getNombre() << " (valor: $" << cuentaOperacionActual->getSaldoActual() << ")";
		std::cout << "\n\nTotal actual: $" << abs(aumentoTotal);
		std::cout << "\nLimite: $" << abs(limite);
	}

	bool AumentadorPartida::validarAumentoActual(std::string strCantidad)
	{
		aumentoActual = validarInt(strCantidad, cuentaOperacionActual->getSaldoActual(), limite, 1, (limite - abs(aumentoTotal)));
		if (aumentoActual != 0)
		{
			ajustarSignoAumentoActual();
			return true;
		}
		else {
			std::cout << "\nValor ingresado no valido, intentelo nuevamente.";
			_getch();
			return false;
		}
	}

	void AumentadorPartida::ajustarSignoAumentoActual()
	{
		if (modoAumento == ModoAumento::Apertura)
		{
			aumentoActual = aumentoActual * ((cuentaOperacionActual->getTipo() == Cuenta::TipoCuenta::PASIVO_OPERATIVO) ? -1 : 1);
		}
		else {
			aumentoActual = aumentoActual * ((modoAumento == ModoAumento::Debe) ? 1 : -1);
		}
	}

	void AumentadorPartida::efectuarAumento()
	{
		aumentoTotal += aumentoActual;
		modificarCuenta(cuentaOperacionActual, aumentoActual);
	}

	bool AumentadorPartida::noHayLimite()
	{
		return limite == 0;
	}

	void AumentadorPartida::permitirFinalizar()
	{
		std::string strFinalizar;
		system("CLS");
		std::cout << "1. Continuar\n2.Finalizar\nElija una opcion: ";
		std::getline(data, strFinalizar);
		salir = (validarInt(strFinalizar, 1, 2, {}, {}) == 2) ? true : false;
	}