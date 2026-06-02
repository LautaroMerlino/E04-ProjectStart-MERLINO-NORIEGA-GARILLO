cmake_minimum_required(VERSION 3.10)

# Configuración del proyecto de firmware
project(E04_Firmware C CXX ASM)

# Configurar el tipo de construcción
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Configuración del compilador
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Directorio de salida para binarios
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Incluir directorios de código fuente
add_subdirectory(keyboard)
add_subdirectory(menu)

# Crear ejecutable principal (placeholder - se actualizará según el microcontrolador)
# add_executable(firmware.elf main.c)

# Generar archivos HEX y BIN (opcional, para flashing a microcontrolador)
# add_custom_command(TARGET firmware.elf POST_BUILD
#   COMMAND ${CMAKE_OBJCOPY} -Oihex firmware.elf firmware.hex
#   COMMAND ${CMAKE_OBJCOPY} -Obinary firmware.elf firmware.bin
# )

message(STATUS "Firmware project configured successfully")
