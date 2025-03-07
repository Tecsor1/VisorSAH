# Proyecto de KD-Tree en C++ con Qt

Este proyecto es un prototipo que permite la visualización en 3D de las AABBs (Axis-Aligned Bounding Boxes) generadas a partir de un archivo OBJ. El KD-Tree es una estructura de datos utilizada para organizar puntos en un espacio k-dimensional. Este proyecto incluye un componente gráfico basado en Qt que permite representar tanto el árbol KD como las AABBs creadas para el archivo OBJ en una interfaz visual.

## Dependencias

Este proyecto depende de las siguientes bibliotecas:

- **Qt 6**: Para la interfaz gráfica y la visualización 3D. Se requieren los siguientes módulos de Qt 6:
  - `Core`
  - `Gui`
  - `Widgets`
  - `3DCore`
  - `3DRender`
  - `3DExtras`

- **C++17** o superior: El proyecto está configurado para utilizar C++17 como estándar de compilación.

## Instrucciones de Compilación

1. Clona el repositorio y accede al directorio del proyecto:

```
git clone https://github.com/usuario/VisorSAH.git
cd VisorSAH
mkdir build
cd build
cmake ..
make
./visor_sah
