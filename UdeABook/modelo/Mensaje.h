#ifndef MENSAJE_H
#define MENSAJE_H

#include <string>

class Usuario;

class Mensaje
{
private:
    Usuario* emisor;
    Usuario* receptor;
    std::string fechaEnvio;
    std::string texto;
    bool leido;

public:
    Mensaje(Usuario* emisor, Usuario* receptor, const std::string& fechaEnvio,
            const std::string& texto, bool leido = false);

    Usuario* getEmisor() const;
    Usuario* getReceptor() const;
    const std::string& getFechaEnvio() const;
    const std::string& getTexto() const;
    bool estaLeido() const;

    void marcarComoLeido();
};

#endif // MENSAJE_H
