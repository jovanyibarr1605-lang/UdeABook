#ifndef USUARIO_H
#define USUARIO_H

#include <iosfwd>
#include <string>
#include "ListaDinamica.h"

// Album y Mensaje los implementa tu companero (Persona B).
// Con la declaracion adelantada basta para compilar este archivo,
// porque Usuario solo los guarda como punteros.
class Album;
class Mensaje;
class MedidorRecursos;

class Usuario
{
private:
    std::string nombreUsuario;
    std::string fechaNacimiento;
    std::string correo;
    std::string fotoPerfil;
    std::string clave;
    std::string ciudad;
    std::string pais;
    std::string fechaInscripcion;

    ListaDinamica<Usuario*> amigos;
    ListaDinamica<Usuario*> solicitudes;
    ListaDinamica<Album*> albumes;
    ListaDinamica<Mensaje*> mensajesEnviados;
    ListaDinamica<Mensaje*> mensajesRecibidos;

    void liberarAlbumes();

public:
    Usuario();
    Usuario(const std::string& nombreUsuario,
            const std::string& fechaNacimiento,
            const std::string& correo,
            const std::string& fotoPerfil,
            const std::string& clave,
            const std::string& ciudad,
            const std::string& pais,
            const std::string& fechaInscripcion);

    Usuario(const Usuario& otro);
    Usuario& operator=(const Usuario& otro);
    ~Usuario();

    bool operator==(const Usuario& otro) const;
    friend std::ostream& operator<<(std::ostream& salida, const Usuario& usuario);

    // --- Getters ---
    const std::string& getNombreUsuario() const;
    const std::string& getFechaNacimiento() const;
    const std::string& getCorreo() const;
    const std::string& getFotoPerfil() const;
    const std::string& getClave() const;
    const std::string& getCiudad() const;
    const std::string& getPais() const;
    const std::string& getFechaInscripcion() const;

    ListaDinamica<Usuario*>& getAmigos();
    const ListaDinamica<Usuario*>& getAmigos() const;

    ListaDinamica<Usuario*>& getSolicitudes();
    const ListaDinamica<Usuario*>& getSolicitudes() const;

    ListaDinamica<Album*>& getAlbumes();
    const ListaDinamica<Album*>& getAlbumes() const;

    ListaDinamica<Mensaje*>& getMensajesEnviados();
    const ListaDinamica<Mensaje*>& getMensajesEnviados() const;

    ListaDinamica<Mensaje*>& getMensajesRecibidos();
    const ListaDinamica<Mensaje*>& getMensajesRecibidos() const;

    void setFotoPerfil(const std::string& nuevaFoto);
    void setClave(const std::string& nuevaClave);
    void setCiudad(const std::string& nuevaCiudad);
    void setPais(const std::string& nuevoPais);

    bool tieneAmigo(const Usuario* candidato, MedidorRecursos* medidor = nullptr) const;
    bool tieneSolicitudPendienteDe(const Usuario* candidato,
                                   MedidorRecursos* medidor = nullptr) const;
    bool agregarAmigo(Usuario* nuevoAmigo, MedidorRecursos* medidor = nullptr);
    bool agregarSolicitudDe(Usuario* solicitante, MedidorRecursos* medidor = nullptr);
    bool retirarSolicitudDe(const Usuario* solicitante, MedidorRecursos* medidor = nullptr);
    void agregarAlbum(Album* album);
    void agregarMensajeEnviado(Mensaje* mensaje);
    void agregarMensajeRecibido(Mensaje* mensaje);

    void enviarSolicitud(Usuario& destino, MedidorRecursos* medidor = nullptr);
    void aceptarSolicitud(Usuario& solicitante, MedidorRecursos* medidor = nullptr);
};

#endif // USUARIO_H
