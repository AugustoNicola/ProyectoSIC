#include "pch.h"
#include "CppUnitTest.h"

#include <vector>
#include "../clases/src/clases.h"
#include "../ProyectoSIC/src/Main.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PruebasUnitarias
{
	TEST_CLASS(Clase)
	{
	public:

		std::vector<DiaOperaciones> DIAS;
		int i;

		Clase()
		{
			DiaOperaciones dia = DiaOperaciones("01/01");
			Operacion oper = Operacion("Apertura");
			oper.nuevaLinea(buscarCuenta("Caja"), 1000);
			oper.nuevaLinea(buscarCuenta("Acciones"), 1000);
			oper.nuevaLinea(buscarCuenta("Proovedores"), -100);
			oper.nuevaLinea(buscarCuenta("Capital"), -1900);
			dia.nuevaOperacion(oper);
			DIAS.push_back(dia);


			dia = DiaOperaciones("02/01");
			oper = Operacion("fc. original C N3");
			oper.nuevaLinea(buscarCuenta("Caja"), -100);
			oper.nuevaLinea(buscarCuenta("Proovedores"), 100);
			dia.nuevaOperacion(oper);
			DIAS.push_back(dia);


			dia = DiaOperaciones("04/01");
			oper = Operacion("fc. original C N6");
			oper.nuevaLinea(buscarCuenta("Mercaderias"), 200);
			oper.nuevaLinea(buscarCuenta("Caja"), -200);
			dia.nuevaOperacion(oper);

			oper = Operacion("fc. original C N7");
			oper.nuevaLinea(buscarCuenta("Mercaderias"), 200);
			oper.nuevaLinea(buscarCuenta("Acciones"), -200);
			dia.nuevaOperacion(oper);

			DIAS.push_back(dia);
			i = 1;
		}
		TEST_METHOD(Basica)
		{
			Assert::AreEqual((std::string)"01/01", DIAS[0].fecha);
		}
	};
}
