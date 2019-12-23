#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <pthread.h> 
#include <unistd.h>

#include <dirent.h> 
#include <string.h>
#include "global_data.h"
#include "global_data_extern.h"

#include "load_obj.h"
#include "display.h"
#include "io.h"
#include "util.h"
#include "definitions.h"
#include "load_obj.h"
#include "math_util.h"
#include "toml_util.h"

/*Menu*/
static int objetos_menu_id;
static int val = 0;
static int window;
static int menuid;

void* init_gui(void *args) { 
    initializeGUI(args);
}

/** GENERAL INITIALIZATION **/
void initialization (){
    transformation_matrix *trans_mat;

    /*Initialization of all the variables with the default values*/
    _ortho_x_min = KG_ORTHO_X_MIN_INIT;
    _ortho_x_max = KG_ORTHO_X_MAX_INIT;
    _ortho_y_min = KG_ORTHO_Y_MIN_INIT;
    _ortho_y_max = KG_ORTHO_Y_MAX_INIT;
    _ortho_z_min = KG_ORTHO_Z_MIN_INIT;
    _ortho_z_max = KG_ORTHO_Z_MAX_INIT;

    _window_ratio = (GLdouble) KG_WINDOW_WIDTH / (GLdouble) KG_WINDOW_HEIGHT;

    /*Definition of the background color*/
    glClearColor(KG_COL_BACK_R, KG_COL_BACK_G, KG_COL_BACK_B, KG_COL_BACK_A);

    /*Definition of the method to draw the objects*/
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    _first_camera = create_camera(
                                0, 0, 1,
                                0, 0, 0,
                                0, 1, 0,
                                PERSPECTIVE_MODE
    );
   
    _selected_camera = _first_camera;
    _axes_camera = malloc(sizeof(camera3d));
    copy_mat(_first_camera->mat, _axes_camera->mat);
    seconds_for_update = (float) (1 / UPDATES_PER_SECOND);

    /*Static Objects*/
    object3d *scene_decor = cargar_objeto_static("scenes/isla_result.obj");
    //_first_static = scene_decor;
    //_selected_static = scene_decor;

    /*Configure Culling*/
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    /*Configure lightning*/
    glEnable(GL_LIGHTING);
    lights = malloc(sizeof(ilum)* 8);
    point3 camera_pos_spot = { 0, 0, 0};
    point3 camera_pos_sun = { 0, 5, 0}; 
    lights[0] = create_ilum_default(SUN);
    lights[0] ->pos = camera_pos_sun; 
    lights[1] = create_ilum_default(POINT);
    lights[2] = create_ilum_default(SPOT);
    lights[3] = create_ilum_default(SPOT);
    lights[3] ->diffuse = create_color3( 1, 1, 1);
    lights[4] = create_ilum_default(SUN);
    lights[4] ->diffuse = create_color3( 0, 1, 0);
    lights[5] = create_ilum_default(POINT);
    lights[5] ->diffuse = create_color3( 0, 0, 1);
    lights[6] = create_ilum_default(SUN);
    lights[6] ->diffuse = create_color3( 1, 0, 1);
    lights[7] = create_ilum_default(POINT);
    lights[7] ->diffuse = create_color3( 1, 1, 0);

    /*Detect first time using application*/
    char line[80];
    FILE *f = fopen("first_flag.conf","rw");
    if(f == NULL){
        f = fopen("first_flag.conf","a+");
        if (fscanf(f, "\n%[^\n]", line) <= 0){
            fwrite("Este trabajo es Specular! ;)\n",1,sizeof("Este trabajo es Specular! ;)\n"),f);
            //Set up special scene
            render_static = 1;
            render_mode = RENDER_COMBINED;
            wire_width = 3.0f;
            displace_camera(_selected_camera, 0, 4, -2);
        }
        fclose(f);
    }
    printf("Reading config . . .\n");
    printf("Result -> %s\n",read_config());
    
    /*GUI*/
    gui_selected_obj = malloc(sizeof(char)*100);
    gui_selected_scene = malloc(sizeof(char)*100);
    gui_camera = malloc(sizeof(float)*9);
    for(int i = 0; i < 9; i++){
        gui_camera[i] = 0;
    }
    strcpy(gui_selected_obj, "objektuak/abioia.obj");
    strcpy(gui_selected_scene, "scene/isla_result.obj"); 
    
    axes = cargar_objeto("objektuak/axes.obj");
    scale_selected_object(axes, LOCAL_MODE, 0.20, 0.20, 0.20);
    if(_first_object == axes){
        _first_object = _first_object->next;
        _selected_object = _first_object;
    }

    rainbow_array = malloc(sizeof(color3) * 8);
    rainbow_array[0] = create_color3( 1, 0, 0);
    rainbow_array[1] = create_color3( 1, 1, 0);
    rainbow_array[2] = create_color3( 0, 1, 0);
    rainbow_array[3] = create_color3( 0, 1, 1);
    rainbow_array[4] = create_color3( 1, 0, 1);
    rainbow_array[5] = create_color3( 1, 0.75, 0.5);
    rainbow_array[6] = create_color3( 0.5, 1, 0.7);
    rainbow_array[7] = create_color3( 1, 0, 0);
}

/*Menu*/
// Funcion que lee los archivos de la carpeta ./objektuak
void menu(int value){
	if(value == 0){
		glutDestroyWindow(window);
		exit(0);
	}else{
		val=value;
		printf("%s \n\n\n", objectlist[val].text);
		char* fname = malloc(sizeof (char)*128); /* Note that scanf adds a null character at the end of the vector*/
    	int read = 0;
    	object3d *auxiliar_object = 0;
    	/*Allocate memory for the structure and read the file*/
        auxiliar_object = (object3d *) malloc(sizeof (object3d));
        read = read_wavefront(objectlist[val].text, auxiliar_object);
        switch (read) {
        /*Errors in the reading*/
        case 1:
            printf("%s: %s\n", fname, KG_MSSG_FILENOTFOUND);
            break;
        case 2:
            printf("%s: %s\n", fname, KG_MSSG_INVALIDFILE);
            break;
        case 3:
            printf("%s: %s\n", fname, KG_MSSG_EMPTYFILE);
            break;
        /*Read OK*/
        case 0:
            /*Insert the new object in the list*/
            auxiliar_object->next = _first_object;
            _first_object = auxiliar_object;
            _selected_object = _first_object;
            printf("%s\n",KG_MSSG_FILEREAD);
            break;
        } 
	}
	// you would want to redraw now
	glutPostRedisplay();
}

// Funcion que crea el menu
void createMenu(void){
	//////////
	// MENU //
	//////////


	DIR *d;
	struct dirent *dir;
	int i = 1;
	struct objetoidname* objectname; 
	objectlist = malloc(80*sizeof(struct objetoidname));
	// Create a submenu, this has to be done first
	objetos_menu_id = glutCreateMenu(menu);
	d = opendir("./objektuak");
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			if (!strcmp(dir -> d_name, "..") || !strcmp(dir -> d_name, ".")){
				continue;
			}
            if(strstr(dir -> d_name, ".mtl") != NULL){
                continue;
            }

			glutAddMenuEntry(dir->d_name, i);
			objectname = malloc(sizeof(struct objetoidname));
			strcpy(objectname -> text, "objektuak/");
			strcat(objectname -> text, dir->d_name);
			objectlist[i] = *objectname;
			i+=1;
		}
		closedir(d);
	}

	// Create the menu, this menu becomes the current menu
	menuid = glutCreateMenu(menu);

	// Create an entry
	glutAddSubMenu("Cargar Objeto", objetos_menu_id);
	
	// Let the menu respond on the right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
}

/** MAIN FUNCTION **/
int main(int argc, char** argv) {

    /*First of all, print the help information*/
    print_help();

    /* glut initializations */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
    glutInitWindowSize(KG_WINDOW_WIDTH, KG_WINDOW_HEIGHT);
    glutInitWindowPosition(KG_WINDOW_X, KG_WINDOW_Y);
    glutCreateWindow(KG_WINDOW_TITLE);
    createMenu();
    
    /* set the callback functions */
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special_keyboard);
    glutIdleFunc(update);
    glutMouseFunc(mouse);
    glutMotionFunc(mouse_movement);
    /* this initialization has to be AFTER the creation of the window */
    initialization();
    
    /*GUI*/
    if(use_gui == 1){
        GUI *args = malloc(sizeof(GUI));
        args->argc = argc;
        args->argv = argv;
        thread1 = (pthread_t*) malloc(sizeof(pthread_t));
        pthread_create(thread1, NULL, init_gui, args);
    }
    /* start the main loop */
    glutMainLoop();
    return 0;
}
