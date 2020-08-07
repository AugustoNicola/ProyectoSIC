#pragma once

#include <string>
#include <vector>

struct RegistroPrecio
{
	unsigned int precio;
	int delta;
	unsigned int existenciasActuales;

	void modificar(int delta)
	{
		this->delta += delta;
		existenciasActuales += delta;
	}

	RegistroPrecio(unsigned int _precio, int _delta, unsigned int _existenciasPrevias)
		: precio(_precio), delta(_delta), existenciasActuales(_existenciasPrevias)
	{
		existenciasActuales += delta; 
	};
};

struct DiaMercaderia
{
	std::string fecha;
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

	void crearRegistro(unsigned int precio, int delta, unsigned int existenciasActuales)
	{
		registros.push_back(RegistroPrecio(precio, delta, existenciasActuales));
	}

	void borrarRegistroPorPrecio(unsigned int precioAEliminar)
	{
		for (unsigned int i = 0; i < registros.size(); i++)
		{
			if (registros[i].precio == precioAEliminar)
			{
				registros.erase(registros.begin()+i);
			}
		}
	}

	void controlarRegistros()
	{
		for (unsigned int i = 0; i < registros.size(); i++)
		{
			if (registros[i].delta == 0)
			{
				registros.erase(registros.begin() + i);
			}
		}
	}

	DiaMercaderia(std::string _fecha)
		: fecha(_fecha) { registros = {}; }
};

struct ExistenciasPrecioMercaderia
{
	unsigned int precio;
	unsigned int existencias;

	void modificar(int delta)
	{
		existencias += delta;
	}

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
			modificarExistenciasDePrecio(precio, delta);

			efectuarControl();
		}
	}

	void registrarVenta(std::string fecha, unsigned int delta)
	{
		if (cantidadDeVentaValida(delta))
		{
			while (quedanMercaderiasPorVender(delta))
			{
				int mercaderiasVendidasEnOperacionActual = 0;
				unsigned int existenciasPrecioMasBajoActual = getPrecioMasBajo()->existencias;
				while (delta > 0 && existenciasPrecioMasBajoActual > 0)
				{
					mercaderiasVendidasEnOperacionActual++;
					delta--;
					existenciasPrecioMasBajoActual--;
				}
				// utiliza registrarCompra como venta utilizando un numero negativo
				// a la vez se encarga de eliminar el precio en caso de estar agotado
				registrarCompra(fecha, getPrecioMasBajo()->precio, -1 * mercaderiasVendidasEnOperacionActual);
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
		return (getExistenciasPrecio(precio) >= -delta);
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
			Dias.back().crearRegistro(precio, delta, getExistenciasPrecio(precio));
		}
	}

	void crearNuevoDiaYRegistro(std::string fecha, unsigned int precio, int delta)
	{
		Dias.push_back(DiaMercaderia(fecha));
		Dias.back().crearRegistro(precio, delta, getExistenciasPrecio(precio));
	}
	
	void modificarExistenciasDePrecio(unsigned int precio, int delta)
	{
		if (precioExiste(precio))
		{
			getPrecio(precio)->modificar(delta);
		}
	}


	bool cantidadDeVentaValida(unsigned int cantidadIntentada)
	{
		return ( (cantidadIntentada <= getExistenciasTotales()) 
			  && (cantidadIntentada > 0) );
	}

	bool quedanMercaderiasPorVender(unsigned int mercaderiasPorVender) { return (mercaderiasPorVender > 0); }

	

	// controla que el ultimo dia tenga registros y que estos registros tengan delta
	// tambien controla que no haya precios sin existencias
	void efectuarControl()
	{
		Dias.back().controlarRegistros();
		
		if (!Dias.back().hayRegistros())
		{
			Dias.erase(Dias.end());
		}

		for (unsigned int i = 0; i < PreciosActuales.size(); i++)
		{
			if (PreciosActuales[i].existencias == 0)
			{
				PreciosActuales.erase(PreciosActuales.begin() + i);
			}
		}

	}

public:

	ExistenciasPrecioMercaderia* getPrecioMasBajo()
	{
		if (hayExistencias())
		{
			ExistenciasPrecioMercaderia* precioMasBajo = &ExistenciasPrecioMercaderia(99999);
			for (unsigned int i = 0; i < PreciosActuales.size(); i++)
			{
				if (PreciosActuales[i].precio < precioMasBajo->precio)
				{
					precioMasBajo = &PreciosActuales[i];
				}
			}
			return precioMasBajo;
		}
		else {
			return nullptr;
		}
	}

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

	std::string getNombre() const { return Nombre; }
	unsigned int getExistenciasPrecio(unsigned int precioBuscado) const
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
	unsigned int getExistenciasTotales() const
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

	std::vector<unsigned int> getPreciosActuales() const
	{
		std::vector<unsigned int> preciosActuales = {};
		if (hayExistencias())
		{
			for (unsigned int i = 0; i < PreciosActuales.size(); i++)
			{
				preciosActuales.push_back(PreciosActuales[i].precio);
			}
		}
		return preciosActuales;
	}
	ExistenciasPrecioMercaderia* getPrecio(unsigned int precioBuscado)
	{
		if (!PreciosActuales.empty())
		{
			for (unsigned int i = 0; i < PreciosActuales.size(); i++)
			{
				if (PreciosActuales[i].precio == precioBuscado)
				{
					return &PreciosActuales[i];
				}
			}
		}
	}

	const DiaMercaderia* getDiaPorFecha(std::string fechaBuscada) const
	{
		if (hayDias())
		{
			for (unsigned int i = 0; i < Dias.size(); i++)
			{
				if (Dias[i].fecha == fechaBuscada)
				{
					return const_cast<const DiaMercaderia*>(&Dias[i]);
				}
			}
		}
		return nullptr;
	}
	std::vector<const DiaMercaderia*> getDias() const
	{
		std::vector<const DiaMercaderia*> dias;
		if (hayDias())
		{
			for (unsigned int i = 0; i < Dias.size(); i++)
			{
				dias.push_back( const_cast<const DiaMercaderia*>(&Dias[i]) );
			}
		}
		return dias;
	}

	Mercaderia(std::string nom) : Nombre(nom) { Dias = {}; };
};

