#pragma once

#include <iostream>
#include <conio.h>

#include "clases.h"
#include "Vectores.h"
#include "Varias.h"

class SeleccionadorDeMercaderias
{
private:
	bool esCompra;

	std::vector<Mercaderia*> mercaderiasDisponibles;
	Mercaderia* mercaderiaElegida;


public:
	SeleccionadorDeMercaderias(bool _esCompra);
private:
	bool intentarElegirMercaderia();
	bool hayOpcionesDeMercaderiasValidas();
	void cargarOpcionesDeMercaderia();
	bool validarOpcionDeMercaderia(std::string strOpcion);
	bool opcionCrearMercaderiaElegida(int opcionElegida);
	
	void cargarOpcionesPrecio();
	//siempre hay opciones validas
	void elegirOpcionPrecio();
	bool validarOpcionPrecio();
	//crear precio
	//usar precio

	void elegirCantidad();
	void mostrarInfo();
	bool validarCantidad();
	bool validarPrecioDeVenta(); //solo en venta, quiza bool?

	void cargarRespuesta(); //no es void?

public:
	//getters de la respuesta
};

