#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include <string>
#include "ListaDinamica.h"
#include "Usuario.h"
#include "MedidorRecursos.h"

// Funcion I: carga/guardado de datos. No se muestra en el menu del usuario.
//
// Formato de usuarios.txt (un registro por linea, separador '|'):
//   usuario | nacimiento | correo | fotoPerfil | clave | ciudad | pais | inscripcion
//
// Las demas listas (amistades, solicitudes, albumes, fotos, mensajes) las
// cargan las funciones de Persona B una vez que exista Usuario cargado,
// porque todas referencian usuarios por nombre.

namespace Persistencia
{
// Separa una linea en sus campos respetando el escape de '\|', '\\' y '\n'
// (mismo esquema que describe el informe para mensajes.txt). Reutilizable
// por Persona B para amistades.txt, solicitudes.txt, albumes.txt, etc.
ListaDinamica<std::string> separarCampos(const std::string& linea, char separador = '|');

// Inverso de separarCampos: escapa separador, '\\' y saltos de linea
// dentro de un campo antes de escribirlo en el archivo.
std::string escaparCampo(const std::string& campo, char separador = '|');

// Lee usuarios.txt y construye un Usuario por cada registro valido.
// Un registro con menos campos de los esperados se descarta (no se cuenta
// como iteracion util, pero si se contabiliza la lectura en el medidor).
ListaDinamica<Usuario*> cargarUsuarios(const std::string& rutaArchivo, MedidorRecursos& medidor);

// Escribe la lista completa de usuarios en formato usuarios.txt,
// sobreescribiendo el archivo. Se usa tras cualquier operacion que
// cambie el estado logico (ej. actualizar la clave o la foto de perfil).
void guardarUsuarios(const std::string& rutaArchivo, const ListaDinamica<Usuario*>& usuarios);

// Busca un usuario cargado por nombre exacto; nullptr si no existe.
Usuario* buscarPorNombre(const ListaDinamica<Usuario*>& usuarios, const std::string& nombreUsuario);
}

#endif // PERSISTENCIA_H