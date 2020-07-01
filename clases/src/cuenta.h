#pragma once
#include <string>
#include <vector>

/**
 * @brief Representa cada dia en el que la cuenta es modificada.
 *
 * @param dia: string con el dia actual (DD/MM)
 * @param valorActual: valor de la cuenta al final del dia
 * @param delta: variacion de la cuenta al final del dia
 */
struct DiaCuenta
{
	std::string dia;
	float valorActual;
	float delta;
	DiaCuenta(std::string d, float vAnterior, float delt) : dia(d), valorActual(vAnterior), delta(delt) { valorActual += delt; };
};

/**
 * @brief Representa una cuenta individual con sus datos de cada cuenta, así como una lista de los dias en los que fue modificada.
 * 
 * @param nombre: nombre de la cuenta
 * @param modoDebito: define si se debita cuando es positivo (true) o cuando es negativo (false).
 * @param dias: Lista de cada dia en los que se ha modificado la cuenta de tipo DiaCuenta.
 * @param tipo: enum para limitar las aplicaciones de la cuenta a situaciones coherentes.
 */
class Cuenta
{
public:
	/**
	 * @brief usado para limitar las apariciones de la cuenta (Activo, Pasivo, Resultado Negativo, Otro).
	 *  y tambien filtros (Operacionales)
	 */
	enum Tipo {
		ACTIVO, PASIVO, R_NEG, OTRO,
		F_OPER
	};
private:
	bool modoDebito;
public:
	std::string nombre;
	Cuenta::Tipo tipo;
	std::vector<DiaCuenta> dias;

	/**
	 * @brief Agrega un nuevo valor de dia a la cuenta.
	 * 
	 * @param fecha: fecha del dia (DD/MM)
	 * @param delta: modificacion a la cuenta (positiva o negativa)
	 */
	void nuevoDiaCuenta(std::string fecha, float delta) { dias.push_back(DiaCuenta(fecha, dias.back().valorActual, delta)); }

	Cuenta(std::string nom, bool mD, Tipo t) : nombre(nom), modoDebito(mD), tipo(t) { dias = {}; };
};