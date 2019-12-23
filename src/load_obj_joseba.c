#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <malloc.h>
#include <GL/glut.h>
#include "definitions.h"
#include "object_functions.h"
#include "math_util.h"

#define MAXLINE 200

/*
 * Auxiliar function to process each line of the file
 */
static int sreadint(char *lerroa, int *zenbakiak)
{
    char *s = lerroa;
    int i, zbk, kont = 0;

    while (sscanf(s, " %d%n", &zbk, &i) > 0)
    {
        s += i;
        zenbakiak[kont++] = zbk;
    }
    return (kont);
}

//Funcion que inicializa la matriz del objeto

static void gen_initial_transfortion_matrix(object3d *objectptr)
{
    transformation_matrix *trans_mat;
    trans_mat = malloc(sizeof(transformation_matrix));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, trans_mat->mat);
    trans_mat->next = NULL;
    objectptr->transformation_history = trans_mat;
}
/*
 * Auxiliar function to process each line of the file
 */
static int sreadint2(char *lerroa, int *zenbakiak)
{
    char *s = lerroa;
    int i, zbk, kont = 0;

    while (sscanf(s, " %d%n", &zbk, &i) > 0)
    {
        s += i;
        while ((*s != ' ') && (*s != '\0'))
            s++; // jump vector normal information
        zenbakiak[kont++] = zbk;
    }
    // printf("%d numbers in the line\n", kont);
    return (kont);
}

// Funcion que lee un archivo .mtl
mtl* read_mtl(char *file_name ){
    // printf("Empezando a leer material . . .\n");
    FILE *obj_file;
    color3 color;
    mtl *primer_material = NULL;
    char line[80], line_1[80], aux[45];

    if ((obj_file = fopen(file_name, "r")) == NULL){
        printf("No se ha encontrado material asociado a %s\n",file_name);
        return primer_material;
    }

    primer_material = malloc(sizeof(mtl));
    mtl *ultimo_material = primer_material;

    while (fscanf(obj_file, "\n%[^\n]", line) > 0){
        /*Ignore Comments*/
        if(!strcmp(&line[0],"#"))
            continue;
        
        /*Read First material*/
        if(line[0] == 'n'){

            mtl *aux = malloc(sizeof(mtl));
            ultimo_material->next = (mtl *)aux;
            ultimo_material = aux;

            strcpy(aux->name,&line[7]);
            continue;
        }
        /*Leer Difuse*/
        if(line[1] == 'd'){
            sscanf(line + 2, "%lf%lf%lf\n", &(ultimo_material->kd.r), &(ultimo_material->kd.g), &(ultimo_material->kd.b));
        }
        /*Leer ambient*/
        if(line[1] == 'a'){
            sscanf(line + 2, "%lf%lf%lf\n", &(ultimo_material->ka.r), &(ultimo_material->ka.g), &(ultimo_material->ka.b));
        }
        /*Leer Specular*/
        if(line[0] == 'K' && line[1] == 's'){
            sscanf(line + 2, "%lf%lf%lf\n", &(ultimo_material->ks.r), &(ultimo_material->ks.g), &(ultimo_material->ks.b));
        }
        /*Leer Emission*/
        if(line[1] == 'e'){
            sscanf(line + 2, "%lf%lf%lf\n", &(ultimo_material->ke.r), &(ultimo_material->ke.g), &(ultimo_material->ke.b));
        }


    }
    return primer_material->next;
}

/**
 * @brief Function to read wavefront files (*.obj)
 * @param file_name Path of the file to be read
 * @param object_ptr Pointer of the object3d type structure where the data will be stored
 * @return Result of the reading: 0=Read ok, 1=File not found, 2=Invalid file, 3=Empty file
 */
int read_wavefront(char *file_name, object3d *object_ptr)
{
    vertex *vertex_table;
    face *face_table;
    int num_vertices = -1, num_faces = -1, count_vertices = 0, count_faces = 0;
    FILE *obj_file;
    char line[MAXLINE], line_1[MAXLINE], aux[45];
    int k;
    int i, j;
    int values[MAXLINE];
    vector3 aux_vector3 = {0, 0, 0};
    mtl *material_list;
    int has_mtl_flag = 0;
    
    mtl *default_mat = malloc(sizeof(mtl));
    default_mat->kd.r = (GLfloat) 129/255;
    default_mat->kd.g = (GLfloat) 156/255;
    default_mat->kd.b = (GLfloat) 135/255;

    default_mat->ka.r = 0;
    default_mat->ka.g = 0;
    default_mat->ka.b = 0;
    default_mat->ks.r = 0;
    default_mat->ks.g = 0;
    default_mat->ks.b = 0;
    default_mat->ke.r = 0;
    default_mat->ke.g = 0;
    default_mat->ke.b = 0;

    /*
     * The function reads twice the file. In the first read the number of
     * vertices and faces is obtained. Then, memory is allocated for each
     * of them and in the second read the actual information is read and
     * loaded. Finally, the object structure is created
     */
    if ((obj_file = fopen(file_name, "r")) == NULL)
        return (1);
    
    while (fscanf(obj_file, "\n%[^\n]", line) > 0)
    {
        
        /*Reading any posble corresponding .mtl file*/
        if(line[0] == 'm'){
            char buffer[80] = "scenes/";
            strcat(buffer,&line[7]);
            material_list = read_mtl(buffer);
            if(material_list != NULL)
                has_mtl_flag = 1;
        }

        i = 0;
        while (line[i] == ' ')
            i++;
        if ((line[0] == '#') && ((int)strlen(line) > 5))
        {
            i += 2;
            j = 0;
            while (line[i] != ' ')
                line_1[j++] = line[i++];
            i++;
            line_1[j] = '\0';
            j = 0;
            while ((line[i] != ' ') && (line[i] != '\0'))
                aux[j++] = line[i++];
            aux[j] = 0;
            if (strcmp(aux, "vertices") == 0)
            {
                num_vertices = atoi(line_1);
            }
            if (strncmp(aux, "elements", 7) == 0)
            {
                num_faces = atoi(line_1);
            }
        }
        else
        {
            if (strlen(line) > 6)
            {
                if (line[i] == 'f' && line[i + 1] == ' ')
                    count_faces++;
                else if (line[i] == 'v' && line[i + 1] == ' ')
                    count_vertices++;
            }
        }
    }
    fclose(obj_file);
    // printf("1 pasada: num vert = %d (%d), num faces = %d(%d) \n", num_vertices, count_vertices, num_faces, count_faces);
    if ((num_vertices != -1 && num_vertices != count_vertices) || (num_faces != -1 && num_faces != count_faces))
    {
        printf("WARNING: full file format: (%s)\n", file_name);
        //return (2);
    }
    if (num_vertices == 0 || count_vertices == 0)
    {
        printf("No vertex found: (%s)\n", file_name);
        return (3);
    }
    if (num_faces == 0 || count_faces == 0)
    {
        printf("No faces found: (%s)\n", file_name);
        return (3);
    }

    num_vertices = count_vertices;
    num_faces = count_faces;

    vertex_table = (vertex *)malloc(num_vertices * sizeof(vertex));
    face_table = (face *)malloc(num_faces * sizeof(face));

    obj_file = fopen(file_name, "r");
    k = 0;
    j = 0;

    for (i = 0; i < num_vertices; i++)
        vertex_table[i].num_faces = 0;

    mtl *active;

    while (fscanf(obj_file, "\n%[^\n]", line) > 0)
    {
        switch (line[0])
        {
        case 'u':
            if(!has_mtl_flag)
                break;
            char buff[80];
            strcpy(buff,&line[7]);
            active = material_list;
            while(strcmp(active->name,buff)){
                active = active->next;
            }

            break;
        case 'v':
            if (line[1] == ' ') // vn and vt not interested
            {
                sscanf(line + 2, "%lf%lf%lf", &(vertex_table[k].coord.x),
                       &(vertex_table[k].coord.y), &(vertex_table[k].coord.z));
                vector3 aux_init = { 0, 0, 0};
                vertex_table[k].normal = aux_init;
                k++;
            }
            // else if(line[1] == 'n'){
            //     sscanf(line + 2, "%lf%lf%lf", &(face_table[j].normal_vec.x),
            //            &(face_table[j].normal_vec.y), &(face_table[j].normal_vec.z));
            // }
            break;

        case 'f':
            if (line[1] == ' ') // fn not interested
            {
                for (i = 2; i <= (int)strlen(line); i++)
                    line_1[i - 2] = line[i];
                line_1[i - 2] = '\0';
                face_table[j].num_vertices = sreadint2(line_1, values);
                //printf("f %d vertices\n",face_table[j].num_vertices);
                face_table[j].vertex_table = (int *)malloc(face_table[j].num_vertices * sizeof(int));
                for (i = 0; i < face_table[j].num_vertices; i++)
                {
                    face_table[j].vertex_table[i] = values[i] - 1;
                    //printf(" %d ",values[i] - 1);
                    vertex_table[face_table[j].vertex_table[i]].num_faces++;
                }
                //Computing correspoding normal vector for given face j
                face_table[j].normal_vec = obtain_normal_3_point3(vertex_table[face_table[j].vertex_table[0]].coord,
                                                                    vertex_table[face_table[j].vertex_table[1]].coord,
                                                                    vertex_table[face_table[j].vertex_table[2]].coord);

                color3 col;
                if(has_mtl_flag)
                    face_table[j].material = active; 
                else
                    face_table[j].material =default_mat;
                
                j++;
            }
            break;
        }
    }

    fclose(obj_file);

    // Finished reading file, computing vertex normals
    for(j = 0; j < num_faces; j++){
        for(i = 0; i < face_table[j].num_vertices; i++){
            vertex_table[face_table[j].vertex_table[i]].normal = add_vector3(vertex_table[face_table[j].vertex_table[i]].normal, face_table[j].normal_vec);
        }
    }
    for (j = 0; j < num_vertices; j++)
    {
        vertex_table[j].normal = normalize_vector3(vertex_table[j].normal);
    }
    
    // printf("2 pasada\n");

    /*
     * Information read is introduced in the structure */

    object_ptr->vertex_table = vertex_table;
    object_ptr->face_table = face_table;
    object_ptr->num_vertices = num_vertices;
    object_ptr->num_faces = num_faces;
    gen_initial_transfortion_matrix(object_ptr);
    object_ptr->ttl = -1;
    object_ptr->acceleration = aux_vector3;
    object_ptr->velocity = aux_vector3;
    point3 aux_point = {0,0,0};
    object_ptr->origin_pos = aux_point;
    object_ptr->scale[0] = 1;
    object_ptr->scale[1] = 1;
    object_ptr->scale[2] = 1;
    object_ptr->has_collided = 0;
    object_ptr->is_selectable = 1;
    /*funciones*/
    object_ptr->onUpdate        =   &basic_OnUpdate;
    object_ptr->onStart         =   &no_op;
    object_ptr->onCollideSelf   =   &no_op;
    object_ptr->onCollideOther  =   &no_op;
    object_ptr->onNoCollision   =   &no_op;

    /*
     * The maximum and minimum coordinates are obtained **/
    object_ptr->max.x = object_ptr->vertex_table[0].coord.x;
    object_ptr->max.y = object_ptr->vertex_table[0].coord.y;
    object_ptr->max.z = object_ptr->vertex_table[0].coord.z;
    object_ptr->min.x = object_ptr->vertex_table[0].coord.x;
    object_ptr->min.y = object_ptr->vertex_table[0].coord.y;
    object_ptr->min.z = object_ptr->vertex_table[0].coord.z;

    for (i = 1; i < object_ptr->num_vertices; i++)
    {
        if (object_ptr->vertex_table[i].coord.x < object_ptr->min.x)
            object_ptr->min.x = object_ptr->vertex_table[i].coord.x;

        if (object_ptr->vertex_table[i].coord.y < object_ptr->min.y)
            object_ptr->min.y = object_ptr->vertex_table[i].coord.y;

        if (object_ptr->vertex_table[i].coord.z < object_ptr->min.z)
            object_ptr->min.z = object_ptr->vertex_table[i].coord.z;

        if (object_ptr->vertex_table[i].coord.x > object_ptr->max.x)
            object_ptr->max.x = object_ptr->vertex_table[i].coord.x;

        if (object_ptr->vertex_table[i].coord.y > object_ptr->max.y)
            object_ptr->max.y = object_ptr->vertex_table[i].coord.y;

        if (object_ptr->vertex_table[i].coord.z > object_ptr->max.z)
            object_ptr->max.z = object_ptr->vertex_table[i].coord.z;
    }
    point3 bound_center = { (object_ptr->max.x + object_ptr->min.x) / 2,
                            (object_ptr->max.y + object_ptr->min.y) / 2,
                            (object_ptr->max.z + object_ptr->min.z) / 2};
                            //0,0,0};
    // object_ptr->origin_pos = bound_center;
    float max_dist = 0,auxf = 0;
    for(i = 0; i < object_ptr->num_vertices; i++){
        auxf = distance_3d(object_ptr->vertex_table[i].coord,object_ptr->origin_pos);
        if(auxf > max_dist)
            max_dist = auxf;
    }
    object_ptr->bound_radius = max_dist;
    float aux_total = (object_ptr->max.x-object_ptr->min.x > object_ptr->max.y-object_ptr->min.y )?
                                ((object_ptr->max.x-object_ptr->min.x  > object_ptr->max.z-object_ptr->min.z)?object_ptr->max.x-object_ptr->min.x :object_ptr->max.z-object_ptr->min.z)
                            :((object_ptr->max.y-object_ptr->min.y > object_ptr->max.z-object_ptr->min.z)?object_ptr->max.y-object_ptr->min.y:object_ptr->max.z-object_ptr->min.z);;
    object_ptr->scale[0] = (object_ptr->max.x-object_ptr->min.x)/aux_total;
    object_ptr->scale[1] = (object_ptr->max.y-object_ptr->min.y)/aux_total;
    object_ptr->scale[2] = (object_ptr->max.z-object_ptr->min.z)/aux_total;
    //  sqrtf(pow(object_ptr->origin_pos.x-object_ptr->max.x,2) +
    //                                  pow(object_ptr->origin_pos.y-object_ptr->max.y,2) +
    //                                  pow(object_ptr->origin_pos.z-object_ptr->max.z,2));
    return (0);
}
