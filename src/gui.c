#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <gtk/gtk.h>
#include "definitions.h"
#include "io.h"
#include <string.h>
#include "util.h"
#include <GL/glut.h>
#include "load_obj.h"
#include "math_util.h"


/* OBJETOS */
typedef struct {
    GtkWidget *w_rb_1;
    GtkWidget *w_rb_2;
    GtkWidget *w_rb_3;
    GtkWidget *w_rb_4;
    GtkWidget *w_rb_5;
    GtkWidget *w_rb_6;
    GtkWidget *w_rb_7;
    GtkWidget *w_rb_8;
    GtkWidget *w_rb_9;
    GtkWidget *w_rb_10;
    GtkWidget *w_rb_11;
    GtkWidget *w_rb_12;
    GtkWidget *w_rb_13;
    GtkWidget *w_rb_14;
    GtkWidget *w_lbl_choice;
} app_widgets;

app_widgets *widgets;

/* ESCENARIOS */
typedef struct {
    GtkWidget *w_rb_1;
    GtkWidget *w_rb_2;
} scene_wid;

scene_wid *escenario;

/* CAMARA */
typedef struct {
    GtkWidget *ex;
    GtkWidget *ey;
    GtkWidget *ez;
    GtkWidget *atx;
    GtkWidget *aty;
    GtkWidget *atz;
    GtkWidget *vx;
    GtkWidget *vy;
    GtkWidget *vz;
} camara;

camara *cam_val;

/* Iluminacion */

typedef struct{
    GtkWidget *a_r;
    GtkWidget *a_g;
    GtkWidget *a_b;
    GtkWidget *d_r;
    GtkWidget *d_g;
    GtkWidget *d_b;
    GtkWidget *s_r;
    GtkWidget *s_g;
    GtkWidget *s_b;
    GtkWidget *is_on;
    GtkWidget *type;
}ilum_color;

typedef struct{
    ilum_color *iluminacion0;
    ilum_color *iluminacion1;
    ilum_color *iluminacion2;
    ilum_color *iluminacion3;
    ilum_color *iluminacion4;
    ilum_color *iluminacion5;
    ilum_color *iluminacion6;
    ilum_color *iluminacion7;
}luces;

luces *luz_lista;

GtkBuilder  *builder; 
GtkWidget   *window;
GtkWidget   *mode_type;
GtkWidget   *mode_transformation_type;
GtkWidget   *reference_mode_type;
GtkWidget   *proyection_mode_type;
GtkWidget   *actualx;
GtkWidget   *actualy;
GtkWidget   *actualz;

extern int update_gui;

extern int transformation_mode;
extern int reference_mode;
extern int manipulation_mode;
extern int camera_mode;
extern camera3d * _selected_camera;
extern object3d * _selected_object;

extern int render_static;

extern char *gui_selected_obj;
int toggled_obj = 0;

extern char *gui_selected_scene;
int toggled_scene = 0;

extern float* gui_camera;

extern ilum **lights;
extern int _selected_light;

/*OBJETOS*/

/*Changes the update_gui flag in order to laod the object*/
void on_cargar_objeto_btn_clicked(){
    update_gui = 1;
}

/*Selects the object to be loaded*/
void on_rb_toggled(GtkToggleButton *togglebutton)
{
    if(toggled_obj) 
        toggled_obj = 0;
    else
        toggled_obj = 1;
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->w_rb_1)) && toggled_obj) {
        gtk_label_set_text(GTK_LABEL(widgets->w_lbl_choice), "Seleccionado: abioia.obj");     // update label
        strcpy(gui_selected_obj, "objektuak/abioia.obj");
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->w_rb_2)) && toggled_obj) {
        gtk_label_set_text(GTK_LABEL(widgets->w_lbl_choice), "Seleccionado: aur.obj");     // update label
        strcpy(gui_selected_obj, "objektuak/aur.obj");
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->w_rb_3)) && toggled_obj) {
        gtk_label_set_text(GTK_LABEL(widgets->w_lbl_choice), "Seleccionado: aurpegi.obj");     // update label
        strcpy(gui_selected_obj, "objektuak/aurpegi.obj");
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->w_rb_4)) && toggled_obj) {
        gtk_label_set_text(GTK_LABEL(widgets->w_lbl_choice), "Seleccionado: cub.obj");     // update label
        strcpy(gui_selected_obj, "objektuak/cub.obj");
        }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->w_rb_5)) && toggled_obj) {
        gtk_label_set_text(GTK_LABEL(widgets->w_lbl_choice), "Seleccionado: cubo.obj");     // update label
        strcpy(gui_selected_obj, "objektuak/cubo.obj");
        }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->w_rb_6)) && toggled_obj) {
        gtk_label_set_text(GTK_LABEL(widgets->w_lbl_choice), "Seleccionado: deathstr.obj");     // update label
        strcpy(gui_selected_obj, "objektuak/deathstr.obj");
        }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->w_rb_7)) && toggled_obj) {
        gtk_label_set_text(GTK_LABEL(widgets->w_lbl_choice), "Seleccionado: destroyr.obj");     // update label
        strcpy(gui_selected_obj, "objektuak/destroyr.obj");
        }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->w_rb_8)) && toggled_obj) {
        gtk_label_set_text(GTK_LABEL(widgets->w_lbl_choice), "Seleccionado: intercep.obj");     // update label
        strcpy(gui_selected_obj, "objektuak/intercep.obj");
        //printf("%s\n", selected_obj_load);
        }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->w_rb_9)) && toggled_obj) {
        gtk_label_set_text(GTK_LABEL(widgets->w_lbl_choice), "Seleccionado: kuboa.obj");     // update label
        strcpy(gui_selected_obj, "objektuak/kuboa.obj");
        }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->w_rb_10)) && toggled_obj) {
        gtk_label_set_text(GTK_LABEL(widgets->w_lbl_choice), "Seleccionado: logoehu.obj");     // update label
        strcpy(gui_selected_obj, "objektuak/logoehu.obj");
        }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->w_rb_11)) && toggled_obj) {
        gtk_label_set_text(GTK_LABEL(widgets->w_lbl_choice), "Seleccionado: logoehu_ona.obj");     // update label
        strcpy(gui_selected_obj, "objektuak/logoehu_ona.obj");
        }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->w_rb_12)) && toggled_obj) {
        gtk_label_set_text(GTK_LABEL(widgets->w_lbl_choice), "Seleccionado: mirua.obj");     // update label
        strcpy(gui_selected_obj, "objektuak/mirua.obj");
        }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->w_rb_13)) && toggled_obj) {
        gtk_label_set_text(GTK_LABEL(widgets->w_lbl_choice), "Seleccionado: r_falke.obj");     // update label
        strcpy(gui_selected_obj, "objektuak/r_falke.obj");
        }
     else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->w_rb_14)) && toggled_obj) {
        gtk_label_set_text(GTK_LABEL(widgets->w_lbl_choice), "Seleccionado: x_wing.obj");     // update label
        strcpy(gui_selected_obj, "objektuak/x_wing.obj");
        }
}

/*ESCENARIOS */

/* Activates/desactivates the scene*/
void on_activar_escenario_btn_clicked(){
    render_static = (render_static)?0:1;
    glutPostRedisplay();
}

/*Changes the loaded scene*/
void on_escenario_toggled(GtkToggleButton *togglebutton)
{
    if(toggled_scene) 
        toggled_scene = 0;
    else
        toggled_scene = 1;

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(escenario->w_rb_1)) && toggled_scene) {
        strcpy(gui_selected_scene, "scenes/isla_result.obj");
        update_gui = 2;
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(escenario->w_rb_2))&& toggled_scene) {
        strcpy(gui_selected_scene, "scenes/desierto_result.obj");
        update_gui = 2;
    }
}


/* FIN ESCENARIOS*/

/* CAMARAS */

/*gets the text of the text_view*/
char *get_text_of_textview(GtkWidget *text_view) {
    GtkTextIter start, end;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer((GtkTextView *)text_view);
    gchar *text;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    return text;
}


/*Adds to the camera buffer the values attention point, eye and vertical vector*/
void on_create_camera_clicked(){
    gui_camera[0] = atof(get_text_of_textview(cam_val->ex));
    gui_camera[1] = atof(get_text_of_textview(cam_val->ey));
    gui_camera[2] = atof(get_text_of_textview(cam_val->ez));
    gui_camera[3] = atof(get_text_of_textview(cam_val->atx));
    gui_camera[4] = atof(get_text_of_textview(cam_val->aty));
    gui_camera[5] = atof(get_text_of_textview(cam_val->atz));
    gui_camera[6] = atof(get_text_of_textview(cam_val->vx));
    gui_camera[7] = atof(get_text_of_textview(cam_val->vy));
    gui_camera[8] = atof(get_text_of_textview(cam_val->vz));
    update_gui = 3;
}



/* FIN CAMARAS */

/* ILUMINACION */

/*Devuelve la luz i*/
ilum_color* get_luz_i(int i){
    switch(i){
        case 0:
            return luz_lista->iluminacion0;
        case 1:
            return luz_lista->iluminacion1;
        case 2:
            return luz_lista->iluminacion2;
        case 3:
            return luz_lista->iluminacion3;
        case 4:
            return luz_lista->iluminacion4;
        case 5:
            return luz_lista->iluminacion5;
        case 6:
            return luz_lista->iluminacion6;
        case 7:
            return luz_lista->iluminacion7;
    
    }

}

/* Carga los widgets de GTK relacionado con la luz i*/
void cargar_luz(int i, ilum_color *iluminacion){
        char dest[80];
        char j[10];
        sprintf(j, "%d", i);
        strcpy(dest, "luz");
        strcat(dest, j);
        strcat(dest, "_main_a_r");
        iluminacion->a_r = GTK_WIDGET(gtk_builder_get_object(builder, dest));

        strcpy(dest, "luz");
        strcat(dest, j);
        strcat(dest, "_main_a_g");
        iluminacion->a_g = GTK_WIDGET(gtk_builder_get_object(builder, dest));

        strcpy(dest, "luz");
        strcat(dest, j);
        strcat(dest, "_main_a_b");
        iluminacion->a_b = GTK_WIDGET(gtk_builder_get_object(builder, dest));

        strcpy(dest, "luz");
        strcat(dest, j);
        strcat(dest, "_main_d_r");
        iluminacion->d_r = GTK_WIDGET(gtk_builder_get_object(builder, dest));

        strcpy(dest, "luz");
        strcat(dest, j);
        strcat(dest, "_main_d_g");
        iluminacion->d_g = GTK_WIDGET(gtk_builder_get_object(builder, dest));

        strcpy(dest, "luz");
        strcat(dest, j);
        strcat(dest, "_main_d_b");
        iluminacion->d_b = GTK_WIDGET(gtk_builder_get_object(builder, dest));

        strcpy(dest, "luz");
        strcat(dest, j);
        strcat(dest, "_main_s_r");
        iluminacion->s_r = GTK_WIDGET(gtk_builder_get_object(builder, dest));

        strcpy(dest, "luz");
        strcat(dest, j);
        strcat(dest, "_main_s_g");
        iluminacion->s_g = GTK_WIDGET(gtk_builder_get_object(builder, dest));

        strcpy(dest, "luz");
        strcat(dest, j);
        strcat(dest, "_main_s_b");
        iluminacion->s_b = GTK_WIDGET(gtk_builder_get_object(builder, dest));

        strcpy(dest, "luz");
        strcat(dest, j);
        strcat(dest, "_is_on");
        iluminacion->is_on = GTK_WIDGET(gtk_builder_get_object(builder, dest));

        strcpy(dest, "luz");
        strcat(dest, j);
        strcat(dest, "_type");
        iluminacion->type = GTK_WIDGET(gtk_builder_get_object(builder, dest));

}

/*Escribe los valores por defecto de cada luz en su respectiva caja*/
void luces_actuales(){
    for(int i= 0; i< 8; i++){
        float luz1[] = {lights[i]->ambient.r*255, lights[i]->ambient.g*255, lights[i]->ambient.b*255, 
                    lights[i]->diffuse.r*255, lights[i]->diffuse.g*255, lights[i]->diffuse.b*255,
                    lights[i]->specular.r*255, lights[i]->specular.g*255, lights[i]->specular.b*255};
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(get_luz_i(i)->a_r), luz1[0]);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(get_luz_i(i)->a_g), luz1[1]);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(get_luz_i(i)->a_b), luz1[2]);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(get_luz_i(i)->d_r), luz1[3]);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(get_luz_i(i)->d_g), luz1[4]);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(get_luz_i(i)->d_b), luz1[5]);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(get_luz_i(i)->s_r), luz1[6]);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(get_luz_i(i)->s_g), luz1[7]);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(get_luz_i(i)->s_b), luz1[8]);
    }
}

/*Actualiza el color de la luz i*/
void on_luz_main_change(int i){
    ilum_color *selected = get_luz_i(i);
    lights[i]->ambient.r = (float) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(selected->a_r))/255;
    lights[i]->ambient.g = (float) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(selected->a_g))/255;
    lights[i]->ambient.b = (float) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(selected->a_b))/255;
    lights[i]->diffuse.r = (float) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(selected->d_r))/255;
    lights[i]->diffuse.g = (float) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(selected->d_g))/255;
    lights[i]->diffuse.b = (float) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(selected->d_b))/255;
    lights[i]->specular.r = (float) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(selected->s_r))/255;
    lights[i]->specular.g = (float) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(selected->s_g))/255;
    lights[i]->specular.b = (float) gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(selected->s_b))/255;
    glutPostRedisplay();
}
/*Actualiza el color de la luz 0*/
void on_luz1_main_a_r_change_value(){
    on_luz_main_change(0);
}
/*Actualiza el color de la luz 1*/
void on_luz2_main_a_r_change_value(){
    on_luz_main_change(1);
}
/*Actualiza el color de la luz 2*/
void on_luz3_main_a_r_change_value(){
    on_luz_main_change(2);
}
/*Actualiza el color de la luz 3*/
void on_luz4_main_a_r_change_value(){
    on_luz_main_change(3);
}
/*Actualiza el color de la luz 4*/
void on_luz5_main_a_r_change_value(){
    on_luz_main_change(4);
}
/*Actualiza el color de la luz 5*/
void on_luz6_main_a_r_change_value(){
    on_luz_main_change(5);
}
/*Actualiza el color de la luz 6*/
void on_luz7_main_a_r_change_value(){
    on_luz_main_change(6);
}
/*Actualiza el color de la luz 7*/
void on_luz8_main_a_r_change_value(){
    on_luz_main_change(7);
}
/* FIN ILUMINACIOn */

/* MODOS SELECCIONADOS */

/*Actualizacion de los valores de los flags*/
gboolean timeout(){
    char x[70];
    char y[70];
    char z[70];
    if(manipulation_mode == OBJECT_MODE){
        gtk_label_set_text(GTK_LABEL(mode_type), "Objeto");
        gtk_label_set_text(GTK_LABEL(reference_mode_type),(reference_mode == GLOBAL_MODE)?"Global":"Local");
        if (_selected_object != NULL){       
            sprintf(x, "%f", _selected_object->origin_pos.x);
            sprintf(y, "%f", _selected_object->origin_pos.y);
            sprintf(z, "%f", _selected_object->origin_pos.z);
        }else{
            strcpy(x, "No hay Objeto");
            strcpy(y, "No hay Objeto");
            strcpy(z, "No hay Objeto");
        }
    }else if(manipulation_mode == CAMERA_MODE){
        gtk_label_set_text(GTK_LABEL(mode_type), "Camera");
        gtk_label_set_text(GTK_LABEL(reference_mode_type),(camera_mode == CAMERA_ANALISIS_MODE)?"Analisis":"Free");
        sprintf(x, "%f", _selected_camera->pos.x);
        sprintf(y, "%f", _selected_camera->pos.y);
        sprintf(z, "%f", _selected_camera->pos.z);     
    }else{
        gtk_label_set_text(GTK_LABEL(mode_type), "Iluminacion");
        gtk_label_set_text(GTK_LABEL(reference_mode_type),(reference_mode == GLOBAL_MODE)?"Global":"Local");
        sprintf(x, "%f", lights[_selected_light]->pos.x);
        sprintf(y, "%f", lights[_selected_light]->pos.y);
        sprintf(z, "%f", lights[_selected_light]->pos.z);
    }
    gtk_label_set_text(GTK_LABEL(actualx),x);
    gtk_label_set_text(GTK_LABEL(actualy),y);
    gtk_label_set_text(GTK_LABEL(actualz),z);
    gtk_label_set_text(GTK_LABEL(mode_transformation_type), (transformation_mode == TRANSLATION_MODE)?"Traslation":(transformation_mode == ROTATION_MODE)?"Rotation":(transformation_mode == SCALE_MODE)?"Scale":"Velocity");
    gtk_label_set_text(GTK_LABEL(proyection_mode_type),(_selected_camera->projection_mode == PERSPECTIVE_MODE)?"Perspective":"Ortho");
    
    for(int i=0; i<8; i++){
    gtk_label_set_text(GTK_LABEL(get_luz_i(i)->is_on),(lights[i]->is_on == 1)?"Si":"No");
    gtk_label_set_text(GTK_LABEL(get_luz_i(i)->type),(lights[i]->mode == SUN)? "Sol":(lights[i]->mode == SPOT) ? "Foco" : "Bombilla");
    }
}

/* FIN MODOS SELECCIONADOS */

/* Al darle al boton de cerrar se cierra el gtk*/
void on_window_main_destroy()
{
    gtk_main_quit();
}

/*Inicializa los widgets principales de la GUI*/
void initializeGUI(GUI *args){
    gtk_disable_setlocale();
    gtk_init(&(args->argc), &(args->argv));

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "gui.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);
    gtk_window_set_title((GtkWindow*) window,"PROYECTO GC2 CARLOS DOMINGUEZ Y MARKEL CORTAZAR");

    /*MODOS SELECCIONADOS*/
    mode_type = GTK_WIDGET(gtk_builder_get_object(builder, "mode_type"));
    mode_transformation_type = GTK_WIDGET(gtk_builder_get_object(builder, "mode_transformation_type"));
    reference_mode_type = GTK_WIDGET(gtk_builder_get_object(builder, "reference_mode_type"));
    proyection_mode_type = GTK_WIDGET(gtk_builder_get_object(builder, "proyection_mode_type"));
    actualx = GTK_WIDGET(gtk_builder_get_object(builder, "actual_x"));
    actualy = GTK_WIDGET(gtk_builder_get_object(builder, "actual_y"));
    actualz = GTK_WIDGET(gtk_builder_get_object(builder, "actual_z"));
    g_timeout_add (200, (GSourceFunc)timeout, window);
    /*FIN MODOS SELECCIONADOS*/

    /*OBJETOS*/
    widgets = g_slice_new(app_widgets);
    widgets->w_rb_1 = GTK_WIDGET(gtk_builder_get_object(builder, "abioia"));
    widgets->w_rb_2 = GTK_WIDGET(gtk_builder_get_object(builder, "aur"));
    widgets->w_rb_3 = GTK_WIDGET(gtk_builder_get_object(builder, "aurpegi"));
    widgets->w_rb_4 = GTK_WIDGET(gtk_builder_get_object(builder, "cub"));
    widgets->w_rb_5 = GTK_WIDGET(gtk_builder_get_object(builder, "cubo"));
    widgets->w_rb_6 = GTK_WIDGET(gtk_builder_get_object(builder, "deathstr"));
    widgets->w_rb_7 = GTK_WIDGET(gtk_builder_get_object(builder, "destroyr"));
    widgets->w_rb_8 = GTK_WIDGET(gtk_builder_get_object(builder, "intercep"));
    widgets->w_rb_9 = GTK_WIDGET(gtk_builder_get_object(builder, "kuboa"));
    widgets->w_rb_10 = GTK_WIDGET(gtk_builder_get_object(builder, "logoehu"));
    widgets->w_rb_11 = GTK_WIDGET(gtk_builder_get_object(builder, "logoehu_ona"));
    widgets->w_rb_12 = GTK_WIDGET(gtk_builder_get_object(builder, "mirua"));
    widgets->w_rb_13 = GTK_WIDGET(gtk_builder_get_object(builder, "r_falke"));
    widgets->w_rb_14 = GTK_WIDGET(gtk_builder_get_object(builder, "x_wing"));
    widgets->w_lbl_choice = GTK_WIDGET(gtk_builder_get_object(builder, "selected_obj"));
    /*FIN OBJETOS*/

    /*ESCENARIO*/
    escenario = g_slice_new(scene_wid);
    escenario ->w_rb_1 = GTK_WIDGET(gtk_builder_get_object(builder, "escenario_isla_radio"));
    escenario ->w_rb_2 = GTK_WIDGET(gtk_builder_get_object(builder, "escenario_desierto_radio"));
    /*FIN ESCENARIO*/
    
    /*ESCENARIO*/
    cam_val = g_slice_new(camara);
    cam_val->ex = GTK_WIDGET(gtk_builder_get_object(builder, "ex"));
    cam_val->ey = GTK_WIDGET(gtk_builder_get_object(builder, "ey"));
    cam_val->ez = GTK_WIDGET(gtk_builder_get_object(builder, "ez"));
    cam_val->atx = GTK_WIDGET(gtk_builder_get_object(builder, "atx"));
    cam_val->aty = GTK_WIDGET(gtk_builder_get_object(builder, "aty"));
    cam_val->atz = GTK_WIDGET(gtk_builder_get_object(builder, "atz"));
    cam_val->vx = GTK_WIDGET(gtk_builder_get_object(builder, "vx"));
    cam_val->vy = GTK_WIDGET(gtk_builder_get_object(builder, "vy"));
    cam_val->vz = GTK_WIDGET(gtk_builder_get_object(builder, "vz"));
    /*FIN ESCENARIO*/
    
    /*ILUMINACION*/
    luz_lista = g_slice_new(luces);
    luz_lista->iluminacion0 = g_slice_new(ilum_color);
    luz_lista->iluminacion1 = g_slice_new(ilum_color);
    luz_lista->iluminacion2 = g_slice_new(ilum_color);
    luz_lista->iluminacion3 = g_slice_new(ilum_color);
    luz_lista->iluminacion4 = g_slice_new(ilum_color);
    luz_lista->iluminacion5 = g_slice_new(ilum_color);
    luz_lista->iluminacion6 = g_slice_new(ilum_color);
    luz_lista->iluminacion7 = g_slice_new(ilum_color);
    for(int i=0; i<8; i++){
        cargar_luz(i+1, get_luz_i(i));
    }
    luces_actuales();
    /*FIN ILUMINACION*/

    g_object_unref(builder);

    gtk_widget_show(window);                
    gtk_main();
}


