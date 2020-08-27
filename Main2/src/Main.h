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

void OP_Capital();

void OP_Transaccion();

void NotaDC(bool credito);

void EXP_LibroDiario();
void EXP_LibroMayor();
void EXP_EstadoResultados();

std::string formatear(std::string texto);
std::string formatear(int valor);

void initVectores();