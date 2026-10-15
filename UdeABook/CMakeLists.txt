cmake_minimum_required(VERSION 3.19)
project(UdeABook LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

qt_add_executable(UdeABook
    mainDemo.cpp
    ListaDinamica.h
    Medidorrecursos.h
    Medidorrecursos.cpp
    Usuario.h
    Usuario.cpp
    Album.h
    Mensaje.h
    Persistencia.h
    Persistencia.cpp
    Sesion.h
    Sesion.cpp
    Persistencia.h
    Persistencia.cpp
    Sesion.h
    Sesion.cpp
)

target_link_libraries(UdeABook
    PRIVATE
        Qt::Core
)

install(TARGETS UdeABook
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
