#include <vector>
#include "clases.h"

extern const std::vector<Cuenta> CUENTAS = {
	/* ----- activo ----- */
	Cuenta("Caja", true, Cuenta::A_OPER),
	Cuenta("Bco. Cuenta Corriente", true, Cuenta::A_OPER),
	Cuenta("Muebles y Utiles", true, Cuenta::A_OPER),
	Cuenta("Instalaciones", true, Cuenta::A_OPER),
	Cuenta("Equipos de Computacion", true, Cuenta::A_OPER),
	Cuenta("Maquinarias", true, Cuenta::A_OPER),
	Cuenta("Rodados", true, Cuenta::A_OPER),
	Cuenta("Valores a Depositar", true, Cuenta::A_OPER),
	Cuenta("Documento a Cobrar", true, Cuenta::A_OPER),
	Cuenta("Deudores por Ventas", true, Cuenta::A_OPER),
	Cuenta("Deudores Varios", true, Cuenta::A_OPER),

	Cuenta("Mercaderias", true, Cuenta::OTRO),
	/* ----- pasivo ----- */
	Cuenta("Proovedores", false, Cuenta::P_OPER),
	Cuenta("Acreedores Varios", false, Cuenta::P_OPER),
	Cuenta("Cheque Diferido a Pagar", false, Cuenta::P_OPER),
	Cuenta("Documento a Pagar", false, Cuenta::P_OPER),
	/* ----- patrimonio neto ----- */
	Cuenta("Capital", false, Cuenta::OTRO),
	/* ----- resultados ----- */
	Cuenta("Ventas", false, Cuenta::OTRO),
	Cuenta("CMV", true, Cuenta::OTRO),
	Cuenta("Gastos", true, Cuenta::OTRO),
	/* ----- IVA ----- */
	Cuenta("IVA C.F.", true, Cuenta::OTRO),
	Cuenta("IVA D.F.", false, Cuenta::OTRO),
	Cuenta("IVA Saldo a Favor", true, Cuenta::OTRO),
	Cuenta("IVA a Pagar", false, Cuenta::OTRO)
};