#include "Usuario.h"
#include "Album.h"   // Lo crea Persona B. Hace falta la definicion completa
// aqui (no basta con la adelantada de Usuario.h) porque
// el destructor necesita saber como destruir cada Album.

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

    // amigos, solicitudes y mensajes son referencias: se copian los punteros,
    // no los objetos apuntados (Usuario no es propietario de esos objetos).
    amigos = otro.amigos;
    solicitudes = otro.solicitudes;
    mensajesEnviados = otro.mensajesEnviados;
    mensajesRecibidos = otro.mensajesRecibidos;

    // albumes si es propiedad de Usuario: se clona cada Album para que la
    // copia no comparta memoria con el original (evita doble liberacion).
    for (int i = 0; i < otro.albumes.getCantidad(); i++)
    {
        albumes.agregar(new Album(*otro.albumes.obtener(i)));
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

    for (int i = 0; i < albumes.getCantidad(); i++)
    {
        delete albumes.obtener(i);
    }
    albumes = ListaDinamica<Album*>();

    for (int i = 0; i < otro.albumes.getCantidad(); i++)
    {
        albumes.agregar(new Album(*otro.albumes.obtener(i)));
    }

    return *this;
}

Usuario::~Usuario()
{
    for (int i = 0; i < albumes.getCantidad(); i++)
    {
        delete albumes.obtener(i);
    }
    // amigos, solicitudes, mensajesEnviados y mensajesRecibidos NO se liberan
    // aqui: son referencias a objetos que posee otra clase (UdeABook u otro Usuario).
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

bool Usuario::tieneAmigo(Usuario* candidato) const
{
    for (int i = 0; i < amigos.getCantidad(); i++)
    {
        if (amigos.obtener(i) == candidato)
        {
            return true;
        }
    }
    return false;
}

bool Usuario::tieneSolicitudPendienteDe(Usuario* candidato) const
{
    for (int i = 0; i < solicitudes.getCantidad(); i++)
    {
        if (solicitudes.obtener(i) == candidato)
        {
            return true;
        }
    }
    return false;
}

void Usuario::agregarAlbum(Album* album)
{
    albumes.agregar(album);
}

// --- Funcion III (solicitudes de amistad) ---
// TODO(Persona B): completar con las validaciones del informe:
// que el nombre exista, que no sea el usuario activo, que no haya amistad
// previa y que no exista ya una solicitud pendiente (en cualquiera de los
// dos sentidos) antes de agregar a "solicitudes"; y en aceptarSolicitud,
// quitar la referencia pendiente y agregar a ambos en "amigos".

void Usuario::enviarSolicitud(Usuario& destino)
{
    (void)destino; // evita warning de parametro sin usar mientras se implementa
}

void Usuario::aceptarSolicitud(Usuario& solicitante)
{
    (void)solicitante;
}