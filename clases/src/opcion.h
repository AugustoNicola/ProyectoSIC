#pragma once

#include <string>
#include <functional>

struct Opcion
{
	std::string Nombre;
	std::function<void()> Funcion;
	Opcion(std::string _Nombre, std::function<void()> _Funcion) : Nombre(_Nombre), Funcion(_Funcion) {};
};