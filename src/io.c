#include <GL/glut.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <math.h>

#include <gtk/gtk.h>

#include "definitions.h"
#include "util.h"
#include "math_util.h"
#include "load_obj.h"
#include "object_functions.h"

#include "global_data_extern.h"

struct timeval last_time, current_time;
int mousewheel_x_last, mousewheel_y_last;
int left_x_last, left_y_last;
int mousewheel_pressed = 0, left_click_pressed = 0, special_wheel = 0;

/**
 * @brief This function just prints information about the use
 * of the keys
 */
void print_help()
{
    
    printf("Practica de graficos por computador. Este programa \n");
    printf("muestra y transforma objetos 3D.  \n\n");
    printf("\n\n");

    printf("Funciones Principales \n");
    printf("<?>\t\t Ayuda \n");
    printf("<ESC>\t\t Cerrar programa \n");
    printf("<RMB>\t\t Mostrar menu de carga rapida \n");
    printf("\n\n");

    printf("Modo de manipulacion \n");
    printf("<K/k>\t\t Activar modo camara \n");
    printf("<O/o>\t\t Activar modo objeto \n");
    printf("<A/a>\t\t Activar modo iluminacion\n");
    printf("\n\n");

    printf("Modo de transformacion \n");
    printf("<M/m>\t\t Activar modo traslacion \n");
    printf("<B/b>\t\t Activar modo rotacion \n");
    printf("<T/t>\t\t Activar modo escalado \n");
    printf("<V/v>\t\t Activar modo velocidad \n");
    printf("\n\n");

    printf("Manipulacion de Objeto \n");
    printf("<F/f>\t\t Cargar objeto(mirar consola) \n");
    printf("<Ctrl + Z>\t\t Deshacer ultima transformacion \n");
    printf("<TAB>\t\t Seleccionar siguiente objeto \n");
    printf("<supr/del>\t\t Eliminar objeto seleccionado \n");
    printf("<L/l>\t\t Activar referencia local \n");
    printf("<G/g>\t\t Activar referencia global \n");
    printf("<Left>\t\t TRASLADAR -X; ESCALAR -X; ROTAR -Y \n");
    printf("<Right>\t\t TRASLADAR +X; ESCALAR +X; ROTAR +Y \n");
    printf("<Up>\t\t TRASLADAR +Y; ESCALAR +Y; ROTAR +X \n");
    printf("<Down>\t\t TRASLADAR -Y; ESCALAR -Y; ROTAR -X \n");
    printf("<AvPag>\t\t TRASLADAR +Z; ESCALAR +Z; ROTAR +Z \n");
    printf("<RePag>\t\t TRASLADAR -Z; ESCALAR -Z; ROTAR -Z \n");
    printf("<+>\t\t Escala el objeto seleccionado positivamente en todos los ejes \n");
    printf("<->\t\t Escala el objeto seleccionado negativamente en todos los ejes \n");
    printf("<Enter>\t\t Fija le velocidad del obbjeto seleccionado a 0 \n");
    printf("<R/r>\t\t Traslada el objeto seleccionado al origen global \n");
    printf("\n\n");

    printf("Manipulacion de la Camara");
    printf("<F/f>\t\t Cargar objeto(mirar consola) \n");
    printf("<c>\t\t Seleccionar siguiente camara \n");
    printf("<C>\t\t Crea una nueva camara desde la perspectiva del objeto seleccionado \n");
    printf("<P/p>\t\t Alterna entre proyeccion ortografica y de perspectiva \n");
    printf("<L/l>\t\t Activar modo de vuelo libre \n");
    printf("<G/g>\t\t Activar modo analisis \n");
    printf("<Left>\t\t TRASLADAR -X; VOLUMEN-X; ROTAR -Y \n");
    printf("<Right>\t\t TRASLADAR +X; VOLUMEN +X; ROTAR +Y \n");
    printf("<Up>\t\t TRASLADAR +Y; VOLUMEN +Y; ROTAR +X \n");
    printf("<Down>\t\t TRASLADAR -Y; VOLUMEN -Y; ROTAR -X \n");
    printf("<AvPag>\t\t TRASLADAR +Z; VOLUMEN +nf; ROTAR +Z \n");
    printf("<RePag>\t\t TRASLADAR -Z; VOLUMEN -nf; ROTAR -Z \n");
    printf("<+>\t\t Reduce el volumen de vision (aumenta el zoom) \n");
    printf("<->\t\t Aumenta el volumen de vision (reduce el zoom) \n");
    printf("\n\n");

    printf("Manipulacion de la iluminacion");
    printf("<0>\t\t Cambiar tipo de iluminacion de la luz seleccionada\n");
    printf("<1-8>\t\t Seleccionar luz correspondiente\n");
    printf("<F1-F8>\t\t Encendender/Apagar luz correspondiente\n");
    printf("<F9>\t\t Activar/desactivar iluminacion\n");
    printf("<F12>\t\t Cambiar shadel smooth/flat \n");
    printf("<Left>\t\t TRASLADAR -X; ROTAR -Y \n");
    printf("<Right>\t\t TRASLADAR +X; ROTAR +Y \n");
    printf("<Up>\t\t TRASLADAR +Y; ROTAR +X \n");
    printf("<Down>\t\t TRASLADAR -Y; ROTAR -X \n");
    printf("<AvPag>\t\t TRASLADAR +Z; ROTAR +Z \n");
    printf("<RePag>\t\t TRASLADAR -Z; ROTAR -Z \n");
    printf("<+>\t\t Aumentar angulo de apertura \n");
    printf("<->\t\t Disminuir angulo de apertura \n");
    printf("\n\n");

    printf("Funciones Extra \n");
    printf("<Q/q>\t\t Alterna el mostrar/ocultar escenario \n");
    printf("<Z/z>\t\t Alterna entre renderizado de malla/solido/ambos \n");
    printf("<U/u>\t\t Carga el objeto abioia.obj de forma rapida, ideal para el testeo \n");
    printf("<S/s>\t\t Pium Pium\n");
    printf("<'<'>\t\t Reduce el grosor de las lineas en renderizado de malla/combinado \n");
    printf("<'>'>\t\t Aumenta el grosor de las lineas en renderizado de malla/combinado \n");
    printf("<LMB>\t\t Traslacion de la camara\n");
    printf("<MMB>\t\t Rotacion camara modo analisis sobre X=0 Y=0 Z=0\n");
    printf("<Scroll>\t\t alejarse/acercase \n");
    printf("<Ctrl + Scroll>\t\t Aumentar/disminuir sensibilidad del raton \n");
    printf("<Ctrl + MMB>\t\t Rotar camara en modo local \n");
    printf("\n\n");
}

// Esta funcion imprime las flags activadas actualmente

void print_flags( int verbose){
    if(!verbose){
        printf("MODE: %s ",(manipulation_mode == OBJECT_MODE)?"OBJ":(manipulation_mode == CAMERA_MODE)?"CAM":"ILU");
        printf("TRANSFOR: %s\n",(transformation_mode == TRANSLATION_MODE)?"Traslation":(transformation_mode == ROTATION_MODE)?"Rotation":(transformation_mode == SCALE_MODE)?"Scale":"Velocity");
        return;
    }
    char buf[5];
    gcvt(wire_width,5,buf);
    if(manipulation_mode == OBJECT_MODE){
        printf("MODE : OBJECT -> reference = %s\n",(reference_mode == GLOBAL_MODE)?"Global":"Local");
    }else if(manipulation_mode == CAMERA_MODE){
        printf("MODE : CAMERA -> mode = %s, perspective = %s\n",(camera_mode == CAMERA_ANALISIS_MODE)?"Analisis":"Free",
                                                                (_selected_camera->projection_mode == PERSPECTIVE_MODE)?"Perspective":"Ortho");
    }else{
        printf("MODE : ILUMINATION -> reference = %s\n",(reference_mode == GLOBAL_MODE)?"Global":"Local");
    }
    printf("TRANSFORMATION MODE : %s\n",(transformation_mode == TRANSLATION_MODE)?"Traslation":(transformation_mode == ROTATION_MODE)?"Rotation":(transformation_mode == SCALE_MODE)?"Scale":"Velocity");
    printf("RENDER_MODE : %s\n",(render_mode == RENDER_WIREFRAME)?"Wireframe":(render_mode == RENDER_SOLID)?"Solid":"Combined");
    printf("SHOW STATIC : %s\n",(render_static == 0)?"False":"True");
    printf("SHADING MODE : %s\n",(lightning_mode == SHADING_FLAT)?"Flat":"Smooth");
    printf("LIGHT MODE : ");
    for(int i = 0; i < 8; i++){
        printf("[%d(%s)] ", i + 1, (lights[i]->mode == SPOT)?"Spot":(lights[i]->mode == POINT)?"Point":"Sun");
    }
    printf("\n");
    printf("---------------------------------------------------------\n");
}

/**
 * @brief Callback function to control the basic keys
 * @param key Key that has been pressed
 * @param x X coordinate of the mouse pointer when the key was pressed
 * @param y Y coordinate of the mouse pointer when the key was pressed
 */
void keyboard(unsigned char key, int x, int y)
{
    char *fname = malloc(sizeof(char) * 128); /* Note that scanf adds a null character at the end of the vector*/
    int read = 0;
    object3d *auxiliar_object;
    GLdouble wd, he, midx, midy;

    switch (key)
    {
    case 'q':
    case 'Q':
        render_static = (render_static)?0:1;
        break;
    case '<':
        wire_width --;
        if(wire_width < MIN_WIREFRAME_WIDTH)
            wire_width = MIN_WIREFRAME_WIDTH;
        break;
    case '>':
        wire_width ++;
        if(wire_width > MAX_WIREFRAME_WIDTH)
            wire_width = MAX_WIREFRAME_WIDTH;
        break;
    case 'z':
    case 'Z':
        render_mode ++;
        if(render_mode == RENDER_MODES)
            render_mode = 0;
        break;
    case 'a':
    case 'A':
        manipulation_mode = ILUM_MODE;
        break;
    case 's':
    case 'S':
        if(_selected_object == NULL)
            break;
        object3d * current_obj_ptr = _selected_object;
        /*Load object*/
        auxiliar_object = cargar_objeto("objektuak/missile.obj");
        _selected_object = current_obj_ptr;
        auxiliar_object->transformation_history  = _selected_object->transformation_history;
        /*Function Set-up*/
        auxiliar_object->onUpdate = &missile_Update;
        auxiliar_object->onCollideOther = &missile_OnCollide;
        /*TTL*/
        auxiliar_object->ttl = UPDATES_PER_SECOND * 5;
        auxiliar_object->is_selectable = 0;
        /*Transformation*/
        displace_selected_object(auxiliar_object, LOCAL_MODE, 0, 0, 20);
        rotate_selected_object(auxiliar_object,LOCAL_MODE,180,1,0,0);
        scale_selected_object(auxiliar_object,LOCAL_MODE,0.50,0.5,0.5);
        break;
    case 13:
        if(_selected_object == NULL)
            break;
        if(_selected_object != NULL)
            set_object_velocity(_selected_object,0,0,0);
        break;
    case 'r':/*Boton para testeo*/
    case 'R':
        if(_selected_object != NULL && manipulation_mode == OBJECT_MODE){
            set_object_position(_selected_object,0,0,0);
            if(camera_mode == CAMERA_ANALISIS_MODE)
                camera_to_object_perspective(_selected_camera,_selected_object);

        }else if(manipulation_mode == CAMERA_MODE && camera_mode == CAMERA_FREECAM_MODE){
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(  _selected_camera->pos.x, _selected_camera->pos.y, _selected_camera->pos.z,
                        -_selected_camera->pos.x, -_selected_camera->pos.y, -_selected_camera->pos.z,
                        _selected_camera->verticality_vec.x, _selected_camera->verticality_vec.y, _selected_camera->verticality_vec.z);
            glGetFloatv(GL_MODELVIEW_MATRIX,_selected_camera->mat);
        }
        break;
    case 'c':
        _selected_camera = _selected_camera->next;
        /*The selection is circular, thus if we move out of the list we go back to the first element*/
        if (_selected_camera == 0)
            _selected_camera = _first_camera;
        break;
    case 'C':
        if(_selected_object == NULL)
            break;
        camera3d *new_cam = create_camera(0,0,0,
                                          0,0,0,
                                          0,0,0,
                                          PERSPECTIVE_MODE); 
        
        _selected_camera = new_cam;       
        camera_to_object_perspective(new_cam,_selected_object);
        _selected_camera->parent = _selected_object;
        manipulation_mode = OBJECT_MODE;
        break;
    case 'k':
    case 'K':
        manipulation_mode = CAMERA_MODE;
        break;
    case 'o':
    case 'O':
        manipulation_mode = OBJECT_MODE;
        break;
    case 'g':
    case 'G':
        if(manipulation_mode == OBJECT_MODE || manipulation_mode == ILUM_MODE)
            reference_mode = GLOBAL_MODE;
        else{
            if(_selected_object != NULL){
                camera_mode = CAMERA_ANALISIS_MODE;
                cambiar_sistema_referencia_camara(_selected_camera,_selected_object);
            }
        }
        break;

    case 'l':
    case 'L':
        if(manipulation_mode == OBJECT_MODE || manipulation_mode == ILUM_MODE)
            reference_mode = LOCAL_MODE;
        else
            camera_mode = CAMERA_FREECAM_MODE;
        break;
    case 'f':
    case 'F':
        if(manipulation_mode == OBJECT_MODE){
            /*Ask for file*/
            printf("%s", KG_MSSG_SELECT_FILE);
            scanf("%s", fname);
            cargar_objeto(fname);
        }else{
            vector3 atPosition, eyePosition, verticality;
            camera3d *new_camera_ptr;
            transformation_matrix *trans_mat;

            read_3f(&atPosition, "Introduce el at position ->");
            read_3f(&eyePosition, "Introduce el eye position ->");
            read_3f(&verticality, "Introduce el vector que determina la verticalidad ->");
            printf("%lf %lf %lf \n", atPosition.x,atPosition.y,atPosition.z);
            printf("%lf %lf %lf \n", eyePosition.x,eyePosition.y,eyePosition.z);
            printf("%lf %lf %lf \n", verticality.x,verticality.y,verticality.z);

            new_camera_ptr = create_camera(
                eyePosition.x,eyePosition.y,eyePosition.z,
                atPosition.x,atPosition.y,atPosition.z,
                verticality.x,verticality.y,verticality.z,
                PERSPECTIVE_MODE
            );

            _selected_camera = new_camera_ptr;
        }
        break;
    case 9: /* <TAB> */
        if(_selected_object == NULL) 
            break;
        // _selected_object = _selected_object->next;
        // if (_selected_object == 0)
        //         _selected_object = _first_object;
        
        select_next_valid_object();

        if( camera_mode == CAMERA_ANALISIS_MODE )
            cambiar_sistema_referencia_camara(_selected_camera,_selected_object);

        break;

    case 127: /* <SUPR> */
        /*Erasing an object depends on whether it is the first one or not*/
        if(_selected_object == NULL)
            break;
        eliminar_objeto(_selected_object);
        if( _selected_object == NULL  && camera_mode == CAMERA_ANALISIS_MODE){
            camera_mode = CAMERA_FREECAM_MODE;
            break;
        }
        if(manipulation_mode == CAMERA_MODE && camera_mode == CAMERA_ANALISIS_MODE )
            if(_selected_object != NULL){
                cambiar_sistema_referencia_camara(_selected_camera,_selected_object);
            }

        break;

    case '-':
        if (glutGetModifiers() == GLUT_ACTIVE_CTRL || manipulation_mode == CAMERA_MODE)
        {

            if(_selected_camera->projection_mode == ORTHOGONAL_MODE){
                /*Increase the projection plane; compute the new dimensions*/
                wd = (_selected_camera->ortho_right - _selected_camera->ortho_left) / KG_STEP_ZOOM;
                he = (_selected_camera->ortho_top - _selected_camera->ortho_bottom) / KG_STEP_ZOOM;
                /*In order to avoid moving the center of the plane, we get its coordinates*/
                midx = (_selected_camera->ortho_right + _selected_camera->ortho_left) / 2;
                midy = (_selected_camera->ortho_top + _selected_camera->ortho_bottom) / 2;
                /*The the new limits are set, keeping the center of the plane*/
                _selected_camera->ortho_right = midx + wd / 2;
                _selected_camera->ortho_left = midx - wd / 2;
                _selected_camera->ortho_top = midy + he / 2;
                _selected_camera->ortho_bottom = midy - he / 2;
            }else{
                /*Increase the projection plane; compute the new dimensions*/
                wd = (_selected_camera->persp_right - _selected_camera->persp_left) / KG_STEP_ZOOM;
                he = (_selected_camera->persp_top - _selected_camera->persp_bottom) / KG_STEP_ZOOM;
                /*In order to avoid moving the center of the plane, we get its coordinates*/
                midx = (_selected_camera->persp_right + _selected_camera->persp_left) / 2;
                midy = (_selected_camera->persp_top + _selected_camera->persp_bottom) / 2;
                /*The the new limits are set, keeping the center of the plane*/
                _selected_camera->persp_right = midx + wd / 2;
                _selected_camera->persp_left = midx - wd / 2;
                _selected_camera->persp_top = midy + he / 2;
                _selected_camera->persp_bottom = midy - he / 2;
            }
            
        }else if(_selected_object!= NULL && manipulation_mode == OBJECT_MODE && transformation_mode == SCALE_MODE){
            scale_selected_object(_selected_object, LOCAL_MODE,1 - DEFAULT_OBJECT_SCALE_RATIO, 1 - DEFAULT_OBJECT_SCALE_RATIO, 1 - DEFAULT_OBJECT_SCALE_RATIO);
        }else if(manipulation_mode == ILUM_MODE && lights[_selected_light]->mode == SPOT){
            lights[_selected_light]->amplitude -= 5; 
            if(lights[_selected_light]->amplitude < 0)
                lights[_selected_light]->amplitude = 0;
        }
        break;

    case '+':
        //INPLEMENTA EZAZU CTRL + + KONBINAZIOAREN FUNTZIOANLITATEA
        if (glutGetModifiers() == GLUT_ACTIVE_CTRL || manipulation_mode == CAMERA_MODE) 
            if(_selected_camera->projection_mode == ORTHOGONAL_MODE){
                /*Increase the projection plane; compute the new dimensions*/
                wd = (_selected_camera->ortho_right - _selected_camera->ortho_left) * KG_STEP_ZOOM;
                he = (_selected_camera->ortho_top - _selected_camera->ortho_bottom) * KG_STEP_ZOOM;
                /*In order to avoid moving the center of the plane, we get its coordinates*/
                midx = (_selected_camera->ortho_right + _selected_camera->ortho_left) / 2;
                midy = (_selected_camera->ortho_top + _selected_camera->ortho_bottom) / 2;
                /*The the new limits are set, keeping the center of the plane*/
                _selected_camera->ortho_right = midx + wd / 2;
                _selected_camera->ortho_left = midx - wd / 2;
                _selected_camera->ortho_top = midy + he / 2;
                _selected_camera->ortho_bottom = midy - he / 2;
            }else{
                /*Increase the projection plane; compute the new dimensions*/
                wd = (_selected_camera->persp_right - _selected_camera->persp_left) * KG_STEP_ZOOM;
                he = (_selected_camera->persp_top - _selected_camera->persp_bottom) * KG_STEP_ZOOM;
                /*In order to avoid moving the center of the plane, we get its coordinates*/
                midx = (_selected_camera->persp_right + _selected_camera->persp_left) / 2;
                midy = (_selected_camera->persp_top + _selected_camera->persp_bottom) / 2;
                /*The the new limits are set, keeping the center of the plane*/
                _selected_camera->persp_right = midx + wd / 2;
                _selected_camera->persp_left = midx - wd / 2;
                _selected_camera->persp_top = midy + he / 2;
                _selected_camera->persp_bottom = midy - he / 2;
            }
        else if(_selected_object!= NULL && manipulation_mode == OBJECT_MODE && transformation_mode == SCALE_MODE)
            scale_selected_object(_selected_object, LOCAL_MODE, 1 + DEFAULT_OBJECT_SCALE_RATIO, 1 + DEFAULT_OBJECT_SCALE_RATIO, 1 + DEFAULT_OBJECT_SCALE_RATIO);
        else if(manipulation_mode == ILUM_MODE && lights[_selected_light]->mode == SPOT){
            lights[_selected_light]->amplitude += 5; 
            if(lights[_selected_light]->amplitude > 90)
                lights[_selected_light]->amplitude = 90;
        } 
        break;
    case 'p': 
    case 'P':
        _selected_camera->projection_mode = (_selected_camera->projection_mode == ORTHOGONAL_MODE)?PERSPECTIVE_MODE:ORTHOGONAL_MODE;
        /*Allocate memory for the structure and read the file*/
        break;
    case 'u': /*Carga rápida de un objeto para testeo*/
    case 'U':
        cargar_objeto(quickload_path);
        break;
    case 26 :
        if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
        {
            if(_selected_object != NULL && manipulation_mode == OBJECT_MODE)
                undo_last_object_change(_selected_object);
        }
        break;
    case 'm':
    case 'M':
        transformation_mode = TRANSLATION_MODE;
        break;
    case 'b':
    case 'B':
        transformation_mode = ROTATION_MODE;
        break;
    case 't':
    case 'T':
        transformation_mode = SCALE_MODE;
        break;
    case 'v':
    case 'V':
        transformation_mode = VELOCITY_MODE;
        break;
    case '?':
        print_help();
        break;
    case 27: /* <ESC> */
        if(use_gui == 1){
            gtk_main_quit();
            pthread_join(*thread1, NULL);
        }
        exit(0);
        break;
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
        _selected_light = (int)(key - 49);
        break;
    case '0':
        if(_selected_light < 4)
            break;
        lights[_selected_light]->mode = (lights[_selected_light]->mode == SUN)? POINT : SUN; //? SPOT:(lights[_selected_light]->mode == SPOT) 
        break;
    default:
        /*In the default case we just print the code of the key. This is usefull to define new cases*/
        printf("%d %c\n", key, key);
    }

    //Display state of all flags
    if(cmd_show_status)
        print_flags(cmd_verbose);
    /*In case we have do any modification affecting the displaying of the object, we redraw them*/
    glutPostRedisplay();
}

// Funcion que controla las funciones de las teclas especiales
void special_keyboard(int key, int x, int y)
{   
    object3d *aux_obj_ptr = _selected_object;
    if (manipulation_mode == OBJECT_MODE && aux_obj_ptr == NULL && key != GLUT_KEY_F1 && key != GLUT_KEY_F2 && key != GLUT_KEY_F3 && key != GLUT_KEY_F4 && key != GLUT_KEY_F5 && key != GLUT_KEY_F6 && key != GLUT_KEY_F7 && key != GLUT_KEY_F8 && key != GLUT_KEY_F9 && key != GLUT_KEY_F10 && key != GLUT_KEY_F11 && key != GLUT_KEY_F12){
        return;
    }
    switch (key){
    case GLUT_KEY_UP:
        switch (transformation_mode)
        {
        case TRANSLATION_MODE:
            if (manipulation_mode == OBJECT_MODE)
            {
                displace_selected_object(aux_obj_ptr, reference_mode,  0, DEFAULT_DISPLACEMENT_DISTANCE, 0);
            }else if(manipulation_mode ==CAMERA_MODE && camera_mode ==  CAMERA_FREECAM_MODE){
                displace_camera(_selected_camera, 0, DEFAULT_DISPLACEMENT_DISTANCE, 0);
            }else if(manipulation_mode == ILUM_MODE && _selected_light != 2 && _selected_light != 3){
                displace_ilum(_selected_light, 0, DEFAULT_DISPLACEMENT_DISTANCE, 0);
            }
            break;
        case ROTATION_MODE:
            if(manipulation_mode == CAMERA_MODE){
                if(camera_mode ==CAMERA_ANALISIS_MODE){
                    camera_rotation_point(_selected_camera, DEFAULT_ROTATION_DEGREES,_selected_camera->mat[0], _selected_camera->mat[4], _selected_camera->mat[8], _selected_object->origin_pos);
                }else{
                    rotate_camera_local(_selected_camera, DEFAULT_ROTATION_DEGREES, DEFAULT_DISPLACEMENT_DISTANCE, 0 ,0);
                }
            }else if(manipulation_mode == OBJECT_MODE)
                rotate_selected_object(aux_obj_ptr, reference_mode, DEFAULT_ROTATION_DEGREES, DEFAULT_DISPLACEMENT_DISTANCE, 0, 0);
            else if(manipulation_mode == ILUM_MODE && lights[_selected_light]->mode == SPOT && _selected_light != 2 && _selected_light != 3)
                rotate_ilum(lights[_selected_light], reference_mode, DEFAULT_ROTATION_DEGREES, DEFAULT_DISPLACEMENT_DISTANCE, 0, 0);               
            break;
        case SCALE_MODE:
            if(manipulation_mode == CAMERA_MODE){
                if(_selected_camera->projection_mode == ORTHOGONAL_MODE){
                    _selected_camera->ortho_top += DEFAULT_CAMERA_VOLUME_SCALE_ORTHO;
                    _selected_camera->ortho_bottom -= DEFAULT_CAMERA_VOLUME_SCALE_ORTHO;
                }else{
                    _selected_camera->persp_top += DEFAULT_CAMERA_VOLUME_SCALE_PERSP;
                    _selected_camera->persp_bottom -= DEFAULT_CAMERA_VOLUME_SCALE_PERSP;
                }
            }else
                scale_selected_object(aux_obj_ptr, reference_mode,  1, 1 + DEFAULT_OBJECT_SCALE_RATIO, 1);
            break;
        case VELOCITY_MODE:
            if(manipulation_mode == OBJECT_MODE)
                set_object_velocity(aux_obj_ptr, 0, DEFAULT_OBJECT_SPEED, 0);
            break;
        }
        break;
    case GLUT_KEY_DOWN:
        switch (transformation_mode)
        {
        case TRANSLATION_MODE:
            if (manipulation_mode == OBJECT_MODE)
            {
                displace_selected_object(aux_obj_ptr, reference_mode,  0, -DEFAULT_DISPLACEMENT_DISTANCE, 0);
            }else if(manipulation_mode ==CAMERA_MODE && camera_mode ==  CAMERA_FREECAM_MODE){
                displace_camera(_selected_camera, 0, -DEFAULT_DISPLACEMENT_DISTANCE, 0);
            }else if(manipulation_mode == ILUM_MODE && _selected_light != 2 && _selected_light != 3){
                displace_ilum(_selected_light, 0, -DEFAULT_DISPLACEMENT_DISTANCE, 0);
            }
            break;
        case ROTATION_MODE:
            if(manipulation_mode == CAMERA_MODE){
                if(camera_mode ==CAMERA_ANALISIS_MODE){
                    camera_rotation_point(_selected_camera, -DEFAULT_ROTATION_DEGREES,_selected_camera->mat[0], _selected_camera->mat[4], _selected_camera->mat[8], _selected_object->origin_pos);
                }else{
                    rotate_camera_local(_selected_camera, -DEFAULT_ROTATION_DEGREES, DEFAULT_DISPLACEMENT_DISTANCE, 0 ,0);
                }
            }else if(manipulation_mode == OBJECT_MODE)
                rotate_selected_object(aux_obj_ptr, reference_mode, -DEFAULT_ROTATION_DEGREES, DEFAULT_DISPLACEMENT_DISTANCE, 0, 0);
            else if(manipulation_mode == ILUM_MODE && lights[_selected_light]->mode == SPOT && _selected_light != 2 && _selected_light != 3)
                rotate_ilum(lights[_selected_light], reference_mode,-DEFAULT_ROTATION_DEGREES, DEFAULT_DISPLACEMENT_DISTANCE, 0, 0);
            break;
        case SCALE_MODE:
            if(manipulation_mode == CAMERA_MODE){
                if(_selected_camera->projection_mode == ORTHOGONAL_MODE){
                    _selected_camera->ortho_top -= DEFAULT_CAMERA_VOLUME_SCALE_ORTHO;
                    _selected_camera->ortho_bottom += DEFAULT_CAMERA_VOLUME_SCALE_ORTHO;
                }else{
                    _selected_camera->persp_top -= DEFAULT_CAMERA_VOLUME_SCALE_PERSP;
                    _selected_camera->persp_bottom += DEFAULT_CAMERA_VOLUME_SCALE_PERSP;
                }
            }else
                scale_selected_object(aux_obj_ptr, reference_mode,  1, 1 - DEFAULT_OBJECT_SCALE_RATIO, 1);
            break;
        case VELOCITY_MODE:
            if(manipulation_mode == OBJECT_MODE)
                set_object_velocity(aux_obj_ptr, 0, -DEFAULT_OBJECT_SPEED, 0);
            break;
        }
        break;
    case GLUT_KEY_RIGHT:
        switch (transformation_mode)
        {
        case TRANSLATION_MODE:
            if (manipulation_mode == OBJECT_MODE)
            {
                displace_selected_object(aux_obj_ptr, reference_mode,  DEFAULT_DISPLACEMENT_DISTANCE, 0, 0);
            }else if(manipulation_mode ==CAMERA_MODE && camera_mode ==  CAMERA_FREECAM_MODE){
                displace_camera(_selected_camera, DEFAULT_DISPLACEMENT_DISTANCE, 0, 0);
            }else if(manipulation_mode == ILUM_MODE && _selected_light != 2 && _selected_light != 3){
                displace_ilum(_selected_light, DEFAULT_DISPLACEMENT_DISTANCE, 0, 0);
            }
            break;
        case ROTATION_MODE:
            if(manipulation_mode == CAMERA_MODE){
                if(camera_mode ==CAMERA_ANALISIS_MODE){
                    camera_rotation_point(_selected_camera, DEFAULT_ROTATION_DEGREES,_selected_camera->mat[1], _selected_camera->mat[5], _selected_camera->mat[9], _selected_object->origin_pos);
                }else{
                    rotate_camera_local(_selected_camera, DEFAULT_ROTATION_DEGREES, 0, DEFAULT_DISPLACEMENT_DISTANCE,0);
                }
            }else if(manipulation_mode == OBJECT_MODE)
                rotate_selected_object(aux_obj_ptr, reference_mode, DEFAULT_ROTATION_DEGREES, 0, DEFAULT_DISPLACEMENT_DISTANCE, 0);
            else if(manipulation_mode == ILUM_MODE  && _selected_light != 2 && _selected_light != 3)
                rotate_ilum(lights[_selected_light], reference_mode,DEFAULT_ROTATION_DEGREES, 0, DEFAULT_DISPLACEMENT_DISTANCE, 0);
            break;
        case SCALE_MODE:
            if(manipulation_mode == CAMERA_MODE){
                if(_selected_camera->projection_mode == ORTHOGONAL_MODE){
                    _selected_camera->ortho_right += DEFAULT_CAMERA_VOLUME_SCALE_ORTHO;
                    _selected_camera->ortho_left -= DEFAULT_CAMERA_VOLUME_SCALE_ORTHO;
                }else{
                    _selected_camera->persp_right += DEFAULT_CAMERA_VOLUME_SCALE_PERSP;
                    _selected_camera->persp_left -= DEFAULT_CAMERA_VOLUME_SCALE_PERSP;
                }
            }else
                scale_selected_object(aux_obj_ptr, reference_mode,  1 + DEFAULT_OBJECT_SCALE_RATIO, 1 , 1);
            break;
        case VELOCITY_MODE:
            if(manipulation_mode == OBJECT_MODE)
                set_object_velocity(aux_obj_ptr, DEFAULT_OBJECT_SPEED, 0, 0);
            break;
        }
        break;
    case GLUT_KEY_LEFT:
        switch (transformation_mode)
        {
        case TRANSLATION_MODE:
            if (manipulation_mode == OBJECT_MODE){
                displace_selected_object(aux_obj_ptr, reference_mode,  -DEFAULT_DISPLACEMENT_DISTANCE, 0, 0);
            }else if(manipulation_mode ==CAMERA_MODE && camera_mode ==  CAMERA_FREECAM_MODE){
                displace_camera(_selected_camera, -DEFAULT_DISPLACEMENT_DISTANCE, 0, 0);
            }else if(manipulation_mode == ILUM_MODE && _selected_light != 2 && _selected_light != 3){
                displace_ilum(_selected_light, -DEFAULT_DISPLACEMENT_DISTANCE, 0, 0);
            }
            break;
        case ROTATION_MODE:
            if(manipulation_mode == CAMERA_MODE){
                if(camera_mode ==CAMERA_ANALISIS_MODE){
                    camera_rotation_point(_selected_camera, -DEFAULT_ROTATION_DEGREES,_selected_camera->mat[1], _selected_camera->mat[5], _selected_camera->mat[9], _selected_object->origin_pos);
                }else{
                    rotate_camera_local(_selected_camera, -DEFAULT_ROTATION_DEGREES, 0, DEFAULT_DISPLACEMENT_DISTANCE,0);          
                }
            }else if(manipulation_mode == OBJECT_MODE)
                rotate_selected_object(aux_obj_ptr, reference_mode, -DEFAULT_ROTATION_DEGREES, 0, DEFAULT_DISPLACEMENT_DISTANCE, 0);
            else if(manipulation_mode == ILUM_MODE  && _selected_light != 2 && _selected_light != 3)
                rotate_ilum(lights[_selected_light], reference_mode,-DEFAULT_ROTATION_DEGREES, 0, DEFAULT_DISPLACEMENT_DISTANCE, 0);
            break;
        case SCALE_MODE:
            if(manipulation_mode == CAMERA_MODE){
                if(_selected_camera->projection_mode == ORTHOGONAL_MODE){
                    _selected_camera->ortho_right -= DEFAULT_CAMERA_VOLUME_SCALE_ORTHO;
                    _selected_camera->ortho_left  += DEFAULT_CAMERA_VOLUME_SCALE_ORTHO;
                }else{
                    _selected_camera->persp_right -= DEFAULT_CAMERA_VOLUME_SCALE_PERSP;
                    _selected_camera->persp_left  += DEFAULT_CAMERA_VOLUME_SCALE_PERSP;
                }
            }else
                scale_selected_object(aux_obj_ptr, reference_mode,  1 - DEFAULT_OBJECT_SCALE_RATIO, 1 , 1);
            break;
        case VELOCITY_MODE:
            if(manipulation_mode == OBJECT_MODE)
                set_object_velocity(aux_obj_ptr, -DEFAULT_OBJECT_SPEED, 0, 0);
            break;
        }
        break;
    case GLUT_KEY_PAGE_UP:
            
        switch (transformation_mode)
        {
        case TRANSLATION_MODE:
            if (manipulation_mode == OBJECT_MODE)
            {
                displace_selected_object(aux_obj_ptr, reference_mode,  0, 0, DEFAULT_DISPLACEMENT_DISTANCE);
            }else if(manipulation_mode ==CAMERA_MODE && camera_mode ==  CAMERA_FREECAM_MODE){
                displace_camera(_selected_camera, 0, 0, DEFAULT_DISPLACEMENT_DISTANCE);
            }else if(manipulation_mode ==CAMERA_MODE && camera_mode == CAMERA_ANALISIS_MODE){
                displace_camera(_selected_camera,0, 0, DEFAULT_DISPLACEMENT_DISTANCE);
            }else if(manipulation_mode == ILUM_MODE && _selected_light != 2 && _selected_light != 3){
                displace_ilum(_selected_light, 0, 0, DEFAULT_DISPLACEMENT_DISTANCE);
            }
            break;
        case ROTATION_MODE:
            if(manipulation_mode == CAMERA_MODE){
                if(camera_mode ==CAMERA_ANALISIS_MODE){
                    camera_rotation_point(_selected_camera, DEFAULT_ROTATION_DEGREES,_selected_camera->mat[2], _selected_camera->mat[6], _selected_camera->mat[10], _selected_object->origin_pos);
                }else{
                    rotate_camera_local(_selected_camera, DEFAULT_ROTATION_DEGREES, 0, 0, DEFAULT_DISPLACEMENT_DISTANCE);
                }
            }else if(manipulation_mode == OBJECT_MODE)
                rotate_selected_object(aux_obj_ptr, reference_mode, DEFAULT_ROTATION_DEGREES, 0, 0, DEFAULT_DISPLACEMENT_DISTANCE);
            else if(manipulation_mode == ILUM_MODE && lights[_selected_light]->mode == SPOT && _selected_light != 2 && _selected_light != 3)
                rotate_ilum(lights[_selected_light], reference_mode,DEFAULT_ROTATION_DEGREES, 0, 0, DEFAULT_DISPLACEMENT_DISTANCE);
            break;
        case SCALE_MODE:
            if(manipulation_mode == CAMERA_MODE){
                if(_selected_camera->projection_mode == ORTHOGONAL_MODE){
                    _selected_camera->ortho_far  += DEFAULT_CAMERA_VOLUME_SCALE_ORTHO;
                    _selected_camera->ortho_near += DEFAULT_CAMERA_VOLUME_SCALE_ORTHO;
                }else{
                    _selected_camera->persp_far  += DEFAULT_CAMERA_VOLUME_SCALE_PERSP;
                    _selected_camera->persp_near += DEFAULT_CAMERA_VOLUME_SCALE_PERSP;
                }
            }else
                scale_selected_object(aux_obj_ptr, reference_mode,  1, 1 , 1+ DEFAULT_OBJECT_SCALE_RATIO);
            break;
        case VELOCITY_MODE:
            if(manipulation_mode == OBJECT_MODE)
                set_object_velocity(aux_obj_ptr, 0, 0, DEFAULT_OBJECT_SPEED);
            break;
        }
        break;
    case GLUT_KEY_PAGE_DOWN:
        switch (transformation_mode)
        {
        case TRANSLATION_MODE:
            if (manipulation_mode == OBJECT_MODE)
                displace_selected_object(aux_obj_ptr, reference_mode,  0, 0, -DEFAULT_DISPLACEMENT_DISTANCE);
            else if(manipulation_mode ==CAMERA_MODE && camera_mode ==  CAMERA_FREECAM_MODE)
                displace_camera(_selected_camera, 0, 0, -DEFAULT_DISPLACEMENT_DISTANCE);
            else if(manipulation_mode ==CAMERA_MODE && camera_mode == CAMERA_ANALISIS_MODE){
                if(distance_3d(_selected_camera->pos,_selected_object->origin_pos) > _selected_object->bound_radius){
                    displace_camera(_selected_camera,0, 0, -DEFAULT_DISPLACEMENT_DISTANCE);
                }
            }else if(manipulation_mode == ILUM_MODE && _selected_light != 2 && _selected_light != 3){
                displace_ilum(_selected_light, 0, 0, -DEFAULT_DISPLACEMENT_DISTANCE);
            }
            break;
        case ROTATION_MODE:
            if(manipulation_mode == CAMERA_MODE){
                if(camera_mode ==CAMERA_ANALISIS_MODE){
                    camera_rotation_point(_selected_camera, -DEFAULT_ROTATION_DEGREES,_selected_camera->mat[2], _selected_camera->mat[6], _selected_camera->mat[10], _selected_object->origin_pos);
                }else{
                    rotate_camera_local(_selected_camera, -DEFAULT_ROTATION_DEGREES, 0, 0, DEFAULT_DISPLACEMENT_DISTANCE);
                }
            }else if(manipulation_mode == OBJECT_MODE)
                rotate_selected_object(aux_obj_ptr, reference_mode, -DEFAULT_ROTATION_DEGREES, 0, 0, DEFAULT_DISPLACEMENT_DISTANCE);
            else if(manipulation_mode == ILUM_MODE && lights[_selected_light]->mode == SPOT && _selected_light != 2 && _selected_light != 3)
                rotate_ilum(lights[_selected_light], reference_mode,-DEFAULT_ROTATION_DEGREES, 0, 0, DEFAULT_DISPLACEMENT_DISTANCE);
            break;
        case SCALE_MODE:
            if(manipulation_mode == CAMERA_MODE){
                if(_selected_camera->projection_mode == ORTHOGONAL_MODE){
                    _selected_camera->ortho_far  -= DEFAULT_CAMERA_VOLUME_SCALE_ORTHO;
                    _selected_camera->ortho_near -= DEFAULT_CAMERA_VOLUME_SCALE_ORTHO;
                }else{
                    _selected_camera->persp_far  -= DEFAULT_CAMERA_VOLUME_SCALE_PERSP;
                    _selected_camera->persp_near -= DEFAULT_CAMERA_VOLUME_SCALE_PERSP;
                    if(_selected_camera->persp_near <= 0)
                        _selected_camera->persp_near == 0.01;
                    if(_selected_camera->persp_near > _selected_camera->persp_far)
                        _selected_camera->persp_far = 2 * _selected_camera->persp_near;
                }
            }else
                scale_selected_object(aux_obj_ptr, reference_mode,  1, 1 , 1 - DEFAULT_OBJECT_SCALE_RATIO);
            break;
        case VELOCITY_MODE:
            if(manipulation_mode == OBJECT_MODE)
                set_object_velocity(aux_obj_ptr, 0, 0, -DEFAULT_OBJECT_SPEED);
            break;
        }
        break;
    case GLUT_KEY_F1:
        lights[0]->is_on = (lights[0]->is_on == 1)?0:1;
        if(lights[0]->is_on)
            glEnable(GL_LIGHT0);
        else
            glDisable(GL_LIGHT0);
        break;
    case GLUT_KEY_F2:
        lights[1]->is_on = (lights[1]->is_on == 1)?0:1;
        if(lights[1]->is_on)
            glEnable(GL_LIGHT1);
        else
            glDisable(GL_LIGHT1);
        break;
    case GLUT_KEY_F3:
        lights[2]->is_on = (lights[2]->is_on == 1)?0:1;
        if(lights[2]->is_on)
            glEnable(GL_LIGHT2);
        else
            glDisable(GL_LIGHT2);
        break;
    case GLUT_KEY_F4:
        lights[3]->is_on = (lights[3]->is_on == 1)?0:1;
        if(lights[3]->is_on)
            glEnable(GL_LIGHT3);
        else
            glDisable(GL_LIGHT3);
        break;
    case GLUT_KEY_F5:
        lights[4]->is_on = (lights[4]->is_on == 1)?0:1;
        if(lights[4]->is_on)
            glEnable(GL_LIGHT4);
        else
            glDisable(GL_LIGHT4);
        break;
    case GLUT_KEY_F6:
        lights[5]->is_on = (lights[5]->is_on == 1)?0:1;
        if(lights[5]->is_on)
            glEnable(GL_LIGHT5);
        else
            glDisable(GL_LIGHT5);
        break;
    case GLUT_KEY_F7:
        lights[6]->is_on = (lights[6]->is_on == 1)?0:1;
        if(lights[6]->is_on)
            glEnable(GL_LIGHT6);
        else
            glDisable(GL_LIGHT6);
        break;
    case GLUT_KEY_F8:
        lights[7]->is_on = (lights[7]->is_on == 1)?0:1;
        if(lights[7]->is_on)
            glEnable(GL_LIGHT7);
        else
            glDisable(GL_LIGHT7);
        
        break;
    case GLUT_KEY_F9:
        if(lightning_mode == LIGHTNING_ON){
            lightning_mode = LIGHTNING_OFF;
            glDisable(GL_LIGHTING);
        }else{
            lightning_mode = LIGHTNING_ON;
            glEnable(GL_LIGHTING);
        }
        break;
    case GLUT_KEY_F12:
        shading_mode = (shading_mode == SHADING_FLAT)? SHADING_SMOOTH : SHADING_FLAT;
        if(shading_mode == SHADING_FLAT){
            glShadeModel(GL_FLAT);
        }else
        {
            glShadeModel(GL_SMOOTH);
        }
        
        break;
    default:
        break;
    }

    /*In case we have do any modification affecting the displaying of the object, we redraw them*/
    glutPostRedisplay();
}

// Funcion con las opciones que puedes hacer con el raton
void mouse(int button, int state, int x, int y){
   // Wheel reports as button 3(scroll up) and button 4(scroll down)
    if ((button == 3) || (button == 4)) // It's a wheel event
    {   
        if(glutGetModifiers() == GLUT_ACTIVE_CTRL)
            if (button == 3){
                mouse_sensitivity = (mouse_sensitivity < 1)?1:mouse_sensitivity - 0.25;
            }else{
                mouse_sensitivity = (mouse_sensitivity > 20)?20:mouse_sensitivity + 0.25;;
            }
        else{
            if(manipulation_mode == CAMERA_MODE){
                if (button == 3){
                    if(camera_mode == CAMERA_FREECAM_MODE ||
                    ( camera_mode== CAMERA_ANALISIS_MODE && _selected_object!= NULL && distance_3d(_selected_camera->pos,_selected_object->origin_pos) > _selected_object->bound_radius) ){
                        displace_camera(_selected_camera, 0, 0, -DEFAULT_DISPLACEMENT_DISTANCE);
                    }
                }else{
                    displace_camera(_selected_camera, 0, 0, DEFAULT_DISPLACEMENT_DISTANCE);
                }
            }else if(manipulation_mode == OBJECT_MODE && _selected_object != NULL){
                if (button == 3){
                    scale_selected_object(_selected_object, LOCAL_MODE, 1 + DEFAULT_OBJECT_SCALE_RATIO, 1 + DEFAULT_OBJECT_SCALE_RATIO, 1 + DEFAULT_OBJECT_SCALE_RATIO);
                }else{
                    scale_selected_object(_selected_object, LOCAL_MODE, 1 - DEFAULT_OBJECT_SCALE_RATIO, 1 - DEFAULT_OBJECT_SCALE_RATIO, 1 - DEFAULT_OBJECT_SCALE_RATIO);
                }
            }
            // Each wheel event reports like a button click, GLUT_DOWN then GLUT_UP
            glutPostRedisplay();

        }

    }else{  // normal button event
        if(button == GLUT_MIDDLE_BUTTON){
            mousewheel_pressed = (button == GLUT_MIDDLE_BUTTON && (state == GLUT_DOWN));
            if(mousewheel_pressed){
                mousewheel_x_last = x;
                mousewheel_y_last = y;
                
            }
            special_wheel = (glutGetModifiers() == GLUT_ACTIVE_CTRL);
        }
        if(button == GLUT_LEFT_BUTTON){
            left_click_pressed = (button == GLUT_LEFT_BUTTON && (state == GLUT_DOWN));
            if(left_click_pressed){
                left_x_last = x;
                left_y_last = y;
            }
        }
    }
}

// Funcion con las opciones que puedes hacer con el raton cuando se mueve
void mouse_movement(int x, int y){
    if(mousewheel_pressed == 1){
        int deltaX = mousewheel_x_last - x, deltaY = mousewheel_y_last - y;
        float dist = sqrt( pow(deltaX,2) + pow(deltaY,2));
        if(camera_mode == CAMERA_FREECAM_MODE){
            if(special_wheel){
                rotate_camera_local(_selected_camera, (float)deltaX/mouse_sensitivity, 0, mouse_inverted_Y * 1, 0);
                rotate_camera_local(_selected_camera, (float)deltaY/mouse_sensitivity, mouse_inverted_X * 1, 0, 0); 
            }else{
                camera_rotation_point(_selected_camera, mouse_inverted_X * (float)deltaX/mouse_sensitivity, _selected_camera->mat[1], _selected_camera->mat[5], _selected_camera->mat[9], create_point_3(0 , 0, 0));
                camera_rotation_point(_selected_camera, mouse_inverted_Y * (float)deltaY/mouse_sensitivity, _selected_camera->mat[0], _selected_camera->mat[4], _selected_camera->mat[8], create_point_3(0 , 0, 0)); 
            }
        }else if(camera_mode == CAMERA_ANALISIS_MODE){
            camera_rotation_point(_selected_camera, mouse_inverted_X * (float)deltaX/mouse_sensitivity, _selected_camera->mat[1], _selected_camera->mat[5], _selected_camera->mat[9], _selected_object->origin_pos);
            camera_rotation_point(_selected_camera, mouse_inverted_Y * (float)deltaY/mouse_sensitivity, _selected_camera->mat[0], _selected_camera->mat[4], _selected_camera->mat[8], _selected_object->origin_pos); 
        }
        mousewheel_x_last = x;
        mousewheel_y_last = y;
    }else if(left_click_pressed){
        int deltaX = left_x_last - x, deltaY = left_y_last - y;
        float dist = sqrt( pow(deltaX,2) + pow(deltaY,2));
        if(camera_mode == CAMERA_FREECAM_MODE){
            displace_camera(_selected_camera, mouse_inverted_X * ((float)deltaX/mouse_sensitivity) / 4, 0, 0 );//_selected_camera->mat[0] * ((float)deltaX/mouse_sensitivity) / 4, _selected_camera->mat[1] * ((float)deltaX/mouse_sensitivity) / 4, _selected_camera->mat[2] * ((float)deltaX/mouse_sensitivity) / 4);
            displace_camera(_selected_camera, 0, mouse_inverted_Y * -((float)deltaY/mouse_sensitivity) / 4, 0);//-  _selected_camera->mat[4] * ((float)deltaY/mouse_sensitivity) / 4, - _selected_camera->mat[5] * ((float)deltaY/mouse_sensitivity) / 4, - _selected_camera->mat[6] * ((float)deltaY/mouse_sensitivity) / 4);
            //camera_rotation_point(_selected_camera, (float)deltaY/mouse_sensitivity, _selected_camera->mat[0], _selected_camera->mat[4], _selected_camera->mat[8], create_point_3(0 , 0, 0)); 
        }
        left_x_last = x;
        left_y_last = y;
    }
}

// Funcion que se ejecuta mientra no hay nada que hacer
void update()
{   
    camera3d *new_camera_ptr;
    double secs = 0;
    object3d *current_obj_ptr = _first_object;
    gettimeofday(&current_time, NULL);
    secs = (double)(current_time.tv_usec - last_time.tv_usec) / 1000000 + (double)(current_time.tv_sec - last_time.tv_sec);
       
    if(use_gui){
        if (update_gui == GUI_UPDATE_OBJ){
            cargar_objeto(gui_selected_obj);
            glutPostRedisplay();
            update_gui = 0;
        }else if (update_gui == GUI_UPDATE_STC){
            cargar_objeto_static(gui_selected_scene);
            glutPostRedisplay();
            update_gui = 0;
        }else if(update_gui == GUI_UPDATE_CAM){
            new_camera_ptr = create_camera( gui_camera[0], gui_camera[1], gui_camera[2],
                                            gui_camera[3], gui_camera[4], gui_camera[5],
                                            gui_camera[6], gui_camera[7], gui_camera[8],PERSPECTIVE_MODE);
            _selected_camera = new_camera_ptr;
            glutPostRedisplay();
            update_gui = 0;
        }
    }

    
    if (secs > seconds_for_update)
    {
        last_time = current_time;
        while (current_obj_ptr != NULL)
        {
            /*Ejecutar OnUpdate del objeto*/
            current_obj_ptr->onUpdate(current_obj_ptr);
            /*Siguiente objeto*/
            current_obj_ptr = current_obj_ptr->next;
        }

        basic_collider();
        glutPostRedisplay();
    }else if (redraw)
        glutPostRedisplay();
        
    return;
    
    
}
