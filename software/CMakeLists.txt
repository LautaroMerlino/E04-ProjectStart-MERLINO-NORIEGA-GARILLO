cmake_minimum_required(VERSION 3.10)

# Configuración del proyecto de software
project(E04_Software C CXX)

# Configurar el tipo de construcción
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Configuración del compilador
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Directorio de salida para binarios
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Añadir directorios de código fuente
# add_subdirectory(src)

# Crear ejecutable principal (placeholder - se actualizará según estructura real)
# add_executable(software src/main.c)

# Habilitar testing
enable_testing()

# Buscar ejecutables de testing
# add_subdirectory(tests)

message(STATUS "Software project configured successfully")
