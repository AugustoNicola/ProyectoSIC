#include "pch.h"
#include "CppUnitTest.h"

#include "AumentadorPartida.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AumentadorPartida_Test
{
	TEST_CLASS(Prueba)
	{
	public:
		AumentadorPartida aumentador;
		Prueba() : aumentador(Cuenta::TipoCuenta::ACTIVO_OPERATIVO, ModoAumento::Debe, "Mensaje", {}) {}
		TEST_METHOD(Basico)
		{
			Assert::IsNotNull(&aumentador);
		}
	};
}