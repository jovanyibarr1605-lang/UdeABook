#include "Album.h"

#include "Foto.h"
#include "Medidorrecursos.h"
#include "Usuario.h"

Album::Album() : privacidad(Privacidad::Privado), propietario(nullptr)
{
}

Album::Album(const std::string& nombre, const std::string& rutaAlmacenamiento,
             Privacidad privacidad)
    : nombre(nombre), rutaAlmacenamiento(rutaAlmacenamiento), privacidad(privacidad),
      propietario(nullptr)
{
}

Album::Album(const Album& otro)
    : nombre(otro.nombre), rutaAlmacenamiento(otro.rutaAlmacenamiento),
      privacidad(otro.privacidad), propietario(nullptr)
{
    for (int i = 0; i < otro.fotos.getCantidad(); i++)
    {
        fotos.agregar(new Foto(*otro.fotos.obtener(i)));
    }
}

Album& Album::operator=(const Album& otro)
{
    if (this == &otro)
    {
        return *this;
    }

    liberarFotos();
    nombre = otro.nombre;
    rutaAlmacenamiento = otro.rutaAlmacenamiento;
    privacidad = otro.privacidad;
    propietario = nullptr;
    for (int i = 0; i < otro.fotos.getCantidad(); i++)
    {
        fotos.agregar(new Foto(*otro.fotos.obtener(i)));
    }
    return *this;
}

Album::~Album()
{
    liberarFotos();
}

void Album::liberarFotos()
{
    for (int i = 0; i < fotos.getCantidad(); i++)
    {
        delete fotos.obtener(i);
    }
    fotos.limpiar();
}

const std::string& Album::getNombre() const { return nombre; }
const std::string& Album::getRutaAlmacenamiento() const { return rutaAlmacenamiento; }
Privacidad Album::getPrivacidad() const { return privacidad; }
Usuario* Album::getPropietario() const { return propietario; }
const ListaDinamica<Foto*>& Album::getFotos() const { return fotos; }
ListaDinamica<Foto*>& Album::getFotos() { return fotos; }

void Album::setPropietario(Usuario* nuevoPropietario)
{
    propietario = nuevoPropietario;
}

void Album::agregarFoto(Foto* foto)
{
    if (foto != nullptr)
    {
        fotos.agregar(foto);
    }
}

Foto* Album::buscarFotoPorRuta(const std::string& ruta) const
{
    for (int i = 0; i < fotos.getCantidad(); i++)
    {
        if (fotos.obtener(i)->getRutaArchivo() == ruta)
        {
            return fotos.obtener(i);
        }
    }
    return nullptr;
}

bool Album::esVisiblePara(const Usuario* visitante, MedidorRecursos* medidor) const
{
    if (privacidad == Privacidad::Publico || visitante == propietario)
    {
        return true;
    }
    return privacidad == Privacidad::Amigos && propietario != nullptr && visitante != nullptr &&
           propietario->tieneAmigo(visitante, medidor);
}
