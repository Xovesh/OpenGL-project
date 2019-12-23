#include "definitions.h"

int colision_check(object3d *subject,object3d *target);
void basic_collider();
/*Extra*/

void no_op(object3d *target);
void basic_OnUpdate(object3d *current_obj_ptr);
void test_onNoColScale(object3d *obj_ptr);

void missile_Update(object3d *obj_ptr);
void missile_OnCollide(object3d * obj_ptr);
void missile_DisposeSelf(object3d *obj_ptr);