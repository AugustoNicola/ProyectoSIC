#include "pch.h"
#include "CppUnitTest.h"

#include "../clases/src/clases.h"
//#include "../ProyectoSIC/src/Main.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

template<> static std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<Cuenta>(const Cuenta& cuenta) {	return L"Cuenta."; }
template<> static std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<Cuenta::TipoCuenta>(const Cuenta::TipoCuenta &tipoCuenta) { return L"TipoCuenta."; }

namespace Clase_Cuenta
{
	TEST_CLASS(Constructor)
	{
	public:
		Cuenta cuenta = Cuenta("MiCuenta", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO);

		TEST_METHOD(Nombre)
		{
			Assert::AreEqual(static_cast<std::string>("MiCuenta"), cuenta.getNombre());
		}
		TEST_METHOD(ModoDebitado)
		{
			Assert::AreEqual(true, cuenta.getModoDebitado());
		}
		TEST_METHOD(Tipo)
		{
			Assert::AreEqual(Cuenta::TipoCuenta::ACTIVO_OPERATIVO, cuenta.getTipo());
		}
		TEST_METHOD(SaldoActual)
		{
			Assert::AreEqual(0, cuenta.getSaldoActual());
		}
		
	};
	
	TEST_CLASS(Metodo_hayDias)
	{
	public:
		Cuenta cuenta;

		Metodo_hayDias() : cuenta("MiCuenta", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO) {}

		TEST_METHOD(SinDias)
		{
			Assert::IsFalse(cuenta.hayDias());
		}
		TEST_METHOD(ConUnDia)
		{
			cuenta.registrarModificacion("01/03", 300);
			Assert::IsTrue(cuenta.hayDias());
		}
		TEST_METHOD(ConMultiplesDias)
		{
			cuenta.registrarModificacion("01/03", 300);
			cuenta.registrarModificacion("03/03", 300);
			Assert::IsTrue(cuenta.hayDias());
		}
	};

	TEST_CLASS(Metodo_registrarModificacion)
	{
	public:
		Cuenta cuenta;
		Metodo_registrarModificacion() : cuenta("MiCuenta", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO) {}

		TEST_METHOD(Suma_UnDia)
		{
			cuenta.registrarModificacion("01/01", 100);
			Assert::IsTrue(cuenta.hayDias());
			Assert::AreEqual(100, cuenta.getSaldoActual());
		}
		TEST_METHOD(Resta_UnDia)
		{
			cuenta.registrarModificacion("01/01", -100);
			Assert::IsTrue(cuenta.hayDias());
			Assert::AreEqual(-100, cuenta.getSaldoActual());
		}
		TEST_METHOD(Suma_MultiplesDias)
		{
			cuenta.registrarModificacion("01/01", 100);
			cuenta.registrarModificacion("02/02", 250);
			Assert::IsTrue(cuenta.hayDias());
			Assert::AreEqual(350, cuenta.getSaldoActual());
		}
		TEST_METHOD(Resta_MultiplesDias)
		{
			cuenta.registrarModificacion("01/01", 250);
			cuenta.registrarModificacion("02/02", -100);
			Assert::IsTrue(cuenta.hayDias());
			Assert::AreEqual(150, cuenta.getSaldoActual());
		}
	};
	
	TEST_CLASS(Metodo_fechaExiste)
	{
	public:
		Cuenta cuenta;
		Metodo_fechaExiste() : cuenta("MiCuenta", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO) {}

		TEST_METHOD(SinFechas)
		{
			Assert::IsFalse(cuenta.fechaExiste("01/01"));
		}
		TEST_METHOD(InputIncorrecto_SinFechas)
		{
			Assert::IsFalse(cuenta.fechaExiste(""));
		}
		TEST_METHOD(InputIncorrecto_ConFechas)
		{
			cuenta.registrarModificacion("01/01", 100);
			Assert::IsFalse(cuenta.fechaExiste(""));
			Assert::IsFalse(cuenta.fechaExiste("01/02"));
		}
		TEST_METHOD(FechaIncorrecta)
		{
			cuenta.registrarModificacion("01/01", 100);
			Assert::IsFalse(cuenta.fechaExiste("02/02"));
		}
		TEST_METHOD(InputCorrecto_UnaFecha)
		{
			cuenta.registrarModificacion("01/01", 100);
			Assert::IsTrue(cuenta.fechaExiste("01/01"));
		}
		TEST_METHOD(InputCorrecto_MultiplesFechas)
		{
			cuenta.registrarModificacion("01/01", 100);
			cuenta.registrarModificacion("02/02", 100);
			Assert::IsTrue(cuenta.fechaExiste("01/01"));
			Assert::IsTrue(cuenta.fechaExiste("02/02"));
		}
	};
	
	TEST_CLASS(Metodo_getDiaPorFecha)
	{
	public:
		Cuenta cuenta;
		Metodo_getDiaPorFecha() : cuenta("MiCuenta", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO) {}

		TEST_METHOD(SinDias)
		{
			const DiaCuenta* diaCuenta = cuenta.getDiaPorFecha("01/01");
			Assert::IsNull(diaCuenta);
		}

		TEST_METHOD(DiaExistente_UnDia)
		{
			cuenta.registrarModificacion("01/01", 100);
			const DiaCuenta* diaCuenta = cuenta.getDiaPorFecha("01/01");
			Assert::IsNotNull(diaCuenta);
			Assert::AreEqual(100, diaCuenta->delta);
			Assert::AreEqual(100, diaCuenta->valorActual);
			Assert::AreEqual(static_cast<std::string>("01/01"), diaCuenta->fecha);
		}
		TEST_METHOD(DiaInexistente_UnDia)
		{
			cuenta.registrarModificacion("01/01", 100);
			const DiaCuenta* diaCuenta = cuenta.getDiaPorFecha("02/01");
			Assert::IsNull(diaCuenta);
		}

		TEST_METHOD(DiaExistente_MultiplesDias)
		{
			cuenta.registrarModificacion("01/01", 100);
			cuenta.registrarModificacion("02/02", 200);
			const DiaCuenta* diaCuenta = cuenta.getDiaPorFecha("02/02");
			Assert::IsNotNull(diaCuenta);
			Assert::AreEqual(200, diaCuenta->delta);
			Assert::AreEqual(300, diaCuenta->valorActual);
			Assert::AreEqual(static_cast<std::string>("02/02"), diaCuenta->fecha);
		}
		TEST_METHOD(DiaInexistente_MultiplesDias)
		{
			cuenta.registrarModificacion("01/01", 100);
			cuenta.registrarModificacion("02/02", 200);
			const DiaCuenta* diaCuenta = cuenta.getDiaPorFecha("07/08");
			Assert::IsNull(diaCuenta);
		}
	};

	TEST_CLASS(Metodo_getDiaPorPosicion)
	{
	public:
		Cuenta cuenta;
		Metodo_getDiaPorPosicion() : cuenta("MiCuenta", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO) {}

		TEST_METHOD(SinDias)
		{
			Assert::IsNull(cuenta.getDiaPorPosicion(0));
		}

		TEST_METHOD(DiaExistente_UnDia)
		{
			cuenta.registrarModificacion("01/01", 100);
			const DiaCuenta* diaCuenta = cuenta.getDiaPorPosicion(0);
			Assert::IsNotNull(diaCuenta);
			Assert::AreEqual(100, diaCuenta->delta);
			Assert::AreEqual(100, diaCuenta->valorActual);
			Assert::AreEqual(static_cast<std::string>("01/01"), diaCuenta->fecha);
		}
		TEST_METHOD(DiaInexistente_UnDia)
		{
			cuenta.registrarModificacion("01/01", 100);
			const DiaCuenta* diaCuenta = cuenta.getDiaPorPosicion(2);
			Assert::IsNull(diaCuenta);
		}

		TEST_METHOD(DiaExistente_MultiplesDias)
		{
			cuenta.registrarModificacion("01/01", 100);
			cuenta.registrarModificacion("02/02", 200);
			const DiaCuenta* diaCuenta = cuenta.getDiaPorPosicion(1);
			Assert::IsNotNull(diaCuenta);
			Assert::AreEqual(200, diaCuenta->delta);
			Assert::AreEqual(300, diaCuenta->valorActual);
			Assert::AreEqual(static_cast<std::string>("02/02"), diaCuenta->fecha);
		}
		TEST_METHOD(DiaInexistente_MultiplesDias)
		{
			cuenta.registrarModificacion("01/01", 100);
			cuenta.registrarModificacion("02/02", 200);
			const DiaCuenta* diaCuenta = cuenta.getDiaPorPosicion(5);
			Assert::IsNull(diaCuenta);
		}
	};
	
	TEST_CLASS(Metodo_getDias)
	{
	public:
		Cuenta cuenta;
		Metodo_getDias() : cuenta("MiCuenta", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO) {}

		TEST_METHOD(SinDias)
		{
			const std::vector<DiaCuenta> dias = cuenta.getDias();

			Assert::IsTrue(dias.empty());
		}

		TEST_METHOD(UnDia)
		{
			cuenta.registrarModificacion("01/01", 300);
			const std::vector<DiaCuenta> dias = cuenta.getDias();

			Assert::AreEqual(static_cast<std::string>("01/01"), dias[0].fecha);
			Assert::AreEqual(300, dias[0].delta);
			Assert::AreEqual(300, dias[0].valorActual);
		}

		TEST_METHOD(MultiplesDias)
		{
			cuenta.registrarModificacion("01/01", 300);
			cuenta.registrarModificacion("02/02", 200);
			const std::vector<DiaCuenta> dias = cuenta.getDias();

			Assert::AreEqual(static_cast<std::string>("01/01"), dias[0].fecha);
			Assert::AreEqual(300, dias[0].delta);
			Assert::AreEqual(300, dias[0].valorActual);
			Assert::AreEqual(static_cast<std::string>("02/02"), dias[1].fecha);
			Assert::AreEqual(200, dias[1].delta);
			Assert::AreEqual(500, dias[1].valorActual);
		}
	};

	TEST_CLASS(Operador_Comparacion)
	{
	public:
		Cuenta cuenta;
		Operador_Comparacion() : cuenta("MiCuenta", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO) {}

		TEST_METHOD(InputValido)
		{
			Assert::IsTrue(cuenta == "MiCuenta");
		}
		TEST_METHOD(InputInvalido)
		{
			Assert::IsFalse(cuenta == "ABC");
			Assert::IsFalse(cuenta == "");
		}
	};
}
