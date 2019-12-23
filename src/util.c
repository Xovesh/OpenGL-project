#include "definitions.h"
#include "math_util.h"
#include <malloc.h>
#include <GL/glut.h>
#include <stdio.h>
#include <sys/time.h>
#include "load_obj.h"

#include "global_data_extern.h"


/*Utility functions*/

//Imprime la matriz que se le pasa como entrada
void print_matrix(char *msg, float *matrix){
    printf("%s\n",msg);
    for (int i = 0; i < 4; i++){
        for(int j = 0; j< 4; j++){
            printf("%lf ",matrix[4 * i +j]);
        }
        printf("\n");
    }
    printf("\n");
}

//Imprime el vector que se le pasa como entrada
void print_vector(char *msg, vector3 vec){
    printf("%s\n x:%lf y:%lf z:%lf\n",msg,vec.x,vec.y,vec.z);
}

//Imprime la informacion sobre el vector de tipo point3 que se le pasa como entrada
void print_point(char *msg, point3 point){
    printf("%s\n x:%lf y:%lf z:%lf\n",msg,point.x,point.y,point.z);
}

//Imprime informacion sobre el objeto que se le pasa como entrada
void print_object(char *msg, object3d *obj){
    printf("%s\n", msg);
    printf("Pointer @%p    | Vertex Table @%p\n", obj, obj->vertex_table);
    printf("Vertex num: %d | Face num: %d\n", obj->num_vertices, obj->num_faces);
    print_matrix("Mat :", obj->transformation_history->mat);
}

//Hace que _selected object apunte al proximo objeto seleccionable
void select_next_valid_object(){
    object3d  *auxiliar_object;
    int loop_flag = 0;

    if(_selected_object != NULL)
        auxiliar_object = _selected_object->next;
    else
        auxiliar_object = _first_object;
    
    while(true){
        if(auxiliar_object == NULL && loop_flag == 0){
            loop_flag = 1;
            auxiliar_object = _first_object;
            if(_first_object == NULL)
                break;
        }else if(auxiliar_object == NULL && loop_flag == 1)
            break;
        if(auxiliar_object->is_selectable)
            break;
        auxiliar_object = auxiliar_object->next;
    }
    _selected_object = auxiliar_object;
}

/*Camara*/

//Funcion que crea una camara utilizando gluLookAt

camera3d* create_camera( float posX, float posY, float posZ,
                         float atX , float atY , float atZ ,
                         float verticalX, float verticalY, float verticalZ,
                         int projection_mode ){

    camera3d *new_camera = malloc(sizeof(camera3d));
    point3 pos = { posX, posY, posZ};
    point3 at  = { atX , atY , atZ };
    vector3 up = { verticalX, verticalY, verticalZ};
    
    new_camera-> pos = pos;
    new_camera-> attention_point = at;
    new_camera-> verticality_vec = up;
    new_camera ->projection_mode = projection_mode;

    new_camera->ortho_top =     KG_ORTHO_Y_MAX_INIT;
    new_camera->ortho_bottom =  KG_ORTHO_Y_MIN_INIT;
    new_camera->ortho_left =    KG_ORTHO_X_MIN_INIT;
    new_camera->ortho_right =   KG_ORTHO_Y_MAX_INIT;
    new_camera->ortho_far =     KG_ORTHO_Z_MAX_INIT;
    new_camera->ortho_near =    KG_ORTHO_Z_MIN_INIT;

    new_camera->persp_top =     KG_PERSP_Y_MAX_INIT;
    new_camera->persp_bottom =  KG_PERSP_Y_MIN_INIT;
    new_camera->persp_left =    KG_PERSP_X_MIN_INIT;
    new_camera->persp_right =   KG_PERSP_X_MAX_INIT;
    new_camera->persp_far =     KG_PERSP_Z_MAX_INIT;
    new_camera->persp_near =    KG_PERSP_Z_MIN_INIT;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
            gluLookAt(
                      pos.x, pos.y, pos.z,
                       at.x,  at.y,  at.z,
                      up.x, up.y, up.z);

    glGetFloatv(GL_MODELVIEW_MATRIX,new_camera->mat);
    new_camera ->next = _first_camera;
    _first_camera = new_camera;

    return new_camera;
}

//Funcion que coloca la camara sobre un objeto

void camera_to_object_perspective(camera3d *camera, object3d *object){

    // vectores X, Y, Z y E

    vector3 vec_X = { object->transformation_history->mat[0], object->transformation_history->mat[1], object->transformation_history->mat[2]  };
    vector3 vec_Y = { object->transformation_history->mat[4], object->transformation_history->mat[5], object->transformation_history->mat[6]  };
    vector3 vec_Z = { object->transformation_history->mat[8], object->transformation_history->mat[9], object->transformation_history->mat[10]  };
    vector3 vec_E = { object->transformation_history->mat[12], object->transformation_history->mat[13], object->transformation_history->mat[14]  };

     // vector E modifica que coloca la camara por encima del objeto

    vector3 vec_mod_E = add_vector3(vec_E, mult_scalar_with_vector3( vec_Y, object->max.y) ) ;
    
    point3 new_pos_mod = {vec_mod_E.x, vec_mod_E.y, vec_mod_E.z};
    camera->pos = new_pos_mod;
    
    // Nueva matriz de la camara

    GLfloat aux_mat[16] = {
           object->transformation_history->mat[0], object->transformation_history->mat[4] , object->transformation_history->mat[8]   , 0 ,
           object->transformation_history->mat[1], object->transformation_history->mat[5] , object->transformation_history->mat[9]   , 0 ,
           object->transformation_history->mat[2], object->transformation_history->mat[6] , object->transformation_history->mat[10]  , 0 ,
           -vector3_mult(vec_mod_E,vec_X), -(vector3_mult(vec_mod_E,vec_Y)), -vector3_mult(vec_mod_E,vec_Z), 1  
    };

    // Guardamos la matriz en su lugar

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(aux_mat);
    glGetFloatv(GL_MODELVIEW_MATRIX,camera->mat);
    redraw = 1;
}

// Funcion que desplaza la camara en modo local
void displace_camera( camera3d* camera, float x, float y, float z){

    // Matriz traspuesta

    GLfloat aux_mat[16] = {
           camera->mat[0] , camera->mat[4]  , camera->mat[8]    , 0,
           camera->mat[1] , camera->mat[5]  , camera->mat[9]    , 0,
           camera->mat[2] , camera->mat[6]  , camera->mat[10]   , 0,
           camera->pos.x  , camera->pos.y   , camera->pos.z     , 1  
    };
    
    // Le aplicamos la traslacion

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixf(aux_mat);
    glTranslatef(x , y, z);
    glGetFloatv(GL_MODELVIEW_MATRIX,camera->mat);
    
    point3 new_pos = { camera->mat[12], camera->mat[13], camera->mat[14]};
    camera->pos = new_pos;
    
    // sacamos vectores Xc, Yc, Zc y E

    vector3 vec_X = { camera->mat[0], camera->mat[1], camera->mat[2]  };
    vector3 vec_Y = { camera->mat[4], camera->mat[5], camera->mat[6]  };
    vector3 vec_Z = { camera->mat[8], camera->mat[9], camera->mat[10] };
    vector3 vec_E = { camera->mat[12], camera->mat[13], camera->mat[14] };

    // Aplicamos la traspuesta de nuevo

    GLfloat aux_mat_2[16] = {
           camera->mat[0] , camera->mat[4]  , camera->mat[8]    , 0,
           camera->mat[1] , camera->mat[5]  , camera->mat[9]    , 0,
           camera->mat[2] , camera->mat[6]  , camera->mat[10]   , 0,
           -1*vector3_mult(vec_E,vec_X), -1*vector3_mult(vec_E,vec_Y) ,-1*vector3_mult(vec_E, vec_Z) , 1  
    };

    // Guardamos la matriz en su lugar

    glLoadMatrixf(aux_mat_2);
    glGetFloatv(GL_MODELVIEW_MATRIX,camera->mat);
    
    point3 new_lookAt = {camera->attention_point.x + x ,camera->attention_point.y + y, camera->attention_point.z + z};
    camera->attention_point = new_lookAt;
    redraw = 1;
}

// Funcion que rota la camara en modo local

void rotate_camera_local( camera3d* camera, float angle, float x, float y, float z){
    // Matriz traspuesta

    GLfloat aux_mat[16] = {
           camera->mat[0] , camera->mat[4]  , camera->mat[8]    , 0,
           camera->mat[1] , camera->mat[5]  , camera->mat[9]    , 0,
           camera->mat[2] , camera->mat[6]  , camera->mat[10]   , 0,
           camera->pos.x  , camera->pos.y   , camera->pos.z     , 1  
    };

    // Le aplicamos la rotacion

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixf(aux_mat);
    glRotatef(angle ,x , y, z);
    glGetFloatv(GL_MODELVIEW_MATRIX,camera->mat);
    aux_mat[12] = camera->pos.x;
    aux_mat[13] = camera->pos.y;
    aux_mat[14] = camera->pos.z;

    // sacamos vectores Xc, Yc, Zc y E

    vector3 vec_E = { camera->mat[12], camera->mat[13], camera->mat[14] };
    vector3 vec_X = { camera->mat[0], camera->mat[1], camera->mat[2]  };
    vector3 vec_Y = { camera->mat[4], camera->mat[5], camera->mat[6]  };
    vector3 vec_Z = { camera->mat[8], camera->mat[9], camera->mat[10] };

    // Aplicamos la traspuesta de nuevo

    GLfloat aux_mat_2[16] = {
           camera->mat[0] , camera->mat[4]  , camera->mat[8]    , 0,
           camera->mat[1] , camera->mat[5]  , camera->mat[9]    , 0,
           camera->mat[2] , camera->mat[6]  , camera->mat[10]   , 0,
           -1*vector3_mult(vec_E,vec_X), -1*vector3_mult(vec_E,vec_Y) ,-1*vector3_mult(vec_E, vec_Z) , 1  
    };
    
    // Guardamos la matriz en su lugar

    glLoadMatrixf(aux_mat_2);
    glGetFloatv(GL_MODELVIEW_MATRIX,camera->mat);
    
    redraw = 1;
}

// Modo analisis de la camara sobre el punto de atencion at

void camera_rotation_point(camera3d *camera, float degrees, float x, float y, float z, point3 at){
    
    // Matriz que coloca el punto de atencion en el 0 0 0
    
    float minus_at_mat[16] = {1,0,0,0,
                              0,1,0,0,
                              0,0,1,0,
                              -at.x,-at.y,-at.z,1};
    
    // Matriz que coloca el punto de atencion en su posicion original

    float at_mat[16] = {  1,0,0,0,
                          0,1,0,0,
                          0,0,1,0,
                          at.x,at.y,at.z,1};

    // Matriz traspuesta de la camara

    GLfloat aux_mat[16] = {
        camera->mat[0] , camera->mat[4]  , camera->mat[8]    , 0,
        camera->mat[1] , camera->mat[5]  , camera->mat[9]    , 0,
        camera->mat[2] , camera->mat[6]  , camera->mat[10]   , 0,
        camera->pos.x  , camera->pos.y   , camera->pos.z     , 1  
    };
    
    // Matriz de rotacion

    float rotation_mat[16], result[16];
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(degrees, x, y, z);
    glGetFloatv(GL_MODELVIEW_MATRIX,rotation_mat);
    
    // Aplicamos la multilicacion de matrices

    glLoadIdentity();
    glMultMatrixf(at_mat);
    glMultMatrixf(rotation_mat);
    glMultMatrixf(minus_at_mat);
    glMultMatrixf(aux_mat);
    glGetFloatv(GL_MODELVIEW_MATRIX,result);

    point3 aux_eye = {result[12],result[13],result[14]};
    camera ->pos = aux_eye;

    // sacamos vectores Xc, Yc, Zc y E

    vector3 vec_X = { result[0],  result[1],  result[2]  };
    vector3 vec_Y = { result[4],  result[5],  result[6]  };
    vector3 vec_Z = { result[8],  result[9],  result[10] };
    vector3 vec_E = { result[12], result[13], result[14] };
    
    // Aplicamos la traspuesta de nuevo
    
    GLfloat aux_mat_2[16] = {
           result[0] , result[4]  , result[8]    , 0,
           result[1] , result[5]  , result[9]    , 0,
           result[2] , result[6]  , result[10]   , 0,
           -1*vector3_mult(vec_E,vec_X), -1*vector3_mult(vec_E,vec_Y) ,-1*vector3_mult(vec_E, vec_Z) , 1  
    };
    
    // Guardamos la matriz en su lugar

    glLoadMatrixf(aux_mat_2);
    glGetFloatv(GL_MODELVIEW_MATRIX,camera->mat);
    redraw = 1;
}

// Funcion que cambia el sistema de referencia al de un objeto

void cambiar_sistema_referencia_camara(camera3d *camera, object3d *target){
    if( target == NULL || !target->is_selectable)
        return;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(  camera->pos.x, camera->pos.y, camera->pos.z,
                target->origin_pos.x, target->origin_pos.y, target->origin_pos.z,
                target->transformation_history->mat[4], target->transformation_history->mat[5], target->transformation_history->mat[6] );
                
    glGetFloatv(GL_MODELVIEW_MATRIX,camera->mat);
    redraw = 1;
}

/*Object*/

/**
 * @brief Esta funcion libera el espacio de memoria de un struct object3d
 * @param face *cara_a_liberar_ptr Puntero a objeto a liberar 
 **/
void liberar_objeto(object3d *obj_a_liberar_ptr){
    int i;
    for (i = 0; i < obj_a_liberar_ptr->num_faces; i++)
    {
        free(obj_a_liberar_ptr->face_table[i].vertex_table);
    }
    free(obj_a_liberar_ptr->vertex_table);
    free(obj_a_liberar_ptr->face_table);
    free(obj_a_liberar_ptr);
}

// Funcion que elimina un objeto dado como parametro

void eliminar_objeto(object3d *obj){
    if (obj == NULL)
            return;
    if (obj == _first_object)
    {
        if(_first_object == NULL)
            return;
        /*To remove the first object we just set the first as the current's next*/
        _first_object = _first_object->next;
        /*Once updated the pointer to the first object it is save to free the memory*/
        liberar_objeto(obj);
        /*Finally, set the selected to the new first one*/
    }
    else
    {
        /*In this case we need to getmissile_Update the previous element to the one we want to erase*/
        object3d *auxiliar_object;
        auxiliar_object = _first_object;
        while (auxiliar_object->next != obj)
            auxiliar_object = auxiliar_object->next;
        /*Now we bypass the element to erase*/
        auxiliar_object->next = obj->next;
        /*free the memory*/
        liberar_objeto(obj);
    }
        select_next_valid_object();
}

// Funcion que actualiza el punto de origen de un objeto

void update_object_origin(object3d *target){
    target->origin_pos.x = target->transformation_history->mat[12];
    target->origin_pos.y = target->transformation_history->mat[13];
    target->origin_pos.z = target->transformation_history->mat[14];
}

// Funcion que deshace la ultima transformacion hecha a un objeto

void undo_last_object_change(object3d *obj){
    if (_selected_object != NULL && _selected_object->transformation_history->next != NULL)
    {
        transformation_matrix *aux_mat;
        aux_mat = _selected_object->transformation_history->next;
        free(_selected_object->transformation_history);
        _selected_object->transformation_history = aux_mat;
    }
    update_object_origin(obj);
    camera3d *aux = _first_camera;
    while (aux != NULL)
    {
        if(aux->parent == obj)
        {
            camera_to_object_perspective(aux,obj);
        }
        aux = aux->next;
    }
    redraw = 1;
}

// Funcion que desplaza un objeto

void displace_selected_object(object3d *target, int reference_mode , float x, float y, float z){
    transformation_matrix *aux_mat_ptr;
    aux_mat_ptr = malloc(sizeof(transformation_matrix));

    if (reference_mode == LOCAL_MODE){
        // Modo Local

        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(target->transformation_history->mat);
        glTranslatef(x, y, z);
    }else{

        // Modo Global

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(x, y, z);
        glMultMatrixf(target->transformation_history->mat);
    }

    // Guardamos la matriz

    glGetFloatv(GL_MODELVIEW_MATRIX, aux_mat_ptr->mat);
    aux_mat_ptr->next = target->transformation_history;
    target->transformation_history = aux_mat_ptr;
    update_object_origin(target);

    if(camera_mode == CAMERA_ANALISIS_MODE && target == _selected_object && target->is_selectable)
        cambiar_sistema_referencia_camara(_selected_camera, target);

    camera3d *aux = _first_camera;
    
    // Actualizamos (si tiene) la posicion de la camara

    while (aux != NULL)
    {
        if(aux->parent == target)
        {
            camera_to_object_perspective(aux,target);
            break;
        }
        aux = aux->next;
    }
    
    redraw = 1;
}

// Funcion que coloca el objeto en una posicion X, Y y Z respecto del sistema de referencia global

void set_object_position(object3d *target, float x, float y, float z){
    displace_selected_object(target, GLOBAL_MODE, -target->origin_pos.x, -target->origin_pos.y, -target->origin_pos.z);
    displace_selected_object(target, GLOBAL_MODE, x, y, z);
    redraw = 1;
}

// Funcion que rota un objeto

void rotate_selected_object(object3d *target, int reference_mode, float angle, float x, float y, float z){
    transformation_matrix *aux_mat_ptr;
    aux_mat_ptr = malloc(sizeof(transformation_matrix));
    if (reference_mode == LOCAL_MODE){

        // Modo Local
    
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(target->transformation_history->mat);
        glRotatef(angle, x, y, z);
    }else{
        
        // Modo Global

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef(angle, x, y, z);
        glMultMatrixf(target->transformation_history->mat);
    }
    
    // Guardamos la matriz

    glGetFloatv(GL_MODELVIEW_MATRIX, aux_mat_ptr->mat);
    aux_mat_ptr->next = target->transformation_history;
    target->transformation_history = aux_mat_ptr;
    update_object_origin(target);

    if(camera_mode == CAMERA_ANALISIS_MODE)
        cambiar_sistema_referencia_camara(_selected_camera, target);
    
    // Actualizamos (si tiene) la posicion de la camara

    camera3d *aux = _first_camera;
    while (aux != NULL)
    {
        if(aux->parent == target)
        {
            if(reference_mode == LOCAL_MODE)
                camera_to_object_perspective(aux,target);
            else{
                point3 global_origin = {0, 0, 0};
                camera_rotation_point(aux, angle, x, y, z, global_origin);
            }
            break;
        }
        aux = aux->next;
    }
    redraw = 1;
}

// Funcion que escala un objeto

void scale_selected_object(object3d *target, int reference_mode, float x, float y, float z){
    
    transformation_matrix *aux_mat_ptr;
    aux_mat_ptr = malloc(sizeof(transformation_matrix));
    target->scale[0] *= x;
    target->scale[1] *= y;
    target->scale[2] *= z;
    if (reference_mode == LOCAL_MODE){
        
        // Modo Local

        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(target->transformation_history->mat);
        glScalef(x, y, z);
    }else{
        
        // Modo Global

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glScalef(x, y, z);
        glMultMatrixf(target->transformation_history->mat);
    }
    
    // Guardamos la matriz

    glGetFloatv(GL_MODELVIEW_MATRIX, aux_mat_ptr->mat);
    aux_mat_ptr->next = target->transformation_history;
    target->transformation_history = aux_mat_ptr;
    update_object_origin(target);
    redraw = 1;
}

// Funcion que coloca el objeto en el origen

void reset_object_to_origin(object3d *target){
    set_object_position(target,0,0,0);
    update_object_origin(target);
    redraw = 1;
}

/*Movimiento*/

// Funcion que añade velocidad a un objeto

void change_object_velocity(object3d *target, float x, float y, float z){
    vector3 aux = {x, y, z};
    target->velocity = add_vector3(target->velocity, aux);
}

// Funcion que establece una velocidad a un objeto

void set_object_velocity(object3d *target, float x, float y, float z){
    vector3 aux = {x, y, z};
    target->velocity = aux;
}

/*Colision*/

// Funcion que comprueba si dos objetos estan colisionando

int colision_check(object3d *subject,object3d *target){
    return distance_3d(subject->origin_pos,target->origin_pos) < (subject->bound_radius + target->bound_radius);
}

/*Input management*/

// Funcion que lee un vector de tres elementos desde el teclado

void read_3f(vector3 *result, char *msg){
    printf("%s", msg);
    scanf("%lf %lf %lf", &(result->x), &(result->y), &(result->z));
}

/*Load object*/

// Funcion que carga un objeto

object3d* cargar_objeto(char *object_path){

    object3d *auxiliar_object;
    int read = 0;

    auxiliar_object = (object3d *)calloc( 1, sizeof(object3d));
    read = read_wavefront(object_path, auxiliar_object);
    switch (read)
    {
    /*Errors in the reading*/
    case 1:
        printf("%s: %s\n", object_path, KG_MSSG_FILENOTFOUND);
        break;
    case 2:
        printf("%s: %s\n", object_path, KG_MSSG_INVALIDFILE);
        break;
    case 3:
        printf("%s: %s\n", object_path, KG_MSSG_EMPTYFILE);
        break;
    /*Read OK*/
    case 0:
        /*Insert the new object in the list*/

        auxiliar_object->next = _first_object;
        _first_object = auxiliar_object;
        _selected_object = _first_object;
        //print_object("Cargar objeto",_first_object);
        printf("%s\n", KG_MSSG_FILEREAD);
        break;
    }
    redraw = 1;
    return auxiliar_object;
}

// Funcion que carga un objeto estatico

object3d* cargar_objeto_static(char *object_path){
    object3d *auxiliar_object;
    int read = 0;

    auxiliar_object = (object3d *)malloc(sizeof(object3d));
    read = read_wavefront(object_path, auxiliar_object);
    switch (read)
    {
    /*Errors in the reading*/
    case 1:
        printf("%s: %s\n", object_path, KG_MSSG_FILENOTFOUND);
        break;
    case 2:
        printf("%s: %s\n", object_path, KG_MSSG_INVALIDFILE);
        break;
    case 3:
        printf("%s: %s\n", object_path, KG_MSSG_EMPTYFILE);
        break;
    /*Read OK*/
    case 0:
        /*Insert the new object in the list*/
        auxiliar_object->next = _first_static;
        _first_static = auxiliar_object;
        _selected_static = _first_static;
        printf("%s\n", KG_MSSG_FILEREAD);
        break;
    }
    
    return auxiliar_object;
}

/*ILUMINATION*/

// Funcion que crea una luz con los valores pasados

ilum* create_ilum_custom(ilum_type mode , color3 ambient, color3 diffuse, color3 specular, point3 pos , int is_on, GLfloat exp, GLfloat amp, vector3 spot_dir){
    ilum *result = malloc(sizeof(ilum));
    result->mode = mode;
    result->is_on = is_on;
    result->ambient = ambient;
    result->diffuse = diffuse;
    result->specular = specular;
    result->pos = pos;
    result->amplitude = (amp > 90)? 90 : (amp < 0)? 0: amp;
    result->spot_exponent = (exp > 128)? 128 : (exp < 0)? 0 : exp;
    result->spot_direction = spot_dir;
    result->parent_obj = NULL;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef( pos.x, pos.y, pos.z);
    glGetFloatv(GL_MODELVIEW_MATRIX, result->mat);
    return result;
}

// Funcion que crea una luz por defecto

ilum* create_ilum_default(ilum_type mode){
    ilum *result = malloc(sizeof(ilum));
    color3 ambient  = { 0, 0, 0};
    color3 diffuse  = { 1, 1, 1};
    color3 specular = { 0.1, 0.1, 0.1};
    point3 pos = { 0, 5, 0};
    vector3 spot_dir = { 0, 0, -1};
    return create_ilum_custom(mode ,ambient,diffuse,specular , pos , 0, 2, 45, spot_dir);
}

// Funcion que actualiza la posicion de una luz

void displace_ilum_point(int index, point3 pos){
    lights[index]->pos =  add_point3( lights[index]->pos, pos);
}

// Funcion que desplaza una luz

void displace_ilum(int index, float x, float y, float z){
    point3 aux = { x, y, z};
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(lights[index]->mat);
    glTranslatef( x, y, z);
    glGetFloatv(GL_MODELVIEW_MATRIX, lights[index]->mat);
    displace_ilum_point(index , aux);
}

// Funcion que rota una luz
void rotate_ilum(ilum *target, int reference_mode, float angle, float x, float y, float z){
    if (reference_mode == LOCAL_MODE){
        // Modo local
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(target->mat);
        glRotatef(angle, x, y, z);
        glGetFloatv(GL_MODELVIEW_MATRIX, target->mat);
    }else{
        // Modo Global
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef(angle, x, y, z);
        glMultMatrixf(target->mat);
        glGetFloatv(GL_MODELVIEW_MATRIX, target->mat);
        target->pos = create_point_3( target->mat[12], target->mat[13], target->mat[14]);
    }
}

// Funcion que proporciona el color para el material arcoiris en base al tiempo
color3 get_rainbow_color_t(){
    // Encontrar punto en el tiempo
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    double secs = 0;
    secs = (double)(current_time.tv_usec) / 1000000 + (double)(current_time.tv_sec);
    int base_index = (int) secs;
    double weigth = secs - base_index;
    base_index = base_index % 8;
    // Interpolar para hallar el color resultante
    color3 base = rainbow_array[base_index];
    color3 next = rainbow_array[(base_index == 7)? 0 : base_index + 1];
    return create_color3( base.r * (1 - weigth) + next.r * weigth,
                          base.g * (1 - weigth) + next.g * weigth,
                          base.b * (1 - weigth) + next.b * weigth);
}
