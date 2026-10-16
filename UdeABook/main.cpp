#include <iostream>
#include <string>

#include "Foto.h"
#include "Sesion.h"
#include "UdeABook.h"

namespace
{
std::string leerTexto(const std::string& etiqueta)
{
    std::cout << etiqueta;
    std::string texto;
    std::getline(std::cin, texto);
    return texto;
}

int leerEntero(const std::string& etiqueta, int valorPorDefecto = -1)
{
    std::string texto = leerTexto(etiqueta);
    if (texto.empty())
    {
        return valorPorDefecto;
    }

    int resultado = 0;
    int indice = 0;
    bool negativo = false;
    if (texto[0] == '-')
    {
        negativo = true;
        indice = 1;
    }
    for (; indice < static_cast<int>(texto.size()); indice++)
    {
        if (texto[indice] < '0' || texto[indice] > '9')
        {
            return valorPorDefecto;
        }
        resultado = resultado * 10 + (texto[indice] - '0');
    }
    return negativo ? -resultado : resultado;
}

void mostrarMedicion(const MedidorRecursos& medidor)
{
    std::cout << "Iteraciones: " << medidor.getIteraciones()
              << " | Memoria estimada actual: " << medidor.getMemoriaActual()
              << " bytes" << std::endl;
}

void mostrarMensajes(const ListaDinamica<Mensaje*>& mensajes)
{
    if (mensajes.estaVacia())
    {
        std::cout << "No hay mensajes para mostrar." << std::endl;
        return;
    }

    for (int i = 0; i < mensajes.getCantidad(); i++)
    {
        Mensaje* mensaje = mensajes.obtener(i);
        std::cout << "[" << mensaje->getFechaEnvio() << "] "
                  << mensaje->getEmisor()->getNombreUsuario() << ": "
                  << mensaje->getTexto() << std::endl;
    }
}

int amigosComunes(const Usuario* primero, const Usuario* segundo, MedidorRecursos& medidor)
{
    int cantidad = 0;
    const ListaDinamica<Usuario*>& amigos = primero->getAmigos();
    for (int i = 0; i < amigos.getCantidad(); i++)
    {
        medidor.contarIteracion();
        if (segundo->tieneAmigo(amigos.obtener(i), &medidor))
        {
            cantidad++;
        }
    }
    return cantidad;
}

void mostrarFotosPerfil(const ListaDinamica<Usuario*>& fotos)
{
    if (fotos.estaVacia())
    {
        std::cout << "No hay amigos disponibles para mostrar." << std::endl;
        return;
    }

    std::cout << "Fotos de perfil seleccionadas:" << std::endl;
    for (int i = 0; i < fotos.getCantidad(); i++)
    {
        std::cout << " - " << fotos.obtener(i)->getNombreUsuario() << ": "
                  << fotos.obtener(i)->getFotoPerfil() << std::endl;
    }
}

void administrarAmistades(UdeABook& plataforma, Usuario* usuarioActivo)
{
    MedidorRecursos medidor;
    while (true)
    {
        std::cout << "\n--- Administrar amistades ---\n"
                  << "1. Enviar solicitud\n"
                  << "2. Ver y aceptar solicitudes recibidas\n"
                  << "0. Volver\n";
        int opcion = leerEntero("Opcion: ");
        if (opcion == 0)
        {
            return;
        }

        if (opcion == 1)
        {
            std::string destino = leerTexto("Nombre de usuario destino: ");
            medidor.reiniciar();
            bool enviada = plataforma.enviarSolicitud(usuarioActivo, destino, medidor);
            if (enviada)
            {
                plataforma.guardarDatos();
                std::cout << "Solicitud enviada." << std::endl;
            }
            else
            {
                std::cout << "No se puede enviar la solicitud. Verifica que exista, que no sea "
                          << "tu propio usuario y que no haya una amistad o solicitud pendiente."
                          << std::endl;
            }
            mostrarMedicion(medidor);
        }
        else if (opcion == 2)
        {
            const ListaDinamica<Usuario*>& solicitudes = usuarioActivo->getSolicitudes();
            if (solicitudes.estaVacia())
            {
                std::cout << "No tienes solicitudes pendientes." << std::endl;
                continue;
            }

            std::cout << "Solicitudes recibidas:" << std::endl;
            for (int i = 0; i < solicitudes.getCantidad(); i++)
            {
                std::cout << " - " << solicitudes.obtener(i)->getNombreUsuario() << std::endl;
            }
            std::string solicitante = leerTexto("Escribe el usuario a aceptar o deja vacio para volver: ");
            if (solicitante.empty())
            {
                continue;
            }
            medidor.reiniciar();
            bool aceptada = plataforma.aceptarSolicitud(usuarioActivo, solicitante, medidor);
            if (aceptada)
            {
                plataforma.guardarDatos();
                std::cout << "Amistad creada correctamente." << std::endl;
            }
            else
            {
                std::cout << "No fue posible aceptar esa solicitud." << std::endl;
            }
            mostrarMedicion(medidor);
        }
        else
        {
            std::cout << "Opcion invalida." << std::endl;
        }
    }
}

void visitarPerfil(UdeABook& plataforma, Usuario* usuarioActivo)
{
    std::string nombreAmigo = leerTexto("Nombre del amigo a visitar: ");
    MedidorRecursos medidor;
    ListaDinamica<Album*> albumes = plataforma.albumesVisibles(usuarioActivo, nombreAmigo, medidor);
    if (albumes.estaVacia())
    {
        std::cout << "No hay albumes visibles o el usuario no es tu amigo." << std::endl;
        mostrarMedicion(medidor);
        return;
    }

    std::cout << "Albumes visibles de " << nombreAmigo << ":" << std::endl;
    for (int i = 0; i < albumes.getCantidad(); i++)
    {
        std::cout << i + 1 << ". " << albumes.obtener(i)->getNombre() << std::endl;
    }
    mostrarMedicion(medidor);

    int opcionAlbum = leerEntero("Numero de album: ");
    if (opcionAlbum < 1 || opcionAlbum > albumes.getCantidad())
    {
        std::cout << "Album invalido." << std::endl;
        return;
    }

    Album* album = albumes.obtener(opcionAlbum - 1);
    if (album->getFotos().estaVacia())
    {
        std::cout << "El album no tiene fotos." << std::endl;
        return;
    }

    int indice = 0;
    while (true)
    {
        Foto* foto = album->getFotos().obtener(indice);
        std::cout << "\nFoto " << indice + 1 << "/" << album->getFotos().getCantidad()
                  << "\nRuta: " << foto->getRutaArchivo()
                  << "\nLikes: " << foto->getConteoLikes()
                  << "\n1. Siguiente  2. Previa  3. Dar like  4. Quitar like  0. Salir\n";
        int opcion = leerEntero("Opcion: ");
        if (opcion == 0)
        {
            return;
        }
        if (opcion == 1)
        {
            indice = (indice + 1) % album->getFotos().getCantidad();
        }
        else if (opcion == 2)
        {
            indice = (indice - 1 + album->getFotos().getCantidad()) %
                     album->getFotos().getCantidad();
        }
        else if (opcion == 3 || opcion == 4)
        {
            medidor.reiniciar();
            if (plataforma.cambiarLike(usuarioActivo, album, indice, opcion == 3, medidor))
            {
                plataforma.guardarDatos();
                std::cout << (opcion == 3 ? "Like registrado." : "Like retirado.") << std::endl;
            }
            mostrarMedicion(medidor);
        }
        else
        {
            std::cout << "Opcion invalida." << std::endl;
        }
    }
}

void verSugerencias(UdeABook& plataforma, Usuario* usuarioActivo)
{
    int cantidad = leerEntero("Cantidad de sugerencias (1 a 20): ");
    if (cantidad < 1 || cantidad > 20)
    {
        std::cout << "Debes ingresar un valor entre 1 y 20." << std::endl;
        return;
    }

    MedidorRecursos medidor;
    ListaDinamica<Usuario*> sugerencias = plataforma.sugerirAmigos(usuarioActivo, cantidad, medidor);
    if (sugerencias.estaVacia())
    {
        std::cout << "No se encontraron sugerencias." << std::endl;
        mostrarMedicion(medidor);
        return;
    }

    std::cout << "Sugerencias ordenadas por amigos en comun:" << std::endl;
    for (int i = 0; i < sugerencias.getCantidad(); i++)
    {
        std::cout << i + 1 << ". " << sugerencias.obtener(i)->getNombreUsuario()
                  << " (" << amigosComunes(usuarioActivo, sugerencias.obtener(i), medidor)
                  << " amigos en comun)" << std::endl;
    }
    mostrarMedicion(medidor);

    int elegida = leerEntero("Numero para enviar solicitud o 0 para volver: ", 0);
    if (elegida < 1 || elegida > sugerencias.getCantidad())
    {
        return;
    }
    medidor.reiniciar();
    bool enviada = plataforma.enviarSolicitud(usuarioActivo,
                                                sugerencias.obtener(elegida - 1)->getNombreUsuario(),
                                                medidor);
    if (enviada)
    {
        plataforma.guardarDatos();
        std::cout << "Solicitud enviada." << std::endl;
    }
    else
    {
        std::cout << "No fue posible enviar la solicitud." << std::endl;
    }
    mostrarMedicion(medidor);
}

void administrarMensajes(UdeABook& plataforma, Usuario* usuarioActivo)
{
    MedidorRecursos medidor;
    while (true)
    {
        std::cout << "\n--- Administrar mensajes ---\n"
                  << "1. Enviar mensaje\n"
                  << "2. Leer mensajes nuevos\n"
                  << "3. Buscar mensajes por rango de fecha\n"
                  << "0. Volver\n";
        int opcion = leerEntero("Opcion: ");
        if (opcion == 0)
        {
            return;
        }

        if (opcion == 1)
        {
            std::string receptor = leerTexto("Nombre del amigo: ");
            std::string texto = leerTexto("Mensaje (maximo 512 caracteres): ");
            std::string fecha = leerTexto("Fecha YYYY-MM-DD HH:MM:SS (vacio = 2026-10-15 12:00:00): ");
            if (fecha.empty())
            {
                fecha = "2026-10-15 12:00:00";
            }
            medidor.reiniciar();
            bool enviado = plataforma.enviarMensaje(usuarioActivo, receptor, texto, fecha, medidor);
            if (enviado)
            {
                plataforma.guardarDatos();
                std::cout << "Mensaje enviado." << std::endl;
            }
            else
            {
                std::cout << "No se puede enviar: el receptor debe ser un amigo y el texto no "
                          << "puede estar vacio ni superar 512 caracteres." << std::endl;
            }
            mostrarMedicion(medidor);
        }
        else if (opcion == 2)
        {
            medidor.reiniciar();
            ListaDinamica<Mensaje*> nuevos = plataforma.leerMensajesNuevos(usuarioActivo, medidor);
            mostrarMensajes(nuevos);
            if (!nuevos.estaVacia())
            {
                plataforma.guardarDatos();
            }
            mostrarMedicion(medidor);
        }
        else if (opcion == 3)
        {
            std::string inicio = leerTexto("Fecha inicial YYYY-MM-DD HH:MM:SS: ");
            std::string fin = leerTexto("Fecha final YYYY-MM-DD HH:MM:SS: ");
            medidor.reiniciar();
            ListaDinamica<Mensaje*> encontrados =
                plataforma.buscarMensajesPorFecha(usuarioActivo, inicio, fin, medidor);
            mostrarMensajes(encontrados);
            mostrarMedicion(medidor);
        }
        else
        {
            std::cout << "Opcion invalida." << std::endl;
        }
    }
}

void ejecutarSesion(UdeABook& plataforma, Usuario* usuarioActivo)
{
    std::cout << "\nBienvenido, " << usuarioActivo->getNombreUsuario() << "." << std::endl;
    SesionLogin sesion(usuarioActivo);
    MedidorRecursos medidorInicio;
    ListaDinamica<Usuario*> fotos = sesion.fotosIniciales(medidorInicio);
    plataforma.actualizarMedicion(medidorInicio, sizeof(SesionLogin));
    mostrarFotosPerfil(fotos);
    mostrarMedicion(medidorInicio);

    while (true)
    {
        std::cout << "\n=== Ude@Book ===\n"
                  << "1. Administrar amistades\n"
                  << "2. Visitar perfil de un amigo\n"
                  << "3. Ver sugerencias de amistad\n"
                  << "4. Administrar mensajes\n"
                  << "5. Actualizar fotos de perfil de amigos\n"
                  << "0. Cerrar sesion\n";
        int opcion = leerEntero("Opcion: ");
        if (opcion == 0)
        {
            plataforma.guardarDatos();
            return;
        }
        if (opcion == 1)
        {
            administrarAmistades(plataforma, usuarioActivo);
        }
        else if (opcion == 2)
        {
            visitarPerfil(plataforma, usuarioActivo);
        }
        else if (opcion == 3)
        {
            verSugerencias(plataforma, usuarioActivo);
        }
        else if (opcion == 4)
        {
            administrarMensajes(plataforma, usuarioActivo);
        }
        else if (opcion == 5)
        {
            if (!sesion.tieneRefrescoDisponible())
            {
                std::cout << "Ya utilizaste la unica actualizacion permitida." << std::endl;
                continue;
            }
            MedidorRecursos medidor;
            ListaDinamica<Usuario*> refrescadas = sesion.refrescarFotos(medidor);
            plataforma.actualizarMedicion(medidor, sizeof(SesionLogin));
            mostrarFotosPerfil(refrescadas);
            mostrarMedicion(medidor);
        }
        else
        {
            std::cout << "Opcion invalida." << std::endl;
        }
    }
}
}

int main()
{
    UdeABook plataforma;
    MedidorRecursos medidorCarga;
    if (!plataforma.cargarDatos("datos", medidorCarga))
    {
        medidorCarga.reiniciar();
        plataforma.generarDatosDemostracion(1000, medidorCarga);
        std::cout << "Se generaron datos de demostracion con 1000 usuarios.\n"
                  << "Credenciales iniciales: usuario0001 / clave123" << std::endl;
    }
    else
    {
        std::cout << "Datos cargados: " << plataforma.getUsuarios().getCantidad()
                  << " usuarios y " << plataforma.getMensajes().getCantidad()
                  << " mensajes." << std::endl;
    }
    mostrarMedicion(medidorCarga);

    while (true)
    {
        std::cout << "\n--- Ingreso a Ude@Book ---\n"
                  << "Deja el usuario vacio para terminar." << std::endl;
        std::string nombreUsuario = leerTexto("Usuario: ");
        if (nombreUsuario.empty())
        {
            break;
        }
        std::string clave = leerTexto("Clave: ");

        MedidorRecursos medidorLogin;
        Usuario* usuarioActivo = plataforma.iniciarSesion(nombreUsuario, clave, medidorLogin);
        if (usuarioActivo == nullptr)
        {
            std::cout << "Credenciales invalidas." << std::endl;
            mostrarMedicion(medidorLogin);
            continue;
        }
        ejecutarSesion(plataforma, usuarioActivo);
    }

    plataforma.guardarDatos();
    std::cout << "Datos guardados. Hasta pronto." << std::endl;
    return 0;
}
