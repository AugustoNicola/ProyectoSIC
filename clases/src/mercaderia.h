#pragma once
#include <string>
#include <vector>

/**
 * @brief Elemento con la informacion de un dia de compra de producto.
 * 
 * @param delta: representa la cantidad de unidades compradas
 * @param precioUnitario: indica el precio al que se compro el producto, usado para metodos de costeo
 */
struct DiaMerca
{
	int delta;
	int precioUnitario;
	DiaMerca(int delt, int pU) : delta(delt), precioUnitario(pU) {};
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