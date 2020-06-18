#include <iostream>
#include "clases.h"
void func() {};
Opcion* opciones[1] = { &Opcion("Operacion Convencional", &func) };

int main()
{
	bool loop = true; //Controla la ejecucion del programa
	int op;

	while (loop)
	{
		std::cout << "=============== PROYECTO SIC ===============\n";
		std::cout << "Seleccione una opción:\n";
		for (int i = 0; i < 1; i++)
		{
			std::cout << i++ << ". " << opciones[i]->nombre << "\n";
		}
		
		std::cout << "2. Salir\n";
		std::cin >> op;
		switch (op)
		{
		case 1:

			break;
		default:
			break;
		}
	};

	return 0;
}