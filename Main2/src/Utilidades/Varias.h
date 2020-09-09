#pragma once

#include <optional>
#include <iostream>
#include <regex>
#include <limits.h>
#include <conio.h>
#include "conmanip.h"

#include "Globales.h"
#include "Vectores.h"

void error();
void pedirNuevaFecha(std::string mensaje = "Ingrese la nueva fecha", std::optional<std::string> strHeader = {});
void header(std::string texto, unsigned int espacios = 0);
bool validarStr(std::string str);
bool validarFecha(std::string str);
int validarInt(std::string str, std::optional<int> min = INT_MIN, std::optional<int> max = INT_MAX, std::optional<int> valorC = {}, std::optional<int> valorT = {});
std::string formatearDinero(int saldo);
void modificarCuenta(Cuenta*,int);
Cuenta* buscarCuenta(std::string);
void commitOperacion(Operacion* op);
Operacion* pedirNombreDocx(Operacion* op);