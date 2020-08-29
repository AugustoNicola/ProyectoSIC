#include "AumentadorPartida.h"

Cuenta::TipoCuenta AumentadorPartida::filtroCuentas;
ModoAumento AumentadorPartida::modoAumento;
std::string AumentadorPartida::mensajeEleccionCuenta;
int AumentadorPartida::limite;
bool AumentadorPartida::salir;

int AumentadorPartida::aumentoTotal;
int AumentadorPartida::aumentoActual;
Cuenta* AumentadorPartida::cuentaOperacionActual;

int AumentadorPartida::realizarAumento(Cuenta::TipoCuenta _filtroCuentas, ModoAumento _modoAumento, std::string _mensajeEleccionCuenta, std::optional<int> _limite)
{
	filtroCuentas = _filtroCuentas;
	modoAumento = _modoAumento;
	mensajeEleccionCuenta = _mensajeEleccionCuenta;
	limite = _limite.has_value() ? _limite.value() : 0;
	salir = false;
	aumentoActual = 0;
	aumentoTotal = 0;

	while (!condicionDeSalidaAlcanzada())
	{
		cuentaOperacionActual = SeleccionadorDeCuentas::elegirCuenta(filtroCuentas, modoAumento, mensajeEleccionCuenta);

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
	return aumentoTotal;
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
		mostrarInformacion();
		std::cout << "\n\nSeleccione la cantidad: $";
		std::cin >> strAumentoActual;
	} while (!validarAumentoActual(strAumentoActual));
}

void AumentadorPartida::mostrarInformacion()
{
	header(cuentaOperacionActual->getNombre() += (std::string)" (Saldo Actual: $" += std::to_string(cuentaOperacionActual->getSaldoActual()) += ")", 2);
	std::cout << "Total actual: $" << abs(aumentoTotal);
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
	header("MODIFICACION REGISTRADA", 2);
	std::cout << "1. Continuar\n2.Finalizar\n\nElija una opcion: ";
	std::cin >> strFinalizar;
	salir = (validarInt(strFinalizar, {}, {}, 1, 2) == 2) ? true : false;
}