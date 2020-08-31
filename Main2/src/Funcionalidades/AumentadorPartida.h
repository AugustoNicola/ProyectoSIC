#include <vector>
#include <string>
#include <optional>
#include <iostream>
#include <fstream>
#include <conio.h>

#include "Globales.h"
#include "Vectores.h"
#include "Varias.h"
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
	static bool condicionDeSalidaAlcanzada();
	static bool intentarElegirCuenta();
	static bool cuentaOperacionActualEsMercaderia();
	static void elegirAumentoActual();
	static void mostrarInformacion();
	static bool validarAumentoActual(std::string strCantidad);
	static void ajustarSignoAumentoActual();
	static void efectuarAumento();
	static bool noHayLimite();
	static void permitirFinalizar();
};