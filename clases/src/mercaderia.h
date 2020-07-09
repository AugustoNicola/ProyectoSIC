#pragma once

#include <string>
#include <vector>

/**
 * @brief Estructura que almacena el estado de un producto a determinado precio segun los dias.
 * 
 * @param dia: string que guarda la fecha "DD/MM"
 * @param delta: variacion del stock respecto a la fecha anterior
 * @param cantidad: cantidad de productos en el dia
 */
struct DiaMerca
{
	std::string dia;
	int delta;
	unsigned int cantidad;
	DiaMerca(std::string d, unsigned int cantAnterior, int delt) : dia(d), cantidad(cantAnterior), delta(delt) { cantidad += delt;  }
};

/**
 * @brief Estructura individual de un producto a cierto precio. Contiene un vector con el stock por dias.
 * 
 * @param precio: precio diferenciativo de esta subclase.
 * @param dias: vector con la situacion por dia del producto.
 */
struct PrecioMerca
{
	int precio;
	std::vector<DiaMerca> dias;

	/* Comprueba si hay existencias en este momento */
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
		/* verifica que haya dias previos */
		dias.push_back(DiaMerca(fecha, ((!dias.empty()) ? dias.back().cantidad : 0), modificacion));
	}

	PrecioMerca(int p) : precio(p) { dias = {}; };
};

/**
 * @brief Clase con la informacion de un tipo de producto, asi como un registro de su stock, por precio.
 * 
 * @param nombre: string con el nombre diferenciativo del producto.
 * @param precios: vector con una lista de todos los precios unitarios a los que se adquirio el producto.
 */
class Mercaderia
{
public:
	std::string nombre;
	std::vector<PrecioMerca> precios;

	/* Comprueba si en el momento actual hay existencias en al menos uno de los precios de la mercaderia */
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

	/**
	 * @brief Agrega un nuevo dia a la lista de la mercaderia al precio indicado.
	 * 
	 * @param dia: fecha del dia (DD/MM)
	 * @param precio: precio al que esta el producto en la lista que se debe ampliar
	 * @param delta: modificacion de existencias
	 */
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
					precios[i].dias.push_back(DiaMerca(dia, precios[i].dias.back().cantidad, delta)); // agregar dia al precio
					break;
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

