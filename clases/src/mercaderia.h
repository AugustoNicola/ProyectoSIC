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
};

/**
 * @brief Estructura individual de un producto a cierto precio. Contiene un vector con el stock por dias.
 * 
 * @param precio: precio diferenciativo de esta subclase.
 * @param dias: vector con la situacion por dia del producto.
 */
struct PrecioMerca
{
	float precio;
	std::vector<DiaMerca> dias;
};

/**
 * @brief Clase con la informacion de un tipo de producto, asi como un registro de su stock, por precio.
 * 
 * @param nombre: string con el nombre diferenciativo del producto.
 * @param precios: vector con una lista de todos los precios unitarios a los que se adquirio el producto.
 */
class Mercaderia
{
private:
	std::string nombre;
public:
	std::vector<PrecioMerca> precios;
};

