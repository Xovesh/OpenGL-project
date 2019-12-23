#include <math.h>
#include "definitions.h"
#include <stdio.h>

/*POINT*/

//Funcion que realiza la suma entre dos vectores de tipo point3

point3 add_point3(point3 a, point3 b){
    point3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
} 

//Funcion que calcula la norma entre dos puntos

float distance_3d(point3 a,point3 b){
    return sqrtf(pow(a.x-b.x,2) + pow(a.y-b.y,2) + pow(a.z-b.z,2));
}

//Funcion que inicializa una estructura del tipo point3

point3 create_point_3( float x, float y, float z){
    point3 res = {x, y, z};
    return res;
}

//Funcion que inicializa una estructura del tipo color3

color3 create_color3( float r, float g, float b){
    color3 result = { r, g, b};
    return result;
}
/*MATRIX*/

//Funcion que devuelve la traspuesta de la matriz

GLfloat* transpose_mat_4x4(GLfloat *mat){
     GLfloat aux_mat[16] = {
           mat[0] , mat[4]  , mat[8]    , mat[12],
           mat[1] , mat[5]  , mat[9]    , mat[13],
           mat[2] , mat[6]  , mat[10]   , mat[14],
           mat[3] , mat[7]  , mat[11]   , mat[15]  
    };
    GLfloat *return_mat = aux_mat;
    return return_mat;
}

/*VECTOR*/

//Pendiente

//Funcion que normaliza un vector3

vector3 normalize_vector3( vector3 a){
    float mod = sqrt(pow(a.x,2) + pow(a.y,2) + pow(a.z,2) );
    vector3 result = { a.x / mod, a.y / mod, a.z / mod};
    return result;
}

//Funcion que realiza la multiplicacion entre dos vectores

float vector3_mult(vector3 a, vector3 b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

//Funcion que obtiene el vector normal dados dos vectores

vector3 obtain_normal(vector3 a, vector3 b){
    vector3 normal = { a.y * b.z - b.y * a.z, -( a.x * b.z - b.x * a.z), a.x * b.y - b.x * a.y};
    return normal;
}

//Funcion que obtiene el vector normal dados tres puntos

vector3 obtain_normal_3_point3( point3 a, point3 b, point3 c){
    vector3 vec_a = { b.x - a.x, b.y - a.y, b.z - a.z};
    vector3 vec_b = { c.x - a.x, c.y - a.y, c.z - a.z};
    return obtain_normal( vec_a, vec_b);
}

//Funcion que multiplica un escalar por un vector

vector3 mult_scalar_with_vector3(vector3 a, float b){
    vector3 result;
    result.x = a.x * b;
    result.y = a.y * b;
    result.z = a.z * b;
    return result;
}

//Funcion que convierte una estructura point3 a vector3

vector3 point_to_vector(point3 a){
    vector3 result;
    result.x = a.x ;
    result.y = a.y ;
    result.z = a.z ;
    return result;
}

//Funcion que realiza la suma entre dos vectores de tipo vector3

vector3 add_vector3(vector3 a, vector3 b){
    vector3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

//Funcion que realiza la resta entre dos vectores de tipo vector3

vector3 sub_vector3(vector3 a, vector3 b){
    vector3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

//Funcion que devuelve una copia de la matriz

void copy_mat(GLfloat *from, GLfloat *to){
     for(int i=0; i<16; i++){
        to[i] = from[i];
    }
}

