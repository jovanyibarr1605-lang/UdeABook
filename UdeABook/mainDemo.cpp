#include <iostream>
#include <fstream>
#include "ListaDinamica.h"
#include "MedidorRecursos.h"
#include "Usuario.h"
#include "Persistencia.h"
#include "Sesion.h"

// Demo de integracion de la parte de Persona A:
//   cargar usuarios.txt -> iniciar sesion -> mostrar 3 fotos ponderadas -> guardar
//
// IMPORTANTE: la carga de amistades.txt todavia no existe (la hace tu
// companero), asi que aqui armamos las amistades a mano, simulando lo que
// su loader va a hacer despues. Cuando el tenga su Persistencia de
// amistades.txt lista, ese paso se reemplaza por su funcion.

static void crearUsuariosTxtDeEjemplo()
{
    ListaDinamica<Usuario*> usuarios;
    usuarios.agregar(new Usuario("ana", "2000-05-01", "ana@mail.com", "fotos/ana.png", "clave123", "Medellin", "Colombia", "2026-01-10 10:00:00"));
    usuarios.agregar(new Usuario("luis", "1999-02-20", "luis@mail.com", "fotos/luis.png", "clave456", "Bogota", "Colombia", "2026-01-11 09:00:00"));
    usuarios.agregar(new Usuario("sofia", "2001-07-15", "sofia@mail.com", "fotos/sofia.png", "clave789", "Cali", "Colombia", "2026-01-12 11:00:00"));
    usuarios.agregar(new Usuario("juan", "1998-03-10", "juan@mail.com", "fotos/juan.png", "claveabc", "Pereira", "Colombia", "2026-01-13 08:30:00"));
    usuarios.agregar(new Usuario("maria", "2002-11-05", "maria@mail.com", "fotos/maria.png", "clavexyz", "Cartagena", "Colombia", "2026-01-14 14:00:00"));

    Persistencia::guardarUsuarios("usuarios.txt", usuarios);

    for (int i = 0; i < usuarios.getCantidad(); i++)
    {
        delete usuarios.obtener(i);
    }
}

int main()
{
    // 1) Si usuarios.txt no existe todavia (primera corrida), se crea uno de ejemplo.
    std::ifstream prueba("usuarios.txt");
    if (!prueba.good())
    {
        crearUsuariosTxtDeEjemplo();
        std::cout << "Se creo usuarios.txt de ejemplo (5 usuarios)." << std::endl;
    }
    prueba.close();

    // 2) Carga real con el MedidorRecursos (funcion I).
    MedidorRecursos medidorCarga;
    ListaDinamica<Usuario*> usuarios = Persistencia::cargarUsuarios("usuarios.txt", medidorCarga);

    std::cout << "\n=== Carga ===" << std::endl;
    std::cout << "Usuarios cargados: " << usuarios.getCantidad() << std::endl;
    std::cout << "Iteraciones: " << medidorCarga.getIteraciones()
              << " | Memoria: " << medidorCarga.getMemoriaActual() << " bytes" << std::endl;

    // 3) Amistades a mano (esto lo reemplaza el loader de amistades.txt de tu companero).
    Usuario* ana = Persistencia::buscarPorNombre(usuarios, "ana");
    Usuario* luis = Persistencia::buscarPorNombre(usuarios, "luis");
    Usuario* sofia = Persistencia::buscarPorNombre(usuarios, "sofia");
    Usuario* juan = Persistencia::buscarPorNombre(usuarios, "juan");
    Usuario* maria = Persistencia::buscarPorNombre(usuarios, "maria");

    if (ana && luis && sofia && juan && maria)
    {
        // ana es amiga de los otros 4 (amistad bidireccional: se agrega en ambos sentidos)
        Usuario* amigosDeAna[4] = { luis, sofia, juan, maria };
        for (int i = 0; i < 4; i++)
        {
            ana->getAmigos().agregar(amigosDeAna[i]);
            amigosDeAna[i]->getAmigos().agregar(ana);
        }
        // luis y sofia tambien son amigos entre si (para que tengan amigos en comun con ana)
        luis->getAmigos().agregar(sofia);
        sofia->getAmigos().agregar(luis);
    }

    // 4) Login (funcion II).
    std::cout << "\n=== Login ===" << std::endl;
    MedidorRecursos medidorLogin;
    Usuario* activo = autenticar(usuarios, "ana", "clave123", medidorLogin);

    if (activo == nullptr)
    {
        std::cout << "Credenciales invalidas." << std::endl;
    }
    else
    {
        std::cout << "Bienvenida, " << *activo << std::endl;

        SesionLogin sesion(activo);

        if (sesion.usuarioTieneMenosDeTresAmigos())
        {
            std::cout << "Tienes menos de 3 amigos; se muestran los disponibles." << std::endl;
        }

        ListaDinamica<Usuario*> fotos = sesion.fotosIniciales(medidorLogin);
        std::cout << "Fotos de perfil sugeridas: ";
        for (int i = 0; i < fotos.getCantidad(); i++)
        {
            std::cout << fotos.obtener(i)->getNombreUsuario() << "(" << fotos.obtener(i)->getFotoPerfil() << ") ";
        }
        std::cout << std::endl;

        std::cout << "Refresco disponible? " << (sesion.tieneRefrescoDisponible() ? "si" : "no") << std::endl;
        ListaDinamica<Usuario*> fotosRefrescadas = sesion.refrescarFotos(medidorLogin);
        std::cout << "Tras refrescar: ";
        for (int i = 0; i < fotosRefrescadas.getCantidad(); i++)
        {
            std::cout << fotosRefrescadas.obtener(i)->getNombreUsuario() << " ";
        }
        std::cout << std::endl;
        std::cout << "Refresco disponible? " << (sesion.tieneRefrescoDisponible() ? "si" : "no (ya se uso)") << std::endl;

        std::cout << "\nIteraciones totales de la funcionalidad de login: " << medidorLogin.getIteraciones()
                  << " | Memoria: " << medidorLogin.getMemoriaActual() << " bytes" << std::endl;
    }

    // 5) Guardar (demuestra que persistir de vuelta no rompe nada).
    Persistencia::guardarUsuarios("usuarios.txt", usuarios);

    // 6) Liberar memoria propia de la demo (UdeABook sera quien lo haga en el programa final).
    for (int i = 0; i < usuarios.getCantidad(); i++)
    {
        delete usuarios.obtener(i);
    }

    return 0;
}