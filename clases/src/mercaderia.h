#pragma once

#include <string>
#include <vector>


struct DiaMerca
{
	std::string dia;
	int delta;
	unsigned int cantidad;

	void aumentar(int delt)
	{
		delta += delt;
		cantidad += delt;
	}

	DiaMerca(std::string d, unsigned int cantAnterior, int delt) : dia(d), cantidad(cantAnterior), delta(delt) { cantidad += delt;  }
};


struct PrecioMerca
{
	int precio;
	std::vector<DiaMerca> dias;

	bool hayExistencias()
	{
		if (!dias.empty())
		{
			/// hay dias
			if (dias.back().cantidad > 0) { return true; }
		}
		return false;
	}

	void nuevoDiaPrecioMerca(std::string fecha, int modificacion)
	{
		if (!dias.empty())
		{
			/// hay fechas
			for (int i = 0; i < dias.size(); i++)
			{
				if (fecha == dias[i].dia)
				{
					/// fecha encontrada, sumar valor actual al previo
					// verifica si se debe aumentar positivo (debe) o negativo (haber)
					dias[i].aumentar(modificacion);
					return;
				}
			}
			///fecha no encontrada, crear
			dias.push_back(DiaMerca(fecha, dias.back().cantidad, modificacion));
		} else {
			/// no hay fechas, crear primera
			dias.push_back(DiaMerca(fecha, 0, modificacion));
		}
		/* verifica que haya dias previos */
		dias.push_back(DiaMerca(fecha, ((!dias.empty()) ? dias.back().cantidad : 0), modificacion));
	}

	PrecioMerca(int p) : precio(p) { dias = {}; };
};

class Mercaderia
{
public:
	std::string nombre;
	std::vector<PrecioMerca> precios;

	bool hayExistencias()
	{
		if (!precios.empty())
		{
			/// hay precios
			/* recorre precios */
			for (unsigned int i = 0; i < precios.size(); i++)
			{
				if (!precios[i].dias.empty())
				{
					// si hay existencias, devolver true
					if (precios[i].dias.back().cantidad > 0) { return true; }
				}
			}
		}
		return false; // no se encontraron existencias en toda la busqueda
	}

	void nuevoDiaMercaderia(std::string dia, int precio, int delta)
	{
		/* Verifica si ya hay precios */
		if (!precios.empty())
		{
			/// hay al menos un precio
			for (int i = 0; i < precios.size(); i++)
			{
				/* Verifica si ya se listo este precio */
				if(precio == precios[i].precio)
				{
					/// precio encontrado, detener loop
					precios[i].nuevoDiaPrecioMerca(dia, delta); // agregar dia al precio
					return;
				}
				/// precio no encontrado, listar
				precios.push_back(PrecioMerca(precio)); //lista el precio
				precios.back().dias.push_back(DiaMerca(dia, precios.back().dias.back().cantidad, delta)); // agregar dia al nuevo precio
			}
		} else {
			/// no hay ningun precio
			precios.push_back(PrecioMerca(precio)); //lista el precio
			precios[0].dias.push_back(DiaMerca(dia, precios[0].dias.back().cantidad, delta)); // agrega dia al nuevo precio
		}
	};

	Mercaderia(std::string nom) : nombre(nom) { precios = {}; };
};

