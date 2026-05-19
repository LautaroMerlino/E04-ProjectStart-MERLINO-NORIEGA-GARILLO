cmake_minimum_required(VERSION 3.10)
project(GES)

set(CMAKE_C_STANDARD 11)  # Estándar de C

# Agregar la carpeta de encabezados
include_directories(inc)

# Buscar automáticamente todos los archivos .c en la carpeta
file(GLOB SOURCES "src/*.c")

# Crear el ejecutable con los archivos encontrados
add_executable(GES ${SOURCES})