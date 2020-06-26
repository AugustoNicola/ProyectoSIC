#ifndef OPERACION_DEF
#define OPERACION_DEF

#include <string>
#include <vector>
#include "cuenta.h"
/**
 * @brief Elemento con la informacion de una linea del registro
 * 
 * @param cuenta: puntero al tipo de cuenta que es utilizada
 * @param modificacion: numero que se anota en la partida doble; positivo para debe, negativo para haber
 */
struct Linea
{
	const Cuenta *cuenta;
	int modificacion;
	Linea(Cuenta *c, int modif) : cuenta(c), modificacion(modif) {};
};

/**
 * @brief Contiene cada una de las lineas que conforman a la operacion, seguida de su documento correspondiente
 *
 * @param documento: string con el nombre del documento
 * @param lineas: vector que contiene a todas las lineas de una operacion
 */
class Operacion
{
private:
	std::string documento;
	std::vector<Linea> lineas;
public:
	Operacion(std::string doc) : documento(doc) {};
};

#endif