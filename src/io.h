#ifndef IO_H
#define IO_H
//Callback function to control the basic keys
void keyboard(unsigned char key, int x, int y);
//This function just prints information about the use of the keys
void print_help();
// Esta funcion imprime las flags activadas actualmente
void print_flags();
// Funcion que controla las funciones de las teclas especiales
void special_keyboard(int key, int x, int y);
// Funcion que se ejecuta mientra no hay nada que hacer
void update();
// Funcion con las opciones que puedes hacer con el raton
void mouse(int button, int state, int x, int y);
// Funcion con las opciones que puedes hacer con el raton cuando se mueve
void mouse_movement(int x, int y);
#endif // IO_H
