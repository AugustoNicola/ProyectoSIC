#include "SeleccionadorDeMercaderias.h"

using namespace conmanip;

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
		} else if (Tipo == TipoOperacion::REINTEGRO) {
			elegirPrecioCompra();
			elegirCantidadCompra();
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
			cargarMercaderias();

			std::cout << settextcolor(colorBase) << "\n\nElija una opcion: ";
			std::cin >> settextcolor(colorInput) >> strOpcion;
		} while (!validarMercaderia(strOpcion));

		if (cancelar) { return false; }
		return true;
	} else {
		std::cout << settextcolor(colorError) << "\nNo hay mercaderias disponibles!\n\nPresione cualquier tecla para cancelar";
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
	case SeleccionadorDeMercaderias::TipoOperacion::REINTEGRO:
		header("REINTEGRO DE MERCADERIAS", 1);
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
		std::cout << settextcolor(console_text_colors::light_green) 
			<< "\n" << contadorOpciones << ". " << "Nueva mercaderia";
		contadorOpciones++;
	}
	if (PermitirCancelar)
	{
		std::cout << settextcolor(colorError)
			<< "\n" << contadorOpciones << ". " << "Cancelar";
	}
}

bool SeleccionadorDeMercaderias::validarMercaderia(std::string strOpcion)
{
	int opcionElegida = validarInt(strOpcion, 1, mercaderiasDisponibles.size() 
		+ (Tipo == TipoOperacion::COMPRA ? 1 : 0) + (PermitirCancelar ? 1 : 0));

	if (opcionElegida == 0)
	{
		error();
		return false;
	}

	if (opcionCrearMercaderiaElegida(opcionElegida))
	{
		std::string strNuevaMercaderia;
		do
		{
			header("NUEVA MERCADERIA", 2);
			std::cout << "Ingrese el nombre de la nueva mercaderia: ";
			std::cin >> settextcolor(colorInput);
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

		std::cout << settextcolor(colorBase)
			<< (Tipo == TipoOperacion::COMPRA ? "\n\nIngrese el precio de compra de la mercaderia: $" 
			: "\n\nIngrese el precio unitario de las mercaderias que se nos devuelve: $");
		std::cin >> settextcolor(colorInput) >> strPrecio;
	} while (!validarPrecioCompra(strPrecio));
}

void SeleccionadorDeMercaderias::listarPreciosMercaderia()
{
	header(mercaderiaElegida->getNombre() += ": PRECIOS UNITARIOS", 1);
	for (const ExistenciasPrecioMercaderia& registro : mercaderiaElegida->getExistencias())
	{
		std::cout << settextcolor(colorBase) << "\n-" 
			<< settextcolor(console_text_colors::light_magenta) << "$" << registro.precio 
			<< settextcolor(colorBase) << " (" 
			<< settextcolor(console_text_colors::light_yellow) << registro.existencias
			<< settextcolor(colorBase) << " existencia/s)";
	}
	if (!mercaderiaElegida->hayExistencias())
	{
		std::cout << settextcolor(colorError) << "\nTodavia no hay precios unitarios!";
	}
}

bool SeleccionadorDeMercaderias::validarPrecioCompra(std::string strPrecio)
{
	precioCompra = validarInt(strPrecio, 1);
	if (precioCompra == 0)
	{
		error();
		return false;
	}
	return true;
}

void SeleccionadorDeMercaderias::elegirCantidadCompra()
{
	std::string strCantidad;
	do
	{
		system("CLS");
		std::cout << settextcolor(colorBase) << "=============== " << mercaderiaElegida->getNombre() << " ("
			<< settextcolor(colorDatos) << formatearDinero(precioCompra)
			<< settextcolor(colorBase) << " c/u) ===============\n\n";

		std::cout << "Existencias: "
			<< settextcolor(colorDatos) << (mercaderiaElegida->getExistenciasPrecio(precioCompra))
			<< settextcolor(colorBase) << " unidades";

		std::cout << "\n\nElija la cantidad de mercaderia que " 
			<< (Tipo == TipoOperacion::COMPRA ? "se compra: " : "se nos devuelve: $");
		std::cin >> settextcolor(colorInput) >> strCantidad;

	} while (!validarCantidadCompra(strCantidad));
}

bool SeleccionadorDeMercaderias::validarCantidadCompra(std::string strCantidad)
{
	cantidadMercaderias = validarInt(strCantidad, 1);
	if (cantidadMercaderias == 0)
	{
		error();
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
		std::cout << settextcolor(colorBase) << "\n\nIngrese la cantidad de mercaderia que se vende: ";
		std::cin >> settextcolor(colorInput) >> strCantidad;
	} while (!validarCantidadVenta(strCantidad));
}

bool SeleccionadorDeMercaderias::validarCantidadVenta(std::string strCantidad)
{
	cantidadMercaderias = validarInt(strCantidad, 1, mercaderiaElegida->getExistenciasTotales(), mercaderiaElegida->getExistenciasTotales(), mercaderiaElegida->getExistenciasTotales());
	if (cantidadMercaderias == 0)
	{
		error();
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
		system("CLS");
		std::cout << settextcolor(colorBase) << "===============" << mercaderiaElegida->getNombre() << " ("
			<< settextcolor(colorDatos) << cantidadMercaderias
			<< settextcolor(colorBase) << " unidad/es vendida/s ===============\n\n";
		
		std::cout << (Tipo == TipoOperacion::COMPRA ? "Elija el precio de venta de la/s mercaderia/s vendida/s: $" 
			: "Elija el precio al que se vendieron la/s mercaderia/s devuelta/s: $");
		std::cin >> settextcolor(colorInput) >> strPrecio;
	} while (!validarPrecioVenta(strPrecio));
}

bool SeleccionadorDeMercaderias::validarPrecioVenta(std::string strPrecio)
{
	precioVenta = validarInt(strPrecio, 1);
	if (precioVenta == 0)
	{
		error();
		return false;
	}
	totalGanadoVenta = precioVenta * cantidadMercaderias;
	return true;
}

void SeleccionadorDeMercaderias::elegirPrecioDevolucion()
{
	std::string strPrecio;
	do
	{
		listarPreciosMercaderia();

		std::cout << settextcolor(colorBase) <<"\n\nIngrese el precio de las mercaderias devueltas: $";
		std::cin >> settextcolor(colorInput) >> strPrecio;
	} while (!validarPrecioDevolucion(strPrecio));
}

bool SeleccionadorDeMercaderias::validarPrecioDevolucion(std::string strPrecio)
{
	precioDevolucion = validarInt(strPrecio, 1);
	if (precioDevolucion == 0 || mercaderiaElegida->getExistenciasPrecio(precioDevolucion) == 0)
	{
		error();
		return false;
	}
	return true;
}

void SeleccionadorDeMercaderias::elegirCantidadDevolucion()
{
	std::string strCantidad;
	do
	{
		system("CLS");
		std::cout << settextcolor(colorBase) << "=============== " << mercaderiaElegida->getNombre() << " ("
			<< settextcolor(colorDatos) << formatearDinero(precioDevolucion)
			<< settextcolor(colorBase) << " c/u) ===============\n\n" << "Existencias: "
			<< settextcolor(colorDatos) << (mercaderiaElegida->getExistenciasPrecio(precioDevolucion))
			<< settextcolor(colorBase) << " unidades" << "\n\nElija la cantidad de mercaderia que se devuelve: ";
		std::cin >> settextcolor(colorInput) >> strCantidad;

	} while (!validarCantidadDevolucion(strCantidad));
}

bool SeleccionadorDeMercaderias::validarCantidadDevolucion(std::string strCantidad)
{
	cantidadMercaderias = validarInt(strCantidad, 1, mercaderiaElegida->getExistenciasPrecio(precioDevolucion), {}, mercaderiaElegida->getExistenciasPrecio(precioDevolucion));
	if (cantidadMercaderias == 0)
	{
		error();
		return false;
	}
	else {
		totalGanadoDevolucion = mercaderiaElegida->registrarCompra(fecha, precioDevolucion, -cantidadMercaderias) * -1;
		return true;
	}
}

Mercaderia* SeleccionadorDeMercaderias::getMercaderia() const { return mercaderiaElegida; }
unsigned int SeleccionadorDeMercaderias::getTotalGastadoCompra() const { return totalGastadoCompra; }
unsigned int SeleccionadorDeMercaderias::getTotalPerdidoVenta() const { return totalPerdidoVenta; }
unsigned int SeleccionadorDeMercaderias::getTotalGanadoVenta() const { return totalGanadoVenta; }
unsigned int SeleccionadorDeMercaderias::getTotalGanadoDevolucion() const { return totalGanadoDevolucion; }
unsigned int SeleccionadorDeMercaderias::getCantidad() const { return cantidadMercaderias; }