#pragma once

#include <string>
#include <vector>

/**
 * @brief Elemento con la informacion de un dia de compra de producto.
 * 
 * @param cantidad: representa la cantidad de unidades en el momento
 * @param precioUnitario: indica el precio al que se compro el producto, usado para metodos de costeo
 */
struct DiaMerca
{
	int cantidad;
	unsigned int precioUnitario;
	DiaMerca(int delt, unsigned int pU) : cantidad(delt), precioUnitario(pU) {};
};

/**
 * @brief Clase con la informacion de un tipo de producto, asi como un registro de sus utilidades por precio.
 */
class Mercaderia
{
private:
	std::string nombre;
	std::vector<DiaMerca> dias;
public:
	Mercaderia(std::string nom) : nombre(nom) {};
};

