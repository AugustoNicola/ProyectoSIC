#include "SeleccionadorDeMercaderias.h"

SeleccionadorDeMercaderias::SeleccionadorDeMercaderias(bool _permitirCancelar, bool _esCompra)
	: esCompra(_esCompra), PermitirCancelar(_permitirCancelar)
{
	esCompra = _esCompra;
	PermitirCancelar = _permitirCancelar;

	if (intentarElegirMercaderia())
	{
		if (esCompra)
		{
			elegirPrecioCompra();
			elegirCantidadCompra();
		} else {
			elegirCantidadVenta();
			elegirPrecioVenta();
		}
	}
};

bool SeleccionadorDeMercaderias::intentarElegirMercaderia()
{
	if (hayOpcionesMercaderiasValidas())
	{
		std::string strOpcion;
		do
		{
			cargarOpcionesMercaderia();

			std::cout << "\n\nElija una opcion: ";
			std::cin >> strOpcion;
		} while (!validarOpcionMercaderia(strOpcion));

		if (cancelar) { return false; }
		return true;
	} else {
		std::cout << "\nNo hay mercaderias disponibles!\n\nPresione cualquier tecla para cancelar";
		_getch();
		return false;
	}
}

bool SeleccionadorDeMercaderias::hayOpcionesMercaderiasValidas()
{
	if (!esCompra)
	{
		if (!MERCADERIAS.empty())
		{
			for (const Mercaderia& mercaderia : MERCADERIAS)
			{
				if (mercaderia.hayExistencias()) { return true; }
			}
		}
		return false;
	}
	return true;
}

void SeleccionadorDeMercaderias::cargarOpcionesMercaderia()
{
	unsigned int contadorOpciones = 1;

	header((esCompra ? "COMPRA DE MERCADERIAS" : "VENTA DE MERCADERIAS"), 1);
	for (const Mercaderia& mercaderia : MERCADERIAS)
	{
		if (esCompra || mercaderia.hayExistencias())
		{
			std::cout << "\n" << contadorOpciones << ". " << mercaderia.getNombre();
			mercaderiasDisponibles.push_back((Mercaderia*)&mercaderia);
			contadorOpciones++;
		}
	}
	if (esCompra)
	{
		std::cout << "\n" << contadorOpciones << ". " << "Nueva mercaderia";
		contadorOpciones++;
	}
	if (PermitirCancelar)
	{
		std::cout << "\n" << contadorOpciones << ". " << "Cancelar";
	}
}

bool SeleccionadorDeMercaderias::validarOpcionMercaderia(std::string strOpcion)
{
	int opcionElegida = validarInt(strOpcion, 1, mercaderiasDisponibles.size() 
		+ (esCompra ? 1 : 0) + (PermitirCancelar ? 1 : 0));

	if (opcionElegida == 0)
	{
		/// valor no valido
		std::cout << "\n\nValor ingresado no valido, intentelo nuevamente.";
		_getch();
		return false;
	}

	if (opcionCrearMercaderiaElegida(opcionElegida))
	{
		std::string strNuevaMercaderia;
		do
		{
			header("NUEVA MERCADERIA", 2);
			std::cout << "Ingrese el nombre de la nueva mercaderia: ";
			std::getline(std::cin, strNuevaMercaderia);
		} while (!validarStr(strNuevaMercaderia));

		MERCADERIAS.push_back(Mercaderia(strNuevaMercaderia));
		mercaderiaElegida = &(MERCADERIAS.back());
	} else if(opcionCancelarElegida(opcionElegida)) {
		cancelar = true;
		return true;
	} else {
		mercaderiaElegida = mercaderiasDisponibles[opcionElegida - 1];
		return true;
	}
}

bool SeleccionadorDeMercaderias::opcionCrearMercaderiaElegida(int opcionElegida)
{
	return (esCompra ? opcionElegida == mercaderiasDisponibles.size() + 1 : false);
}
bool SeleccionadorDeMercaderias::opcionCancelarElegida(int opcionElegida)
{
	return (PermitirCancelar ? opcionElegida == mercaderiasDisponibles.size() + (esCompra ? 1 : 0) + 1 : false);
}

void SeleccionadorDeMercaderias::elegirPrecioCompra()
{
	std::string strOpcionPrecio;
	do
	{
		listarPreciosMercaderia();

		std::cout << "\n\nIngrese el precio de compra de la mercaderia: $";
		std::cin >> strOpcionPrecio;
	} while (!validarOpcionPrecioCompra(strOpcionPrecio));
}

void SeleccionadorDeMercaderias::listarPreciosMercaderia()
{
	header(mercaderiaElegida->getNombre() += ": PRECIOS UNITARIOS", 1);
	for (const ExistenciasPrecioMercaderia& registro : mercaderiaElegida->getExistencias())
	{
		std::cout << "\n- $" << registro.precio << " (" << registro.existencias << " existencia/s)";
	}
	if (!mercaderiaElegida->hayExistencias()) { std::cout << "\nTodavia no hay precios unitarios!"; }
}

bool SeleccionadorDeMercaderias::validarOpcionPrecioCompra(std::string strOpcionPrecio)
{
	precioCompraElegido = validarInt(strOpcionPrecio, 1);
	if (precioCompraElegido == 0)
	{
		/// valor no valido
		std::cout << "\n\nValor ingresado no valido, intentelo nuevamente.";
		_getch();
		return false;
	}
	return true;
}

void SeleccionadorDeMercaderias::elegirCantidadCompra()
{
	std::string strOpcionCantidad;
	do
	{
		header(mercaderiaElegida->getNombre() += std::string(" ($") += std::to_string(precioCompraElegido) += " c/u)", 2);
		std::cout << "Existencias: " << (mercaderiaElegida->getExistenciasPrecio(precioCompraElegido)) << " unidades";
		std::cout << "\n\nElija la cantidad de mercaderia que se compra: ";
		std::cin >> strOpcionCantidad;

	} while (!validarCantidadCompra(strOpcionCantidad));
}

bool SeleccionadorDeMercaderias::validarCantidadCompra(std::string strOpcionCantidad)
{
	cantidadElegida = validarInt(strOpcionCantidad, 1);
	if (cantidadElegida == 0)
	{
		/// valor no valido
		std::cout << "\n\nValor ingresado no valido, intentelo nuevamente.";
		_getch();
		return false;
	} else {
		totalGastadoCompra = mercaderiaElegida->registrarCompra(fecha, precioCompraElegido, cantidadElegida);
		return true;
	}
}

void SeleccionadorDeMercaderias::elegirCantidadVenta()
{
	std::string strOpcionCantidad;
	do
	{
		listarPreciosMercaderia();
		std::cout << "\n\nIngrese la cantidad de mercaderia que se vende: ";
		std::cin >> strOpcionCantidad;
	} while (!validarCantidadVenta(strOpcionCantidad));
}

bool SeleccionadorDeMercaderias::validarCantidadVenta(std::string strOpcionCantidad)
{
	cantidadElegida = validarInt(strOpcionCantidad, 1, mercaderiaElegida->getExistenciasTotales(), mercaderiaElegida->getExistenciasTotales(), mercaderiaElegida->getExistenciasTotales());
	if (cantidadElegida == 0)
	{
		/// valor no valido
		std::cout << "\n\nValor ingresado no valido, intentelo nuevamente.";
		_getch();
		return false;
	} else {
		totalPerdidoVenta = mercaderiaElegida->registarVenta(fecha, cantidadElegida);
		return true;
	}
}

void SeleccionadorDeMercaderias::elegirPrecioVenta()
{
	std::string strOpcionPrecio;
	do
	{
		header(mercaderiaElegida->getNombre() += (std::string)" (" += std::to_string(cantidadElegida) += " unidades vendida/s", 2);
		std::cout << "Elija el precio de venta de la/s mercaderia/s vendida/s: $";
		std::cin >> strOpcionPrecio;
	} while (!validarPrecioVenta(strOpcionPrecio));
}

bool SeleccionadorDeMercaderias::validarPrecioVenta(std::string strOpcionPrecio)
{
	int precioVentaElegido = validarInt(strOpcionPrecio, 1);
	if (precioVentaElegido == 0)
	{
		std::cout << "\n\nValor ingresado no valido, intentelo nuevamente.";
		_getch();
		return false;
	}
	totalGanadoVenta = precioVentaElegido * cantidadElegida;
	return true;
}

Mercaderia* SeleccionadorDeMercaderias::getMercaderia() { return mercaderiaElegida; }
unsigned int SeleccionadorDeMercaderias::getTotalGastadoCompra() { return totalGastadoCompra; }
unsigned int SeleccionadorDeMercaderias::getTotalPerdidoVenta() { return totalPerdidoVenta; }
unsigned int SeleccionadorDeMercaderias::getTotalGanadoVenta() { return totalGanadoVenta; }
unsigned int SeleccionadorDeMercaderias::getCantidad() { return cantidadElegida; }