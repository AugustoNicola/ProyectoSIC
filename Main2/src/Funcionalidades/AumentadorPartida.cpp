#include "AumentadorPartida.h"

using namespace conmanip;

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
				
				SeleccionadorDeMercaderias operacionMercaderia(true, SeleccionadorDeMercaderias::TipoOperacion::COMPRA);
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
		std::cout << settextcolor(colorBase) << "\n\nSeleccione la cantidad: $";
		std::cin >> settextcolor(colorInput) >> strAumentoActual;
	} while (!validarAumentoActual(strAumentoActual));
}

void AumentadorPartida::mostrarInformacion()
{

	console_text_colors color;
	switch (cuentaOperacionActual->getTipo())
	{
	case Cuenta::TipoCuenta::ACTIVO_OPERATIVO:
	case Cuenta::TipoCuenta::ACTIVO_NO_OPERATIVO:
		color = console_text_colors::light_green;
		break;
	case Cuenta::TipoCuenta::PASIVO_OPERATIVO:
		color = console_text_colors::light_red;
		break;
	case Cuenta::TipoCuenta::GASTO_OPERATIVO:
		color = console_text_colors::light_cyan;
		break;
	}

	system("CLS");
	std::cout << settextcolor(colorBase) << "=============== "
		<< settextcolor(color) << cuentaOperacionActual->getNombre()
		<< settextcolor(colorBase) << " (Saldo Actual: "
		<< settextcolor(colorDatos) << formatearDinero(cuentaOperacionActual->getSaldoActual())
		<< settextcolor(colorBase) << ") ==============="
		<< "\n\nTotal aumentado: " 
		<< settextcolor(colorDatos) << formatearDinero(aumentoTotal)
		<< settextcolor(colorBase) << "\nLimite: " 
		<< settextcolor(colorDatos) << formatearDinero(abs(limite));
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
	std::cout << settextcolor(console_text_colors::light_green) << "1. Continuar"
		<< settextcolor(colorError) << "\n2.Finalizar"
		<< settextcolor(colorBase) << "\n\nElija una opcion: ";
	std::cin >> settextcolor(colorInput) >> strFinalizar;
	salir = (validarInt(strFinalizar, 1, 2) == 2) ? true : false;
}