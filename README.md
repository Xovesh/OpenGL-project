# OpenGL project
 Created a 3D engine using OpenGL

## Contents

1. Compilation and execution
2. Pictures
3. Controls

## Compilation

> If you want to compile it by yourself

Requirements
```
sudo apt-get update
sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev
sudo apt-get install libgtk-3-dev
```

GCC
```
gcc *.c -lGL -lGLU -lglut -lm `pkg-config --libs --cflags gtk+-3.0` -lpthread -rdynamic -o output
```

> Otherwise

Execution

```
./output
```

## Pictures

### GUI

![Screenshot](pictures/objetos.png)

> Adding a new object to the scene.

![Screenshot](pictures/escenarios.png)

> Changing the static scenary

![Screenshot](pictures/camara.png)

> Create a new Camera.

![Screenshot](pictures/iluminacion.png)

> Changing the ilumination.

![Screenshot](pictures/ayuda.png)

> Help (Spanish) English translation in GitHub (https://github.com/Xovesh/OpenGL-project).

### Without ilumination

![Screenshot](pictures/1.png)

> General scene with some objects (Orange selected object) (Using wireframe and solid renderization).

![Screenshot](pictures/2.png)

> General scene using only wireframe renderization.

![Screenshot](pictures/3.png)

> General scene using only solid renderization.

![Screenshot](pictures/4.png)

> General scene using solid and wireframe renderization.

![Screenshot](pictures/5.png)

> Scene with 120k polygons.

![Screenshot](pictures/7v2.png)

> Menu to charge and object fast (RMB).

![Screenshot](pictures/8.png)

> Scene with objects in different position, rotated and different scale.

### With ilumination

![Screenshot](pictures/sun_point_comp.png)

> Comparison between sun and point(bulb) lights.

![Screenshot](pictures/flat_smooth_comp.png)

> Comparison between flat and smooth shading.

![Screenshot](pictures/isla_dos_puntos.png)

> Using two lights (Different diffuse color).

![Screenshot](pictures/isla_dos_soles.png)

> Using two suns (Different diffuse color).

![Screenshot](pictures/desierto_camara.png)

> Desert scene using camera sportlight.

![Screenshot](pictures/desierto_modelos.png)

> Scene with some objects using lights.

![Screenshot](pictures/desierto_sol_camara.png)

> Desert scene using a sun and a spotlight.

![Screenshot](pictures/foco_reducido.png)

> Camera spotlight with reduce opening angle.


## Controls