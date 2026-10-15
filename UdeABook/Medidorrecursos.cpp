#include "MedidorRecursos.h"

MedidorRecursos::MedidorRecursos()
{
    iteraciones = 0;
    memoriaActual = 0;
}

void MedidorRecursos::contarIteracion()
{
    iteraciones++;
}

void MedidorRecursos::contarIteraciones(long long cantidad)
{
    iteraciones += cantidad;
}

void MedidorRecursos::sumarBytes(size_t bytes)
{
    memoriaActual += bytes;
}

void MedidorRecursos::restarBytes(size_t bytes)
{
    // Evita que una resta mayor al acumulado produzca un size_t negativo (desbordado).
    if (bytes > memoriaActual)
    {
        memoriaActual = 0;
    }
    else
    {
        memoriaActual -= bytes;
    }
}

void MedidorRecursos::reiniciar()
{
    iteraciones = 0;
    memoriaActual = 0;
}

long long MedidorRecursos::getIteraciones() const
{
    return iteraciones;
}

size_t MedidorRecursos::getMemoriaActual() const
{
    return memoriaActual;
}