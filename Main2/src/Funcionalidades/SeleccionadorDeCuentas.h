#pragma once

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
	static bool PermitirCancelar;
	static Cuenta::TipoCuenta FiltroCuentas;
	static ModoAumento modoAumento;
	static std::string MsgEleccionCuenta;

	static std::vector<Cuenta*> cuentasSeleccionables;
	static std::string leyendaActivo;
	static std::string leyendaPasivo;
	static std::string leyendaGasto;

public:
	static Cuenta* elegirCuenta(bool _permitirCancelar, Cuenta::TipoCuenta _filtroCuentas, ModoAumento _modoAumento, std::string _msgEleccionCuenta);
	static void configurarModoAumento();
	static void configurarFiltroCuentas();
	static void mostrarCuentas();
	static void separadorDeTipo(Cuenta::TipoCuenta tipo);
};