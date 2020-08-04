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
	static enum class TipoCuenta {
		ACTIVO_OPERATIVO, PASIVO_OPERATIVO, GASTO_OPERATIVO,
		ACTIVO_NO_OPERATIVO, PASIVO_NO_OPERATIVO, GASTO_NO_OPERATIVO,
		GANANCIA, PATRIMONIO_NETO,
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
			getDiaPorFechaParaModificar(fecha)->modificarDiaExistente(delta);
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
	DiaCuenta* getDiaPorFechaParaModificar(std::string fechaBuscada)
	{
		for (int i = 0; i < dias.size(); i++)
		{
			if (dias[i].fecha == fechaBuscada) { return &dias[i]; }
		}
		return nullptr;
	}

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
	
	const DiaCuenta* getDiaPorPosicion(unsigned int posicion) const
	{
		return ((posicion < dias.size()) ? &dias[posicion] : nullptr);
	}
	const DiaCuenta* getDiaPorFecha(std::string fechaBuscada) const
	{
		for (int i = 0; i < dias.size(); i++)
		{
			if (dias[i].fecha == fechaBuscada) { return &dias[i]; }
		}
		return nullptr;
	}

	std::vector<DiaCuenta> getDias()
	{
		std::vector<DiaCuenta> vectorRespuesta;

		if (hayDias())
		{
			for (unsigned int i = 0; i < dias.size(); i++)
			{
				vectorRespuesta.push_back(dias[i]);
			}
		}
		return vectorRespuesta;
	}

	bool operator==(std::string nombreBuscado)
	{
		return (Nombre == nombreBuscado);
	}
	
	Cuenta(std::string _Nombre, bool _ModoDebitado, Cuenta::TipoCuenta _Tipo) : Nombre(_Nombre), ModoDebitado(_ModoDebitado), Tipo(_Tipo) { dias = {}; };

};
