#pragma once

#include <optional>

#include "clases.h"


Cuenta* buscarCuenta(std::string);
void modificarCuenta(Cuenta*,int);
int validarInt(std::string,std::optional<int>,std::optional<int>,std::optional<int>,std::optional<int>);