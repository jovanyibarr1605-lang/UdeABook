#include "Sesion.h"
#include <random>

Usuario* autenticar(const ListaDinamica<Usuario*>& usuarios,
                    const std::string& nombreUsuario,
                    const std::string& clave,
                    MedidorRecursos& medidor)
{
    for (int i = 0; i < usuarios.getCantidad(); i++)
    {
        medidor.contarIteracion();

        Usuario* candidato = usuarios.obtener(i);
        if (candidato->getNombreUsuario() == nombreUsuario && candidato->getClave() == clave)
        {
            return candidato;
        }
    }

    return nullptr;
}

SesionLogin::SesionLogin(Usuario* usuario)
{
    usuarioActivo = usuario;
    refrescoDisponible = true;
}

double SesionLogin::pesoPorAmigosComunes(int cantidadComunes)
{
    if (cantidadComunes <= 10)
    {
        return 1.0;
    }
    if (cantidadComunes <= 20)
    {
        return 2.5;
    }
    return 3.5;
}

int SesionLogin::contarAmigosComunes(Usuario* candidato) const
{
    int comunes = 0;
    ListaDinamica<Usuario*>& amigosDelCandidato = candidato->getAmigos();

    for (int i = 0; i < amigosDelCandidato.getCantidad(); i++)
    {
        if (usuarioActivo->tieneAmigo(amigosDelCandidato.obtener(i)))
        {
            comunes++;
        }
    }

    return comunes;
}

ListaDinamica<Usuario*> SesionLogin::sortearFotos(MedidorRecursos& medidor)
{
    ListaDinamica<Usuario*> seleccionados;

    // Lista temporal de candidatos (amigos directos) y sus pesos.
    ListaDinamica<Usuario*> candidatos;
    ListaDinamica<double> pesos;

    ListaDinamica<Usuario*>& amigos = usuarioActivo->getAmigos();
    for (int i = 0; i < amigos.getCantidad(); i++)
    {
        medidor.contarIteracion();
        Usuario* candidato = amigos.obtener(i);
        int comunes = contarAmigosComunes(candidato);
        candidatos.agregar(candidato);
        pesos.agregar(pesoPorAmigosComunes(comunes));
    }

    static std::mt19937 generador(std::random_device{}());

    int objetivos = (candidatos.getCantidad() < 3) ? candidatos.getCantidad() : 3;

    for (int ronda = 0; ronda < objetivos; ronda++)
    {
        medidor.contarIteracion(); // sorteo ponderado = operacion externa, se registra

        double total = 0.0;
        for (int i = 0; i < pesos.getCantidad(); i++)
        {
            total += pesos.obtener(i);
        }

        std::uniform_real_distribution<double> distribucion(0.0, total);
        double dardo = distribucion(generador);

        double acumulado = 0.0;
        int indiceElegido = pesos.getCantidad() - 1; // respaldo por redondeo
        for (int i = 0; i < pesos.getCantidad(); i++)
        {
            acumulado += pesos.obtener(i);
            if (dardo <= acumulado)
            {
                indiceElegido = i;
                break;
            }
        }

        seleccionados.agregar(candidatos.obtener(indiceElegido));

        // Se retira temporalmente de la lista antes del siguiente sorteo.
        candidatos.eliminarEn(indiceElegido);
        pesos.eliminarEn(indiceElegido);
    }

    return seleccionados;
}

ListaDinamica<Usuario*> SesionLogin::fotosIniciales(MedidorRecursos& medidor)
{
    ultimaSeleccion = sortearFotos(medidor);
    return ultimaSeleccion;
}

ListaDinamica<Usuario*> SesionLogin::refrescarFotos(MedidorRecursos& medidor)
{
    if (!refrescoDisponible)
    {
        return ultimaSeleccion;
    }

    ultimaSeleccion = sortearFotos(medidor);
    refrescoDisponible = false;
    return ultimaSeleccion;
}

bool SesionLogin::tieneRefrescoDisponible() const
{
    return refrescoDisponible;
}

bool SesionLogin::usuarioTieneMenosDeTresAmigos() const
{
    return usuarioActivo->getAmigos().getCantidad() < 3;
}