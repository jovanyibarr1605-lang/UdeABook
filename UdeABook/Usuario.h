#ifndef USUARIO_H
#define USUARIO_H

#include <iostream>
#include <string>
#include "ListaDinamica.h"

// Album y Mensaje los implementa tu companero (Persona B).
// Con la declaracion adelantada basta para compilar este archivo,
// porque Usuario solo los guarda como punteros.
class Album;
class Mensaje;

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

    // No propietarias: solo referencian a Usuario/Mensaje que viven en otro lado
    // (en UdeABook o en la lista de amigos de cada quien).
    ListaDinamica<Usuario*> amigos;
    ListaDinamica<Usuario*> solicitudes; // solicitudes de amistad pendientes recibidas

    // Propietaria: Usuario crea y destruye sus propios albumes.
    ListaDinamica<Album*> albumes;

    // No propietarias: UdeABook conserva la instancia canonica de cada Mensaje.
    ListaDinamica<Mensaje*> mensajesEnviados;
    ListaDinamica<Mensaje*> mensajesRecibidos;

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

    // --- Setters (solo de los campos que tiene sentido modificar en sesion) ---
    void setFotoPerfil(const std::string& nuevaFoto);
    void setClave(const std::string& nuevaClave);
    void setCiudad(const std::string& nuevaCiudad);
    void setPais(const std::string& nuevoPais);

    // --- Helpers de estructura (listos para usar) ---
    bool tieneAmigo(Usuario* candidato) const;
    bool tieneSolicitudPendienteDe(Usuario* candidato) const;
    void agregarAlbum(Album* album); // Usuario pasa a ser propietario de ese Album*

    // --- Logica de negocio: la implementa Persona B (funcion III) ---
    // Declaradas aqui porque son parte de la interfaz publica de Usuario,
    // pero el cuerpo (validaciones de solicitud repetida, cruzada, etc.)
    // se define en Usuario.cpp junto con el resto de funcion III.
    void enviarSolicitud(Usuario& destino);
    void aceptarSolicitud(Usuario& solicitante);
};

#endif // USUARIO_H