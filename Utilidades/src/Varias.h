#pragma once

#include <optional>
#include <iostream>
#include <regex>

#include "Vectores.h"
#include "Globales.h"

Cuenta* buscarCuenta(std::string);
void modificarCuenta(Cuenta*,int);
int validarInt(std::string,std::optional<int>,std::optional<int>,std::optional<int>,std::optional<int>);