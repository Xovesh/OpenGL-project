#include "definitions.h"

/*Transformaciones básicas*/
//Imprime la matriz que se le pasa como entrada
void print_matrix(char *msg,float *matrix);
//Imprime el vector que se le pasa como entrada
void print_vector(char *msg, vector3 vec);
//Imprime la informacion sobre el vector de tipo point3 que se le pasa como entrada
void print_point(char *msg, point3 point);
//Imprime informacion sobre el objeto que se le pasa como entrada
void print_object(char *msg, object3d *obj);
//Esta funcion libera el espacio de memoria de un struct object3d
void liberar_objeto(object3d *obj_a_liberar_ptr);
//Hace que _selected object apunte al proximo objeto seleccionable
void select_next_valid_object();
//Funcion que crea una camara utilizando gluLookAt
camera3d* create_camera( float posX, float posY, float posZ,
                        float atX , float atY , float atZ ,
                        float verticalX, float verticalY, float verticalZ,
                        int projection_mode );
// Funcion que elimina un objeto dado como parametro
void eliminar_objeto(object3d *obj);
// Funcion que deshace la ultima transformacion hecha a un objeto
void undo_last_object_change(object3d *obj);
// Funcion que actualiza el punto de origen de un objeto
void update_object_origin(object3d *target);
// Funcion que desplaza un objeto
void displace_selected_object(object3d *target, int reference_mode , float x, float y, float z);
// Funcion que coloca el objeto en una posicion X, Y y Z respecto del sistema de referencia global
void set_object_position(object3d *target, float x, float y, float z);
// Funcion que rota un objeto
void rotate_selected_object(object3d *target, int reference_mode, float angle, float x, float y, float z);
// Funcion que escala un objeto
void scale_selected_object(object3d *target, int reference_mode, float x, float y, float z);
// Funcion que coloca el objeto en el origen
void reset_object_to_origin(object3d *target);
// Modo analisis de la camara sobre el punto de atencion at
void camera_rotation_point(camera3d *camera, float degrees, float x, float y, float z, point3 at);
// Funcion que desplaza la camara en modo local
void displace_camera( camera3d* camera, float x, float y, float z);
// Funcion que rota la camara en modo local
void rotate_camera_local( camera3d* camera, float angle, float x, float y, float z);
//Funcion que coloca la camara sobre un objeto
void camera_to_object_perspective(camera3d *camera, object3d *object);
// Funcion que cambia el sistema de referencia al de un objeto
void cambiar_sistema_referencia_camara(camera3d *camera, object3d *target);

/*Movimiento*/
// Funcion que añade velocidad a un objeto
void change_object_velocity(object3d *target, float x, float y, float z);
// Funcion que establece una velocidad a un objeto
void set_object_velocity(object3d *target, float x, float y, float z);

/*Colision*/
// Funcion que comprueba si dos objetos estan colisionando
int colision_check(object3d *subject,object3d *target);
/*Input*/
// Funcion que lee un vector de tres elementos desde el teclado
void read_3f(vector3 *result, char *msg);
// Funcion que carga un objeto
object3d* cargar_objeto(char *object_path);
// Funcion que carga un objeto estatico
object3d* cargar_objeto_static(char *object_path);
// Funcion que carga un objeto que se va a destruir despues
object3d* cargar_objeto_disposable(char *object_path);
/*Ilum*/
// Funcion que crea una luz con los valores pasados
ilum* create_ilum_custom(ilum_type mode , color3 ambient, color3 diffuse, color3 speculars, point3 pos, int is_on);
// Funcion que crea una luz por defecto
ilum* create_ilum_default(ilum_type mode);
// Funcion que actualiza la posicion de una luz
void displace_ilum_point(int index, point3 pos);
// Funcion que desplaza una luz
void displace_ilum(int index, float x, float y, float z);
// Funcion que rota una luz
void rotate_ilum(ilum *target, int reference_mode, float angle, float x, float y, float z);
// Funcion que proporciona el color para el material arcoiris
color3 get_rainbow_color_t();
