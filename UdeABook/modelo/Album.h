#ifndef ALBUM_H
#define ALBUM_H

#include <string>
#include "ListaDinamica.h"

class Foto;
class Usuario;
class MedidorRecursos;

enum class Privacidad
{
    Publico,
    Amigos,
    Privado
};

class Album
{
private:
    std::string nombre;
    std::string rutaAlmacenamiento;
    Privacidad privacidad;
    Usuario* propietario;
    ListaDinamica<Foto*> fotos;

    void liberarFotos();

public:
    Album();
    Album(const std::string& nombre, const std::string& rutaAlmacenamiento,
          Privacidad privacidad);
    Album(const Album& otro);
    Album& operator=(const Album& otro);
    ~Album();

    const std::string& getNombre() const;
    const std::string& getRutaAlmacenamiento() const;
    Privacidad getPrivacidad() const;
    Usuario* getPropietario() const;
    const ListaDinamica<Foto*>& getFotos() const;
    ListaDinamica<Foto*>& getFotos();

    void setPropietario(Usuario* nuevoPropietario);
    void agregarFoto(Foto* foto);
    Foto* buscarFotoPorRuta(const std::string& ruta) const;
    bool esVisiblePara(const Usuario* visitante, MedidorRecursos* medidor = nullptr) const;
};

#endif // ALBUM_H
