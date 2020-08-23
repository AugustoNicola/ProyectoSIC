#include "SeleccionadorDeMercaderias.h"

SeleccionadorDeMercaderias::SeleccionadorDeMercaderias(bool _esCompra)
	: esCompra(_esCompra)
{
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
	std::vector <Mercaderia*> mercaderiasDisponibles = {};

	system("CLS");
	std::cout << "=============== " << ((esCompra) ? "COMPRA" : "VENTA") << " DE MERCADERIAS ===============\n";
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
	}
}

bool SeleccionadorDeMercaderias::validarOpcionMercaderia(std::string strOpcion)
{
	int opcionElegida = validarInt(strOpcion, {}, {}, 1, mercaderiasDisponibles.size() + (esCompra ? 1 : 0));

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
		system("CLS");
		std::cout << "=============== NUEVA MERCADERIA ===============";
		std::cout << "\n\nIngrese el nombre de la nueva mercaderia: ";
		std::cin >> strNuevaMercaderia;

		MERCADERIAS.push_back(Mercaderia(strNuevaMercaderia));
		mercaderiaElegida = &(MERCADERIAS.back());
	} else {
		mercaderiaElegida = mercaderiasDisponibles[opcionElegida - 1];
	}
	return true;
}

bool SeleccionadorDeMercaderias::opcionCrearMercaderiaElegida(int opcionElegida)
{
	return opcionElegida == mercaderiasDisponibles.size() + 1;
}

void SeleccionadorDeMercaderias::elegirPrecioCompra()
{
	std::string strOpcionPrecio;
	do
	{
		listarPreciosMercaderia();

		std::cout << "\n\Ingrese el precio de compra de la mercaderia: $";
		std::cin >> strOpcionPrecio;
	} while (!validarOpcionPrecioCompra(strOpcionPrecio));
}

void SeleccionadorDeMercaderias::listarPreciosMercaderia()
{
	system("CLS");
	std::cout << "=============== " << mercaderiaElegida->getNombre() << ": PRECIOS UNITARIOS ===============\n";
	for (const ExistenciasPrecioMercaderia& registro : mercaderiaElegida->getExistencias())
	{
		std::cout << "\n- $" << registro.precio << " (" << registro.existencias << " existencia/s)";
	}
}

bool SeleccionadorDeMercaderias::validarOpcionPrecioCompra(std::string strOpcionPrecio)
{
	precioCompraElegido = validarInt(strOpcionPrecio, {}, {}, 1);
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
		system("CLS");
		std::cout << "=============== " << mercaderiaElegida->getNombre() << " ($" << precioCompraElegido << " c/u) ===============";
		std::cout << "\nExistencias: " << (mercaderiaElegida->getExistenciasPrecio(precioCompraElegido)) << " unidades";
		std::cout << "\n\nElija la cantidad de mercaderia que se compra: ";
		std::cin >> strOpcionCantidad;

	} while (!validarCantidadCompra(strOpcionCantidad));
}

bool SeleccionadorDeMercaderias::validarCantidadCompra(std::string strOpcionCantidad)
{
	cantidadElegida = validarInt(strOpcionCantidad, {}, {}, 1);
	if (cantidadElegida == 0)
	{
		/// valor no valido
		std::cout << "\n\nValor ingresado no valido, intentelo nuevamente.";
		_getch();
		return false;
	} else {
		mercaderiaElegida->intentarCompra(fecha, precioCompraElegido, cantidadElegida);
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
	cantidadElegida = validarInt(strOpcionCantidad, {}, mercaderiaElegida->getExistenciasTotales(), 1, mercaderiaElegida->getExistenciasTotales());
	if (cantidadElegida == 0)
	{
		/// valor no valido
		std::cout << "\n\nValor ingresado no valido, intentelo nuevamente.";
		_getch();
		return false;
	} else {
		mercaderiaElegida->intentarVenta(fecha, cantidadElegida);
		return true;
	}
}

void SeleccionadorDeMercaderias::elegirPrecioVenta()
{
	std::string strOpcionPrecio;
	do
	{
		system("CLS");
		std::cout << "=============== " << mercaderiaElegida->getNombre() << " (" << cantidadElegida << " unidades vendida/s) ===============";
		std::cout << "\n\nElija el precio de venta de la/s mercaderia/s vendida/s: $";
		std::cin >> strOpcionPrecio;
	} while (!validarPrecioVenta(strOpcionPrecio));
}

bool SeleccionadorDeMercaderias::validarPrecioVenta(std::string strOpcionPrecio)
{
	precioVentaElegido = validarInt(strOpcionPrecio, {}, {}, 1);
	if (precioVentaElegido == 0)
	{
		std::cout << "\n\nValor ingresado no valido, intentelo nuevamente.";
		_getch();
		return false;
	}
	return true;
}

Mercaderia* SeleccionadorDeMercaderias::getMercaderia() { return mercaderiaElegida; }
unsigned int SeleccionadorDeMercaderias::getPrecioCompra() {return precioCompraElegido;}
unsigned int SeleccionadorDeMercaderias::getPrecioVenta() {return precioVentaElegido;}
unsigned int SeleccionadorDeMercaderias::getCantidad() {return cantidadElegida;}