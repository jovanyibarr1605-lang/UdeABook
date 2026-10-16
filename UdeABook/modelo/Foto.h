#ifndef FOTO_H
#define FOTO_H

#include <string>

class Foto
{
private:
    std::string fechaCaptura;
    long long tamanoBytes;
    std::string rutaArchivo;
    int conteoLikes;

public:
    Foto();
    Foto(const std::string& fechaCaptura, long long tamanoBytes,
         const std::string& rutaArchivo, int conteoLikes = 0);

    const std::string& getFechaCaptura() const;
    long long getTamanoBytes() const;
    const std::string& getRutaArchivo() const;
    int getConteoLikes() const;

    void darLike();
    void quitarLike();
};

#endif // FOTO_H
