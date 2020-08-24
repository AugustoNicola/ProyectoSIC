#include "AumentadorPartida.h"

	SeleccionadorDeCuentas::SeleccionadorDeCuentas(Cuenta::TipoCuenta _filtroCuentas, ModoAumento _modoAumento, std::string _msgEleccionCuenta)
		: FiltroCuentas(_filtroCuentas), modoAumento(_modoAumento), MsgEleccionCuenta(_msgEleccionCuenta)
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
			if (modoAumento != ModoAumento::Haber) { cuentasSeleccionables.push_back(buscarCuenta("Mercaderias")); }
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


	AumentadorPartida::AumentadorPartida(Cuenta::TipoCuenta _filtroCuentas, ModoAumento _modoAumento, std::string _mensajeEleccionCuenta, std::optional<int> _limite)
		: filtroCuentas(_filtroCuentas), modoAumento(_modoAumento), mensajeEleccionCuenta(_mensajeEleccionCuenta), limite(_limite.has_value() ? _limite.value() : 0)
	{
		SeleccionadorDeCuentas seleccionador(filtroCuentas, modoAumento, mensajeEleccionCuenta);

		while (!condicionDeSalidaAlcanzada())
		{
			cuentaOperacionActual = seleccionador.elegirCuenta();

			if (cuentaOperacionActualEsMercaderia())
			{
				/// hay mercaderias!
				
				SeleccionadorDeMercaderias operacionMercaderia(true);
				aumentoActual = operacionMercaderia.getTotalGastadoCompra();
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
			return limite <= aumentoTotal;
		}
		else if (limite < 0) {
			return limite >= aumentoTotal;
		}
		else {
			return salir;
		}
	}

	bool AumentadorPartida::cuentaOperacionActualEsMercaderia()
	{
		return *cuentaOperacionActual == "Mercaderias";
	}

	void AumentadorPartida::elegirAumentoActual()
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

	void AumentadorPartida::mostrarInformacion()
	{
		std::cout << "\n\nCuenta elegida: " << cuentaOperacionActual->getNombre() << " (valor: $" << abs(cuentaOperacionActual->getSaldoActual()) << ")";
		std::cout << "\n\nTotal actual: $" << abs(aumentoTotal);
		std::cout << "\nLimite: $" << abs(limite);
	}

	bool AumentadorPartida::validarAumentoActual(std::string strCantidad)
	{
		aumentoActual = validarInt(strCantidad, abs(cuentaOperacionActual->getSaldoActual()), abs(limite), 1, (noHayLimite() ? INT_MAX : abs(limite - aumentoTotal) )   );
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
		if (aumentoActual != 0)
		{
			aumentoTotal += aumentoActual;
			modificarCuenta(cuentaOperacionActual, aumentoActual);
		}
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
		std::cin >> strFinalizar;
		salir = (validarInt(strFinalizar, {}, {}, 1, 2) == 2) ? true : false;
	}

	int AumentadorPartida::getAumentoTotal() const { return aumentoTotal; }