#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> 
#include <string.h>
#include "toml.h"
#include "definitions.h"
#include "util.h"
#include "math_util.h"
#include "global_data_extern.h"

// Obtiene un punto contres doubles de un array en la tabla dada
point3 get_point3(toml_table_t *parent, const char* key){
    double x, y, z;
    toml_array_t *aux = toml_array_in(parent, key);
    const char *test = toml_raw_at( aux, 0);
    toml_rtod(test, &x);
    test = toml_raw_at( aux, 1);
    toml_rtod(test, &y);
    test = toml_raw_at( aux, 2);
    toml_rtod(test, &z);
    return create_point_3( x, y, z);
}

// Reemplaza la linea 20 para saber si es la primera vez que se ejecuta el proyecto
int replace_line(){
        FILE *fptr1, *fptr2;
        int lno = 20, linectr = 0;
        char str[255] ,fname[255] = "config.toml";        
        char newln[255] = "\tfirst-run = false\n", temp[] = "temp_config.toml";
		
        fptr1 = fopen(fname, "r");
        if (!fptr1) 
        {
                printf("Unable to open the input file!!\n");
                return 0;
        }
        fptr2 = fopen(temp, "w");
        if (!fptr2) 
        {
                printf("Unable to open a temporary file to write!!\n");
                fclose(fptr1);
                return 0;
        }
         // copy all contents to the temporary file other except specific line
        while (!feof(fptr1)) 
        {
            strcpy(str, "\0");
            fgets(str, 255, fptr1);
            if (!feof(fptr1)) 
            {
                linectr++;
                if (linectr != lno) 
                    {
                        fprintf(fptr2, "%s", str);
                    } 
                    else 
                    {
                        fprintf(fptr2, "%s", newln);
                    }
                }
        }
        fclose(fptr1);
        fclose(fptr2);
        remove(fname);
        rename(temp, fname);
}
// Configura los modos
void mode_config(toml_table_t *conf){
    const char* manipulation, *transformation, *reference, *camera;
    toml_table_t *init = toml_table_in(conf, "INITIAL-MODE");
    printf("MODE CONFIG\n");
    manipulation  = toml_raw_in(init, "manipulation-mode");
    transformation = toml_raw_in(init, "transformation-mode");
    reference = toml_raw_in(init, "reference-mode");
    camera = toml_raw_in(init, "camera-mode");
    //Manipulation
    if(strstr(manipulation ,"CAMERA"))
        manipulation_mode = CAMERA_MODE;
    else if(strstr(manipulation, "ILUMINATION"))
        manipulation_mode = ILUM_MODE;
    else if(strstr(manipulation, "OBJECT"))
        manipulation_mode = OBJECT_MODE;
    else{
        printf("\tmanipulation-mode is wrongly configured, resorting to default . . .\n");
    }
    //Transformation
    if(strstr(transformation ,"TRANSLATION"))
        transformation_mode = TRANSLATION_MODE;
    else if(strstr(transformation, "ILUMINATION"))
        transformation_mode = ROTATION_MODE;
    else if(strstr(transformation, "SCALE"))
        transformation_mode = SCALE_MODE;
    else if(strstr(transformation, "VELOCITY"))
        transformation_mode = VELOCITY_MODE;
    else{
        printf("\transformation-mode is wrongly configured, resorting to default . . .\n");
    }
    //Reference
    if(strstr(reference ,"GLOBAL"))
        reference_mode = GLOBAL_MODE;
    else if(strstr(reference, "LOCAL"))
        reference_mode = LOCAL_MODE;
    else{
        printf("\reference-mode is wrongly configured, resorting to default . . .\n");
    }
    //Camera
    if(strstr(camera ,"FREE"))
        camera_mode = CAMERA_FREECAM_MODE;
    else if(strstr(camera, "ANALISIS"))
        camera_mode = CAMERA_ANALISIS_MODE;
    else{
        printf("\tcamera-mode is wrongly configured, resorting to default . . .\n");
    }


}
// Configura la UI
void ui_config(toml_table_t *conf){
    printf("UI CONFIG\n");
    toml_table_t *ui = toml_table_in(conf, "USER-INTERFACE");
    const char  *cmd_verbose_raw = toml_raw_in(ui, "cmd-verbose"),
                *cmd_status_raw = toml_raw_in(ui, "cmd-status"),
                *gui_raw = toml_raw_in(ui, "gui"),
                *axes_raw = toml_raw_in(ui, "show-axes"),
                *ax_size_raw = toml_raw_in(ui, "axes-size");
    if(toml_rtob(cmd_verbose_raw, &cmd_verbose) == -1){
        printf("\tcmd-verbose is wrongly configured, resorting to default . . .\n");
        cmd_verbose = 0;
    }
    if(toml_rtob(cmd_status_raw, &cmd_show_status) == -1){
        printf("\tcmd-status is wrongly configured, resorting to default . . .\n");
        cmd_show_status = 1;
    }
    if(toml_rtob(gui_raw, &use_gui) == -1){
        printf("\tgui is wrongly configured, resorting to default . . .\n");
        use_gui = 1;
    }else{
        // printf("\n");
    }
    if(toml_rtob(axes_raw, &show_axes) == -1){
        printf("\tshow-axes is wrongly configured, resorting to default . . .\n");
        show_axes = 1;
    }
    if(toml_rtod(ax_size_raw, &axes_percentage) == -1){
        printf("%s %lf\n",ax_size_raw, axes_percentage);
        printf("\tshow-size is wrongly configured, resorting to default . . .\n");
        axes_percentage = 0.2;
    }else{
        if(axes_percentage > 1)
            axes_percentage = 1;
        if(axes_percentage < 0)
            axes_percentage = 0;
    }
}
// Configura el renderizado
void render_config(toml_table_t *conf){
    printf("RENDER CONFIG\n");
    toml_table_t *render = toml_table_in(conf, "RENDERING");
    const char  *mode_raw = toml_raw_in(render, "default-render-mode"),
                *wire_raw = toml_raw_in(render, "default-wireframe-width"),
                *scenes_raw = toml_raw_in(render, "render-scenes");
    
    if(toml_rtob(scenes_raw, &render_static) == -1){
        printf("\trender-scenes is wrongly configured, resorting to default . . .\n");
        render_static = 0;
    }
    if(strstr(mode_raw ,"WIRE"))
        render_mode = RENDER_WIREFRAME;
    else if(strstr(mode_raw, "SOLID"))
        render_mode = RENDER_SOLID;
    else if(strstr(mode_raw, "COMBINED"))
        render_mode = RENDER_COMBINED;
    else{
        printf("\tdefault-render-mode is wrongly configured, resorting to default . . .\n");
    }

    if(toml_rtod(wire_raw, &wire_width) == -1){
        printf("\tdefault-wireframe-width is wrongly configured, resorting to default . . .\n");
        wire_width = 1;
    }else{
        if(wire_width > 10)
            wire_width = 10;
        if(wire_width <= 0.1)
            wire_width = 0.1;
    }
}
// Configura la primera vez que se ejecuta el programa
void first_config(toml_table_t *conf){
    toml_table_t *first = toml_table_in(conf, "FIRST-TIME");
    const char  *is_first = toml_raw_in(first, "first-run"),
                *cam_pos_raw = toml_raw_in(first, "first-camera-pos");
    int isFirstFlag = 0;
    if(toml_rtob(is_first, &isFirstFlag) == -1){
        printf("\trender-scenes is wrongly configured, resorting to default . . .\n");
        isFirstFlag = 0;
    }

    if(isFirstFlag){
        // Set flag to false for next executions
        replace_line();
        point3 aux_point = get_point3(first, "first-camera-pos");
        displace_camera(_first_camera, -_first_camera->pos.x, -_first_camera->pos.y, -_first_camera->pos.z); 
        displace_camera(_first_camera, aux_point.x, aux_point.y, aux_point.z);
        render_static = 1;
    }
} 
// Configura algun parametro extra
void fun_config(toml_table_t *conf){
    printf("FUN\n");
    toml_table_t *fun = toml_table_in(conf, "FUN");
    const char  *rainbow_raw = toml_raw_in(fun, "selected-rainbow-material");
    if(toml_rtob(rainbow_raw, &selected_rainbow_mat) == -1){
        printf("\tselected-rainbow-material is wrongly configured, resorting to default . . .\n");
        selected_rainbow_mat = 0;
    }
}
// Configura la carga rápida de objeto
void quick_config(toml_table_t *conf){
    printf("QUICKLOAD\n");
    toml_table_t *quick = toml_table_in(conf, "QUICKLOAD");
    const char  *start_raw = toml_raw_in(quick, "start-quickload"),
                *path_raw = toml_raw_in(quick, "default-quickload-object-path");
    int aux = 0;
    if(toml_rtob(start_raw, &aux) == -1){
        printf("\tstart-quickload is wrongly configured, resorting to default . . .\n");
    }
    if(toml_rtos(path_raw, &quickload_path) == -1){
        printf("\tdefault-quickload-object-path is wrongly configured, resorting to default . . .\n");
        quickload_path = "objektuak/abioia.obj";
    }
    point3 pos = get_point3(quick, "default-quickload-first-object-position");
    point3 scale = get_point3(quick, "default-quickload-first-object-scale");

    if(aux){
        object3d *temp = cargar_objeto(quickload_path);
        displace_selected_object(_first_object, GLOBAL_MODE, pos.x, pos.y, pos.z);
        scale_selected_object(_first_object, GLOBAL_MODE, scale.x,scale.y,scale.z);
    }
}
// Configura la iluminacion
void light_config(toml_table_t *conf){
    printf("LIGHTING\n");
    toml_table_t *light = toml_table_in(conf, "LIGHTING");
    const char  *on_raw = toml_raw_in(light, "default-on"),
                *shading_raw = toml_raw_in(light, "default-shading");

    if(toml_rtob(on_raw, &lightning_mode) == -1){
        printf("\tdefault-on is wrongly configured, resorting to default . . .\n");
        lightning_mode = LIGHTNING_OFF;
    }

    if(strstr(shading_raw ,"SMOOTH"))
        shading_mode = SHADING_SMOOTH;
    else if(strstr(shading_raw, "FLAT"))
        shading_mode = SHADING_FLAT;
    else{
        printf("\tdefault-shading is wrongly configured, resorting to default . . .\n");
    }

    toml_array_t *aux = toml_array_in(light, "default-lights-on");
    int length = toml_array_nelem(aux);
    
    for(int i = 0; i < length; i++){
        const char *test = toml_raw_at( aux, i);
        int light_index = atoi(test);
        if(light_index > 0 && light_index <= 8){
            lights[light_index - 1] ->is_on = 1;
            glEnable(0x4000 + light_index - 1);
        }
        redraw = 1;
    }


}

void mouse_config(toml_table_t *conf){
    printf("MOUSE\n");
    toml_table_t *mouse = toml_table_in(conf, "MOUSE");
    const char  *sens_raw = toml_raw_in(mouse, "starting-sensitivity"),
                *x_raw = toml_raw_in(mouse, "axis-x-inverted"),
                *y_raw = toml_raw_in(mouse, "axis-y-inverted");

    if(toml_rtod(sens_raw, &mouse_sensitivity) == -1){
        printf("\tstarting-sensitivity is wrongly configured, resorting to default . . .\n");
        mouse_sensitivity = 10;
    }else{
        if(mouse_sensitivity > 20)
            mouse_sensitivity = 20;
        if(mouse_sensitivity <= 0.25)
            mouse_sensitivity = 0.25;
    }

    if(toml_rtob(x_raw, &mouse_inverted_X) == -1){
        printf("\taxis-x-inverted is wrongly configured, resorting to default . . .\n");
        mouse_inverted_X = 1;
    }else{
        if(mouse_inverted_X == 1)
            mouse_inverted_X = -1;
    }
    if(mouse_inverted_X == 0)
        mouse_inverted_X = 1;

    if(toml_rtob(y_raw, &mouse_inverted_Y) == -1){
        printf("\taxis-y-inverted is wrongly configured, resorting to default . . .\n");
        mouse_inverted_Y = 1;
    }else{
        if(mouse_inverted_Y == 1)
            mouse_inverted_Y = -1;
    }
    if(mouse_inverted_Y == 0)
        mouse_inverted_Y = 1;

}

// Lee el archivo config.toml y llama a los métodos de configuracion
char* read_config(){
    FILE *fp;
    char *FNAME = "config.toml";
    char errbuf[200];
    //Parse config.toml file
    if (0 == (fp = fopen(FNAME, "r"))) {
	    return "File not recognised";
    }
    toml_table_t *conf = toml_parse_file( fp, errbuf, sizeof(errbuf));
    fclose(fp);
    if (0 == conf) {
        printf("%s\n",errbuf);
	    return "Error in %s",FNAME;
    }
    mode_config(conf);
    ui_config(conf);
    render_config(conf);
    first_config(conf);
    fun_config(conf);
    quick_config(conf);
    light_config(conf);
    mouse_config(conf);
    //Free memory from conf table
    toml_free(conf);
    return "Config read correctly";
}

