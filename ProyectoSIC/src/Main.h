#pragma once
#include <optional>
#include <vector>
#include <string>

Cuenta* buscarCuenta(std::string nombre);
std::string formatear(std::string texto);
void imprimeValoresColumnaLibroMayor(std::ofstream& LibroMayor, std::vector<int> valores, unsigned int pos);
int validarInt(std::string str, std::optional<int> valorC, std::optional<int> valorT, std::optional<int> min, std::optional<int> max);
bool validarFecha(std::string str);
void pedirNuevaFecha(std::optional<std::string> mensaje = "Ingrese la nueva fecha");
void modificarCuenta(Cuenta* cuenta, int modificacion);
Operacion* pedirNombreDocx(Operacion* op);
void commitOperacion(Operacion* op);
enum tipoPartida;
Cuenta* elegirCuenta(Cuenta::Tipo t, tipoPartida tipoPart, std::string mensaje);
struct operMercaderia;
operMercaderia seleccionarMercaderia(bool compra);
int aumentarPartida(Cuenta::Tipo t, tipoPartida tipoPartida, std::string mensaje, std::optional<int> limite);
void NotaDC(bool credito);
void EXP_LibroDiario();
void EXP_LibroMayor();
void EXP_EstadoResultados();
void OP_NuevaFecha();
void OP_Capital();
void OP_Transaccion();
void OP_VentaMercaderias();
void OP_CompraMercaderias();
void OP_NCred();
void OP_NDeb();
