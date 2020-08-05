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
			std::vector<DiaCuenta> dias = cuenta.getDias();

			Assert::IsTrue(dias.empty());
		}

		TEST_METHOD(UnDia)
		{
			cuenta.registrarModificacion("01/01", 300);
			std::vector<DiaCuenta> dias = cuenta.getDias();

			Assert::AreEqual(static_cast<std::string>("01/01"), dias[0].fecha);
			Assert::AreEqual(300, dias[0].delta);
			Assert::AreEqual(300, dias[0].valorActual);
		}

		TEST_METHOD(MultiplesDias)
		{
			cuenta.registrarModificacion("01/01", 300);
			cuenta.registrarModificacion("02/02", 200);
			std::vector<DiaCuenta> dias = cuenta.getDias();

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

namespace Clase_Mercaderia
{
	TEST_CLASS(Constructor)
	{
	public:
		Mercaderia mercaderia;
		Constructor() : mercaderia("Producto") {}

		TEST_METHOD(Nombre)
		{
			Assert::AreEqual("Producto", mercaderia.getNombre());
		}
		TEST_METHOD(Metodo_hayExistencias)
		{
			Assert::IsFalse(mercaderia.hayExistencias());
		}
	};

	TEST_CLASS(Metodo_registrarCompra)
	{
	public:
		Mercaderia mercaderia;
		Metodo_registrarCompra() : mercaderia("Producto") {}

		TEST_METHOD(UnDia_UnPrecio)
		{
			mercaderia.registrarCompra("01/01", 200, 4);

			Assert::AreEqual(4, mercaderia.getExistenciasTotales());
		}
		TEST_METHOD(Suma_UnPrecio)
		{
			mercaderia.registrarCompra("01/01", 200, 4);
			mercaderia.registrarCompra("01/01", 200, 2);

			Assert::AreEqual(6, mercaderia.getExistenciasTotales());
		}
		TEST_METHOD(Resta_UnPrecio)
		{
			mercaderia.registrarCompra("01/01", 200, 4);
			mercaderia.registrarCompra("02/02", 200, -1);

			Assert::AreEqual(3, mercaderia.getExistenciasTotales());
		}
		TEST_METHOD(ExcesoResta_UnPrecio)
		{
			mercaderia.registrarCompra("01/01", 200, 4);
			mercaderia.registrarCompra("01/01", 200, -5);

			Assert::AreEqual(4, mercaderia.getExistenciasTotales());
		}

		TEST_METHOD(UnDia_MultiplesPrecios)
		{
			mercaderia.registrarCompra("01/01", 200, 4);
			mercaderia.registrarCompra("01/01", 300, 2);

			Assert::AreEqual(6, mercaderia.getExistenciasTotales());
			Assert::AreEqual(4, mercaderia.getExistenciasPrecio(200));
			Assert::AreEqual(2, mercaderia.getExistenciasPrecio(300));
		}
		TEST_METHOD(Suma_MultiplesDias_MultiplesPrecios)
		{
			mercaderia.registrarCompra("01/01", 200, 4);
			mercaderia.registrarCompra("02/02", 200, 3);
			mercaderia.registrarCompra("03/03", 300, 2);

			Assert::AreEqual(7, mercaderia.getExistenciasPrecio(200));
			Assert::AreEqual(2, mercaderia.getExistenciasPrecio(300));
		}
		TEST_METHOD(Resta_MultiplesDias_MultiplesPrecios)
		{
			mercaderia.registrarCompra("01/01", 200, 4);
			mercaderia.registrarCompra("02/02", 300, 2);
			mercaderia.registrarCompra("03/04", 200, -3);

			Assert::AreEqual(1, mercaderia.getExistenciasPrecio(200));
			Assert::AreEqual(2, mercaderia.getExistenciasPrecio(300));
		}
	};

	TEST_CLASS(Metodo_registrarVenta)
	{
	public:
		Mercaderia mercaderia;

		Metodo_registrarVenta() : mercaderia("Producto") {}
		TEST_METHOD(UnPrecio)
		{
			mercaderia.registrarCompra("01/01", 200, 4);
			mercaderia.registrarVenta("02/02", 1);

			Assert::AreEqual(3, mercaderia.getExistenciasTotales());
		}
		TEST_METHOD(Suma_UnPrecio)
		{
			mercaderia.registrarCompra("01/01", 200, 4);
			mercaderia.registrarVenta("02/02", 1);
			mercaderia.registrarVenta("02/02", 3);

			Assert::AreEqual(0, mercaderia.getExistenciasTotales());
		}
		TEST_METHOD(MultiplesPrecios)
		{
			mercaderia.registrarCompra("01/01", 200, 4);
			mercaderia.registrarCompra("01/01", 300, 5);
			mercaderia.registrarVenta("02/02", 3);

			Assert::AreEqual(1, mercaderia.getExistenciasPrecio(200));
			Assert::AreEqual(5, mercaderia.getExistenciasPrecio(300));


			mercaderia.registrarVenta("02/02", 4);

			Assert::AreEqual(0, mercaderia.getExistenciasPrecio(200));
			Assert::AreEqual(2, mercaderia.getExistenciasPrecio(300));
		}
		TEST_METHOD(ExcesoDeVentas_SinPrecios)
		{
			mercaderia.registrarVenta("01/01", 1);

			Assert::AreEqual(0, mercaderia.getExistenciasTotales());
		}
		TEST_METHOD(ExcesoDeVentas_UnPrecio)
		{
			mercaderia.registrarCompra("01/01", 200, 4);
			mercaderia.registrarVenta("02/02", 7);

			Assert::AreEqual(4, mercaderia.getExistenciasPrecio(200)); //no deberia vender nada
		}
		TEST_METHOD(ExcesoDeVentas_MultiplesPrecios)
		{
			mercaderia.registrarCompra("01/01", 300, 2);
			mercaderia.registrarCompra("01/01", 200, 4);
			
			mercaderia.registrarVenta("02/02", 7);
			Assert::AreEqual(2, mercaderia.getExistenciasPrecio(300));
			Assert::AreEqual(4, mercaderia.getExistenciasPrecio(200));

			mercaderia.registrarVenta("03/03", 3);
			mercaderia.registrarVenta("03/03", 5);
			Assert::AreEqual(0, mercaderia.getExistenciasPrecio(300));
			Assert::AreEqual(3, mercaderia.getExistenciasPrecio(200));
		}
	};

	TEST_CLASS(Metodo_getPreciosActuales)
	{
	public:
		Mercaderia mercaderia;
		Metodo_getPreciosActuales() : mercaderia("Producto") {}

		TEST_METHOD(SinPrecios)
		{
			Assert::IsTrue(mercaderia.getPreciosActuales().empty());
		}
		TEST_METHOD(UnPrecio)
		{
			mercaderia.registrarCompra("01/01", 200, 3);

			Assert::IsFalse(mercaderia.getPreciosActuales().empty());
			Assert::AreEqual(200, mercaderia.getPreciosActuales()[0]);
		}
		TEST_METHOD(MultiplesPrecios_SinBorrado)
		{
			mercaderia.registrarCompra("01/01", 200, 3);
			mercaderia.registrarCompra("02/02", 400, 7);

			Assert::IsFalse(mercaderia.getPreciosActuales().empty());
			Assert::AreEqual(200, mercaderia.getPreciosActuales()[0]);
			Assert::AreEqual(400, mercaderia.getPreciosActuales()[1]);
		}
		TEST_METHOD(MultiplesPrecios_ConBorrado)
		{
			mercaderia.registrarCompra("01/01", 200, 3);
			mercaderia.registrarCompra("02/02", 400, 7);
			mercaderia.registrarVenta("03/03", 4);

			Assert::IsFalse(mercaderia.getPreciosActuales().empty());
			Assert::AreEqual(400, mercaderia.getPreciosActuales()[0]);
		}
	};

	TEST_CLASS(Metodo_buscarDiaPorFecha)
	{
	public:
		Mercaderia mercaderia;

		Metodo_buscarDiaPorFecha() : mercaderia("Producto") {}

		TEST_METHOD(SinFechas)
		{
			Assert::IsNull(mercaderia.buscarDiaPorFecha("01/01"));
		}
		TEST_METHOD(UnaFecha_UnPrecio)
		{
			mercaderia.registrarCompra("01/01", 200, 4);

			auto fecha = mercaderia.buscarDiaPorFecha("01/01");
			Assert::IsNotNull(fecha);
			Assert::AreEqual(200, fecha[0]->precio);
			Assert::AreEqual(4, fecha[0]->delta);
			Assert::AreEqual(4, fecha[0]->valorActual);
		}
		TEST_METHOD(UnaFecha_MultiplesPrecios)
		{
			mercaderia.registrarCompra("01/01", 200, 4);
			mercaderia.registrarCompra("01/01", 500, 3);

			auto fecha = mercaderia.buscarDiaPorFecha("01/01");
			Assert::IsNotNull(fecha);
			Assert::AreEqual(200, fecha[0]->precio);
			Assert::AreEqual(4, fecha[0]->delta);
			Assert::AreEqual(4, fecha[0]->valorActual);

			Assert::AreEqual(500, fecha[1]->precio);
			Assert::AreEqual(3, fecha[1]->delta);
			Assert::AreEqual(3, fecha[1]->valorActual);
		}
		TEST_METHOD(MultiplesFechas_UnPrecio)
		{
			mercaderia.registrarCompra("01/01", 200, 4);
			mercaderia.registrarCompra("02/02", 200, 6);

			auto fecha = mercaderia.buscarDiaPorFecha("02/02");
			Assert::IsNotNull(fecha);
			Assert::AreEqual(200, fecha[0]->precio);
			Assert::AreEqual(6, fecha[0]->delta);
			Assert::AreEqual(10, fecha[0]->valorActual);
		}
		TEST_METHOD(MultiplesFechas_MultiplesPrecios)
		{
			mercaderia.registrarCompra("01/01", 200, 4);
			mercaderia.registrarCompra("01/01", 500, 2);
			mercaderia.registrarCompra("02/02", 200, 6);
			mercaderia.registrarCompra("02/02", 500, 1);

			auto fecha1 = mercaderia.buscarDiaPorFecha("01/01");
			Assert::IsNotNull(fecha1);
			Assert::AreEqual(200, fecha1[0]->precio);
			Assert::AreEqual(4, fecha1[0]->delta);
			Assert::AreEqual(4, fecha1[0]->valorActual);

			Assert::AreEqual(500, fecha1[1]->precio);
			Assert::AreEqual(2, fecha1[1]->delta);
			Assert::AreEqual(2, fecha1[1]->valorActual);


			auto fecha1 = mercaderia.buscarDiaPorFecha("02/02");
			Assert::IsNotNull(fecha1);
			Assert::AreEqual(200, fecha1[0]->precio);
			Assert::AreEqual(6, fecha1[0]->delta);
			Assert::AreEqual(10, fecha1[0]->valorActual);

			Assert::AreEqual(500, fecha1[1]->precio);
			Assert::AreEqual(1, fecha1[1]->delta);
			Assert::AreEqual(3, fecha1[1]->valorActual);
		}
	};

	TEST_CLASS(Metodo_getFechas)
	{
	public:
		Mercaderia mercaderia;

		Metodo_getFechas() : mercaderia("Producto") {}

		TEST_METHOD(SinFechas)
		{
			Assert::IsTrue(mercaderia.getDias().empty());
		}
		TEST_METHOD(ConFechas_UnPrecio)
		{
			mercaderia.registrarCompra("01/01", 200, 4);
			mercaderia.registrarCompra("02/02", 200, 3);
			mercaderia.registrarVenta("03/03", 2);

			auto fechas = mercaderia.getDias();
			Assert::IsFalse(fechas.empty());

			Assert::AreEqual(200, fechas[0][0]->precio);
			Assert::AreEqual(4, fechas[0][0]->delta);
			Assert::AreEqual(4, fechas[0][0]->valorActual);

			Assert::AreEqual(200, fechas[1][0]->precio);
			Assert::AreEqual(3, fechas[1][0]->delta);
			Assert::AreEqual(7, fechas[1][0]->valorActual);

			Assert::AreEqual(200, fechas[2][0]->precio);
			Assert::AreEqual(-2, fechas[2][0]->delta);
			Assert::AreEqual(5, fechas[2][0]->valorActual);
		}
		TEST_METHOD(ConFechas_MultiplesPrecios)
		{
			mercaderia.registrarCompra("01/01", 200, 4);
			mercaderia.registrarCompra("01/01", 500, 2);
			mercaderia.registrarCompra("02/02", 200, 3);
			mercaderia.registrarVenta("03/03", 6);
			mercaderia.registrarVenta("03/03", 2);

			auto fechas = mercaderia.getDias();
			Assert::IsFalse(fechas.empty());

			Assert::AreEqual(200, fechas[0][0]->precio);
			Assert::AreEqual(4, fechas[0][0]->delta);
			Assert::AreEqual(4, fechas[0][0]->valorActual);

			Assert::AreEqual(500, fechas[0][1]->precio);
			Assert::AreEqual(2, fechas[0][1]->delta);
			Assert::AreEqual(2, fechas[0][1]->valorActual);


			Assert::AreEqual(200, fechas[1][0]->precio);
			Assert::AreEqual(3, fechas[1][0]->delta);
			Assert::AreEqual(7, fechas[1][0]->valorActual);


			Assert::AreEqual(200, fechas[2][0]->precio);
			Assert::AreEqual(-6, fechas[2][0]->delta);
			Assert::AreEqual(1, fechas[2][0]->valorActual);

			Assert::AreEqual(500, fechas[2][1]->precio);
			Assert::AreEqual(-2, fechas[2][1]->delta);
			Assert::AreEqual(0, fechas[2][1]->valorActual);
		}
	};
}