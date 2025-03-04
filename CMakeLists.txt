cmake_minimum_required(VERSION 3.16)
project(VisorSAH)

# Agregar directorio de cabeceras
include_directories(src)

# Buscar Qt6 y habilitar los módulos necesarios
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets 3DCore 3DRender 3DExtras)

# Crear el ejecutable
add_executable(visor_sah src/algoritmo_sah.cpp src/main.cpp src/dibujo_arbol.cpp)

# Enlazar las bibliotecas de Qt6 necesarias
target_link_libraries(visor_sah
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::3DCore
    Qt6::3DRender
    Qt6::3DExtras
)

