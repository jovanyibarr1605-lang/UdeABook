# UdeABook

Aplicación de consola en C++ para el Desafío II de Informática II, UdeA, semestre
2026-2. Modela la capa lógica de una red social sin usar contenedores de la STL,
`structs`, herencia ni librerías de terceros para resolver la lógica del problema.

## Compilación y ejecución

El proyecto requiere un compilador compatible con C++17. Desde la carpeta
`UdeABook`:

```powershell
$fuentes = Get-ChildItem -Recurse -Filter *.cpp | Select-Object -ExpandProperty FullName
g++ -std=c++17 -Imodelo -Iservicios -Inucleo -Iutilidades $fuentes -o UdeABook.exe
.\UdeABook.exe
```

También puede abrirse el archivo `UdeABook/CMakeLists.txt` en un IDE con soporte
para CMake.

## Estructura

| Ubicación | Responsabilidad |
| --- | --- |
| `main.cpp` | Punto de entrada y menú de consola. |
| `modelo/` | Entidades del dominio: usuarios, álbumes, fotos y mensajes. |
| `servicios/` | Inicio de sesión y lectura/escritura de los archivos persistentes. |
| `nucleo/` | Coordinación de las operaciones de la plataforma mediante `UdeABook`. |
| `utilidades/` | Lista dinámica propia y medición de recursos. |
| `datos/` | Archivos de datos utilizados por la aplicación. |

En la primera ejecución se crea automáticamente la carpeta `datos` con 1000
usuarios para las demostraciones solicitadas. Las credenciales iniciales son:

```text
usuario0001
clave123
```

## Funcionalidades

- Carga y actualización de datos persistentes.
- Inicio de sesión y selección ponderada de tres fotos de perfil de amigos.
- Envío y aceptación de solicitudes de amistad.
- Visita de perfiles amigos, álbumes visibles y carrusel de fotos.
- Sugerencias de amistad ordenadas por amigos en común.
- Envío, lectura y búsqueda de mensajes por rango de fecha.
- Reporte de iteraciones y memoria estimada al terminar cada operación.

## Persistencia

La carpeta `datos` contiene archivos de texto separados por `|`. Los caracteres
`|`, `\\` y los saltos de línea se escapan para preservar el contenido textual.

| Archivo | Contenido |
| --- | --- |
| `usuarios.txt` | Datos personales, perfil y credenciales de cada usuario. |
| `amistades.txt` | Cada amistad se guarda una sola vez. |
| `solicitudes.txt` | Usuario solicitante y usuario destinatario. |
| `albumes.txt` | Propietario, nombre, ruta y privacidad del álbum. |
| `fotos.txt` | Propietario, álbum, fecha, tamaño, ruta y cantidad de likes. |
| `mensajes.txt` | Emisor, receptor, fecha, texto y estado de lectura. |

La clase `UdeABook` concentra la propiedad de usuarios y mensajes; cada
`Usuario` es dueño de sus álbumes y cada `Album` es dueño de sus fotos. Las
relaciones de amistad y mensajes se mantienen mediante punteros no propietarios,
evitando duplicar o liberar dos veces los mismos objetos.
