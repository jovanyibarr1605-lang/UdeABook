#include "Persistencia.h"
#include <fstream>
#include <sstream>

namespace Persistencia
{

ListaDinamica<std::string> separarCampos(const std::string& linea, char separador)
{
    ListaDinamica<std::string> campos;
    std::string actual;

    for (size_t i = 0; i < linea.size(); i++)
    {
        char c = linea[i];

        if (c == '\\' && i + 1 < linea.size())
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
                actual += c;
            }
        }
        else if (c == separador)
        {
            campos.agregar(actual);
            actual.clear();
        }
        else
        {
            actual += c;
        }
    }

    campos.agregar(actual);
    return campos;
}

std::string escaparCampo(const std::string& campo, char separador)
{
    std::string resultado;

    for (size_t i = 0; i < campo.size(); i++)
    {
        char c = campo[i];

        if (c == '\\')
        {
            resultado += "\\\\";
        }
        else if (c == separador)
        {
            resultado += '\\';
            resultado += separador;
        }
        else if (c == '\n')
        {
            resultado += "\\n";
        }
        else
        {
            resultado += c;
        }
    }

    return resultado;
}

ListaDinamica<Usuario*> cargarUsuarios(const std::string& rutaArchivo, MedidorRecursos& medidor)
{
    ListaDinamica<Usuario*> usuarios;
    std::ifstream archivo(rutaArchivo);

    if (!archivo.is_open())
    {
        return usuarios; // archivo aun no existe -> lista vacia, no es un error fatal
    }

    std::string linea;
    while (std::getline(archivo, linea))
    {
        medidor.contarIteracion(); // una iteracion por registro leido

        if (linea.empty())
        {
            continue;
        }

        ListaDinamica<std::string> campos = separarCampos(linea);

        // usuario | nacimiento | correo | fotoPerfil | clave | ciudad | pais | inscripcion
        if (campos.getCantidad() < 8)
        {
            continue; // registro incompleto: se descarta, no se crea el usuario
        }

        Usuario* nuevo = new Usuario(
            campos.obtener(0),  // nombreUsuario
            campos.obtener(1),  // fechaNacimiento
            campos.obtener(2),  // correo
            campos.obtener(3),  // fotoPerfil
            campos.obtener(4),  // clave
            campos.obtener(5),  // ciudad
            campos.obtener(6),  // pais
            campos.obtener(7)   // fechaInscripcion
            );

        // Evita duplicados: si el nombre ya existe, se descarta el registro repetido.
        if (buscarPorNombre(usuarios, nuevo->getNombreUsuario()) != nullptr)
        {
            delete nuevo;
            continue;
        }

        usuarios.agregar(nuevo);
        medidor.sumarBytes(sizeof(Usuario));
    }

    archivo.close();
    return usuarios;
}

void guardarUsuarios(const std::string& rutaArchivo, const ListaDinamica<Usuario*>& usuarios)
{
    std::ofstream archivo(rutaArchivo, std::ios::trunc);

    if (!archivo.is_open())
    {
        return;
    }

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

    archivo.close();
}

Usuario* buscarPorNombre(const ListaDinamica<Usuario*>& usuarios, const std::string& nombreUsuario)
{
    for (int i = 0; i < usuarios.getCantidad(); i++)
    {
        if (usuarios.obtener(i)->getNombreUsuario() == nombreUsuario)
        {
            return usuarios.obtener(i);
        }
    }
    return nullptr;
}

} // namespace Persistencia