#ifndef SESION_H
#define SESION_H

#include <string>
#include "ListaDinamica.h"
#include "Usuario.h"
#include "MedidorRecursos.h"

// Funcion II: autenticacion y 3 fotos de perfil de amigos con prioridad
// ponderada (ver "Fotos de perfil con prioridad" en el informe).

// Busca el usuario cuyo nombre y clave coincidan exactamente. Cuenta una
// iteracion por usuario revisado (se detiene al encontrar coincidencia).
// Devuelve nullptr si no hay coincidencia.
Usuario* autenticar(const ListaDinamica<Usuario*>& usuarios,
                    const std::string& nombreUsuario,
                    const std::string& clave,
                    MedidorRecursos& medidor);

class SesionLogin
{
private:
    Usuario* usuarioActivo;
    bool refrescoDisponible;
    ListaDinamica<Usuario*> ultimaSeleccion;

    // Peso segun cuantos amigos tiene el candidato en comun con el usuario activo.
    static double pesoPorAmigosComunes(int cantidadComunes);

    int contarAmigosComunes(Usuario* candidato) const;

    // Sorteo ponderado sin repeticion sobre los amigos del usuario activo.
    // Si hay menos de 3 amigos, devuelve los que haya (sin repetirlos).
    ListaDinamica<Usuario*> sortearFotos(MedidorRecursos& medidor);

public:
    explicit SesionLogin(Usuario* usuario);

    // Primer conjunto de hasta 3 fotos, mostrado justo al iniciar sesion.
    ListaDinamica<Usuario*> fotosIniciales(MedidorRecursos& medidor);

    // Unico refresco permitido por sesion. Si ya se uso, devuelve la misma
    // seleccion anterior sin volver a sortear.
    ListaDinamica<Usuario*> refrescarFotos(MedidorRecursos& medidor);

    bool tieneRefrescoDisponible() const;
    bool usuarioTieneMenosDeTresAmigos() const;
};

#endif // SESION_H