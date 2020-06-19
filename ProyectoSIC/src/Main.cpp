#include <iostream>
#include <vector>
#include "clases.h" //header con todas las clases que requiere el programa

//================== LISTAS GLOBALES IMPORTANTES ==================//
std::vector<std::vector<Operacion*>> DIAS = {}; //lista de todas las operaciones que deben exportarse, en orden cronologico
std::vector<Opcion*> OPCIONES = {}; //lista de diferentes templates disponibles
std::vector<Cuenta*> CUENTAS = {}; //lista de todas las cuentas que maneja el programa
std::vector<Mercaderia*> MERCADERIAS = {}; //lista de todos los tipos de mercaderias usados
std::vector<int> IVA = {}; //registro mensual del saldo del iva (positivo para A Favor, negativo para A Pagar

int main()
{
	bool loop = true; //Controla la ejecucion del programa
	int op;

	while (loop)
	{
		std::cout << "=============== PROYECTO SIC ===============\n";
		std::cout << "Seleccione una opcion:\n";
		
		for (int i = 0; i < opciones.size(); i++)
		{std::cout << i+1 << ". " << opciones[i]->nombre << "\n";}
		std::cin >> op;
		if (op < opciones.size()+1)
		{
			opciones[op-1]->pFuncion();
		}
	};

	return 0;
}