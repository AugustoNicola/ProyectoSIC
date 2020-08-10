#pragma once
#include <optional>
#include <vector>
#include <string>
#include "clases.h"

extern std::vector<DiaOperaciones> DIAS;
extern std::vector<Mercaderia> MERCADERIAS;
extern std::vector<Cuenta*> ACTIVOS, PASIVOS, R_NEGS;

void modificarCuenta(Cuenta*,int);
int validarInt(std::string,std::optional<int>,std::optional<int>,std::optional<int>,std::optional<int>);
