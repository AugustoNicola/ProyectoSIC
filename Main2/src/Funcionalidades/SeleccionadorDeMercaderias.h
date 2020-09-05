#pragma once

#include <iostream>
#include <conio.h>

#include "clases.h"
#include "Vectores.h"
#include "Varias.h"

class SeleccionadorDeMercaderias
{
public:
	enum class TipoOperacion
	{
		COMPRA, VENTA, DEVOLUCION
	};
private:
	TipoOperacion Tipo;
	bool PermitirCancelar;
	bool cancelar = false;

	std::vector<Mercaderia*> mercaderiasDisponibles = {};
	Mercaderia* mercaderiaElegida = nullptr;

	int precioCompra = 0;
	unsigned int totalGastadoCompra = 0;

	unsigned int totalPerdidoVenta = 0;
	unsigned int totalGanadoVenta = 0;

	int precioDevolucion = 0;
	int totalGanadoDevolucion = 0;

	int cantidadMercaderias = 0;
public:
	SeleccionadorDeMercaderias(bool _permitirCancelar, TipoOperacion _tipo);
private:
	bool intentarElegirMercaderia();
	bool hayOpcionesMercaderiasValidas();
	void cargarMercaderias();
	bool validarMercaderia(std::string strOpcion);
	bool opcionCrearMercaderiaElegida(int opcionElegida);
	bool opcionCancelarElegida(int opcionElegida);
	
	void elegirPrecioCompra();
	void listarPreciosMercaderia();
	bool validarPrecioCompra(std::string strPrecio);

	void elegirCantidadCompra();
	bool validarCantidadCompra(std::string strCantidad);

	void elegirCantidadVenta();
	bool validarCantidadVenta(std::string strCantidad);
	void elegirPrecioVenta();
	bool validarPrecioVenta(std::string strPrecio);

	void elegirPrecioDevolucion();
	bool validarPrecioDevolucion(std::string strPrecio);
	void elegirCantidadDevolucion();
	bool validarCantidadDevolucion(std::string strCantidad);

public:
	Mercaderia* getMercaderia();
	unsigned int getTotalGastadoCompra();
	unsigned int getTotalPerdidoVenta();
	unsigned int getTotalGanadoVenta();
	unsigned int getTotalGanadoDevolucion();
	unsigned int getCantidad();
};

