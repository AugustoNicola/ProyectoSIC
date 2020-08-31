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

void EXP_LibroDiario(bool mostrarMensaje);
void EXP_LibroMayor(bool mostrarMensaje);
void EXP_EstadoResultados(bool mostrarMensaje);

std::string formatear(std::string texto);
std::string formatear(int valor);

void initVectores();