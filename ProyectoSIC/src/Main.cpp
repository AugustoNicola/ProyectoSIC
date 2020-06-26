#include <iostream>
#include <stdlib.h>
#include <vector>

#include "clases.h"
#include "presets.h" //importa los vectores de CUENTAS y OPCIONES
std::vector<std::vector<Operacion*>> DIAS = {}; //lista de todas las operaciones que deben exportarse, en orden cronologico
std::vector<Mercaderia*> MERCADERIAS = {}; //lista de todos los tipos de mercaderias usados
std::vector<MesIVA> IVA = {}; //registro mensual del IVA


int main()
{

	
	
	bool loop = true; //Controla la ejecucion del programa
	int op;
	// -------- LOOP PRINCIPAL --------
	while (loop)
	{
		system("CLS");
		std::cout << "=============== PROYECTO SIC ===============\n";
		std::cout << "Seleccione una opcion:\n";
		
		/* imprime las opciones y espera la respuesta */
		for (int i = 0; i < OPCIONES.size(); i++)
		{std::cout << i+1 << ". " << OPCIONES[i].nombre << "\n";}
		std::cin >> op;


		/* verificacion de integer dentro de limites */
		if (op < OPCIONES.size() + 1)
		{
			OPCIONES[op - 1].pFuncion();
			loop = false;
		} else {
			std::cin.clear();
			std::cin.ignore(256, '\n');
			std::cout << "\n\nEl valor introducido no es valido (PRESIONE CUALQUIER TECLA PARA CONTINUAR)" << std::endl;
			std::getchar();
		}
		
	};
	
	return 0;
}