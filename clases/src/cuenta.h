#pragma once
#include <string>
#include <vector>
#include <utility>


struct DiaCuenta
{
	std::string fecha;
	int valorActual;
	int delta;

	void modificar(int modificacion)
	{
		delta += modificacion;
		valorActual += modificacion;
	};

	DiaCuenta& operator=(const DiaCuenta& otro)
	{
		fecha = std::string(otro.fecha);
		delta = otro.delta;
		valorActual = otro.valorActual;
		return *this;
	}

	DiaCuenta(std::string _fecha, int _valorAnterior, int _delta) : fecha(_fecha), valorActual(_valorAnterior), delta(_delta) { valorActual += _delta; };
};

class Cuenta
{
public:
	static enum TipoCuenta {
		ACTIVO_OPERATIVO, PASIVO_OPERATIVO, GASTO_OPERATIVO,
		ACTIVO_NO_OPERATIVO, PASIVO_NO_OPERATIVO, GASTO_NO_OPERATIVO, GANANCIA,
		F_OPER
	};

private:
	std::string Nombre;
	bool ModoDebitado;
	std::vector<DiaCuenta> dias;
	Cuenta::TipoCuenta Tipo;

public:
	void registrarModificacion(std::string fecha, int delta)
	{
		if (hayDias() && fechaExiste(fecha))
		{
			getDiaPorFecha(fecha)->modificar(delta);
		}
		else {
			crearNuevoDia(fecha, delta);
		}	
	}

	bool hayDias() const { return !dias.empty();  }
	bool fechaExiste(std::string fecha) const
	{
		for (int i = 0; i < dias.size(); i++)
		{
			if (fecha == dias[i].fecha)
			{
				return true;
			}
		}
		return false;
	}
private:
	void crearNuevoDia(std::string fecha, int delta)
	{
		dias.push_back(DiaCuenta(fecha, getSaldoActual(), delta));
	}

public:
	/* Getters y Constructor */
	int getSaldoActual() const { return (!dias.empty()) ? dias.back().valorActual : 0 ; }
	Cuenta::TipoCuenta getTipo() const { return Tipo; }
	std::string getNombre() const { return Nombre; }
	bool getModoDebitado() const { return ModoDebitado; }
	
	DiaCuenta* getDiaPorPosicion(unsigned int posicion) 
	{
		return ((posicion < dias.size()) ? &dias[posicion] : nullptr);
	}
	DiaCuenta* getDiaPorFecha(std::string fechaBuscada) 
	{
		for (int i = 0; i < dias.size(); i++)
		{
			if (dias[i].fecha == fechaBuscada) { return &dias[i]; }
		}
		return nullptr;
	}
	
	Cuenta(std::string _Nombre, bool _ModoDebitado, Cuenta::TipoCuenta _Tipo) : Nombre(_Nombre), ModoDebitado(_ModoDebitado), Tipo(_Tipo) { dias = {}; };

};
