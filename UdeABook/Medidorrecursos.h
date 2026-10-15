#ifndef MEDIDORRECURSOS_H
#define MEDIDORRECURSOS_H

#include <cstddef>

class MedidorRecursos
{
private:
    long long iteraciones;
    size_t memoriaActual;

public:
    MedidorRecursos();

    void contarIteracion();
    void contarIteraciones(long long cantidad);

    void sumarBytes(size_t bytes);
    void restarBytes(size_t bytes);

    void reiniciar();

    long long getIteraciones() const;
    size_t getMemoriaActual() const;
};

#endif // MEDIDORRECURSOS_H