#include "Mensaje.h"

Mensaje::Mensaje(Usuario* emisor, Usuario* receptor, const std::string& fechaEnvio,
                 const std::string& texto, bool leido)
    : emisor(emisor), receptor(receptor), fechaEnvio(fechaEnvio), texto(texto), leido(leido)
{
}

Usuario* Mensaje::getEmisor() const { return emisor; }
Usuario* Mensaje::getReceptor() const { return receptor; }
const std::string& Mensaje::getFechaEnvio() const { return fechaEnvio; }
const std::string& Mensaje::getTexto() const { return texto; }
bool Mensaje::estaLeido() const { return leido; }
void Mensaje::marcarComoLeido() { leido = true; }
