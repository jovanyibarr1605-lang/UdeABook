#include "UdeABook.h"

#include <filesystem>

#include "Foto.h"
#include "Persistencia.h"
#include "Sesion.h"

namespace
{
std::string numeroConCeros(int numero, int digitos)
{
    std::string texto = std::to_string(numero);
    while (static_cast<int>(texto.size()) < digitos)
    {
        texto = "0" + texto;
    }
    return texto;
}

bool tieneContenido(const std::string& texto)
{
    for (int i = 0; i < static_cast<int>(texto.size()); i++)
    {
        if (texto[i] != ' ' && texto[i] != '\t' && texto[i] != '\n')
        {
            return true;
        }
    }
    return false;
}
}

UdeABook::UdeABook()
{
}

UdeABook::~UdeABook()
{
    liberarDatos();
}

void UdeABook::liberarDatos()
{
    for (int i = 0; i < mensajes.getCantidad(); i++)
    {
        delete mensajes.obtener(i);
    }
    mensajes.limpiar();

    for (int i = 0; i < usuarios.getCantidad(); i++)
    {
        delete usuarios.obtener(i);
    }
    usuarios.limpiar();
}

bool UdeABook::cargarDatos(const std::string& carpeta, MedidorRecursos& medidor)
{
    liberarDatos();
    carpetaDatos = carpeta;
    usuarios = Persistencia::cargarUsuarios(Persistencia::unirRuta(carpeta, "usuarios.txt"), medidor);
    if (usuarios.estaVacia())
    {
        actualizarMedicion(medidor);
        return false;
    }

    Persistencia::cargarAmistades(Persistencia::unirRuta(carpeta, "amistades.txt"), usuarios, medidor);
    Persistencia::cargarSolicitudes(Persistencia::unirRuta(carpeta, "solicitudes.txt"), usuarios, medidor);
    Persistencia::cargarAlbumes(Persistencia::unirRuta(carpeta, "albumes.txt"), usuarios, medidor);
    Persistencia::cargarFotos(Persistencia::unirRuta(carpeta, "fotos.txt"), usuarios, medidor);
    Persistencia::cargarMensajes(Persistencia::unirRuta(carpeta, "mensajes.txt"), usuarios, mensajes, medidor);
    actualizarMedicion(medidor);
    return true;
}

void UdeABook::guardarDatos() const
{
    if (carpetaDatos.empty())
    {
        return;
    }

    Persistencia::guardarUsuarios(Persistencia::unirRuta(carpetaDatos, "usuarios.txt"), usuarios);
    Persistencia::guardarAmistades(Persistencia::unirRuta(carpetaDatos, "amistades.txt"), usuarios);
    Persistencia::guardarSolicitudes(Persistencia::unirRuta(carpetaDatos, "solicitudes.txt"), usuarios);
    Persistencia::guardarAlbumes(Persistencia::unirRuta(carpetaDatos, "albumes.txt"), usuarios);
    Persistencia::guardarFotos(Persistencia::unirRuta(carpetaDatos, "fotos.txt"), usuarios);
    Persistencia::guardarMensajes(Persistencia::unirRuta(carpetaDatos, "mensajes.txt"), mensajes);
}

void UdeABook::generarDatosDemostracion(int cantidadUsuarios, MedidorRecursos& medidor)
{
    liberarDatos();
    if (cantidadUsuarios < 2)
    {
        cantidadUsuarios = 2;
    }

    carpetaDatos = "datos";
    std::filesystem::create_directories(carpetaDatos);

    for (int i = 1; i <= cantidadUsuarios; i++)
    {
        medidor.contarIteracion();
        std::string consecutivo = numeroConCeros(i, 4);
        std::string nombre = "usuario" + consecutivo;
        std::string rutaPerfil = "/users/subruta-fija/" + nombre + "/albums/profile.png";
        usuarios.agregar(new Usuario(nombre, "2000-01-01", nombre + "@udea.edu.co", rutaPerfil,
                                     "clave123", "Medellin", "Colombia", "2026-01-01 08:00:00"));
    }

    for (int i = 0; i < usuarios.getCantidad(); i++)
    {
        for (int salto = 1; salto <= 25 && i + salto < usuarios.getCantidad(); salto++)
        {
            medidor.contarIteracion();
            Usuario* origen = usuarios.obtener(i);
            Usuario* destino = usuarios.obtener(i + salto);
            origen->agregarAmigo(destino);
            destino->agregarAmigo(origen);
        }
    }

    Usuario* usuarioDos = usuarios.obtener(1);
    for (int albumNumero = 1; albumNumero <= 2; albumNumero++)
    {
        std::string nombreAlbum = "Viaje" + std::to_string(albumNumero);
        Album* album = new Album(nombreAlbum,
                                 "/users/subruta-fija/usuario0002/albums/" + nombreAlbum,
                                 albumNumero == 1 ? Privacidad::Amigos : Privacidad::Publico);
        for (int fotoNumero = 1; fotoNumero <= 5; fotoNumero++)
        {
            medidor.contarIteracion();
            std::string ruta = album->getRutaAlmacenamiento() + "/foto" +
                               std::to_string(fotoNumero) + ".png";
            album->agregarFoto(new Foto("2026-01-" + numeroConCeros(fotoNumero, 2),
                                       1500000 + fotoNumero * 1000, ruta, fotoNumero - 1));
        }
        usuarioDos->agregarAlbum(album);
    }

    Usuario* usuarioUno = usuarios.obtener(0);
    for (int i = 0; i < 12; i++)
    {
        medidor.contarIteracion();
        int mes = i < 6 ? 1 : 2;
        int dia = i < 6 ? i * 5 + 1 : (i - 6) * 5 + 1;
        std::string fecha = "2026-" + numeroConCeros(mes, 2) + "-" + numeroConCeros(dia, 2) +
                            " 10:00:00";
        Mensaje* mensaje = new Mensaje(usuarioDos, usuarioUno, fecha,
                                       "Mensaje de demostracion " + std::to_string(i + 1), false);
        mensajes.agregar(mensaje);
        usuarioDos->agregarMensajeEnviado(mensaje);
        usuarioUno->agregarMensajeRecibido(mensaje);
    }

    guardarDatos();
    actualizarMedicion(medidor);
}

Usuario* UdeABook::iniciarSesion(const std::string& nombreUsuario, const std::string& clave,
                                 MedidorRecursos& medidor) const
{
    Usuario* usuario = autenticar(usuarios, nombreUsuario, clave, medidor);
    actualizarMedicion(medidor, sizeof(nombreUsuario) + sizeof(clave));
    return usuario;
}

Usuario* UdeABook::buscarUsuario(const std::string& nombreUsuario, MedidorRecursos& medidor) const
{
    for (int i = 0; i < usuarios.getCantidad(); i++)
    {
        medidor.contarIteracion();
        if (usuarios.obtener(i)->getNombreUsuario() == nombreUsuario)
        {
            actualizarMedicion(medidor, sizeof(nombreUsuario));
            return usuarios.obtener(i);
        }
    }
    actualizarMedicion(medidor, sizeof(nombreUsuario));
    return nullptr;
}

bool UdeABook::enviarSolicitud(Usuario* remitente, const std::string& nombreDestino,
                               MedidorRecursos& medidor)
{
    Usuario* destino = buscarUsuario(nombreDestino, medidor);
    if (remitente == nullptr || destino == nullptr || remitente == destino ||
        remitente->tieneAmigo(destino, &medidor) ||
        remitente->tieneSolicitudPendienteDe(destino, &medidor) ||
        destino->tieneSolicitudPendienteDe(remitente, &medidor))
    {
        actualizarMedicion(medidor, sizeof(remitente) + sizeof(destino) + sizeof(nombreDestino));
        return false;
    }

    remitente->enviarSolicitud(*destino, &medidor);
    actualizarMedicion(medidor, sizeof(remitente) + sizeof(destino) + sizeof(nombreDestino));
    return destino->tieneSolicitudPendienteDe(remitente, &medidor);
}

bool UdeABook::aceptarSolicitud(Usuario* destinatario, const std::string& nombreSolicitante,
                                MedidorRecursos& medidor)
{
    Usuario* solicitante = buscarUsuario(nombreSolicitante, medidor);
    if (destinatario == nullptr || solicitante == nullptr ||
        !destinatario->tieneSolicitudPendienteDe(solicitante, &medidor))
    {
        actualizarMedicion(medidor, sizeof(destinatario) + sizeof(solicitante) +
                                      sizeof(nombreSolicitante));
        return false;
    }

    destinatario->aceptarSolicitud(*solicitante, &medidor);
    actualizarMedicion(medidor, sizeof(destinatario) + sizeof(solicitante) +
                                  sizeof(nombreSolicitante));
    return destinatario->tieneAmigo(solicitante, &medidor);
}

ListaDinamica<Usuario*> UdeABook::sugerirAmigos(Usuario* usuario, int cantidad,
                                                 MedidorRecursos& medidor) const
{
    ListaDinamica<Usuario*> candidatos;
    ListaDinamica<int> coincidencias;
    ListaDinamica<Usuario*> resultado;

    if (usuario == nullptr || cantidad < 1)
    {
        actualizarMedicion(medidor, sizeof(usuario) + sizeof(cantidad));
        return resultado;
    }
    if (cantidad > 20)
    {
        cantidad = 20;
    }

    const ListaDinamica<Usuario*>& amigos = usuario->getAmigos();
    for (int i = 0; i < amigos.getCantidad(); i++)
    {
        Usuario* amigo = amigos.obtener(i);
        const ListaDinamica<Usuario*>& amigosDelAmigo = amigo->getAmigos();
        for (int j = 0; j < amigosDelAmigo.getCantidad(); j++)
        {
            medidor.contarIteracion();
            Usuario* candidato = amigosDelAmigo.obtener(j);
            if (candidato == usuario || usuario->tieneAmigo(candidato, &medidor))
            {
                continue;
            }

            int indice = -1;
            for (int k = 0; k < candidatos.getCantidad(); k++)
            {
                medidor.contarIteracion();
                if (candidatos.obtener(k) == candidato)
                {
                    indice = k;
                    break;
                }
            }
            if (indice == -1)
            {
                candidatos.agregar(candidato);
                coincidencias.agregar(1);
            }
            else
            {
                coincidencias.obtener(indice)++;
            }
        }
    }

    for (int i = 0; i < candidatos.getCantidad() - 1; i++)
    {
        for (int j = i + 1; j < candidatos.getCantidad(); j++)
        {
            medidor.contarIteracion();
            if (coincidencias.obtener(j) > coincidencias.obtener(i))
            {
                int coincidenciaTemporal = coincidencias.obtener(i);
                coincidencias.obtener(i) = coincidencias.obtener(j);
                coincidencias.obtener(j) = coincidenciaTemporal;
                Usuario* usuarioTemporal = candidatos.obtener(i);
                candidatos.obtener(i) = candidatos.obtener(j);
                candidatos.obtener(j) = usuarioTemporal;
            }
        }
    }

    int limite = candidatos.getCantidad() < cantidad ? candidatos.getCantidad() : cantidad;
    for (int i = 0; i < limite; i++)
    {
        resultado.agregar(candidatos.obtener(i));
    }
    actualizarMedicion(medidor, sizeof(usuario) + sizeof(cantidad));
    return resultado;
}

ListaDinamica<Album*> UdeABook::albumesVisibles(Usuario* visitante,
                                                 const std::string& nombreAmigo,
                                                 MedidorRecursos& medidor) const
{
    ListaDinamica<Album*> visibles;
    Usuario* amigo = buscarUsuario(nombreAmigo, medidor);
    if (visitante == nullptr || amigo == nullptr || !visitante->tieneAmigo(amigo, &medidor))
    {
        actualizarMedicion(medidor, sizeof(visitante) + sizeof(amigo) + sizeof(nombreAmigo));
        return visibles;
    }

    const ListaDinamica<Album*>& albumes = amigo->getAlbumes();
    for (int i = 0; i < albumes.getCantidad(); i++)
    {
        medidor.contarIteracion();
        if (albumes.obtener(i)->esVisiblePara(visitante, &medidor))
        {
            visibles.agregar(albumes.obtener(i));
        }
    }
    actualizarMedicion(medidor, sizeof(visitante) + sizeof(amigo) + sizeof(nombreAmigo));
    return visibles;
}

bool UdeABook::cambiarLike(Usuario* visitante, Album* album, int indiceFoto, bool agregar,
                           MedidorRecursos& medidor)
{
    if (visitante == nullptr || album == nullptr || !album->esVisiblePara(visitante, &medidor) ||
        indiceFoto < 0 || indiceFoto >= album->getFotos().getCantidad())
    {
        actualizarMedicion(medidor, sizeof(visitante) + sizeof(album) + sizeof(indiceFoto) +
                                      sizeof(agregar));
        return false;
    }

    if (agregar)
    {
        album->getFotos().obtener(indiceFoto)->darLike();
    }
    else
    {
        album->getFotos().obtener(indiceFoto)->quitarLike();
    }
    medidor.contarIteracion();
    actualizarMedicion(medidor, sizeof(visitante) + sizeof(album) + sizeof(indiceFoto) +
                                  sizeof(agregar));
    return true;
}

bool UdeABook::enviarMensaje(Usuario* emisor, const std::string& nombreReceptor,
                             const std::string& texto, const std::string& fecha,
                             MedidorRecursos& medidor)
{
    Usuario* receptor = buscarUsuario(nombreReceptor, medidor);
    if (emisor == nullptr || receptor == nullptr || !emisor->tieneAmigo(receptor, &medidor) ||
        texto.size() > 512 || !tieneContenido(texto))
    {
        actualizarMedicion(medidor, sizeof(emisor) + sizeof(receptor) + sizeof(nombreReceptor) +
                                      sizeof(texto) + sizeof(fecha));
        return false;
    }

    Mensaje* mensaje = new Mensaje(emisor, receptor, fecha, texto);
    mensajes.agregar(mensaje);
    emisor->agregarMensajeEnviado(mensaje);
    receptor->agregarMensajeRecibido(mensaje);
    medidor.contarIteracion();
    actualizarMedicion(medidor, sizeof(emisor) + sizeof(receptor) + sizeof(nombreReceptor) +
                                  sizeof(texto) + sizeof(fecha));
    return true;
}

void UdeABook::ordenarPorFecha(ListaDinamica<Mensaje*>& lista)
{
    for (int i = 0; i < lista.getCantidad() - 1; i++)
    {
        for (int j = i + 1; j < lista.getCantidad(); j++)
        {
            if (lista.obtener(j)->getFechaEnvio() < lista.obtener(i)->getFechaEnvio())
            {
                Mensaje* temporal = lista.obtener(i);
                lista.obtener(i) = lista.obtener(j);
                lista.obtener(j) = temporal;
            }
        }
    }
}

ListaDinamica<Mensaje*> UdeABook::leerMensajesNuevos(Usuario* usuario,
                                                      MedidorRecursos& medidor)
{
    ListaDinamica<Mensaje*> nuevos;
    if (usuario == nullptr)
    {
        actualizarMedicion(medidor, sizeof(usuario));
        return nuevos;
    }

    ListaDinamica<Mensaje*>& recibidos = usuario->getMensajesRecibidos();
    for (int i = 0; i < recibidos.getCantidad(); i++)
    {
        medidor.contarIteracion();
        if (!recibidos.obtener(i)->estaLeido())
        {
            nuevos.agregar(recibidos.obtener(i));
            recibidos.obtener(i)->marcarComoLeido();
        }
    }
    ordenarPorFecha(nuevos);
    actualizarMedicion(medidor, sizeof(usuario));
    return nuevos;
}

ListaDinamica<Mensaje*> UdeABook::buscarMensajesPorFecha(Usuario* usuario,
                                                          const std::string& fechaInicio,
                                                          const std::string& fechaFin,
                                                          MedidorRecursos& medidor) const
{
    ListaDinamica<Mensaje*> encontrados;
    if (usuario == nullptr || fechaInicio > fechaFin)
    {
        actualizarMedicion(medidor, sizeof(usuario) + sizeof(fechaInicio) + sizeof(fechaFin));
        return encontrados;
    }

    const ListaDinamica<Mensaje*>& recibidos = usuario->getMensajesRecibidos();
    for (int i = 0; i < recibidos.getCantidad(); i++)
    {
        medidor.contarIteracion();
        const std::string& fecha = recibidos.obtener(i)->getFechaEnvio();
        if (fecha >= fechaInicio && fecha <= fechaFin)
        {
            encontrados.agregar(recibidos.obtener(i));
        }
    }
    ordenarPorFecha(encontrados);
    actualizarMedicion(medidor, sizeof(usuario) + sizeof(fechaInicio) + sizeof(fechaFin));
    return encontrados;
}

std::size_t UdeABook::memoriaTexto(const std::string& texto)
{
    return texto.capacity() + 1;
}

std::size_t UdeABook::calcularMemoriaEstructuras() const
{
    std::size_t total = sizeof(UdeABook) + usuarios.getCapacidad() * sizeof(Usuario*) +
                        mensajes.getCapacidad() * sizeof(Mensaje*);

    for (int i = 0; i < usuarios.getCantidad(); i++)
    {
        const Usuario* usuario = usuarios.obtener(i);
        total += sizeof(Usuario);
        total += memoriaTexto(usuario->getNombreUsuario()) + memoriaTexto(usuario->getFechaNacimiento()) +
                 memoriaTexto(usuario->getCorreo()) + memoriaTexto(usuario->getFotoPerfil()) +
                 memoriaTexto(usuario->getClave()) + memoriaTexto(usuario->getCiudad()) +
                 memoriaTexto(usuario->getPais()) + memoriaTexto(usuario->getFechaInscripcion());
        total += usuario->getAmigos().getCapacidad() * sizeof(Usuario*);
        total += usuario->getSolicitudes().getCapacidad() * sizeof(Usuario*);
        total += usuario->getMensajesEnviados().getCapacidad() * sizeof(Mensaje*);
        total += usuario->getMensajesRecibidos().getCapacidad() * sizeof(Mensaje*);
        total += usuario->getAlbumes().getCapacidad() * sizeof(Album*);

        const ListaDinamica<Album*>& albumes = usuario->getAlbumes();
        for (int j = 0; j < albumes.getCantidad(); j++)
        {
            const Album* album = albumes.obtener(j);
            total += sizeof(Album) + memoriaTexto(album->getNombre()) +
                     memoriaTexto(album->getRutaAlmacenamiento()) +
                     album->getFotos().getCapacidad() * sizeof(Foto*);
            const ListaDinamica<Foto*>& fotos = album->getFotos();
            for (int k = 0; k < fotos.getCantidad(); k++)
            {
                total += sizeof(Foto) + memoriaTexto(fotos.obtener(k)->getFechaCaptura()) +
                         memoriaTexto(fotos.obtener(k)->getRutaArchivo());
            }
        }
    }

    for (int i = 0; i < mensajes.getCantidad(); i++)
    {
        total += sizeof(Mensaje) + memoriaTexto(mensajes.obtener(i)->getFechaEnvio()) +
                 memoriaTexto(mensajes.obtener(i)->getTexto());
    }
    return total;
}

void UdeABook::actualizarMedicion(MedidorRecursos& medidor, std::size_t memoriaLocal) const
{
    medidor.fijarMemoriaActual(calcularMemoriaEstructuras() + memoriaLocal);
}

const ListaDinamica<Usuario*>& UdeABook::getUsuarios() const { return usuarios; }
const ListaDinamica<Mensaje*>& UdeABook::getMensajes() const { return mensajes; }
