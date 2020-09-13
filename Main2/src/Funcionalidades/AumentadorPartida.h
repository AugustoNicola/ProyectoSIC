#pragma once

#include "SeleccionadorDeMercaderias.h"
#include "SeleccionadorDeCuentas.h"

class AumentadorPartida
{
private:
	static bool permitirCancelar;
	static Cuenta::TipoCuenta filtroCuentas;
	static ModoAumento modoAumento;
	static std::string mensajeEleccionCuenta;
	static int limite;
	static bool salir;

	static int aumentoTotal;
	static int aumentoActual;
	static Cuenta* cuentaOperacionActual;

public:
	static int realizarAumento(bool _permitirCancelar, Cuenta::TipoCuenta _filtroCuentas, ModoAumento _modoAumento, std::string _mensajeEleccionCuenta, std::optional<int> _limite);
private:
	static void initVariables(bool _permitirCancelar, Cuenta::TipoCuenta _filtroCuentas, ModoAumento _modoAumento, std::string _mensajeEleccionCuenta, std::optional<int> _limite);
	
	static bool condicionDeSalidaAlcanzada();

	static bool intentarElegirCuenta();

	static bool cuentaOperacionActualEsMercaderia();

	static void elegirAumentoActual();
	static void mostrarInformacion();
	static bool validarAumentoActual(std::string strCantidad);
	static bool noHayLimite();
	static void ajustarSignoAumentoActual();

	static void efectuarAumento();

	static void permitirFinalizar();
};