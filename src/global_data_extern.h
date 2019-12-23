#include "definitions.h"
#include <pthread.h> 
#include <unistd.h>

extern int manipulation_mode;
extern int transformation_mode;
extern int reference_mode;
extern int light_rotation_reference;
extern int camera_mode;

extern object3d *_first_object;
extern object3d *_selected_object;

extern object3d * _first_disposable_object;

extern object3d *_first_static;
extern object3d * _selected_static;

extern camera3d * _first_camera ;
extern camera3d * _selected_camera ;
extern camera3d * _axes_camera;
extern int render_static;
extern int render_mode;
extern double wire_width;
extern int redraw;

extern ilum **lights;
extern int _selected_light;

extern GLdouble _ortho_z_min, _ortho_z_max;

extern float seconds_for_update;

/* GUI */
extern int update_gui;
extern char *gui_selected_obj;
extern char *gui_selected_scene;
extern float *gui_camera;
extern pthread_t *thread1;

extern GLdouble _window_ratio;
/*Control de objetos estaticos*/
extern int render_static;

/*Draw modes*/
extern int render_mode;
extern double wire_width;

extern int isFirst;

/*Lightning*/
extern int lightning_mode;
extern int shading_mode;

extern double mouse_sensitivity;
extern int mouse_inverted_X; 
extern int mouse_inverted_Y;

extern int use_gui;

extern int cmd_verbose;
extern int cmd_show_status;

extern object3d *axes;
extern int show_axes;
extern double axes_percentage;
//RAINBOW
extern int selected_rainbow_mat;
extern color3 *rainbow_array;
extern char *quickload_path;