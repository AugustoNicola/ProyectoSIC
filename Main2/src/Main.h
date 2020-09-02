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
void NotaDC(bool credito);

void OP_mostrarLibroDiario();
std::string formatearColumnaNumero(int str);
std::string formatearColumnaCuenta(std::string str);

void OP_mostrarLibroMayor();
std::string formatearNombreCuenta(std::string str);
std::string formatearSaldo(int saldo);

void OP_mostrarEstadoResultados();
std::string formatearResultado(int num);

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