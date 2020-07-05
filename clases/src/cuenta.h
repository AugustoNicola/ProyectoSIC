#pragma once
#include <string>
#include <vector>

/**
 * @brief Representa cada dia en el que la cuenta es modificada.
 *
 * @param dia: string con el dia actual (DD/MM)
 * @param valorActual: valor de la cuenta al final del dia (positivo debe, negativo haber)
 * @param delta: variacion de la cuenta al final del dia
 */
struct DiaCuenta
{
	std::string dia;
	float valorActual;
	float delta;

	/**
	 * @brief Aumenta el delta y el valor actual del dia segun el valor ingresado.
	 * 
	 * @param aum: numero a aumentar (puede ser negativo)
	 * 
	 */
	void aumentar(float aum)
	{
		delta += aum;
		valorActual += aum;
	};

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

	/** @brief Devuelve el valor actual de la cuenta. */
	float valorActual() { return (!dias.empty()) ? dias.back().valorActual : 0.0f ; }

	/**
	 * @brief Agrega un nuevo valor de dia a la cuenta.
	 * 
	 * @param fecha: fecha del dia (DD/MM)
	 * @param delta: modificacion a la cuenta (positiva o negativa)
	 * @param DebeOHaber: booleano especificando la columna
	 */
	void modifDiaCuenta(std::string fecha, float delta, bool DebeOHaber)
	{
		/* verifica si ya hay dias */
		if (!dias.empty())
		{
			/// hay al menos un dia
			/* verifica si ese dia ya se listo*/
			for (int i = 0; i < dias.size(); i++)
			{
				if (fecha == dias[i].dia)
				{
					/// fecha encontrada, sumar valor actual al previo
					// verifica si se debe aumentar positivo (debe) o negativo (haber)
					dias[i].aumentar(delta * ((DebeOHaber) ? 1 : -1));
					break;
				}
				///fecha no encontrada, crear
				dias.push_back(DiaCuenta(fecha, dias.back().valorActual, delta * ((DebeOHaber) ? 1 : -1)));
			}
		} else {
			/// no hay ningun dia, crear primero
			dias.push_back(DiaCuenta(fecha, 0, delta * ((DebeOHaber) ? 1 : -1)));
		}
	}

	Cuenta(std::string nom, bool mD, Tipo t) : nombre(nom), modoDebito(mD), tipo(t) { dias = {}; };
};