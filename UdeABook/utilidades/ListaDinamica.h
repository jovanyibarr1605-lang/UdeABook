#ifndef LISTADINAMICA_H
#define LISTADINAMICA_H
template <typename T>
class ListaDinamica
{
private:
    T* datos;
    int cantidadElementos;
    int capacidadActual;

    void redimensionar()
    {
        int nuevaCapacidad = (capacidadActual == 0) ? 4 : capacidadActual * 2;
        T* nuevoBloque = new T[nuevaCapacidad];

        for (int i = 0; i < cantidadElementos; i++)
        {
            nuevoBloque[i] = datos[i];
        }

        delete[] datos;
        datos = nuevoBloque;
        capacidadActual = nuevaCapacidad;
    }

public:
    ListaDinamica()
    {
        capacidadActual = 4;
        cantidadElementos = 0;
        datos = new T[capacidadActual];
    }

    ListaDinamica(const ListaDinamica<T>& otra)
    {
        capacidadActual = otra.capacidadActual;
        cantidadElementos = otra.cantidadElementos;
        datos = new T[capacidadActual];

        for (int i = 0; i < cantidadElementos; i++)
        {
            datos[i] = otra.datos[i];
        }
    }

    ListaDinamica<T>& operator=(const ListaDinamica<T>& otra)
    {
        if (this == &otra)
        {
            return *this;
        }

        delete[] datos;

        capacidadActual = otra.capacidadActual;
        cantidadElementos = otra.cantidadElementos;
        datos = new T[capacidadActual];

        for (int i = 0; i < cantidadElementos; i++)
        {
            datos[i] = otra.datos[i];
        }

        return *this;
    }

    ~ListaDinamica()
    {
        delete[] datos;
    }

    void agregar(const T& elemento)
    {
        if (cantidadElementos == capacidadActual)
        {
            redimensionar();
        }

        datos[cantidadElementos] = elemento;
        cantidadElementos++;
    }

    bool eliminarEn(int indice)
    {
        if (indice < 0 || indice >= cantidadElementos)
        {
            return false;
        }

        for (int i = indice; i < cantidadElementos - 1; i++)
        {
            datos[i] = datos[i + 1];
        }

        cantidadElementos--;
        return true;
    }

    T& obtener(int indice)
    {
        return datos[indice];
    }

    const T& obtener(int indice) const
    {
        return datos[indice];
    }

    T& operator[](int indice)
    {
        return datos[indice];
    }

    const T& operator[](int indice) const
    {
        return datos[indice];
    }

    void limpiar()
    {
        cantidadElementos = 0;
    }

    int getCantidad() const
    {
        return cantidadElementos;
    }

    int getCapacidad() const
    {
        return capacidadActual;
    }

    bool estaVacia() const
    {
        return cantidadElementos == 0;
    }
};
#endif // LISTADINAMICA_H
