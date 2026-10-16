#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include <string>
#include "ListaDinamica.h"
#include "Usuario.h"
#include "Medidorrecursos.h"
#include "Mensaje.h"

namespace Persistencia
{
ListaDinamica<std::string> separarCampos(const std::string& linea, char separador = '|');
std::string escaparCampo(const std::string& campo, char separador = '|');
std::string unirRuta(const std::string& carpeta, const std::string& nombreArchivo);

ListaDinamica<Usuario*> cargarUsuarios(const std::string& rutaArchivo, MedidorRecursos& medidor);
void guardarUsuarios(const std::string& rutaArchivo, const ListaDinamica<Usuario*>& usuarios);
Usuario* buscarPorNombre(const ListaDinamica<Usuario*>& usuarios, const std::string& nombreUsuario);

void cargarAmistades(const std::string& rutaArchivo, ListaDinamica<Usuario*>& usuarios,
                     MedidorRecursos& medidor);
void cargarSolicitudes(const std::string& rutaArchivo, ListaDinamica<Usuario*>& usuarios,
                       MedidorRecursos& medidor);
void cargarAlbumes(const std::string& rutaArchivo, ListaDinamica<Usuario*>& usuarios,
                   MedidorRecursos& medidor);
void cargarFotos(const std::string& rutaArchivo, ListaDinamica<Usuario*>& usuarios,
                 MedidorRecursos& medidor);
void cargarMensajes(const std::string& rutaArchivo, ListaDinamica<Usuario*>& usuarios,
                    ListaDinamica<Mensaje*>& mensajes, MedidorRecursos& medidor);

void guardarAmistades(const std::string& rutaArchivo, const ListaDinamica<Usuario*>& usuarios);
void guardarSolicitudes(const std::string& rutaArchivo, const ListaDinamica<Usuario*>& usuarios);
void guardarAlbumes(const std::string& rutaArchivo, const ListaDinamica<Usuario*>& usuarios);
void guardarFotos(const std::string& rutaArchivo, const ListaDinamica<Usuario*>& usuarios);
void guardarMensajes(const std::string& rutaArchivo, const ListaDinamica<Mensaje*>& mensajes);

Usuario* buscarPorNombre(const ListaDinamica<Usuario*>& usuarios,
                         const std::string& nombreUsuario,
                         MedidorRecursos* medidor = nullptr);
}

#endif // PERSISTENCIA_H
