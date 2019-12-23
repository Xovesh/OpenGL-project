#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdbool.h>
#include <GL/gl.h>
/** DEFINITIONS **/

#define KG_WINDOW_TITLE                     "Practica GP2 Carlos Dominguez y Markel Cortazar"
#define KG_WINDOW_WIDTH                     600
#define KG_WINDOW_HEIGHT                    400
#define KG_WINDOW_X                         50
#define KG_WINDOW_Y                         50

#define KG_MSSG_SELECT_FILE                 "Idatz ezazu fitxategiaren path-a: "
#define KG_MSSG_FILENOTFOUND                "Fitxategi hori ez da existitzen!!"
#define KG_MSSG_INVALIDFILE                 "Arazoren bat egon da fitxategiarekin ..."
#define KG_MSSG_EMPTYFILE                   "Fitxategia hutsik dago"
#define KG_MSSG_FILEREAD                    "Fitxategiaren irakurketa buruta"

#define KG_STEP_MOVE                        5.0f
#define KG_STEP_ROTATE                      10.0f
#define KG_STEP_ZOOM                        0.75
#define KG_STEP_CAMERA_ANGLE                5.0f

#define KG_ORTHO_X_MIN_INIT                -1
#define KG_ORTHO_X_MAX_INIT                 1
#define KG_ORTHO_Y_MIN_INIT                -1
#define KG_ORTHO_Y_MAX_INIT                 1
#define KG_ORTHO_Z_MIN_INIT                 0
#define KG_ORTHO_Z_MAX_INIT                 100

#define KG_PERSP_X_MIN_INIT                -0.1
#define KG_PERSP_X_MAX_INIT                 0.1
#define KG_PERSP_Y_MIN_INIT                -0.1
#define KG_PERSP_Y_MAX_INIT                 0.1
#define KG_PERSP_Z_MIN_INIT                 0.1
#define KG_PERSP_Z_MAX_INIT                 100

#define KG_COL_BACK_R                       0.30f
#define KG_COL_BACK_G                       0.30f
#define KG_COL_BACK_B                       0.30f
#define KG_COL_BACK_A                       1.00f

#define KG_COL_SELECTED_R                   1.00f
#define KG_COL_SELECTED_G                   0.75f
#define KG_COL_SELECTED_B                   0.00f

#define KG_COL_NONSELECTED_R                1.00f
#define KG_COL_NONSELECTED_G                1.00f
#define KG_COL_NONSELECTED_B                1.00f

#define KG_COL_X_AXIS_R                     1.0f
#define KG_COL_X_AXIS_G                     0.0f
#define KG_COL_X_AXIS_B                     0.0f

#define KG_COL_Y_AXIS_R                     0.0f
#define KG_COL_Y_AXIS_G                     1.0f
#define KG_COL_Y_AXIS_B                     0.0f

#define KG_COL_Z_AXIS_R                     0.0f
#define KG_COL_Z_AXIS_G                     0.0f
#define KG_COL_Z_AXIS_B                     1.0f

#define KG_MAX_DOUBLE                       10E25

#define TRANSLATION_MODE                    0
#define ROTATION_MODE                       1
#define SCALE_MODE                          2
#define VELOCITY_MODE                       3

#define GLOBAL_MODE                         0
#define LOCAL_MODE                          1

#define CAMERA_MODE                         0
#define OBJECT_MODE                         1
#define ILUM_MODE                           2

#define CAMERA_ANALISIS_MODE                0
#define CAMERA_FREECAM_MODE                 1   

#define PERSPECTIVE_MODE                    0
#define ORTHOGONAL_MODE                     1

#define UPDATES_PER_SECOND                  120.0f

#define DEFAULT_ROTATION_DEGREES            5.0f
#define DEFAULT_DISPLACEMENT_DISTANCE       1.0f
#define DEFAULT_CAMERA_VOLUME_SCALE_ORTHO   1.0f
#define DEFAULT_CAMERA_VOLUME_SCALE_PERSP   0.01f
#define DEFAULT_OBJECT_SCALE_RATIO          0.2f
#define DEFAULT_OBJECT_SPEED                0.2f

#define RENDER_MODES                        3
#define RENDER_WIREFRAME                    0
#define RENDER_SOLID                        1
#define RENDER_COMBINED                     2

#define LIGHTNING_OFF                       0
#define LIGHTNING_ON                        1

#define SHADING_SMOOTH                      0
#define SHADING_FLAT                        1

#define MAX_WIREFRAME_WIDTH                 10.0f
#define MIN_WIREFRAME_WIDTH                 1.0f

#define GUI_UPDATE_OBJ                      1
#define GUI_UPDATE_STC                      2
#define GUI_UPDATE_CAM                      3

/** STRUCTURES **/

/****************************
 * Structure to store the   *
 * coordinates of 3D points *
 ****************************/
typedef struct {
    GLdouble x, y, z;
} point3;

/*****************************
 * Structure to store the    *
 * coordinates of 3D vectors *
 *****************************/
typedef struct {
    GLdouble x, y, z;
} vector3;

/****************************
 * Structure to store the   *
 * colors in RGB mode       *
 ****************************/
typedef struct {
    GLdouble r, g, b;
} color3;

struct mtl{
    char name[80];
    color3 kd;      /*Difuse color*/
    color3 ks;      /*Specular color*/
    color3 ke;      /*Emission color*/
    color3 ka;      /*Ambient color*/
    struct mtl *next;
};
typedef struct mtl mtl;

/****************************
 * Structure to store       *
 * objects' vertices         *
 ****************************/
typedef struct {
    point3 coord;                       /* coordinates,x, y, z */
    GLint num_faces;                    /* number of faces that share this vertex */
    vector3 normal;
} vertex;

/****************************
 * Structure to store       *
 * objects' faces or        *
 * polygons                 *
 ****************************/
typedef struct {
    GLint num_vertices;                 /* number of vertices in the face */
    vector3 normal_vec;                 /*Normal vector of the face*/
    mtl *material;
    GLint *vertex_table;                /* table with the index of each vertex */
} face;

struct objetoidname{
    char text[80];
};

struct transformation_matrix{
    GLfloat mat[16];
    struct transformation_matrix *next;
};
typedef struct transformation_matrix transformation_matrix;

struct bounding_sphere{
    point3 center;
    float radius;
};
typedef struct bounding_sphere bounding_sphere;
/****************************
 * Structure to store a     *
 * pile of 3D objects       *
 ****************************/
struct object3d{
    
    GLint num_vertices;                             /* number of vertices in the object*/
    vertex *vertex_table;                           /* table of vertices */
    GLint num_faces;                                /* number of faces in the object */
    face *face_table;                               /* table of faces */
    
    GLfloat scale[3];                               /*Representation of scale of the object*/
    point3 min;                                     /* coordinates' lower bounds */
    point3 max;                                     /* coordinates' bigger bounds */
    GLfloat bound_radius;                           /*Radio de la esfera de colision*/
    int has_collided;
    
    point3 origin_pos;                              /*Position of the origin of the object*/
    vector3 velocity;                               /*Vector de desplazamiento del objeto*/
    vector3 acceleration;                           /*Vector de acceleracion del objeto*/

    void (*onStart)(struct object3d *obj);          /*Funcion a ejectuar al crear el objeto*/
    void (*onUpdate)(struct object3d *obj);         /*Función a ejecutar en actualizar*/
    void (*onCollideSelf)(struct object3d *obj);    /*Función a ejecutar sobre si mismo al colisionar*/
    void (*onCollideOther)(struct object3d *obj);   /*Funcion a ejecutar sobre el objeto con el que se ha colisionado*/
    void (*onNoCollision)(struct object3d *obj);    /*Funcion a ejecutar sobre el objeto si no hay colision*/
    void (*onDispose)(struct object3d *obj);    /*Funcion a ejecutar sobre el objeto si no hay colision*/
    GLint ttl;                                      /*Tiempo de vida del objeto*/
    int is_selectable;
    struct object3d *next;                          /* next element in the pile of objects */
    struct transformation_matrix *transformation_history;  /*History of transformations applied to this object*/
};
typedef struct object3d object3d;

struct object3d_container{
    object3d *obj;
    struct object3d_container *next;
};
typedef struct object3d_container object3d_container;

struct camera3d{
    //Eye, target and up
    point3 pos;
    point3 attention_point;
    vector3 verticality_vec;

    //Matrix
    GLfloat mat[16];
    
    //Access to next camera
    struct camera3d *next;

    //Projection data
    int projection_mode;

    GLdouble ortho_left;
    GLdouble ortho_right;
    GLdouble ortho_top;
    GLdouble ortho_bottom;
    GLdouble ortho_far;
    GLdouble ortho_near;

    GLdouble persp_left;
    GLdouble persp_right;
    GLdouble persp_top;
    GLdouble persp_bottom;
    GLdouble persp_far;
    GLdouble persp_near;

    //Following objects
    object3d *parent;
    object3d_container *children;
};
typedef struct camera3d camera3d;


/*GUI*/
typedef struct {
    int argc;
    char **argv;
} GUI;

enum ilum_type{
    POINT,
    SUN,
    SPOT,
};
typedef enum ilum_type ilum_type;

typedef struct {
    point3 pos;
    GLfloat mat[16];
    
    int is_on;
    ilum_type mode;
    
    color3 ambient;
    color3 diffuse;
    color3 specular;

    GLfloat spot_exponent;
    GLfloat amplitude;
    vector3 spot_direction;

    object3d *parent_obj;
}ilum;

#endif // DEFINITIONS_H
