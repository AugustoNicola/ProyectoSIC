#pragma once

#include <string>
#include <vector>
#include "cuenta.h"


struct Linea
{
	Cuenta *cuenta;
	int delta;
	Linea(Cuenta *_cuenta, int _delta) : cuenta(_cuenta), delta(_delta) {};
};

class Operacion
{
private:
	std::string Documento;
	std::vector<Linea> Lineas;

public:
	void crearLinea(Cuenta *cuenta, int modificacion)
	{
		if (contieneCuenta(cuenta->getNombre()))
		{
			modificarLinea(cuenta, modificacion);
		} else {
			Lineas.push_back(Linea(cuenta, modificacion));
		}
	}

private:
	void modificarLinea(Cuenta* cuenta, int modificacion)
	{
		for (unsigned int i = 0; i < Lineas.size(); i++)
		{
			if (Lineas[i].cuenta->getNombre() == cuenta->getNombre())
			{
				int total = Lineas[i].delta + modificacion;
				if (total != 0)
				{
					Lineas[i].delta = total;
				}
				else {
					Lineas.erase(Lineas.begin() + i);
				}
			}
		}
	}

public:
	std::string getDocumento() const { return Documento; }
	void setDocumento(std::string nuevoDocumento) { Documento = nuevoDocumento; }

	bool hayLineas() const { return !Lineas.empty(); }
	std::vector<const Linea*> getLineas() const
	{
		std::vector<const Linea*> lineas;
		if(hayLineas())
		{
			for (const Linea& linea : Lineas)
			{
				lineas.push_back(const_cast<const Linea*>(&linea));
			}
		}
		return lineas;
	}
	
	bool contieneCuenta(std::string cuentaBuscada) const
	{
		if (hayLineas())
		{
			for (const Linea& linea : Lineas)
			{
				if (linea.cuenta->getNombre() == cuentaBuscada)
				{
					return true;
				}
			}
		}
		return false;
	}

	Operacion() { Lineas = {}; };
	Operacion(std::string _Documento) : Documento(_Documento) { Lineas = {};  };
};

struct DiaOperaciones
{
private:
	std::string Fecha;
	std::vector<Operacion> Operaciones;

public:
	void crearOperacion(Operacion oper) { Operaciones.push_back(oper); }
	bool contieneCuenta(std::string cuentaBuscada)
	{
		if (hayOperaciones())
		{
			for (const Operacion& operacion : Operaciones)
			{
				if (operacion.contieneCuenta(cuentaBuscada))
				{
					return true;
				}
			}
		}
		return false;
	}

	bool hayOperaciones() const { return !Operaciones.empty(); }
	std::vector<const Operacion*> getOperaciones() const
	{
		std::vector<const Operacion*> operaciones;
		if (hayOperaciones())
		{
			for (const Operacion& operacion : Operaciones)
			{
				operaciones.push_back( const_cast<const Operacion*>(&operacion) );
			}
		}
		return operaciones;
	}

	std::string getFecha() const { return Fecha; }
	void setFecha(std::string _Fecha) { Fecha = _Fecha; }
	
	DiaOperaciones(std::string _Fecha) : Fecha(_Fecha) {};
};