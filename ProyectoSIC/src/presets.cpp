#include <vector>
#include "clases.h"

extern const std::vector<Cuenta> CUENTAS = {
	/* ----- activo ----- */
	Cuenta("Caja", true, Cuenta::ACTIVO),
	Cuenta("Fondo Fijo", true, Cuenta::ACTIVO),
	Cuenta("Bco. Cuenta Corriente", true, Cuenta::ACTIVO),
	Cuenta("Bco. Caja de Ahorro", true, Cuenta::ACTIVO),
	Cuenta("Moneda Extranjera", true, Cuenta::ACTIVO),
	Cuenta("Muebles y Utiles", true, Cuenta::ACTIVO),
	Cuenta("Instalaciones", true, Cuenta::ACTIVO),
	Cuenta("Equipos de Computacion", true, Cuenta::ACTIVO),
	Cuenta("Maquinarias", true, Cuenta::ACTIVO),
	Cuenta("Rodados", true, Cuenta::ACTIVO),
	Cuenta("Compras", true, Cuenta::ACTIVO),
	Cuenta("Acciones", true, Cuenta::ACTIVO),
	Cuenta("Patentes", true, Cuenta::ACTIVO),
	Cuenta("Derechos de Autor", true, Cuenta::ACTIVO),
	Cuenta("Marcas de Fabrica", true, Cuenta::ACTIVO),
	Cuenta("Formulas", true, Cuenta::ACTIVO),
	Cuenta("Valores a Depositar", true, Cuenta::ACTIVO),
	Cuenta("Documento a Cobrar", true, Cuenta::ACTIVO),
	Cuenta("Dividendo a Cobrar", true, Cuenta::ACTIVO),

	Cuenta("Deudores por Ventas", true, Cuenta::ACTIVO),
	Cuenta("Deudores Varios", true, Cuenta::ACTIVO),
	Cuenta("Deudores Morosos", true, Cuenta::ACTIVO),
	Cuenta("Deudores en Litigio", true, Cuenta::ACTIVO),
	Cuenta("Socio Cuenta Aporte", true, Cuenta::ACTIVO),
	Cuenta("Gastos Pagados por Adelantado", true, Cuenta::ACTIVO),

	Cuenta("Acciones con Caracter Transitorio", true, Cuenta::ACTIVO),
	Cuenta("Deposito a Plazo Fijo", true, Cuenta::ACTIVO),
	Cuenta("Inmueble destinado a la Renta", true, Cuenta::ACTIVO),
	
	Cuenta("Mercaderias", true, Cuenta::OTRO),

	/* ----- pasivo ----- */
	Cuenta("Proovedores", false, Cuenta::PASIVO),
	Cuenta("Acreedores Varios", false, Cuenta::PASIVO),
	Cuenta("Acreedores Prendarios", true, Cuenta::PASIVO),
	Cuenta("Acreedores Hipotecarios", true, Cuenta::PASIVO),
	Cuenta("Cheque Diferido a Pagar", false, Cuenta::PASIVO),
	Cuenta("Documento a Pagar", false, Cuenta::PASIVO),
	Cuenta("Dividendo a Pagar", true, Cuenta::PASIVO),

	Cuenta("Sueldo a Pagar", true, Cuenta::PASIVO),
	Cuenta("Cargas Sociales a Pagar", true, Cuenta::PASIVO),
	Cuenta("Indemnizaciones a Pagar", true, Cuenta::PASIVO),

	Cuenta("Adelanto en Cuenta Corriente", true, Cuenta::PASIVO),

	Cuenta("Prevision para Despido", true, Cuenta::PASIVO),
	Cuenta("Prevision para Accidentes de Trabajo", true, Cuenta::PASIVO),

	/* ----- patrimonio neto ----- */
	Cuenta("Capital", false, Cuenta::OTRO),

	/* ----- resultados ----- */
	Cuenta("Ventas", false, Cuenta::OTRO),
	Cuenta("CMV", true, Cuenta::OTRO),

	Cuenta("Gastos", true, Cuenta::R_NEG),
	Cuenta("Gastos de Publicidad", true, Cuenta::R_NEG),
	Cuenta("Sueldos", true, Cuenta::R_NEG),
	Cuenta("Cargas Sociales", true, Cuenta::R_NEG),

	Cuenta("Depreciacion de Bienes de Uso", true, Cuenta::R_NEG),
	Cuenta("Intereses Perdidos", true, Cuenta::R_NEG),
	Cuenta("Faltante de Caja", true, Cuenta::R_NEG),
	Cuenta("Deudores Incobrables", true, Cuenta::R_NEG),

	Cuenta("Descuentos Otorgados", true, Cuenta::R_NEG),

	/* ----- IVA ----- */
	Cuenta("IVA C.F.", true, Cuenta::OTRO),
	Cuenta("IVA D.F.", false, Cuenta::OTRO),
	Cuenta("IVA Saldo a Favor", true, Cuenta::OTRO),
	Cuenta("IVA a Pagar", false, Cuenta::OTRO)
};