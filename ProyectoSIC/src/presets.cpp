#include <vector>
#include "clases.h"

extern const std::vector<Cuenta> CUENTAS = {
	/* ----- activo ----- */
	Cuenta("Caja", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Fondo Fijo", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Bco. Cuenta Corriente", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Bco. Caja de Ahorro", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Moneda Extranjera", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Muebles y Utiles", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Instalaciones", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Equipos de Computacion", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Maquinarias", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Rodados", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Compras", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Acciones", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Patentes", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Derechos de Autor", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Marcas de Fabrica", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Formulas", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Valores a Depositar", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Documento a Cobrar", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Dividendo a Cobrar", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),

	Cuenta("Deudores por Ventas", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Deudores Varios", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Deudores Morosos", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Deudores en Litigio", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Socio Cuenta Aporte", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Gastos Pagados por Adelantado", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),

	Cuenta("Acciones con Caracter Transitorio", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Deposito a Plazo Fijo", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	Cuenta("Inmueble destinado a la Renta", true, Cuenta::TipoCuenta::ACTIVO_OPERATIVO),
	
	Cuenta("Mercaderias", true, Cuenta::TipoCuenta::ACTIVO_NO_OPERATIVO),

	/* ----- pasivo ----- */
	Cuenta("Proovedores", false, Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Acreedores Varios", false, Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Acreedores Prendarios", true, Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Acreedores Hipotecarios", true, Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Cheque Diferido a Pagar", false, Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Documento a Pagar", false, Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Dividendo a Pagar", true, Cuenta::TipoCuenta::PASIVO_OPERATIVO),

	Cuenta("Sueldo a Pagar", true, Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Cargas Sociales a Pagar", true, Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Indemnizaciones a Pagar", true, Cuenta::TipoCuenta::PASIVO_OPERATIVO),

	Cuenta("Adelanto en Cuenta Corriente", true, Cuenta::TipoCuenta::PASIVO_OPERATIVO),

	Cuenta("Prevision para Despido", true, Cuenta::TipoCuenta::PASIVO_OPERATIVO),
	Cuenta("Prevision para Accidentes de Trabajo", true, Cuenta::TipoCuenta::PASIVO_OPERATIVO),

	/* ----- patrimonio neto ----- */
	Cuenta("Capital", false, Cuenta::TipoCuenta::PATRIMONIO_NETO),

	/* ----- resultados ----- */
	Cuenta("Ventas", false, Cuenta::TipoCuenta::GANANCIA),
	Cuenta("CMV", true, Cuenta::TipoCuenta::GASTO_NO_OPERATIVO),

	Cuenta("Gastos", true, Cuenta::TipoCuenta::GASTO_OPERATIVO),
	Cuenta("Gastos de Publicidad", true, Cuenta::TipoCuenta::GASTO_OPERATIVO),
	Cuenta("Sueldos", true, Cuenta::TipoCuenta::GASTO_OPERATIVO),
	Cuenta("Cargas Sociales", true, Cuenta::TipoCuenta::GASTO_OPERATIVO),

	Cuenta("Depreciacion de Bienes de Uso", true, Cuenta::TipoCuenta::GASTO_OPERATIVO),
	Cuenta("Intereses Perdidos", true, Cuenta::TipoCuenta::GASTO_OPERATIVO),
	Cuenta("Faltante de Caja", true, Cuenta::TipoCuenta::GASTO_OPERATIVO),
	Cuenta("Deudores Incobrables", true, Cuenta::TipoCuenta::GASTO_OPERATIVO),

	Cuenta("Descuentos Otorgados", true, Cuenta::TipoCuenta::GASTO_OPERATIVO),
};