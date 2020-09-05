#include "SeleccionadorDeMercaderias.h"

SeleccionadorDeMercaderias::SeleccionadorDeMercaderias(bool _permitirCancelar, TipoOperacion _tipo)
	: Tipo(_tipo), PermitirCancelar(_permitirCancelar)
{

	if (intentarElegirMercaderia())
	{
		if (Tipo == TipoOperacion::COMPRA)
		{
			elegirPrecioCompra();
			elegirCantidadCompra();
		} else if (Tipo == TipoOperacion::VENTA) {
			elegirCantidadVenta();
			elegirPrecioVenta();
		} else if (Tipo == TipoOperacion::DEVOLUCION) {
			elegirPrecioDevolucion();
			elegirCantidadDevolucion();
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
			cargarMercaderias();

			std::cout << "\n\nElija una opcion: ";
			std::cin >> strOpcion;
		} while (!validarMercaderia(strOpcion));

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
	if (Tipo != TipoOperacion::COMPRA)
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

void SeleccionadorDeMercaderias::cargarMercaderias()
{
	unsigned int contadorOpciones = 1;

	switch (Tipo)
	{
	case SeleccionadorDeMercaderias::TipoOperacion::COMPRA:
		header("COMPRA DE MERCADERIAS", 1);
		break;
	case SeleccionadorDeMercaderias::TipoOperacion::VENTA:
		header("VENTA DE MERCADERIAS", 1);
		break;
	case SeleccionadorDeMercaderias::TipoOperacion::DEVOLUCION:
		header("DEVOLUCION DE MERCADERIAS", 1);
		break;
	}
	for (const Mercaderia& mercaderia : MERCADERIAS)
	{
		if (Tipo == TipoOperacion::COMPRA || mercaderia.hayExistencias())
		{
			std::cout << "\n" << contadorOpciones << ". " << mercaderia.getNombre();
			mercaderiasDisponibles.push_back((Mercaderia*)&mercaderia);
			contadorOpciones++;
		}
	}
	if (Tipo == TipoOperacion::COMPRA)
	{
		std::cout << "\n" << contadorOpciones << ". " << "Nueva mercaderia";
		contadorOpciones++;
	}
	if (PermitirCancelar)
	{
		std::cout << "\n" << contadorOpciones << ". " << "Cancelar";
	}
}

bool SeleccionadorDeMercaderias::validarMercaderia(std::string strOpcion)
{
	int opcionElegida = validarInt(strOpcion, 1, mercaderiasDisponibles.size() 
		+ (Tipo == TipoOperacion::COMPRA ? 1 : 0) + (PermitirCancelar ? 1 : 0));

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
	return (Tipo == TipoOperacion::COMPRA ? opcionElegida == mercaderiasDisponibles.size() + 1 : false);
}
bool SeleccionadorDeMercaderias::opcionCancelarElegida(int opcionElegida)
{
	return (PermitirCancelar ? opcionElegida == mercaderiasDisponibles.size() + (Tipo == TipoOperacion::COMPRA ? 1 : 0) + 1 : false);
}

void SeleccionadorDeMercaderias::elegirPrecioCompra()
{
	std::string strPrecio;
	do
	{
		listarPreciosMercaderia();

		std::cout << "\n\nIngrese el precio de compra de la mercaderia: $";
		std::cin >> strPrecio;
	} while (!validarPrecioCompra(strPrecio));
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

bool SeleccionadorDeMercaderias::validarPrecioCompra(std::string strPrecio)
{
	precioCompra = validarInt(strPrecio, 1);
	if (precioCompra == 0)
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
	std::string strCantidad;
	do
	{
		header(mercaderiaElegida->getNombre() += std::string(" ($") += std::to_string(precioCompra) += " c/u)", 2);
		std::cout << "Existencias: " << (mercaderiaElegida->getExistenciasPrecio(precioCompra)) << " unidades";
		std::cout << "\n\nElija la cantidad de mercaderia que se compra: ";
		std::cin >> strCantidad;

	} while (!validarCantidadCompra(strCantidad));
}

bool SeleccionadorDeMercaderias::validarCantidadCompra(std::string strCantidad)
{
	cantidadMercaderias = validarInt(strCantidad, 1);
	if (cantidadMercaderias == 0)
	{
		/// valor no valido
		std::cout << "\n\nValor ingresado no valido, intentelo nuevamente.";
		_getch();
		return false;
	} else {
		totalGastadoCompra = mercaderiaElegida->registrarCompra(fecha, precioCompra, cantidadMercaderias);
		return true;
	}
}

void SeleccionadorDeMercaderias::elegirCantidadVenta()
{
	std::string strCantidad;
	do
	{
		listarPreciosMercaderia();
		std::cout << "\n\nIngrese la cantidad de mercaderia que se vende: ";
		std::cin >> strCantidad;
	} while (!validarCantidadVenta(strCantidad));
}

bool SeleccionadorDeMercaderias::validarCantidadVenta(std::string strCantidad)
{
	cantidadMercaderias = validarInt(strCantidad, 1, mercaderiaElegida->getExistenciasTotales(), mercaderiaElegida->getExistenciasTotales(), mercaderiaElegida->getExistenciasTotales());
	if (cantidadMercaderias == 0)
	{
		/// valor no valido
		std::cout << "\n\nValor ingresado no valido, intentelo nuevamente.";
		_getch();
		return false;
	} else {
		totalPerdidoVenta = mercaderiaElegida->registarVenta(fecha, cantidadMercaderias);
		return true;
	}
}

void SeleccionadorDeMercaderias::elegirPrecioVenta()
{
	std::string strPrecio;
	do
	{
		header(mercaderiaElegida->getNombre() += (std::string)" (" += std::to_string(cantidadMercaderias) += " unidades vendida/s", 2);
		std::cout << "Elija el precio de venta de la/s mercaderia/s vendida/s: $";
		std::cin >> strPrecio;
	} while (!validarPrecioVenta(strPrecio));
}

bool SeleccionadorDeMercaderias::validarPrecioVenta(std::string strPrecio)
{
	int precioVentaElegido = validarInt(strPrecio, 1);
	if (precioVentaElegido == 0)
	{
		std::cout << "\n\nValor ingresado no valido, intentelo nuevamente.";
		_getch();
		return false;
	}
	totalGanadoVenta = precioVentaElegido * cantidadMercaderias;
	return true;
}

void SeleccionadorDeMercaderias::elegirPrecioDevolucion()
{
	std::string strPrecio;
	do
	{
		listarPreciosMercaderia();

		std::cout << "\n\nIngrese el precio de las mercaderias devueltas: $";
		std::cin >> strPrecio;
	} while (!validarPrecioDevolucion(strPrecio));
}

bool SeleccionadorDeMercaderias::validarPrecioDevolucion(std::string strPrecio)
{
	precioDevolucion = validarInt(strPrecio, 1);
	if (precioDevolucion == 0 || mercaderiaElegida->getExistenciasPrecio(precioDevolucion) == 0)
	{
		/// valor no valido
		std::cout << "\n\nValor ingresado no valido, intentelo nuevamente.";
		_getch();
		return false;
	}
	return true;
}

void SeleccionadorDeMercaderias::elegirCantidadDevolucion()
{
	std::string strCantidad;
	do
	{
		header(mercaderiaElegida->getNombre() += std::string(" ($") += std::to_string(precioDevolucion) += " c/u)", 2);
		std::cout << "Existencias: " << (mercaderiaElegida->getExistenciasPrecio(precioDevolucion)) << " unidades";
		std::cout << "\n\nElija la cantidad de mercaderia que se devuelve: ";
		std::cin >> strCantidad;

	} while (!validarCantidadDevolucion(strCantidad));
}

bool SeleccionadorDeMercaderias::validarCantidadDevolucion(std::string strCantidad)
{
	cantidadMercaderias = validarInt(strCantidad, 1, mercaderiaElegida->getExistenciasPrecio(precioDevolucion), {}, mercaderiaElegida->getExistenciasPrecio(precioDevolucion));
	if (cantidadMercaderias == 0)
	{
		/// valor no valido
		std::cout << "\n\nValor ingresado no valido, intentelo nuevamente.";
		_getch();
		return false;
	}
	else {
		totalGanadoDevolucion = mercaderiaElegida->registrarCompra(fecha, precioDevolucion, -cantidadMercaderias) * -1;
		return true;
	}
}

Mercaderia* SeleccionadorDeMercaderias::getMercaderia() { return mercaderiaElegida; }
unsigned int SeleccionadorDeMercaderias::getTotalGastadoCompra() { return totalGastadoCompra; }
unsigned int SeleccionadorDeMercaderias::getTotalPerdidoVenta() { return totalPerdidoVenta; }
unsigned int SeleccionadorDeMercaderias::getTotalGanadoVenta() { return totalGanadoVenta; }
unsigned int SeleccionadorDeMercaderias::getTotalGanadoDevolucion() { return totalGanadoDevolucion; }
unsigned int SeleccionadorDeMercaderias::getCantidad() { return cantidadMercaderias; }