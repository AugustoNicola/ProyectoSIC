#include <vector>
#include "clases.h"

extern const std::vector<Cuenta> CUENTAS = {
	/* ----- activo ----- */
	Cuenta("Caja", true, Cuenta::ACTIVO_OPER),
	Cuenta("Fondo Fijo", true, Cuenta::ACTIVO_OPER),
	Cuenta("Bco. Cuenta Corriente", true, Cuenta::ACTIVO_OPER),
	Cuenta("Bco. Caja de Ahorro", true, Cuenta::ACTIVO_OPER),
	Cuenta("Moneda Extranjera", true, Cuenta::ACTIVO_OPER),
	Cuenta("Muebles y Utiles", true, Cuenta::ACTIVO_OPER),
	Cuenta("Instalaciones", true, Cuenta::ACTIVO_OPER),
	Cuenta("Equipos de Computacion", true, Cuenta::ACTIVO_OPER),
	Cuenta("Maquinarias", true, Cuenta::ACTIVO_OPER),
	Cuenta("Rodados", true, Cuenta::ACTIVO_OPER),
	Cuenta("Compras", true, Cuenta::ACTIVO_OPER),
	Cuenta("Acciones", true, Cuenta::ACTIVO_OPER),
	Cuenta("Patentes", true, Cuenta::ACTIVO_OPER),
	Cuenta("Derechos de Autor", true, Cuenta::ACTIVO_OPER),
	Cuenta("Marcas de Fabrica", true, Cuenta::ACTIVO_OPER),
	Cuenta("Formulas", true, Cuenta::ACTIVO_OPER),
	Cuenta("Valores a Depositar", true, Cuenta::ACTIVO_OPER),
	Cuenta("Documento a Cobrar", true, Cuenta::ACTIVO_OPER),
	Cuenta("Dividendo a Cobrar", true, Cuenta::ACTIVO_OPER),

	Cuenta("Deudores por Ventas", true, Cuenta::ACTIVO_OPER),
	Cuenta("Deudores Varios", true, Cuenta::ACTIVO_OPER),
	Cuenta("Deudores Morosos", true, Cuenta::ACTIVO_OPER),
	Cuenta("Deudores en Litigio", true, Cuenta::ACTIVO_OPER),
	Cuenta("Socio Cuenta Aporte", true, Cuenta::ACTIVO_OPER),
	Cuenta("Gastos Pagados por Adelantado", true, Cuenta::ACTIVO_OPER),

	Cuenta("Acciones con Caracter Transitorio", true, Cuenta::ACTIVO_OPER),
	Cuenta("Deposito a Plazo Fijo", true, Cuenta::ACTIVO_OPER),
	Cuenta("Inmueble destinado a la Renta", true, Cuenta::ACTIVO_OPER),
	
	Cuenta("Mercaderias", true, Cuenta::ACTIVO),

	/* ----- pasivo ----- */
	Cuenta("Proovedores", false, Cuenta::PASIVO_OPER),
	Cuenta("Acreedores Varios", false, Cuenta::PASIVO_OPER),
	Cuenta("Acreedores Prendarios", true, Cuenta::PASIVO_OPER),
	Cuenta("Acreedores Hipotecarios", true, Cuenta::PASIVO_OPER),
	Cuenta("Cheque Diferido a Pagar", false, Cuenta::PASIVO_OPER),
	Cuenta("Documento a Pagar", false, Cuenta::PASIVO_OPER),
	Cuenta("Dividendo a Pagar", true, Cuenta::PASIVO_OPER),

	Cuenta("Sueldo a Pagar", true, Cuenta::PASIVO_OPER),
	Cuenta("Cargas Sociales a Pagar", true, Cuenta::PASIVO_OPER),
	Cuenta("Indemnizaciones a Pagar", true, Cuenta::PASIVO_OPER),

	Cuenta("Adelanto en Cuenta Corriente", true, Cuenta::PASIVO_OPER),

	Cuenta("Prevision para Despido", true, Cuenta::PASIVO_OPER),
	Cuenta("Prevision para Accidentes de Trabajo", true, Cuenta::PASIVO_OPER),

	/* ----- patrimonio neto ----- */
	Cuenta("Capital", false, Cuenta::R_POS),

	/* ----- resultados ----- */
	Cuenta("Ventas", false, Cuenta::R_POS),
	Cuenta("CMV", true, Cuenta::R_NEG),

	Cuenta("Gastos", true, Cuenta::R_NEG_OPER),
	Cuenta("Gastos de Publicidad", true, Cuenta::R_NEG_OPER),
	Cuenta("Sueldos", true, Cuenta::R_NEG_OPER),
	Cuenta("Cargas Sociales", true, Cuenta::R_NEG_OPER),

	Cuenta("Depreciacion de Bienes de Uso", true, Cuenta::R_NEG_OPER),
	Cuenta("Intereses Perdidos", true, Cuenta::R_NEG_OPER),
	Cuenta("Faltante de Caja", true, Cuenta::R_NEG_OPER),
	Cuenta("Deudores Incobrables", true, Cuenta::R_NEG_OPER),

	Cuenta("Descuentos Otorgados", true, Cuenta::R_NEG_OPER),

	/* ----- IVA ----- */
	Cuenta("IVA C.F.", true, Cuenta::ACTIVO),
	Cuenta("IVA D.F.", false, Cuenta::PASIVO),
	Cuenta("IVA Saldo a Favor", true, Cuenta::ACTIVO),
	Cuenta("IVA a Pagar", false, Cuenta::PASIVO)
};