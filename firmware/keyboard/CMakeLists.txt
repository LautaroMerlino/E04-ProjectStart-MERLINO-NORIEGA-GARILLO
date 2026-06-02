cmake_minimum_required(VERSION 3.10)

# CMakeLists.txt para submódulo keyboard
add_library(keyboard OBJECT
  # Agregar archivos .c del teclado aquí
  # keyboard.c
)

target_include_directories(keyboard PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
