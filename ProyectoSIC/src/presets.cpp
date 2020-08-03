#include <vector>
#include "clases.h"

extern const std::vector<Cuenta> CUENTAS = {
	/* ----- activo ----- */
	Cuenta("Caja", true, Cuenta::ACTIVO_OPERATIVO),
	Cuenta("Fondo Fijo", true, Cuenta::ACTIVO_OPERATIVO),
	Cuenta("Bco. Cuenta Corriente", true, Cuenta::ACTIVO_OPERATIVO),
	Cuenta("Bco. Caja de Ahorro", true, Cuenta::ACTIVO_OPERATIVO),
	Cuenta("Moneda Extranjera", true, Cuenta::ACTIVO_OPERATIVO),
	Cuenta("Muebles y Utiles", true, Cuenta::ACTIVO_OPERATIVO),
	Cuenta("Instalaciones", true, Cuenta::ACTIVO_OPERATIVO),
	Cuenta("Equipos de Computacion", true, Cuenta::ACTIVO_OPERATIVO),
	Cuenta("Maquinarias", true, Cuenta::ACTIVO_OPERATIVO),
	Cuenta("Rodados", true, Cuenta::ACTIVO_OPERATIVO),
	Cuenta("Compras", true, Cuenta::ACTIVO_OPERATIVO),
	Cuenta("Acciones", true, Cuenta::ACTIVO_OPERATIVO),
	Cuenta("Patentes", true, Cuenta::ACTIVO_OPERATIVO),
	Cuenta("Derechos de Autor", true, Cuenta::ACTIVO_OPERATIVO),
	Cuenta("Marcas de Fabrica", true, Cuenta::ACTIVO_OPERATIVO),
	Cuenta("Formulas", true, Cuenta::ACTIVO_OPERATIVO),
	Cuenta("Valores a Depositar", true, Cuenta::ACTIVO_OPERATIVO),
	Cuenta("Documento a Cobrar", true, Cuenta::ACTIVO_OPERATIVO),
	Cuenta("Dividendo a Cobrar", true, Cuenta::ACTIVO_OPERATIVO),

	Cuenta("Deudores por Ventas", true, Cuenta::ACTIVO_OPERATIVO),
	Cuenta("Deudores Varios", true, Cuenta::ACTIVO_OPERATIVO),
	Cuenta("Deudores Morosos", true, Cuenta::ACTIVO_OPERATIVO),
	Cuenta("Deudores en Litigio", true, Cuenta::ACTIVO_OPERATIVO),
	Cuenta("Socio Cuenta Aporte", true, Cuenta::ACTIVO_OPERATIVO),
	Cuenta("Gastos Pagados por Adelantado", true, Cuenta::ACTIVO_OPERATIVO),

	Cuenta("Acciones con Caracter Transitorio", true, Cuenta::ACTIVO_OPERATIVO),
	Cuenta("Deposito a Plazo Fijo", true, Cuenta::ACTIVO_OPERATIVO),
	Cuenta("Inmueble destinado a la Renta", true, Cuenta::ACTIVO_OPERATIVO),
	
	Cuenta("Mercaderias", true, Cuenta::ACTIVO_NO_OPERATIVO),

	/* ----- pasivo ----- */
	Cuenta("Proovedores", false, Cuenta::PASIVO_OPERATIVO),
	Cuenta("Acreedores Varios", false, Cuenta::PASIVO_OPERATIVO),
	Cuenta("Acreedores Prendarios", true, Cuenta::PASIVO_OPERATIVO),
	Cuenta("Acreedores Hipotecarios", true, Cuenta::PASIVO_OPERATIVO),
	Cuenta("Cheque Diferido a Pagar", false, Cuenta::PASIVO_OPERATIVO),
	Cuenta("Documento a Pagar", false, Cuenta::PASIVO_OPERATIVO),
	Cuenta("Dividendo a Pagar", true, Cuenta::PASIVO_OPERATIVO),

	Cuenta("Sueldo a Pagar", true, Cuenta::PASIVO_OPERATIVO),
	Cuenta("Cargas Sociales a Pagar", true, Cuenta::PASIVO_OPERATIVO),
	Cuenta("Indemnizaciones a Pagar", true, Cuenta::PASIVO_OPERATIVO),

	Cuenta("Adelanto en Cuenta Corriente", true, Cuenta::PASIVO_OPERATIVO),

	Cuenta("Prevision para Despido", true, Cuenta::PASIVO_OPERATIVO),
	Cuenta("Prevision para Accidentes de Trabajo", true, Cuenta::PASIVO_OPERATIVO),

	/* ----- patrimonio neto ----- */
	Cuenta("Capital", false, Cuenta::GANANCIA),

	/* ----- resultados ----- */
	Cuenta("Ventas", false, Cuenta::GANANCIA),
	Cuenta("CMV", true, Cuenta::GASTO_NO_OPERATIVO),

	Cuenta("Gastos", true, Cuenta::GASTO_OPERATIVO),
	Cuenta("Gastos de Publicidad", true, Cuenta::GASTO_OPERATIVO),
	Cuenta("Sueldos", true, Cuenta::GASTO_OPERATIVO),
	Cuenta("Cargas Sociales", true, Cuenta::GASTO_OPERATIVO),

	Cuenta("Depreciacion de Bienes de Uso", true, Cuenta::GASTO_OPERATIVO),
	Cuenta("Intereses Perdidos", true, Cuenta::GASTO_OPERATIVO),
	Cuenta("Faltante de Caja", true, Cuenta::GASTO_OPERATIVO),
	Cuenta("Deudores Incobrables", true, Cuenta::GASTO_OPERATIVO),

	Cuenta("Descuentos Otorgados", true, Cuenta::GASTO_OPERATIVO),
};