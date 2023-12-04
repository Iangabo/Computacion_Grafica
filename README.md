# Computación Gráfica
Este repositorio contiene los trabajos realizados durante el curso de computación gráfica utilizando OpenGl/GLFW en c++.

## Proyecto Final: CUBO Rubik con solver y Animación

## Autores

* **Ian Gabriel Camero Gonzalez** - *Universidad Cdtolica San Pablo* 
* **Mauricio Carazas Segovia** - *Universidad Cdtolica San Pablo* 

## Requisitos
El código de ejemplo del `Cubo de Rubik` necesita CMake 3.20.0 o posterior.
Este proyecto también depende de 4 bibliotecas:

* [GLFW](https://www.glfw.org/)
* [GLAD](https://github.com/Dav1dde/glad)
* [GLM](https://github.com/g-truc/glm)
* [FREEIMAGE](https://freeimage.sourceforge.io/)

## Solver
Utilizamos el algoritmo de dos fases de Herbert Kociemba para resolver el cubo de Rubik, que puedes encontrar en este [GitHub](https://github.com/muodov/kociemba)

## Instructions 📦

- con las siguientes tecla spodras controlar el cubo

| Tecla | Movimiento |
| ------------- | ------------- |
| Q  | R |
| W  | L |
| E  | U |
| R  | D |
| T  | F |
| Y  | B |
| X  | Desarma el cubo con movimietos aleatorios |

- Con las siguientes tecla spodras resolver el cubo

| Tecla | Descrición |
| ------------- | ------------- |
| Z  | Resuelve el cubo despues de haber sido desarmado manualmente  |
| C  | Lo resuelve despues de desarmarlo con la tecla X |

- Animaciones

| Tecla | Descrición |
| ------------- | ------------- |
| 1  | Se crean mas cubos y simulan una explosión |
| 2  | El cubo respira  |

- Camara

| Tecla | Descrición |
| ------------- | ------------- |
| UP | Hace Zoom |
| DOWN | Zoom out |
| LEFT | Move left  |
| RIGHT | Move right  |
| MOUSE | Change camera position  |


