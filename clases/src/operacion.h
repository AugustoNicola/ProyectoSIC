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

	std::string getDocumento() const { return Documento; }
	void setDocumento(std::string nuevoDocumento) { Documento = nuevoDocumento; }

	void crearLinea(Cuenta *cuenta, int modificacion)
	{
		Lineas.push_back(Linea(cuenta, modificacion));
	}

	bool hayLineas() const { return !Lineas.empty(); }
	std::vector<const Linea*> getLineas() const
	{
		std::vector<const Linea*> lineas;
		if(hayLineas())
		{
			for (unsigned int i = 0; i < Lineas.size(); i++)
			{
				lineas.push_back(const_cast<const Linea*>(&Lineas[i]));
			}
		}
		return lineas;
	}
	
	bool contieneCuenta(std::string cuentaBuscada) const
	{
		if (hayLineas())
		{
			for (unsigned int i = 0; i < Lineas.size(); i++)
			{
				if (Lineas[i].cuenta->getNombre() == cuentaBuscada)
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
	std::string fecha;
	std::vector<Operacion> operaciones;

	void nuevaOperacion(Operacion oper) { operaciones.push_back(oper); }

	DiaOperaciones(std::string f) : fecha(f) {};
};