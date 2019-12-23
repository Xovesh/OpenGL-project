#include "definitions.h"

//Funcion que realiza la suma entre dos vectores de tipo point3
point3 add_point3(point3 a, point3 b);
//Funcion que calcula la norma entre dos puntos
float distance_3d(point3 a,point3 b);
//Funcion que inicializa una estructura del tipo point3
point3 create_point_3( float x, float y, float z);
//Funcion que inicializa una estructura del tipo color3
color3 create_color3( float r, float g, float b);
//Funcion que devuelve la traspuesta de la matriz
GLfloat* transpose_mat_4x4(GLfloat *mat);
//Funcion que realiza la suma entre dos vectores de tipo vector3
vector3 add_vector3(vector3 a, vector3 b);
//Funcion que realiza la resta entre dos vectores de tipo vector3
vector3 sub_vector3(vector3 a, vector3 b);
//Funcion que multiplica un escalar por un vector
vector3 mult_scalar_with_vector3(vector3 a, float b);
vector3 sub_vector3(vector3 a, vector3 b);
//Funcion que convierte una estructura point3 a vector3
vector3 point_to_vector(point3 a);
//Funcion que realiza la multiplicacion entre dos vectores
float vector3_mult(vector3 a, vector3 b);
//Funcion que obtiene el vector normal dados dos vectores
vector3 obtain_normal(vector3 a, vector3 b);
//Funcion que obtiene el vector normal dados tres puntos
vector3 obtain_normal_3_point3( point3 a, point3 b, point3 c);
//Funcion que normaliza un vector3
vector3 normalize_vector3( vector3 a);
//Funcion que devuelve una copia de la matriz
void copy_mat(GLfloat *from, GLfloat *to);
