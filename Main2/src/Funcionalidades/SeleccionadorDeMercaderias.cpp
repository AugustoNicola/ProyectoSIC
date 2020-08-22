#include "SeleccionadorDeMercaderias.h"

SeleccionadorDeMercaderias::SeleccionadorDeMercaderias(bool _esCompra)
	: esCompra(_esCompra)
{
	if (intentarElegirMercaderia())
	{
		if (intentarElegirPrecio())
		{
			if (intentarElegirCantidad())
			{
				cargarRespuesta();
			}
		}
	}
	
};

bool SeleccionadorDeMercaderias::intentarElegirMercaderia()
{
	if (hayOpcionesDeMercaderiasValidas())
	{
		//hasta que se seleccione una opcion
		std::string strOpcion;
		do
		{
			cargarOpcionesDeMercaderia();

			std::cout << "\n\nElija una opcion: ";
			std::cin >> strOpcion;
		} while (!validarOpcionDeMercaderia(strOpcion));
		return true;
	} else {
		std::cout << "\nNo hay mercaderias disponibles!\n\nPresione cualquier tecla para cancelar";
		_getch();
		return false;
	}
}

bool SeleccionadorDeMercaderias::hayOpcionesDeMercaderiasValidas()
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

void SeleccionadorDeMercaderias::cargarOpcionesDeMercaderia()
{
	unsigned int contadorDeOpciones = 1;
	std::vector <Mercaderia*> mercaderiasDisponibles = {};

	system("CLS");
	std::cout << "=============== " << ((esCompra) ? "COMPRA" : "VENTA") << " DE MERCADERIAS ===============\n";
	for (const Mercaderia& mercaderia : MERCADERIAS)
	{
		if (esCompra || mercaderia.hayExistencias())
		{
			std::cout << "\n" << contadorDeOpciones << ". " << mercaderia.getNombre();
			mercaderiasDisponibles.push_back((Mercaderia*)&mercaderia);
			contadorDeOpciones++;
		}
	}
	if (esCompra)
	{
		std::cout << "\n" << contadorDeOpciones << ". " << "Nueva mercaderia";
	}
}

bool SeleccionadorDeMercaderias::validarOpcionDeMercaderia(std::string strOpcion)
{
	int opcionElegida;
	opcionElegida = validarInt(strOpcion, {}, {}, 1, mercaderiasDisponibles.size() + (esCompra ? 1 : 0));

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



struct operMercaderia
{
	int cantidad; int precioUnitario; int precioVenta;
};
operMercaderia seleccionarMercaderia(bool compra)
{
	/* crear/encontrar mercaderia */
	

	/* ----- Bucle validacion precio ----- */
	std::vector<PrecioMerca*> posPrecio;
	do
	{
		posPrecio = {}; //vector que asocia cada posicion con su precio-mercaderia
		cont = 1; //contador para el output
		offSet = 0;

		system("CLS");
		std::cout << "=============== " << mercaElegida->getNombre() << ": PRECIOS UNITARIOS ===============\n";

		/* Iteracion de precios */
		for (unsigned int i = 0; i < mercaElegida->precios.size(); i++)
		{
			/* no es necesario verificar que al menos un precio tenga existencias, esta asegurado */

			// si es venta, comprueba que haya existencias para filtrar opciones
			if (compra || mercaElegida->precios[i].hayExistencias())
			{
				std::cout << "\n" << cont << ". " << mercaElegida->getNombre() << " ($" << mercaElegida->precios[i].precio << ")";
				posPrecio.push_back((PrecioMerca*)&mercaElegida->precios[i]); //guarda lugar de memoria del precio-mercaderia actual en vector
				cont++;
			}
		}

		if (compra)
		{
			std::cout << "\n" << cont << ". " << "Nuevo precio unitario";
			offSet = 1;
		}

		/* Input */

		std::cout << "\n\n" << "Elija una opcion: ";
		std::cin >> opStr;

		/* Validacion/return */
		op = validarInt(opStr, {}, {}, 1, posPrecio.size() + offSet);
		if (op == 0)
		{
			/// valor no valido
			std::cout << "\n\nValor ingresado no valido, intentelo nuevamente.";
			_getch();
		}
	} while (op == 0);

	/* crear/encontrar mercaderia */
	system("CLS");
	PrecioMerca* precioElegido;
	if (op == posPrecio.size() + 1)
	{
		/// crear precio-mercaderia
		do
		{
			std::cout << "=============== " << mercaElegida->getNombre() << ": NUEVO PRECIO UNITARIO ===============";
			std::cout << "\n\nIngrese el nuevo precio de compra: $";
			std::cin >> opStr;

			op = validarInt(opStr, {}, {}, 1);
			if (op == 0)
			{
				std::cout << "\n\nValor ingresado no valido, presione cualquier tecla para intentarlo nuevamente";
				_getch();
				system("CLS");
			}
		} while (op == 0);

		mercaElegida->precios.push_back(PrecioMerca(op)); //crea el nuevo precio-mercaderia
		precioElegido = &(mercaElegida->precios.back()); //guarda el puntero al precio-mercaderia
	}
	else {
		/// encontrar mercaderia
		precioElegido = &(mercaElegida->precios[op - 1]);
	}

	/* en este punto ya tenemos una mercaElegida y un precioElegido!! */
	/* ----- Bucle validacion compra/venta ----- */
	int cantidad = 0; unsigned int precioVenta = 0;
	do
	{
		/* validar cantidad */

		system("CLS");
		std::cout << "=============== " << mercaElegida->getNombre() << " ($" << precioElegido->precio << " c/u) ===============";
		std::cout << "\n\nExistencias: " << ((precioElegido->hayExistencias()) ? precioElegido->dias.back().cantidad : 0) << " unidades"; //para compra, puede ser 0
		std::cout << "\n\nElija la cantidad de mercaderia que se " << ((compra) ? "compra" : "vende") << ": ";
		std::cin >> opStr;
		//si es compra, no hay maximo; si es venta, es la cantidad de existencias actuales (que no puede ser 0)
		op = validarInt(opStr, {}, {}, 1, ((!compra) ? precioElegido->dias.back().cantidad : INT_MAX));
		if (op == 0)
		{
			/// cantidad no valida
			std::cout << "\n\nValor ingresado no valido, presione cualquier tecla para intentarlo nuevamente";
			_getch();
			system("CLS");
		}
		else {
			///cantidad valida!

			cantidad = op; //set cantidad

			if (!compra)
			{
				/// necesidad de validar precio venta
				do
				{
					std::cout << "\n\nIngrese el precio unitario al cual se vende la mercaderia: $";
					std::cin >> opStr;
					op = validarInt(opStr, {}, {}, 1);
					if (op == 0)
					{
						/// precio venta no valido
						std::cout << "\n\nValor ingresado no valido, presione cualquier tecla para intentarlo nuevamente";
						_getch();
					}
					else {
						/// precio venta valido!

						precioVenta = op; // set precioVenta (solo en caso de venta)
					}
				} while (precioVenta == 0);
			}
		}
	} while (cantidad == 0);

	// carga la respuesta del struct
	operMercaderia resp;
	resp.cantidad = cantidad * ((compra) ? 1 : -1); resp.precioUnitario = precioElegido->precio; resp.precioVenta = precioVenta;

	precioElegido->nuevoDiaPrecioMerca(fecha, resp.cantidad); //efectua la modificacion de existencias

	return resp;

}
