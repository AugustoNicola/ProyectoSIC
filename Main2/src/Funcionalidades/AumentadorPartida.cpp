#include "AumentadorPartida.h"

bool AumentadorPartida::permitirCancelar;
Cuenta::TipoCuenta AumentadorPartida::filtroCuentas;
ModoAumento AumentadorPartida::modoAumento;
std::string AumentadorPartida::mensajeEleccionCuenta;
int AumentadorPartida::limite;
bool AumentadorPartida::salir;

int AumentadorPartida::aumentoTotal;
int AumentadorPartida::aumentoActual;
Cuenta* AumentadorPartida::cuentaOperacionActual;

int AumentadorPartida::realizarAumento(bool _permitirCancelar, Cuenta::TipoCuenta _filtroCuentas, ModoAumento _modoAumento, std::string _mensajeEleccionCuenta, std::optional<int> _limite)
{
	permitirCancelar = _permitirCancelar;
	filtroCuentas = _filtroCuentas;
	modoAumento = _modoAumento;
	mensajeEleccionCuenta = _mensajeEleccionCuenta;
	limite = _limite.has_value() ? _limite.value() : 0;
	salir = false;
	aumentoActual = 0;
	aumentoTotal = 0;

	while (!condicionDeSalidaAlcanzada())
	{
		if (intentarElegirCuenta())
		{
			if (cuentaOperacionActualEsMercaderia())
			{
				/// hay mercaderias!
				
				SeleccionadorDeMercaderias operacionMercaderia(true, true);
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
		} else {
			return 0;
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

bool AumentadorPartida::intentarElegirCuenta()
{
	cuentaOperacionActual = SeleccionadorDeCuentas::elegirCuenta(permitirCancelar, filtroCuentas, modoAumento, mensajeEleccionCuenta);
	permitirCancelar = false;
	return ( (cuentaOperacionActual != nullptr) ? true : false);
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
	header(cuentaOperacionActual->getNombre() += (std::string)" (Saldo Actual: " += formatearSaldoCuenta(cuentaOperacionActual->getSaldoActual()) += ")", 2);
	std::cout << "Total actual: $" << abs(aumentoTotal);
	std::cout << "\nLimite: $" << abs(limite);
}

std::string AumentadorPartida::formatearSaldoCuenta(int num)
{
	std::string str = std::to_string(abs(num));
	str.insert(0, (num >= 0 ? "$" : "-$"));
	return str;
}

bool AumentadorPartida::validarAumentoActual(std::string strCantidad)
{
	aumentoActual = validarInt(strCantidad, 1, (noHayLimite() ? INT_MAX : abs(limite - aumentoTotal)), abs(cuentaOperacionActual->getSaldoActual()), abs(limite - aumentoTotal));
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
	salir = (validarInt(strFinalizar, 1, 2) == 2) ? true : false;
}