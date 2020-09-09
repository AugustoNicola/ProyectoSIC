#pragma once

#include <iostream>
#include <conio.h>

#include "Varias.h"

class SeleccionadorDeMercaderias
{
public:
	enum class TipoOperacion
	{
		COMPRA, VENTA, DEVOLUCION, REINTEGRO
	};
private:
	TipoOperacion Tipo;
	bool PermitirCancelar;
	bool cancelar = false;

	std::vector<Mercaderia*> mercaderiasDisponibles = {};
	Mercaderia* mercaderiaElegida = nullptr;

	int precioCompra = 0;
	unsigned int totalGastadoCompra = 0;

	int precioVenta = 0;
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
	Mercaderia* getMercaderia() const;
	unsigned int getTotalGastadoCompra() const;
	unsigned int getTotalPerdidoVenta() const;
	unsigned int getTotalGanadoVenta() const;
	unsigned int getTotalGanadoDevolucion() const;
	unsigned int getTotalReintegrado() const;
	unsigned int getCantidad() const;
};

