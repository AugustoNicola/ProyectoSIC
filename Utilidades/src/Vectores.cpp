#include "clases.h"
#include "Vectores.h"

std::vector<Cuenta> CUENTAS = {
	/* ----- activo ----- */
	Cuenta("Caja", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Fondo Fijo", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Bco. Cuenta Corriente", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Bco. Caja de Ahorro", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Moneda Extranjera", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Muebles y Utiles", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Instalaciones", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Equipos de Computacion", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Maquinarias", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Rodados", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Compras", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Acciones", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Patentes", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Derechos de Autor", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Marcas de Fabrica", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Formulas", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Valores a Depositar", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Documento a Cobrar", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Dividendo a Cobrar", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),

	Cuenta("Deudores por Ventas", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Deudores Varios", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Deudores Morosos", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Deudores en Litigio", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Socio Cuenta Aporte", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Gastos Pagados por Adelantado", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),

	Cuenta("Acciones con Caracter Transitorio", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Deposito a Plazo Fijo", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Inmueble destinado a la Renta", Cuenta::TipoCuenta::ACTIVO_OPERATIVO),

	Cuenta("Mercaderias", Cuenta::TipoCuenta::ACTIVO_NO_OPERATIVO),

	/* ----- pasivo ----- */
	Cuenta("Proovedores", Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Acreedores Varios", Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Acreedores Prendarios", Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Acreedores Hipotecarios", Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Cheque Diferido a Pagar", Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Documento a Pagar", Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Dividendo a Pagar", Cuenta::TipoCuenta::PASIVO_OPERATIVO),

	Cuenta("Sueldo a Pagar", Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Cargas Sociales a Pagar", Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Indemnizaciones a Pagar", Cuenta::TipoCuenta::PASIVO_OPERATIVO),

	Cuenta("Adelanto en Cuenta Corriente", Cuenta::TipoCuenta::PASIVO_OPERATIVO),

	Cuenta("Prevision para Despido", Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Prevision para Accidentes de Trabajo", Cuenta::TipoCuenta::PASIVO_OPERATIVO),

	/* ----- patrimonio neto ----- */
	Cuenta("Capital", Cuenta::TipoCuenta::PATRIMONIO_NETO),

	/* ----- resultados ----- */
	Cuenta("Ventas", Cuenta::TipoCuenta::GANANCIA),
	Cuenta("CMV", Cuenta::TipoCuenta::GASTO_NO_OPERATIVO),

	Cuenta("Gastos", Cuenta::TipoCuenta::GASTO_OPERATIVO),
	Cuenta("Gastos de Publicidad", Cuenta::TipoCuenta::GASTO_OPERATIVO),
	Cuenta("Sueldos", Cuenta::TipoCuenta::GASTO_OPERATIVO),
	Cuenta("Cargas Sociales", Cuenta::TipoCuenta::GASTO_OPERATIVO),

	Cuenta("Depreciacion de Bienes de Uso", Cuenta::TipoCuenta::GASTO_OPERATIVO),
	Cuenta("Intereses Perdidos", Cuenta::TipoCuenta::GASTO_OPERATIVO),
	Cuenta("Faltante de Caja", Cuenta::TipoCuenta::GASTO_OPERATIVO),
	Cuenta("Deudores Incobrables", Cuenta::TipoCuenta::GASTO_OPERATIVO),

	Cuenta("Descuentos Otorgados", Cuenta::TipoCuenta::GASTO_OPERATIVO),
};

std::vector<Cuenta*> ACTIVOS = {};
std::vector<Cuenta*> PASIVOS = {};
std::vector<Cuenta*> R_NEGS = {};

int main() {
	for (Cuenta cuenta : CUENTAS)
	{
		if (cuenta.getTipo() == Cuenta::TipoCuenta::ACTIVO_OPERATIVO) { ACTIVOS.push_back((Cuenta*)&cuenta); }
		else if (cuenta.getTipo() == Cuenta::TipoCuenta::PASIVO_OPERATIVO) { PASIVOS.push_back((Cuenta*)&cuenta); }
		else if (cuenta.getTipo() == Cuenta::TipoCuenta::GASTO_OPERATIVO) { R_NEGS.push_back((Cuenta*)&cuenta); }
	}
}