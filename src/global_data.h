#include "definitions.h"
#include <pthread.h> 
#include <unistd.h>

/** GLOBAL VARIABLES **/

GLdouble _window_ratio;                     /*Control of window's proportions */
GLdouble _ortho_x_min,_ortho_x_max;         /*Variables for the control of the orthographic projection*/
GLdouble _ortho_y_min ,_ortho_y_max;        /*Variables for the control of the orthographic projection*/
GLdouble _ortho_z_min,_ortho_z_max;         /*Variables for the control of the orthographic projection*/

object3d * _first_object= 0;                /*List of objects*/
object3d * _selected_object = 0;            /*Object currently selected*/

object3d * _first_disposable_object = 0;    /*Disposable objects*/

object3d * _first_static = 0;               /*Lista de objetos sin interacción*/
object3d * _selected_static = 0;            /*Lista de objetos sin interacción*/

object3d * axes = 0;
camera3d * _axes_camera = 0;

/*Camaras*/
camera3d * _first_camera = 0;
camera3d * _selected_camera = 0;

/*Update time*/
float seconds_for_update;
int redraw = 0;

/*Control de objetos estaticos*/
int render_static = 0;

/*Draw modes*/
int render_mode = RENDER_WIREFRAME;
double wire_width = MIN_WIREFRAME_WIDTH;

//Modes
int manipulation_mode = OBJECT_MODE;
int transformation_mode = TRANSLATION_MODE;
int reference_mode = GLOBAL_MODE;
int camera_mode = CAMERA_FREECAM_MODE;
int light_rotation_reference = LOCAL_MODE;
int isFirst = 0;

/*Lightning*/
int lightning_mode = LIGHTNING_ON;
int shading_mode = SHADING_FLAT;
ilum **lights;
int _selected_light = 0;

/*Lista de objetos en ./objektuak*/
struct objetoidname* objectlist;

/*GUI*/
extern void initializeGUI();
int update_gui = 0;
char *gui_selected_obj;
char *gui_selected_scene;
float *gui_camera;
pthread_t *thread1 = 0;

double mouse_sensitivity = 10;
int mouse_inverted_X = 1, mouse_inverted_Y = 1;
int use_gui = 1;
int cmd_verbose = 0;
int cmd_show_status = 1;
int show_axes = 1;
double axes_percentage = 0.2;

//OTHERS
int selected_rainbow_mat = 0;

//RAINBOW
color3 *rainbow_array;

char *quickload_path = "objektuak/abioia.obj";
