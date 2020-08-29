#pragma once
#include <string>
#include <vector>
#include <utility>


struct DiaCuenta
{
	std::string fecha;
	int valorActual;
	int delta;

	void modificarDiaExistente(int modificacion)
	{
		delta += modificacion;
		valorActual += modificacion;
	};
	
	DiaCuenta(std::string _fecha, int _valorAnterior, int _delta) : fecha(_fecha), valorActual(_valorAnterior), delta(_delta) { valorActual += _delta; };
};

class Cuenta
{
public:
	enum class TipoCuenta {
		ACTIVO_OPERATIVO, PASIVO_OPERATIVO, GASTO_OPERATIVO,
		ACTIVO_NO_OPERATIVO, PASIVO_NO_OPERATIVO, GASTO_NO_OPERATIVO,
		GANANCIA, PATRIMONIO_NETO,
		F_OPER
	};

private:
	std::string Nombre;
	std::vector<DiaCuenta> Dias;
	Cuenta::TipoCuenta Tipo;

public:
	void registrarModificacion(std::string fecha, int delta)
	{
		if (hayDias() && diaExiste(fecha))
		{
			getDiaPorFechaParaModificar(fecha)->modificarDiaExistente(delta);
		}
		else {
			crearNuevoDia(fecha, delta);
		}	
	}

private:
	DiaCuenta* getDiaPorFechaParaModificar(std::string fechaBuscada)
	{
		for (DiaCuenta &dia : Dias)
		{
			if (fechaBuscada == dia.fecha) { return &dia; }
		}
		return nullptr;
	}

	void crearNuevoDia(std::string fecha, int delta)
	{
		Dias.push_back(DiaCuenta(fecha, getSaldoActual(), delta));
	}

public:
	/* Getters y Constructor */
	Cuenta::TipoCuenta getTipo() const { return Tipo; }
	std::string getNombre() const { return Nombre; }
	bool operator==(std::string nombreBuscado)
	{
		return (Nombre == nombreBuscado);
	}
	int getSaldoActual() const { return (!Dias.empty()) ? Dias.back().valorActual : 0 ; }
	
	bool diaExiste(std::string fechaBuscada) const
	{
		for (const DiaCuenta &dia : Dias)
		{
			if (fechaBuscada == dia.fecha)
			{
				return true;
			}
		}
		return false;
	}
	const DiaCuenta* getDiaPorPosicion(unsigned int posicion) const
	{
		return ((posicion < Dias.size()) ? &Dias[posicion] : nullptr);
	}
	const DiaCuenta* getDiaPorFecha(std::string fechaBuscada) const
	{
		for (const DiaCuenta &dia : Dias)
		{
			if (dia.fecha == fechaBuscada) { return &dia; }
		}
		return nullptr;
	}

	bool hayDias() const { return !Dias.empty(); }
	std::vector<DiaCuenta> getDias()
	{
		std::vector<DiaCuenta> vectorRespuesta;

		if (hayDias())
		{
			for (DiaCuenta &dia : Dias)
			{
				vectorRespuesta.push_back(dia);
			}
		}
		return vectorRespuesta;
	}

	
	Cuenta(std::string _Nombre, Cuenta::TipoCuenta _Tipo) : Nombre(_Nombre), Tipo(_Tipo) { Dias = {}; };

};
