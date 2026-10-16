#include "Usuario.h"
#include "Album.h"
#include "Medidorrecursos.h"

#include <ostream>

Usuario::Usuario()
{
    nombreUsuario = "";
    fechaNacimiento = "";
    correo = "";
    fotoPerfil = "";
    clave = "";
    ciudad = "";
    pais = "";
    fechaInscripcion = "";
}

Usuario::Usuario(const std::string& nombreUsuario,
                 const std::string& fechaNacimiento,
                 const std::string& correo,
                 const std::string& fotoPerfil,
                 const std::string& clave,
                 const std::string& ciudad,
                 const std::string& pais,
                 const std::string& fechaInscripcion)
{
    this->nombreUsuario = nombreUsuario;
    this->fechaNacimiento = fechaNacimiento;
    this->correo = correo;
    this->fotoPerfil = fotoPerfil;
    this->clave = clave;
    this->ciudad = ciudad;
    this->pais = pais;
    this->fechaInscripcion = fechaInscripcion;
}

Usuario::Usuario(const Usuario& otro)
{
    nombreUsuario = otro.nombreUsuario;
    fechaNacimiento = otro.fechaNacimiento;
    correo = otro.correo;
    fotoPerfil = otro.fotoPerfil;
    clave = otro.clave;
    ciudad = otro.ciudad;
    pais = otro.pais;
    fechaInscripcion = otro.fechaInscripcion;

    amigos = otro.amigos;
    solicitudes = otro.solicitudes;
    mensajesEnviados = otro.mensajesEnviados;
    mensajesRecibidos = otro.mensajesRecibidos;

    for (int i = 0; i < otro.albumes.getCantidad(); i++)
    {
        Album* copia = new Album(*otro.albumes.obtener(i));
        copia->setPropietario(this);
        albumes.agregar(copia);
    }
}

Usuario& Usuario::operator=(const Usuario& otro)
{
    if (this == &otro)
    {
        return *this;
    }

    nombreUsuario = otro.nombreUsuario;
    fechaNacimiento = otro.fechaNacimiento;
    correo = otro.correo;
    fotoPerfil = otro.fotoPerfil;
    clave = otro.clave;
    ciudad = otro.ciudad;
    pais = otro.pais;
    fechaInscripcion = otro.fechaInscripcion;

    amigos = otro.amigos;
    solicitudes = otro.solicitudes;
    mensajesEnviados = otro.mensajesEnviados;
    mensajesRecibidos = otro.mensajesRecibidos;

    liberarAlbumes();

    for (int i = 0; i < otro.albumes.getCantidad(); i++)
    {
        Album* copia = new Album(*otro.albumes.obtener(i));
        copia->setPropietario(this);
        albumes.agregar(copia);
    }

    return *this;
}

Usuario::~Usuario()
{
    liberarAlbumes();
}

void Usuario::liberarAlbumes()
{
    for (int i = 0; i < albumes.getCantidad(); i++)
    {
        delete albumes.obtener(i);
    }
    albumes.limpiar();
}

bool Usuario::operator==(const Usuario& otro) const
{
    return nombreUsuario == otro.nombreUsuario;
}

std::ostream& operator<<(std::ostream& salida, const Usuario& usuario)
{
    salida << usuario.nombreUsuario << " (" << usuario.ciudad << ", " << usuario.pais << ") - "
           << usuario.amigos.getCantidad() << " amigos";
    return salida;
}

// --- Getters ---

const std::string& Usuario::getNombreUsuario() const { return nombreUsuario; }
const std::string& Usuario::getFechaNacimiento() const { return fechaNacimiento; }
const std::string& Usuario::getCorreo() const { return correo; }
const std::string& Usuario::getFotoPerfil() const { return fotoPerfil; }
const std::string& Usuario::getClave() const { return clave; }
const std::string& Usuario::getCiudad() const { return ciudad; }
const std::string& Usuario::getPais() const { return pais; }
const std::string& Usuario::getFechaInscripcion() const { return fechaInscripcion; }

ListaDinamica<Usuario*>& Usuario::getAmigos() { return amigos; }
const ListaDinamica<Usuario*>& Usuario::getAmigos() const { return amigos; }

ListaDinamica<Usuario*>& Usuario::getSolicitudes() { return solicitudes; }
const ListaDinamica<Usuario*>& Usuario::getSolicitudes() const { return solicitudes; }

ListaDinamica<Album*>& Usuario::getAlbumes() { return albumes; }
const ListaDinamica<Album*>& Usuario::getAlbumes() const { return albumes; }

ListaDinamica<Mensaje*>& Usuario::getMensajesEnviados() { return mensajesEnviados; }
const ListaDinamica<Mensaje*>& Usuario::getMensajesEnviados() const { return mensajesEnviados; }

ListaDinamica<Mensaje*>& Usuario::getMensajesRecibidos() { return mensajesRecibidos; }
const ListaDinamica<Mensaje*>& Usuario::getMensajesRecibidos() const { return mensajesRecibidos; }

// --- Setters ---

void Usuario::setFotoPerfil(const std::string& nuevaFoto) { fotoPerfil = nuevaFoto; }
void Usuario::setClave(const std::string& nuevaClave) { clave = nuevaClave; }
void Usuario::setCiudad(const std::string& nuevaCiudad) { ciudad = nuevaCiudad; }
void Usuario::setPais(const std::string& nuevoPais) { pais = nuevoPais; }

// --- Helpers de estructura ---

bool Usuario::tieneAmigo(const Usuario* candidato, MedidorRecursos* medidor) const
{
    for (int i = 0; i < amigos.getCantidad(); i++)
    {
        if (medidor != nullptr)
        {
            medidor->contarIteracion();
        }
        if (amigos.obtener(i) == candidato)
        {
            return true;
        }
    }
    return false;
}

bool Usuario::tieneSolicitudPendienteDe(const Usuario* candidato, MedidorRecursos* medidor) const
{
    for (int i = 0; i < solicitudes.getCantidad(); i++)
    {
        if (medidor != nullptr)
        {
            medidor->contarIteracion();
        }
        if (solicitudes.obtener(i) == candidato)
        {
            return true;
        }
    }
    return false;
}

bool Usuario::agregarAmigo(Usuario* nuevoAmigo, MedidorRecursos* medidor)
{
    if (nuevoAmigo == nullptr || nuevoAmigo == this || tieneAmigo(nuevoAmigo, medidor))
    {
        return false;
    }
    amigos.agregar(nuevoAmigo);
    return true;
}

bool Usuario::agregarSolicitudDe(Usuario* solicitante, MedidorRecursos* medidor)
{
    if (solicitante == nullptr || solicitante == this || tieneAmigo(solicitante, medidor) ||
        tieneSolicitudPendienteDe(solicitante, medidor))
    {
        return false;
    }
    solicitudes.agregar(solicitante);
    return true;
}

bool Usuario::retirarSolicitudDe(const Usuario* solicitante, MedidorRecursos* medidor)
{
    for (int i = 0; i < solicitudes.getCantidad(); i++)
    {
        if (medidor != nullptr)
        {
            medidor->contarIteracion();
        }
        if (solicitudes.obtener(i) == solicitante)
        {
            solicitudes.eliminarEn(i);
            return true;
        }
    }
    return false;
}

void Usuario::agregarAlbum(Album* album)
{
    if (album != nullptr)
    {
        album->setPropietario(this);
        albumes.agregar(album);
    }
}

void Usuario::agregarMensajeEnviado(Mensaje* mensaje)
{
    if (mensaje != nullptr)
    {
        mensajesEnviados.agregar(mensaje);
    }
}

void Usuario::agregarMensajeRecibido(Mensaje* mensaje)
{
    if (mensaje != nullptr)
    {
        mensajesRecibidos.agregar(mensaje);
    }
}

void Usuario::enviarSolicitud(Usuario& destino, MedidorRecursos* medidor)
{
    if (&destino == this || tieneAmigo(&destino, medidor) ||
        tieneSolicitudPendienteDe(&destino, medidor) ||
        destino.tieneSolicitudPendienteDe(this, medidor))
    {
        return;
    }
    destino.agregarSolicitudDe(this, medidor);
}

void Usuario::aceptarSolicitud(Usuario& solicitante, MedidorRecursos* medidor)
{
    if (retirarSolicitudDe(&solicitante, medidor))
    {
        agregarAmigo(&solicitante, medidor);
        solicitante.agregarAmigo(this, medidor);
    }
}
