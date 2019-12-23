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

> Menu to load a object fast (RMB).

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


## Controls (Spanish version within the project)

Extended version of the control list.

### English

#### General Options

\<ESC\> Exit the application

\<?\>   Shows the control list in the terminal

\<RMB\> Load a object

#### Manipulation mode

\<O/o\> Object Mode

\<K/k\> Camera Mode

\<A/a\> Ilumination Mode

#### Transformation mode

Over the selected object/camara/ilumination

\<M/m\> Translation Mode

\<B/b\> Rotation mode

\<T/t\> Scale mode

\<V/v\> Speed mode


#### Object manipulation

\<F/f\> Loads a object from a path (terminal)

\<Ctrl + Z\> Undo last transformation

\<TAB\> Selects the next object in the list

\<supr\> Deletes the selected object 

\<L/l\> Activates the local mode (Using objects reference system).

\<G/g\> Activar the global mode (Using global reference system).

\<LEFT\> Translates: -X; Scale: -X; Rotate: -Y.

\<RIGHT\> Translates: +X; Scale: +X; Rotate: +Y.

\<UP\> Translates: +Y; Scale: +Y; Rotate: +X.

\<DOWN\> Translates: -Y; Scale: -Y; Rotate: -X.

\<AvPag\>  Translates: +Z; Scale: +Z; Rotate: +Z.

\<RePag\> Translates: -Z; Scale: -Z; Rotate: -Z.

\<+\> Scales the object in all directions (positive scale "Bigger")

\<-\> Scales the object in all directions (negative scale "Smaller").

\<Enter\> Change the speed of the selected object to 0.

\<R/r\> Translates the object to the point X=0; Y=0; Z=0.

#### Camera manipulation

\<F/f\> Loads a new camera given the position vector, attention point and vertical vector.

\<c\> Selects a the next camera in the list.

\<C\> Creates a new camera that follows the selected object.

\<P/p\> Changes the perspective mode between orthogonal and perspective.

\<L/l\> Activates the local mode.

\<G/g\> Analysis mode over the selected object.

\<LEFT\> Translates: -X; Volume: -X; Rotate: -Y.

\<RIGHT\> Translates: +X; Volume: +X; Rotate: +Y.

\<UP\> Translates: +Y; Volume: +Y; Rotate: +X.

\<DOWN\> Translates: -Y; Volume: -Y; Rotate: -X.

\<AvPag\>  Translates: +Z; Volume: +near, + far; Rotate: +Z.

\<RePag\> Translates: -Z; Volume: -near, + far; Rotate: -Z.

\<+\> Positive zoom.

\<-\> Negative zoom.

#### Ilumination manipulation

\<0\> Changes the ilumination type between sun and spotlight.

\<1-8\> Selects the corresponding light.

\<F1-F8\> ON/OFF selected light

\<F9\> ON/OFF Ilumination mode

\<F12\> Changes the shade mode between flat and smooth

\<LEFT\> Translates: -X; Rotate: -Y.

\<RIGHT\> Translates: +X; Rotate: +Y.

\<UP\> Translates: +Y; Rotate: +X.

\<DOWN\> Translates: -Y; Rotate: -X.

\<AvPag\>  Translates: +Z; Rotate: +Z.

\<RePag\> Translates: -Z; Rotate: -Z.

\<+\> Increases opening angle (only spotlight).

\<-\> Decreases opening angle (only spotlight).


#### Others

\<Q/q\> Show/hide scene

\<Z/z\> Alternate between solid/wireframe/both render

\<U/u\> Loads the default object (check config.toml)

\<S/s\> Shoots a projectile. (only works in "./abioia.obj" object)

\<\"<"\>  Increases the width of the lines.

\<\">"\>  Reduces the width of lines.

\<LMB\> Moves the camera to left/right and top/bottom.

\<MMB\> Rotates the camera over the point X=0; Y=0; Z=0.

\<Scroll\> In camera mode: Move forward/backward. In object mode: Scale the object.

\<Ctrl + Scroll\> Increases/reduces the sensibility of the mouse.

\<Ctrl + MMB\> Rotates the camera in local mode.


