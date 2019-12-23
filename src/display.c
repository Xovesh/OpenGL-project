#include "definitions.h"
#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include "math_util.h"
#include "util.h"

#include "global_data_extern.h"

/**
 * @deprecated
 * @brief Function to draw the axes
 */
void draw_axes(){
    /*Draw X axis*/
    glColor3f(KG_COL_X_AXIS_R,KG_COL_X_AXIS_G,KG_COL_X_AXIS_B);
    glBegin(GL_LINES);
    glVertex3d(KG_MAX_DOUBLE,0,0);
    glVertex3d(-1*KG_MAX_DOUBLE,0,0);
    glEnd();
    /*Draw Y axis*/
    glColor3f(KG_COL_Y_AXIS_R,KG_COL_Y_AXIS_G,KG_COL_Y_AXIS_B);
    glBegin(GL_LINES);
    glVertex3d(0,KG_MAX_DOUBLE,0);
    glVertex3d(0,-1*KG_MAX_DOUBLE,0);
    glEnd();
    /*Draw Z axis*/
    glColor3f(KG_COL_Z_AXIS_R,KG_COL_Z_AXIS_G,KG_COL_Z_AXIS_B);
    glBegin(GL_LINES);
    glVertex3d(0,0,KG_MAX_DOUBLE);
    glVertex3d(0,0,-1*KG_MAX_DOUBLE);
    glEnd();
}

/**
 * @brief Callback reshape function. We just store the new proportions of the window
 * @param width New width of the window
 * @param height New height of the window
 */
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    /*  Take care, the width and height are integer numbers, but the ratio is a GLdouble so, in order to avoid
     *  rounding the ratio to integer values we need to cast width and height before computing the ratio */
    _window_ratio = (GLdouble) width / (GLdouble) height;
}


// Funcion que renderiza la iluminacion de un foco
void render_spotlight(int index, int isCameraSpot){
    if(lights[index]->mode != SPOT)
        return; 
    glMatrixMode(GL_MODELVIEW);
    int light_define = 0x4000 + index;
    GLfloat aux_zero[] = {0, 0, 0, 1};
    GLfloat aux_vec4[] = {lights[index]->pos.x, lights[index]->pos.y, lights[index]->pos.z, 1};
    GLfloat aux_vec3[] = {lights[index]->spot_direction.x, lights[index]->spot_direction.y, lights[index]->spot_direction.z};
    GLfloat dif_col[]  = {lights[index]->diffuse.r, lights[index]->diffuse.g, lights[index]->diffuse.b};
    GLfloat amb_col[]  = {lights[index]->ambient.r, lights[index]->ambient.g, lights[index]->ambient.b};
    GLfloat spe_col[]  = {lights[index]->specular.r, lights[index]->specular.g, lights[index]->specular.b};

    glPushMatrix();
    if(!isCameraSpot){
        if(index == 2){
            // Foco con objeto
            if (_selected_object != NULL){
                glMultMatrixf(_selected_object->transformation_history->mat);
                aux_zero[2] = _selected_object->max.z;
                glLightfv(light_define, GL_POSITION, aux_zero);
            }else{
                glPopMatrix();
                return;
            }
        }else{
            // Foco sin objeto
            glMultMatrixf(lights[index]->mat);
            glLightfv(light_define, GL_POSITION, aux_zero);
        }
    }else{
        // Foco Camara
        glLoadIdentity();
        glLightfv(light_define, GL_POSITION, aux_zero);     
    }
    glLightfv(light_define, GL_SPOT_DIRECTION, aux_vec3);
    glLightfv(light_define, GL_DIFFUSE, dif_col);
    glLightfv(light_define, GL_AMBIENT, amb_col);
    glLightfv(light_define, GL_SPECULAR, spe_col);
    glLightf(light_define, GL_SPOT_CUTOFF, lights[index]->amplitude);
    glLightf(light_define, GL_SPOT_EXPONENT,lights[index]->spot_exponent);
    glPopMatrix();
}

// Funcion que renderiza la iluminacion del sol
void render_sun(int index){
    if(lights[index]->mode != SUN)
        return;

    glMatrixMode(GL_MODELVIEW); 
    int light_define = 0x4000 + index;
    GLfloat aux_vec4[] = {lights[index]->pos.x, lights[index]->pos.y, lights[index]->pos.z, 0};
    GLfloat aux_vec3[] = {lights[index]->spot_direction.x, lights[index]->spot_direction.y, lights[index]->spot_direction.z};
    GLfloat dif_col[] = {lights[index]->diffuse.r, lights[index]->diffuse.g, lights[index]->diffuse.b};
    GLfloat amb_col[] = {lights[index]->ambient.r, lights[index]->ambient.g, lights[index]->ambient.b};
    GLfloat spe_col[] = {lights[index]->specular.r, lights[index]->specular.g, lights[index]->specular.b};
    
    glLightfv(light_define, GL_POSITION, aux_vec4);
    // glLightfv(light_define, GL_SPOT_DIRECTION, aux_vec3);
    glLightfv(light_define, GL_DIFFUSE, dif_col);
    glLightfv(light_define, GL_AMBIENT, amb_col);
    glLightfv(light_define, GL_SPECULAR, spe_col);
}

// Funcion que renderiza la iluminacion de una bombilla
void render_pointlight(int index){
    if(lights[index]->mode != POINT)
        return;
    glMatrixMode(GL_MODELVIEW);

    int light_define = 0x4000 + index;
    GLfloat aux_vec4[] = {lights[index]->pos.x, lights[index]->pos.y, lights[index]->pos.z, 1};
    GLfloat dif_col[] = {lights[index]->diffuse.r, lights[index]->diffuse.g, lights[index]->diffuse.b};
    GLfloat amb_col[] = {lights[index]->ambient.r, lights[index]->ambient.g, lights[index]->ambient.b};
    GLfloat spe_col[] = {lights[index]->specular.r, lights[index]->specular.g, lights[index]->specular.b};
    
    glLightfv(light_define, GL_POSITION, aux_vec4);
    glLightfv(light_define, GL_DIFFUSE, dif_col);
    glLightfv(light_define, GL_AMBIENT, amb_col);
    glLightfv(light_define, GL_SPECULAR, spe_col);
}

// Funcion con la iluminacion que es fija
void ilum_compute_fixed(){
    // General setup
    // Camera spotlight
    glMatrixMode(GL_MODELVIEW);
    if(lights[3]->is_on){
       render_spotlight(3,1);
    }
    //Sun
    glLoadMatrixf(_selected_camera->mat);
    if(lights[0]->is_on){
        render_sun(0);
    }
    //Point
    if(lights[1]->is_on){
        render_pointlight(1);
    }
}

// Funcion con la iluminacion que se puede alterar (excepto object spotlight)
void ilum_compute_dyn(){
    /*Suns*/
    for (int i = 4; i < 8; i++){
        if(lights[i]->mode == SUN && lights[i]->is_on)
            render_sun(i);
    }
    /*Points*/
    for (int i = 4; i < 8; i++){
        if(lights[i]->mode == POINT && lights[i]->is_on)
            render_pointlight(i);
    }
    
    /* Object Spotlight*/
    if(lights[2]->is_on){
       render_spotlight(2,0);
    }

    /*Spotlight*/
    // @DEPRECATED debido a un error que no hemos conseguido determinar con las luces dinámicas
    // for (int i = 4; i < 8; i++){
    //     if(lights[i]->mode == SPOT && lights[i]->is_on)
    //         render_spotlight( i, 0);
    // }
    
}

// Funcion que computa la iluminacion
void ilum_compute(){
    ilum_compute_fixed();
    ilum_compute_dyn();
}

/**
 * @brief Callback display function
 */
void display(void) {
    GLint v_index, v, f;
    object3d *aux_obj ;
    /* Clear the screen */
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);

    /* Define the projection */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(_selected_camera->projection_mode == ORTHOGONAL_MODE){
        glOrtho(_selected_camera->ortho_left,_selected_camera->ortho_right,
                _selected_camera->ortho_bottom,_selected_camera->ortho_top,
                _selected_camera->ortho_near,_selected_camera->ortho_far);
    }else{
        if(_selected_camera->persp_near < _selected_camera->persp_far && (_selected_camera->persp_near > 0 &&  _selected_camera->persp_far > 0)){
            glFrustum(_selected_camera->persp_left,_selected_camera->persp_right,
                      _selected_camera->persp_bottom,_selected_camera->persp_top,
                      _selected_camera->persp_near,_selected_camera->persp_far);
        }else{
            glFrustum(_selected_camera->persp_left,_selected_camera->persp_right,
                      _selected_camera->persp_bottom,_selected_camera->persp_top,
                      0.1,100);
        }
    }

    /* Now we start drawing the object */
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(_selected_camera->mat);
    glDisable(GL_LIGHTING);

    /*Ilumniation*/
    if(lightning_mode == LIGHTNING_ON){
        glEnable(GL_LIGHTING);
        ilum_compute();
    }

    /*Render Static objects*/
    if(render_static){
        /*Static Objects*/
        aux_obj = _selected_static;
            /* Select the color, depending on whether the current object is the selected one or not */
            glPushMatrix();
            /* Draw the object; for each face create a new polygon with the corresponding vertices */
            glMultMatrixf(aux_obj->transformation_history->mat);
            for (f = 0; f < aux_obj->num_faces; f++) {
                
                if(render_mode == RENDER_COMBINED || render_mode == RENDER_SOLID){

                    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
                    glBegin(GL_POLYGON);
                    if(shading_mode == SHADING_FLAT)
                        glNormal3d( aux_obj->face_table[f].normal_vec.x,
                                    aux_obj->face_table[f].normal_vec.y,
                                    aux_obj->face_table[f].normal_vec.z);

                    if(lightning_mode == LIGHTNING_ON){
                        GLfloat dif_color[] = {aux_obj->face_table[f].material->kd.r,
                                             aux_obj->face_table[f].material->kd.g,
                                             aux_obj->face_table[f].material->kd.b, 1};
                        GLfloat spe_color[] = {aux_obj->face_table[f].material->ks.r,
                                              aux_obj->face_table[f].material->ks.g,
                                              aux_obj->face_table[f].material->ks.b, 1};
                        GLfloat amb_color[] = {aux_obj->face_table[f].material->ka.r,
                                             aux_obj->face_table[f].material->ka.g,
                                             aux_obj->face_table[f].material->ka.b, 1};
                        glMaterialfv(GL_FRONT, GL_DIFFUSE, dif_color);
                        glMaterialfv(GL_FRONT, GL_SPECULAR, spe_color);
                        glMaterialfv(GL_FRONT, GL_AMBIENT, amb_color);
                    }else
                        glColor3f( aux_obj->face_table[f].material->kd.r, aux_obj->face_table[f].material->kd.g, aux_obj->face_table[f].material->kd.b);  

                    for (v = 0; v < aux_obj->face_table[f].num_vertices; v++) {
                        v_index = aux_obj->face_table[f].vertex_table[v];
                        if(shading_mode == SHADING_SMOOTH)
                            glNormal3d( aux_obj->vertex_table[v_index].normal.x,
                                        aux_obj->vertex_table[v_index].normal.y,
                                        aux_obj->vertex_table[v_index].normal.z);
                        glVertex3d(aux_obj->vertex_table[v_index].coord.x,
                            aux_obj->vertex_table[v_index].coord.y,
                            aux_obj->vertex_table[v_index].coord.z);

                    }
                    glEnd();
                }
                if(render_mode == RENDER_COMBINED || render_mode == RENDER_WIREFRAME){
                    if(render_mode == RENDER_COMBINED)
                        glDisable(GL_LIGHTING);

                    glPolygonMode(GL_FRONT,GL_LINE);
                    glLineWidth(wire_width);   
                    glBegin(GL_POLYGON);
                    glNormal3d( aux_obj->face_table[f].normal_vec.x,
                                aux_obj->face_table[f].normal_vec.y,
                                aux_obj->face_table[f].normal_vec.z);
                    glColor3f( aux_obj->face_table[f].material->kd.r, aux_obj->face_table[f].material->kd.g, aux_obj->face_table[f].material->kd.b);    
                    if(render_mode == RENDER_COMBINED)
                        glColor3f( 0, 0, 0);
                    for (v = 0; v < aux_obj->face_table[f].num_vertices; v++) {
                        v_index = aux_obj->face_table[f].vertex_table[v];
                        glVertex3d(aux_obj->vertex_table[v_index].coord.x,
                            aux_obj->vertex_table[v_index].coord.y,
                            aux_obj->vertex_table[v_index].coord.z);

                    }
                    glEnd();
                    if(render_mode == RENDER_COMBINED && lightning_mode == LIGHTNING_ON)
                        glEnable(GL_LIGHTING);
                }

            }
            glPopMatrix();
    }
    
    // Objetos que se pueden transformar
    /*Now each of the objects in the list*/
    aux_obj =  _first_object;
    
    GLfloat dif_col_selected[] = {KG_COL_SELECTED_R,KG_COL_SELECTED_G,KG_COL_SELECTED_B};
    GLfloat dif_col_nonselected[] = {KG_COL_NONSELECTED_R,KG_COL_NONSELECTED_G,KG_COL_NONSELECTED_B};
    while (aux_obj != NULL) {
        glPushMatrix();
        /* Draw the object; for each face create a new polygon with the corresponding vertices */
        glMultMatrixf(aux_obj->transformation_history->mat);
        for (f = 0; f < aux_obj->num_faces; f++) {

            if(render_mode == RENDER_COMBINED || render_mode == RENDER_SOLID){
                glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
                glBegin(GL_POLYGON);
                if(shading_mode == SHADING_FLAT)
                    glNormal3d(aux_obj->face_table[f].normal_vec.x,aux_obj->face_table[f].normal_vec.y,aux_obj->face_table[f].normal_vec.z);
                if (aux_obj == _selected_object){
                    if(lightning_mode == LIGHTNING_ON){
                        if(selected_rainbow_mat){
                            color3 aux_col = get_rainbow_color_t();
                            GLfloat rainbow_col[] ={ aux_col.r,aux_col.g,aux_col.b};
                            glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,rainbow_col);
                        }else{
                            glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, dif_col_selected);
                        }
                    }else{
                        glColor3f(KG_COL_SELECTED_R,KG_COL_SELECTED_G,KG_COL_SELECTED_B);
                    }
                }else{
                    if(lightning_mode == LIGHTNING_ON){
                        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,dif_col_nonselected);
                    }else{
                        glColor3f(KG_COL_NONSELECTED_R,KG_COL_NONSELECTED_G,KG_COL_NONSELECTED_B);
                    }
                }

                for (v = 0; v < aux_obj->face_table[f].num_vertices; v++) {
                    v_index = aux_obj->face_table[f].vertex_table[v];
                    glVertex3d(aux_obj->vertex_table[v_index].coord.x,
                        aux_obj->vertex_table[v_index].coord.y,
                        aux_obj->vertex_table[v_index].coord.z);
                    if(shading_mode == SHADING_SMOOTH)
                        glNormal3d(aux_obj->vertex_table[v_index].normal.x,aux_obj->vertex_table[v_index].normal.y,aux_obj->vertex_table[v_index].normal.z);
                }
                glEnd();
            }

            if(render_mode == RENDER_COMBINED || render_mode == RENDER_WIREFRAME){
                if(render_mode == RENDER_COMBINED)
                    glDisable(GL_LIGHTING);
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
                glLineWidth(wire_width);
                glBegin(GL_POLYGON);
                if (aux_obj == _selected_object){
                    glColor3f(KG_COL_SELECTED_R,KG_COL_SELECTED_G,KG_COL_SELECTED_B);
                }else{
                    glColor3f(KG_COL_NONSELECTED_R,KG_COL_NONSELECTED_G,KG_COL_NONSELECTED_B);
                }
                if(render_mode == RENDER_COMBINED)
                    glColor3f( 0, 0, 0);
                for (v = 0; v < aux_obj->face_table[f].num_vertices; v++) {
                    v_index = aux_obj->face_table[f].vertex_table[v];
                    glVertex3d(aux_obj->vertex_table[v_index].coord.x,
                        aux_obj->vertex_table[v_index].coord.y,
                        aux_obj->vertex_table[v_index].coord.z);
                }
                glEnd();
                if(render_mode == RENDER_COMBINED && lightning_mode == LIGHTNING_ON)
                    glEnable(GL_LIGHTING);
            }
        }

        /*Display collision sphere*/
        // glTranslatef( aux_obj->origin_pos.x, aux_obj->origin_pos.y, aux_obj->origin_pos.z);
        // GLUquadric *quad = gluNewQuadric();
        // glColor3f(255,0,0);
        // float radius_mult = (aux_obj->scale[0] > aux_obj->scale[1])?
        //                         ((aux_obj->scale[0] > aux_obj->scale[2])?aux_obj->scale[0]:aux_obj->scale[2])
        //                     :((aux_obj->scale[1] > aux_obj->scale[2])?aux_obj->scale[1]:aux_obj->scale[2]);
        // gluSphere(quad,aux_obj->bound_radius * radius_mult,32,32);
        // free(quad);

        glPopMatrix();
        aux_obj = aux_obj->next;
    }
    
    // Mostramos los ejes
    aux_obj = axes;
    if(show_axes){
        glViewport(0,0, glutGet(GLUT_WINDOW_WIDTH) * axes_percentage, glutGet(GLUT_WINDOW_HEIGHT) * axes_percentage);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glDisable(GL_CULL_FACE);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        float aux[16];
        glLoadIdentity();
        glMultMatrixf(axes->transformation_history->mat);
        glMultMatrixf(_selected_camera->mat);
        glGetFloatv(GL_MODELVIEW_MATRIX,aux);
        aux[12] = 0;
        aux[13] = 0;
        aux[14] = 0;
        glLoadMatrixf(aux);
        for (f = 0; f < aux_obj->num_faces; f++) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glBegin(GL_POLYGON);
            glColor3f( aux_obj->face_table[f].material->kd.r, aux_obj->face_table[f].material->kd.g, aux_obj->face_table[f].material->kd.b);
            for (v = 0; v < aux_obj->face_table[f].num_vertices; v++) {
                v_index = aux_obj->face_table[f].vertex_table[v];
                glVertex3d(aux_obj->vertex_table[v_index].coord.x,
                    aux_obj->vertex_table[v_index].coord.y,
                    aux_obj->vertex_table[v_index].coord.z);
            }
            glEnd();

        }
        glViewport(0,0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        if(lightning_mode == LIGHTNING_ON)
            glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE); 
    }

    redraw = 0;
    /*Do the actual drawing*/
    glutSwapBuffers();
}
