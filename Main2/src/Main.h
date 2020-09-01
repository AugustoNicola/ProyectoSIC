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

void OP_mostrarEstadoResultados();

void EXP_LibroDiario(bool mostrarMensaje);
void EXP_LibroMayor(bool mostrarMensaje);
void EXP_EstadoResultados(bool mostrarMensaje);
std::string formatearParaArchivo(std::string texto);
std::string formatearParaArchivo(int valor);

void initVectores();