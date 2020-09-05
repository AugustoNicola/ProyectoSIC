#pragma once

#include <iostream>
#include <fstream>
#include <conio.h>
#include <climits>
#include <cmath>
#include <regex>
#include <vector>
#include <optional>

#include "clases.h"
#include "Globales.h"
#include "Vectores.h"
#include "Varias.h"
#include "AumentadorPartida.h"
#include "SeleccionadorDeMercaderias.h"

bool OP_Apertura();
void OP_Transaccion();

void OP_VentaMercaderias();
void OP_CompraMercaderias();

class OP_Nota
{
private:
	static bool esCredito;

	static const Operacion* operacionModificada;
	static std::vector<const Operacion*> operacionesDisponibles;
	static int posOperacion;

	static const Linea* lineaModificada;
	static std::vector<const Linea*> lineasDisponibles;
	static int posLinea;

	static ModoAumento tipo;
	static int modificacion;
public:
	static void efectuarNota(bool _esCredito);
private:
	static bool intentarElegirOperacion();
	static bool validarOperacion(std::string strOperacion);

	static void elegirLinea();
	static bool validarLinea(std::string strLinea);

	static void operacionMercaderia();

	static void operacionConvencional();
	static bool validarModificacion(std::string strValorAumento);
	static void ajustarSignoModificacion();
};

void OP_mostrarLibroDiario();
std::string formatearColumnaCuenta(std::string str);
std::string formatearColumnaNumero(int num);

void OP_mostrarLibroMayor();
std::string formatearSaldo(int saldo);

void OP_mostrarEstadoResultados();
std::string formatearResultado(int num);

void OP_mostrarFichaStock();
std::string formatearNombreMercaderia(std::string nombre);
std::string mostrarRegistro(const RegistroPrecio& registro);
std::string mostrarExistencia(const ExistenciasPrecioMercaderia& existencia);

std::string formatearCentrado(std::string str, unsigned int longitud, std::string caracter);
std::string formatearCentrado(int num, unsigned int longitud, std::string caracter);

void OP_salir();

void EXP_LibroDiario();
void EXP_LibroMayor();
void EXP_EstadoResultados();
void EXP_FichaStock();

std::string formatearParaArchivo(std::string texto);
std::string formatearParaArchivo(int valor);
unsigned int buscarOCrearExistencia(std::vector<ExistenciasPrecioMercaderia>& Existencias, unsigned int precio);
std::string imprimirRegistro(const RegistroPrecio& registro);
std::string imprimirExistencia(const ExistenciasPrecioMercaderia& existencia);

void initVectores();