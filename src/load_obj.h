#ifndef READ_OBJ_H
#define	READ_OBJ_H

#include "definitions.h"
//Function to read wavefront files (*.obj)
int read_wavefront(char * file_name, object3d * object_ptr);
// Funcion que lee un archivo .mtl
mtl* read_mtl(char *file_name );
#endif	/* READ_OBJ_H */

