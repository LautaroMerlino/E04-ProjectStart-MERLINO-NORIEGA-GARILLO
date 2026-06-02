cmake_minimum_required(VERSION 3.10)

# CMakeLists.txt para submódulo menu
add_library(menu OBJECT
  # Agregar archivos .c del menú aquí
  # menu.c
)

target_include_directories(menu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
