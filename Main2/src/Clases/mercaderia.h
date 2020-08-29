#pragma once

#include <string>
#include <vector>

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
	std::string Fecha;
	std::vector<RegistroPrecio> Registros;

	void crearRegistro(unsigned int precio, int delta, unsigned int existenciasActuales)
	{
		Registros.push_back(RegistroPrecio(precio, delta, existenciasActuales));
	}

	void borrarRegistroPorPrecio(unsigned int precioAEliminar)
	{
		if (hayRegistros())
		{
			for (unsigned int i = 0; i < Registros.size(); i++)
			{
				if (Registros[i].precio == precioAEliminar)
				{
					Registros.erase(Registros.begin() + i);
				}
			}
		}
	}
	
	RegistroPrecio* getRegistroPrecio(unsigned int precio)
	{
		if (hayRegistros())
		{
			for (RegistroPrecio& registro : Registros)
			{
				if (registro.precio == precio) { return &registro; }
			}
		}
		return nullptr;
	}

	void controlarRegistros()
	{
		if (hayRegistros())
		{
			for (unsigned int i = 0; i < Registros.size(); i++)
			{
				if (Registros[i].delta == 0)
				{
					Registros.erase(Registros.begin() + i);
				}
			}
		}
	}

	bool hayRegistros() const { return !Registros.empty(); }

	DiaMercaderia(std::string _fecha)
		: Fecha(_fecha) { Registros = {}; }
};

class Mercaderia
{
private:
	const std::string Nombre;
	std::vector<DiaMercaderia> Dias;
	std::vector<ExistenciasPrecioMercaderia> PreciosActuales;
public:

	int registrarCompra(std::string fecha, unsigned int precio, int delta)
	{
		int totalGastado = 0;
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
		return precio * delta;
	}

	unsigned int registarVenta(std::string fecha, unsigned int delta)
	{
		int gastoTotal = 0;
		int gastoActual = 0;
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
				gastoActual = registrarCompra(fecha, getPrecioMasBajo()->precio, -mercaderiasVendidasEnOperacionActual);
				gastoTotal += gastoActual;
			}
		}
		return abs(gastoTotal);
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
		return ((int)getExistenciasPrecio(precio) >= -delta);
	}

	void crearPrecio(unsigned int precio)
	{
		PreciosActuales.push_back(ExistenciasPrecioMercaderia(precio));
	}

	bool fechaEsUltimoDia(std::string fechaBuscada)
	{
		if (hayDias())
		{
			return (Dias.back().Fecha == fechaBuscada);
		}
		return false;
	}

	void modificarUltimoDia(unsigned int precio, int delta)
	{
		if (RegistroPrecio* registro = Dias.back().getRegistroPrecio(precio))
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

	ExistenciasPrecioMercaderia* getPrecioMasBajo()
	{
		if (hayExistencias())
		{
			ExistenciasPrecioMercaderia* precioMasBajo = &ExistenciasPrecioMercaderia(99999);
			for (ExistenciasPrecioMercaderia& registro : PreciosActuales)
			{
				if (registro.precio < precioMasBajo->precio)
				{
					precioMasBajo = &registro;
				}
			}
			return precioMasBajo;
		}
		else {
			return nullptr;
		}
	}

	// controla que el ultimo dia tenga registros y que estos registros tengan delta
	// tambien controla que no haya precios sin existencias
	void efectuarControl()
	{
		if (hayDias())
		{
			Dias.back().controlarRegistros();

			if (!Dias.back().hayRegistros())
			{
				Dias.erase(Dias.end() - 1);
			}
		}
		if (hayExistencias())
		{
			for (unsigned int i = 0; i < PreciosActuales.size(); i++)
			{
				if (PreciosActuales[i].existencias == 0)
				{
					PreciosActuales.erase(PreciosActuales.begin() + i);
				}
			}
		}
	}

public:

	std::string getNombre() const { return Nombre; }
	
	bool hayDias() const { return !Dias.empty(); }
	std::vector<const DiaMercaderia*> getDias() const
	{
		std::vector<const DiaMercaderia*> dias;
		if (hayDias())
		{
			for (const DiaMercaderia& dia : Dias)
			{
				dias.push_back(&dia);
			}
		}
		return dias;
	}
	const DiaMercaderia* getDiaPorFecha(std::string fechaBuscada) const
	{
		if (hayDias())
		{
			for (const DiaMercaderia& dia : Dias)
			{
				if (dia.Fecha == fechaBuscada)
				{
					return &dia;
				}
			}
		}
		return nullptr;
	}
	
	bool hayExistencias() const { return (!PreciosActuales.empty()); }
	std::vector<ExistenciasPrecioMercaderia> getExistencias() const { return PreciosActuales; }
	unsigned int getExistenciasPrecio(unsigned int precioBuscado) const
	{
		if (hayExistencias())
		{
			for (const ExistenciasPrecioMercaderia& registro : PreciosActuales)
			{
				if (registro.precio == precioBuscado)
				{
					return registro.existencias;
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
			for (const ExistenciasPrecioMercaderia& registro : PreciosActuales)
			{
				total += registro.existencias;
			}
		}
		return total;
	}

	bool precioExiste(unsigned int precioBuscado) const
	{
		if (hayExistencias())
		{
			for (const ExistenciasPrecioMercaderia& registro : PreciosActuales)
			{
				if (registro.precio == precioBuscado)
				{
					return true;
				}
			}
		}
		return false;
	}
	std::vector<unsigned int> getPreciosActuales() const
	{
		std::vector<unsigned int> preciosActuales = {};
		if (hayExistencias())
		{
			for (const ExistenciasPrecioMercaderia& registro : PreciosActuales)
			{
				preciosActuales.push_back(registro.precio);
			}
		}
		return preciosActuales;
	}
	ExistenciasPrecioMercaderia* getPrecio(unsigned int precioBuscado)
	{
		if (hayExistencias())
		{
			for (ExistenciasPrecioMercaderia& registro : PreciosActuales)
			{
				if (registro.precio == precioBuscado)
				{
					return &registro;
				}
			}
		}
		return nullptr;
	}
	
	Mercaderia(std::string nom) : Nombre(nom) { Dias = {}; };
};

