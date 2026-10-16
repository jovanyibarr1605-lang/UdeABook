#ifndef UDEABOOK_H
#define UDEABOOK_H

#include <cstddef>
#include <string>

#include "Album.h"
#include "ListaDinamica.h"
#include "Medidorrecursos.h"
#include "Mensaje.h"
#include "Usuario.h"

class UdeABook
{
private:
    ListaDinamica<Usuario*> usuarios;
    ListaDinamica<Mensaje*> mensajes;
    std::string carpetaDatos;

    void liberarDatos();
    static void ordenarPorFecha(ListaDinamica<Mensaje*>& lista);
    static std::size_t memoriaTexto(const std::string& texto);
    std::size_t calcularMemoriaEstructuras() const;

public:
    UdeABook();
    ~UdeABook();

    bool cargarDatos(const std::string& carpeta, MedidorRecursos& medidor);
    void guardarDatos() const;
    void generarDatosDemostracion(int cantidadUsuarios, MedidorRecursos& medidor);

    Usuario* iniciarSesion(const std::string& nombreUsuario, const std::string& clave,
                           MedidorRecursos& medidor) const;
    Usuario* buscarUsuario(const std::string& nombreUsuario, MedidorRecursos& medidor) const;

    bool enviarSolicitud(Usuario* remitente, const std::string& nombreDestino,
                         MedidorRecursos& medidor);
    bool aceptarSolicitud(Usuario* destinatario, const std::string& nombreSolicitante,
                          MedidorRecursos& medidor);
    ListaDinamica<Usuario*> sugerirAmigos(Usuario* usuario, int cantidad,
                                          MedidorRecursos& medidor) const;

    ListaDinamica<Album*> albumesVisibles(Usuario* visitante, const std::string& nombreAmigo,
                                          MedidorRecursos& medidor) const;
    bool cambiarLike(Usuario* visitante, Album* album, int indiceFoto, bool agregar,
                     MedidorRecursos& medidor);

    bool enviarMensaje(Usuario* emisor, const std::string& nombreReceptor,
                       const std::string& texto, const std::string& fecha,
                       MedidorRecursos& medidor);
    ListaDinamica<Mensaje*> leerMensajesNuevos(Usuario* usuario,
                                               MedidorRecursos& medidor);
    ListaDinamica<Mensaje*> buscarMensajesPorFecha(Usuario* usuario,
                                                    const std::string& fechaInicio,
                                                    const std::string& fechaFin,
                                                    MedidorRecursos& medidor) const;

    void actualizarMedicion(MedidorRecursos& medidor, std::size_t memoriaLocal = 0) const;
    const ListaDinamica<Usuario*>& getUsuarios() const;
    const ListaDinamica<Mensaje*>& getMensajes() const;
};

#endif // UDEABOOK_H
