#pragma once

#include "clases.h"
#include "conmanip.h"

extern std::string fecha;

extern Operacion oper;
extern Operacion* operacionActual;

conmanip::console_text_colors colorBase = conmanip::console_text_colors::white;
conmanip::console_text_colors colorInput = conmanip::console_text_colors::light_magenta;
conmanip::console_text_colors colorDatos = conmanip::console_text_colors::light_cyan;
conmanip::console_text_colors colorEnfoque = conmanip::console_text_colors::light_yellow;
conmanip::console_text_colors colorError = conmanip::console_text_colors::light_red;