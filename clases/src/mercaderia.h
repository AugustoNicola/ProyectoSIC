#pragma once

#include <string>
#include <vector>

struct RegistroPrecio
{
	const unsigned int precio;
	int delta;
	unsigned int existenciasActuales;

	void modificar(int delta)
	{
		this->delta += delta;
		existenciasActuales += delta;
	}

	RegistroPrecio(unsigned int _precio, unsigned int _delta)
		: precio(_precio), delta(_delta), existenciasActuales(_delta) {};
};

struct DiaMercaderia
{
	const std::string fecha;
	std::vector<RegistroPrecio> registros;

	RegistroPrecio* buscarRegistroPrecio(unsigned int precio)
	{
		if (hayRegistros())
		{
			for (unsigned int i = 0; i < registros.size(); i++)
			{
				if (registros[i].precio == precio) { return &registros[i]; }
			}
		}
		return nullptr;
	}

	bool hayRegistros() const { return !registros.empty(); }

	void crearRegistro(unsigned int precio, int delta)
	{
		registros.push_back(RegistroPrecio(precio, delta));
	}

	DiaMercaderia(std::string _fecha)
		: fecha(_fecha) { registros = {}; }
};

struct ExistenciasPrecioMercaderia
{
	const unsigned int precio;
	unsigned int existencias;

	ExistenciasPrecioMercaderia(unsigned int _precio)
		: precio(_precio), existencias(0) {};
};

class Mercaderia
{
private:
	std::string Nombre;
	std::vector<DiaMercaderia> Dias;
	std::vector<ExistenciasPrecioMercaderia> PreciosActuales;
public:

	void registrarCompra(std::string fecha, unsigned int precio, int delta)
	{
		if (asegurarPrecioYDeltaValidos(precio, delta))
		{
			/// precio existente o fue creado exitosamente
			/// delta no deja al precio en negativo
			if (fechaEsUltimoDia(fecha))
			{
				modificarUltimoDia(precio, delta);
			} else {
				crearNuevoDiaYRegistro(fecha, precio, delta);
			}
		}
	}

private:
	bool asegurarPrecioYDeltaValidos(unsigned int precio, int delta)
	{
		if (precioExiste(precio) && existenciasNoSerianNegativas(precio, delta))
		{
			return true;
		} else if (delta > 0) {
			crearPrecio(precio);
			return true;
		}
		return false;
	}

	bool existenciasNoSerianNegativas(unsigned int precio, int delta)
	{
		return (getExistenciasPrecio(precio) >= delta);
	}

	void crearPrecio(unsigned int precio)
	{
		PreciosActuales.push_back(ExistenciasPrecioMercaderia(precio));
	}

	bool fechaEsUltimoDia(std::string fechaBuscada)
	{
		if (hayDias())
		{
			return (Dias.back().fecha == fechaBuscada);
		}
		return false;
	}


	void modificarUltimoDia(unsigned int precio, int delta)
	{
		if (RegistroPrecio* registro = Dias.back().buscarRegistroPrecio(precio))
		{
			registro->modificar(delta);
		} else {
			Dias.back().crearRegistro(precio, delta);
		}
	}

	void crearNuevoDiaYRegistro(std::string fecha, unsigned int precio, int delta)
	{
		Dias.push_back(DiaMercaderia(fecha));
		Dias.back().crearRegistro(precio, delta);
	}
	

public:

	bool precioExiste(unsigned int precioBuscado) const
	{
		if (hayExistencias())
		{
			for (unsigned int i = 0; i < PreciosActuales.size(); i++)
			{
				if (PreciosActuales[i].precio == precioBuscado)
				{
					return true;
				}
			}
		}
		return false;
	}
	bool hayExistencias() const { return (!PreciosActuales.empty()); }
	bool hayDias() const { return !Dias.empty(); }

	int getExistenciasPrecio(unsigned int precioBuscado) const
	{
		if (hayExistencias())
		{
			for (unsigned int i = 0; i < PreciosActuales.size(); i++)
			{
				if (PreciosActuales[i].precio == precioBuscado)
				{
					return PreciosActuales[i].existencias;
				}
			}
		}
		return 0;
	}
	int getExistenciasTotales() const
	{
		unsigned int total = 0;
		if (hayExistencias())
		{
			for (unsigned int i = 0; i < PreciosActuales.size(); i++)
			{
				total += PreciosActuales[i].existencias;
			}
		}
		return total;
	}

	Mercaderia(std::string nom) : Nombre(nom) { Dias = {}; };
};

