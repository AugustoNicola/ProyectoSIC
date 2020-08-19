#include <vector>
#include <string>
#include <optional>
#include <iostream>
#include <fstream>
#include <conio.h>

#include "Globales.h"
#include "Vectores.h"
#include "Varias.h"

enum class ModoAumento
{
	Debe, Haber, Apertura
};

class SeleccionadorDeCuentas
{
private:
	Cuenta::TipoCuenta FiltroCuentas;
	ModoAumento modoAumento;
	std::string MsgEleccionCuenta;

	std::fstream& data;
	std::vector<Cuenta*> cuentasSeleccionables;
	std::string leyendaActivo;
	std::string leyendaPasivo;
	std::string leyendaGasto;

public:
	SeleccionadorDeCuentas(std::fstream& _data, Cuenta::TipoCuenta _filtroCuentas, ModoAumento _modoAumento, std::string _msgEleccionCuenta);
	void configurarModoAumento();
	void configurarFiltroCuentas();
	Cuenta* elegirCuenta();
	void mostrarCuentas();
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

	std::fstream &data;

public:
	AumentadorPartida(std::fstream& _data, Cuenta::TipoCuenta _filtroCuentas, ModoAumento _modoAumento, std::string _mensajeEleccionCuenta, std::optional<int> _limite);
	bool condicionDeSalidaAlcanzada();
	bool cuentaOperacionActualEsMercaderia();
	void elegirAumentoActual();
	void mostrarInformacion();
	bool validarAumentoActual(std::string strCantidad);
	void ajustarSignoAumentoActual();
	void efectuarAumento();
	bool noHayLimite();
	void permitirFinalizar();
};