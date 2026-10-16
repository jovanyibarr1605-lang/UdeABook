#include "Persistencia.h"

#include <fstream>

#include "Album.h"
#include "Foto.h"

namespace
{
std::string textoPrivacidad(Privacidad privacidad)
{
    if (privacidad == Privacidad::Publico)
    {
        return "publico";
    }
    if (privacidad == Privacidad::Amigos)
    {
        return "amigos";
    }
    return "privado";
}

Privacidad privacidadDesdeTexto(const std::string& texto)
{
    if (texto == "publico")
    {
        return Privacidad::Publico;
    }
    if (texto == "amigos")
    {
        return Privacidad::Amigos;
    }
    return Privacidad::Privado;
}

long long convertirEnteroLargo(const std::string& texto)
{
    long long resultado = 0;
    bool negativo = false;
    int indice = 0;
    if (!texto.empty() && texto[0] == '-')
    {
        negativo = true;
        indice = 1;
    }
    for (; indice < static_cast<int>(texto.size()); indice++)
    {
        char caracter = texto[indice];
        if (caracter < '0' || caracter > '9')
        {
            return 0;
        }
        resultado = resultado * 10 + (caracter - '0');
    }
    return negativo ? -resultado : resultado;
}

int convertirEntero(const std::string& texto)
{
    return static_cast<int>(convertirEnteroLargo(texto));
}

bool convertirBooleano(const std::string& texto)
{
    return texto == "1" || texto == "true";
}

Album* buscarAlbum(Usuario* usuario, const std::string& nombreAlbum)
{
    if (usuario == nullptr)
    {
        return nullptr;
    }

    ListaDinamica<Album*>& albumes = usuario->getAlbumes();
    for (int i = 0; i < albumes.getCantidad(); i++)
    {
        if (albumes.obtener(i)->getNombre() == nombreAlbum)
        {
            return albumes.obtener(i);
        }
    }
    return nullptr;
}
}

namespace Persistencia
{
ListaDinamica<std::string> separarCampos(const std::string& linea, char separador)
{
    ListaDinamica<std::string> campos;
    std::string actual;

    for (int i = 0; i < static_cast<int>(linea.size()); i++)
    {
        char caracter = linea[i];
        if (caracter == '\\' && i + 1 < static_cast<int>(linea.size()))
        {
            char siguiente = linea[i + 1];
            if (siguiente == separador)
            {
                actual += separador;
                i++;
            }
            else if (siguiente == '\\')
            {
                actual += '\\';
                i++;
            }
            else if (siguiente == 'n')
            {
                actual += '\n';
                i++;
            }
            else
            {
                actual += caracter;
            }
        }
        else if (caracter == separador)
        {
            campos.agregar(actual);
            actual.clear();
        }
        else
        {
            actual += caracter;
        }
    }
    campos.agregar(actual);
    return campos;
}

std::string escaparCampo(const std::string& campo, char separador)
{
    std::string resultado;
    for (int i = 0; i < static_cast<int>(campo.size()); i++)
    {
        char caracter = campo[i];
        if (caracter == '\\')
        {
            resultado += "\\\\";
        }
        else if (caracter == separador)
        {
            resultado += '\\';
            resultado += separador;
        }
        else if (caracter == '\n')
        {
            resultado += "\\n";
        }
        else
        {
            resultado += caracter;
        }
    }
    return resultado;
}

std::string unirRuta(const std::string& carpeta, const std::string& nombreArchivo)
{
    if (carpeta.empty())
    {
        return nombreArchivo;
    }

    char ultimo = carpeta[carpeta.size() - 1];
    if (ultimo == '/' || ultimo == '\\')
    {
        return carpeta + nombreArchivo;
    }
    return carpeta + "/" + nombreArchivo;
}

ListaDinamica<Usuario*> cargarUsuarios(const std::string& rutaArchivo, MedidorRecursos& medidor)
{
    ListaDinamica<Usuario*> usuarios;
    std::ifstream archivo(rutaArchivo);
    std::string linea;

    while (std::getline(archivo, linea))
    {
        medidor.contarIteracion();
        ListaDinamica<std::string> campos = separarCampos(linea);
        if (campos.getCantidad() < 8 || campos.obtener(0).empty())
        {
            continue;
        }

        if (buscarPorNombre(usuarios, campos.obtener(0), &medidor) != nullptr)
        {
            continue;
        }

        usuarios.agregar(new Usuario(campos.obtener(0), campos.obtener(1), campos.obtener(2),
                                     campos.obtener(3), campos.obtener(4), campos.obtener(5),
                                     campos.obtener(6), campos.obtener(7)));
    }
    return usuarios;
}

void guardarUsuarios(const std::string& rutaArchivo, const ListaDinamica<Usuario*>& usuarios)
{
    std::ofstream archivo(rutaArchivo, std::ios::trunc);
    for (int i = 0; i < usuarios.getCantidad(); i++)
    {
        Usuario* usuario = usuarios.obtener(i);
        archivo << escaparCampo(usuario->getNombreUsuario()) << "|"
                << escaparCampo(usuario->getFechaNacimiento()) << "|"
                << escaparCampo(usuario->getCorreo()) << "|"
                << escaparCampo(usuario->getFotoPerfil()) << "|"
                << escaparCampo(usuario->getClave()) << "|"
                << escaparCampo(usuario->getCiudad()) << "|"
                << escaparCampo(usuario->getPais()) << "|"
                << escaparCampo(usuario->getFechaInscripcion()) << "\n";
    }
}

void cargarAmistades(const std::string& rutaArchivo, ListaDinamica<Usuario*>& usuarios,
                     MedidorRecursos& medidor)
{
    std::ifstream archivo(rutaArchivo);
    std::string linea;
    while (std::getline(archivo, linea))
    {
        medidor.contarIteracion();
        ListaDinamica<std::string> campos = separarCampos(linea);
        if (campos.getCantidad() < 2)
        {
            continue;
        }
        Usuario* primero = buscarPorNombre(usuarios, campos.obtener(0), &medidor);
        Usuario* segundo = buscarPorNombre(usuarios, campos.obtener(1), &medidor);
        if (primero != nullptr && segundo != nullptr && primero != segundo)
        {
            primero->agregarAmigo(segundo, &medidor);
            segundo->agregarAmigo(primero, &medidor);
        }
    }
}

void cargarSolicitudes(const std::string& rutaArchivo, ListaDinamica<Usuario*>& usuarios,
                       MedidorRecursos& medidor)
{
    std::ifstream archivo(rutaArchivo);
    std::string linea;
    while (std::getline(archivo, linea))
    {
        medidor.contarIteracion();
        ListaDinamica<std::string> campos = separarCampos(linea);
        if (campos.getCantidad() < 2)
        {
            continue;
        }
        Usuario* solicitante = buscarPorNombre(usuarios, campos.obtener(0), &medidor);
        Usuario* destinatario = buscarPorNombre(usuarios, campos.obtener(1), &medidor);
        if (solicitante != nullptr && destinatario != nullptr)
        {
            destinatario->agregarSolicitudDe(solicitante, &medidor);
        }
    }
}

void cargarAlbumes(const std::string& rutaArchivo, ListaDinamica<Usuario*>& usuarios,
                   MedidorRecursos& medidor)
{
    std::ifstream archivo(rutaArchivo);
    std::string linea;
    while (std::getline(archivo, linea))
    {
        medidor.contarIteracion();
        ListaDinamica<std::string> campos = separarCampos(linea);
        if (campos.getCantidad() < 4)
        {
            continue;
        }
        Usuario* propietario = buscarPorNombre(usuarios, campos.obtener(0), &medidor);
        if (propietario != nullptr && buscarAlbum(propietario, campos.obtener(1)) == nullptr)
        {
            propietario->agregarAlbum(new Album(campos.obtener(1), campos.obtener(2),
                                                privacidadDesdeTexto(campos.obtener(3))));
        }
    }
}

void cargarFotos(const std::string& rutaArchivo, ListaDinamica<Usuario*>& usuarios,
                 MedidorRecursos& medidor)
{
    std::ifstream archivo(rutaArchivo);
    std::string linea;
    while (std::getline(archivo, linea))
    {
        medidor.contarIteracion();
        ListaDinamica<std::string> campos = separarCampos(linea);
        if (campos.getCantidad() < 6)
        {
            continue;
        }
        Usuario* propietario = buscarPorNombre(usuarios, campos.obtener(0), &medidor);
        Album* album = buscarAlbum(propietario, campos.obtener(1));
        if (album != nullptr && album->buscarFotoPorRuta(campos.obtener(4)) == nullptr)
        {
            album->agregarFoto(new Foto(campos.obtener(2), convertirEnteroLargo(campos.obtener(3)),
                                        campos.obtener(4), convertirEntero(campos.obtener(5))));
        }
    }
}

void cargarMensajes(const std::string& rutaArchivo, ListaDinamica<Usuario*>& usuarios,
                    ListaDinamica<Mensaje*>& mensajes, MedidorRecursos& medidor)
{
    std::ifstream archivo(rutaArchivo);
    std::string linea;
    while (std::getline(archivo, linea))
    {
        medidor.contarIteracion();
        ListaDinamica<std::string> campos = separarCampos(linea);
        if (campos.getCantidad() < 5 || campos.obtener(3).empty() ||
            campos.obtener(3).size() > 512)
        {
            continue;
        }
        Usuario* emisor = buscarPorNombre(usuarios, campos.obtener(0), &medidor);
        Usuario* receptor = buscarPorNombre(usuarios, campos.obtener(1), &medidor);
        if (emisor != nullptr && receptor != nullptr)
        {
            Mensaje* mensaje = new Mensaje(emisor, receptor, campos.obtener(2), campos.obtener(3),
                                            convertirBooleano(campos.obtener(4)));
            mensajes.agregar(mensaje);
            emisor->agregarMensajeEnviado(mensaje);
            receptor->agregarMensajeRecibido(mensaje);
        }
    }
}

void guardarAmistades(const std::string& rutaArchivo, const ListaDinamica<Usuario*>& usuarios)
{
    std::ofstream archivo(rutaArchivo, std::ios::trunc);
    for (int i = 0; i < usuarios.getCantidad(); i++)
    {
        Usuario* usuario = usuarios.obtener(i);
        const ListaDinamica<Usuario*>& amigos = usuario->getAmigos();
        for (int j = 0; j < amigos.getCantidad(); j++)
        {
            if (usuario->getNombreUsuario() < amigos.obtener(j)->getNombreUsuario())
            {
                archivo << escaparCampo(usuario->getNombreUsuario()) << "|"
                        << escaparCampo(amigos.obtener(j)->getNombreUsuario()) << "\n";
            }
        }
    }
}

void guardarSolicitudes(const std::string& rutaArchivo, const ListaDinamica<Usuario*>& usuarios)
{
    std::ofstream archivo(rutaArchivo, std::ios::trunc);
    for (int i = 0; i < usuarios.getCantidad(); i++)
    {
        Usuario* destinatario = usuarios.obtener(i);
        const ListaDinamica<Usuario*>& solicitudes = destinatario->getSolicitudes();
        for (int j = 0; j < solicitudes.getCantidad(); j++)
        {
            archivo << escaparCampo(solicitudes.obtener(j)->getNombreUsuario()) << "|"
                    << escaparCampo(destinatario->getNombreUsuario()) << "\n";
        }
    }
}

void guardarAlbumes(const std::string& rutaArchivo, const ListaDinamica<Usuario*>& usuarios)
{
    std::ofstream archivo(rutaArchivo, std::ios::trunc);
    for (int i = 0; i < usuarios.getCantidad(); i++)
    {
        Usuario* propietario = usuarios.obtener(i);
        const ListaDinamica<Album*>& albumes = propietario->getAlbumes();
        for (int j = 0; j < albumes.getCantidad(); j++)
        {
            Album* album = albumes.obtener(j);
            archivo << escaparCampo(propietario->getNombreUsuario()) << "|"
                    << escaparCampo(album->getNombre()) << "|"
                    << escaparCampo(album->getRutaAlmacenamiento()) << "|"
                    << textoPrivacidad(album->getPrivacidad()) << "\n";
        }
    }
}

void guardarFotos(const std::string& rutaArchivo, const ListaDinamica<Usuario*>& usuarios)
{
    std::ofstream archivo(rutaArchivo, std::ios::trunc);
    for (int i = 0; i < usuarios.getCantidad(); i++)
    {
        Usuario* propietario = usuarios.obtener(i);
        const ListaDinamica<Album*>& albumes = propietario->getAlbumes();
        for (int j = 0; j < albumes.getCantidad(); j++)
        {
            Album* album = albumes.obtener(j);
            const ListaDinamica<Foto*>& fotos = album->getFotos();
            for (int k = 0; k < fotos.getCantidad(); k++)
            {
                Foto* foto = fotos.obtener(k);
                archivo << escaparCampo(propietario->getNombreUsuario()) << "|"
                        << escaparCampo(album->getNombre()) << "|"
                        << escaparCampo(foto->getFechaCaptura()) << "|"
                        << foto->getTamanoBytes() << "|"
                        << escaparCampo(foto->getRutaArchivo()) << "|"
                        << foto->getConteoLikes() << "\n";
            }
        }
    }
}

void guardarMensajes(const std::string& rutaArchivo, const ListaDinamica<Mensaje*>& mensajes)
{
    std::ofstream archivo(rutaArchivo, std::ios::trunc);
    for (int i = 0; i < mensajes.getCantidad(); i++)
    {
        Mensaje* mensaje = mensajes.obtener(i);
        archivo << escaparCampo(mensaje->getEmisor()->getNombreUsuario()) << "|"
                << escaparCampo(mensaje->getReceptor()->getNombreUsuario()) << "|"
                << escaparCampo(mensaje->getFechaEnvio()) << "|"
                << escaparCampo(mensaje->getTexto()) << "|"
                << (mensaje->estaLeido() ? "1" : "0") << "\n";
    }
}

Usuario* buscarPorNombre(const ListaDinamica<Usuario*>& usuarios, const std::string& nombreUsuario,
                         MedidorRecursos* medidor)
{
    for (int i = 0; i < usuarios.getCantidad(); i++)
    {
        if (medidor != nullptr)
        {
            medidor->contarIteracion();
        }
        if (usuarios.obtener(i)->getNombreUsuario() == nombreUsuario)
        {
            return usuarios.obtener(i);
        }
    }
    return nullptr;
}
}
