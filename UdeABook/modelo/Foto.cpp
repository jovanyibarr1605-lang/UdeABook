#include "Foto.h"

Foto::Foto() : tamanoBytes(0), conteoLikes(0)
{
}

Foto::Foto(const std::string& fechaCaptura, long long tamanoBytes,
           const std::string& rutaArchivo, int conteoLikes)
    : fechaCaptura(fechaCaptura), tamanoBytes(tamanoBytes),
      rutaArchivo(rutaArchivo), conteoLikes(conteoLikes)
{
}

const std::string& Foto::getFechaCaptura() const { return fechaCaptura; }
long long Foto::getTamanoBytes() const { return tamanoBytes; }
const std::string& Foto::getRutaArchivo() const { return rutaArchivo; }
int Foto::getConteoLikes() const { return conteoLikes; }

void Foto::darLike()
{
    conteoLikes++;
}

void Foto::quitarLike()
{
    if (conteoLikes > 0)
    {
        conteoLikes--;
    }
}
