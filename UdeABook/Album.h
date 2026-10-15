#// ARCHIVO PROVISIONAL - borralo cuando tu companero suba su Album.h real.
// Solo existe para que Usuario.cpp compile mientras tanto.
#ifndef ALBUM_H
#define ALBUM_H

#include <string>

class Album
{
private:
    std::string nombre;

public:
    Album() : nombre("provisional") {}
    Album(const Album& otro) : nombre(otro.nombre) {}
};

#endif // ALBUM_H