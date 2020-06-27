#pragma once
#include <string>
#include <vector>

/**
 * @brief Representa cada dia en el que la cuenta es modificada.
 *
 * @param dia: string con el dia actual
 * @param valorActual: valor de la cuenta al final del dia
 * @param delta: variacion de la cuenta al final del dia
 */
struct DiaCuenta
{
	std::string dia;
	int valorActual = 0;
	int delta;
	DiaCuenta(std::string d, int delt) : dia(d), delta(delt) { valorActual += delt; };
};

/**
 * @brief Representa una cuenta individual con sus datos de cada cuenta, así como una lista de los dias en los que fue modificada.
 * 
 * @param nombre: nombre de la cuenta
 * @param modoDebito: define si se debita cuando es positivo (true) o cuando es negativo (false).
 * @param dias: Lista de cada dia en los que se ha modificado la cuenta de tipo DiaCuenta
 */
class Cuenta
{
private:
	bool modoDebito;
	std::string nombre;
public:
	std::vector<DiaCuenta> dias;
	Cuenta(std::string nom, bool mD) : nombre(nom), modoDebito(mD) { dias = {}; };
};