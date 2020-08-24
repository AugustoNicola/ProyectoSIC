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

	std::vector<Mercaderia*> mercaderiasDisponibles = {};
	Mercaderia* mercaderiaElegida = nullptr;
	unsigned int totalGastadoCompra = 0;
	unsigned int totalPerdidoVenta = 0;
	unsigned int totalGanadoVenta = 0;
	int precioCompraElegido = 0;
	int cantidadElegida = 0;
public:
	SeleccionadorDeMercaderias(bool _esCompra);
private:
	bool intentarElegirMercaderia();
	bool hayOpcionesMercaderiasValidas();
	void cargarOpcionesMercaderia();
	bool validarOpcionMercaderia(std::string strOpcion);
	bool opcionCrearMercaderiaElegida(int opcionElegida);
	
	void elegirPrecioCompra();
	void listarPreciosMercaderia();
	bool validarOpcionPrecioCompra(std::string strOpcionPrecio);

	void elegirCantidadCompra();
	bool validarCantidadCompra(std::string strOpcionCantidad);

	void elegirCantidadVenta();
	bool validarCantidadVenta(std::string strOpcionCantidad);
	void elegirPrecioVenta();
	bool validarPrecioVenta(std::string strOpcionPrecio);

public:
	Mercaderia* getMercaderia();
	unsigned int getTotalGastadoCompra();
	unsigned int getTotalPerdidoVenta();
	unsigned int getTotalGanadoVenta();
	unsigned int getCantidad();
};

