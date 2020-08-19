#pragma once

#include <optional>
#include <iostream>
#include <regex>
#include <limits.h>

#include "Globales.h"
#include "Vectores.h"

Cuenta* buscarCuenta(std::string);
void modificarCuenta(Cuenta*,int);
int validarInt(std::string str, std::optional<int> valorC = {}, std::optional<int> valorT = {}, std::optional<int> min = INT_MIN, std::optional<int> max = INT_MAX);