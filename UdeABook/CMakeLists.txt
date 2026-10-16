cmake_minimum_required(VERSION 3.19)
project(UdeABook LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(UdeABook
    main.cpp
    modelo/Usuario.cpp
    modelo/Album.cpp
    modelo/Foto.cpp
    modelo/Mensaje.cpp
    servicios/Persistencia.cpp
    servicios/Sesion.cpp
    nucleo/UdeABook.cpp
    utilidades/Medidorrecursos.cpp
)

target_include_directories(UdeABook PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/modelo
    ${CMAKE_CURRENT_SOURCE_DIR}/servicios
    ${CMAKE_CURRENT_SOURCE_DIR}/nucleo
    ${CMAKE_CURRENT_SOURCE_DIR}/utilidades
)
