#include "definitions.h"
#include "util.h"
#include "math_util.h"
#include <stdio.h>

#include "global_data_extern.h"


void no_op(object3d *target){}

void basic_physics(object3d *current_obj_ptr){
    current_obj_ptr->velocity = add_vector3(current_obj_ptr->velocity,current_obj_ptr->acceleration);
    if (current_obj_ptr->velocity.x != 0 || current_obj_ptr->velocity.y != 0 || current_obj_ptr->velocity.z != 0)
        displace_selected_object(current_obj_ptr, reference_mode, current_obj_ptr->velocity.x, current_obj_ptr->velocity.y, current_obj_ptr->velocity.z);

}

void basic_collider(){
    object3d *current_obj_ptr = _first_object;
    object3d *aux_obj_ptr;
    if( current_obj_ptr == NULL)
        return;
    while(current_obj_ptr != NULL){
        aux_obj_ptr = current_obj_ptr->next;
        
        while (aux_obj_ptr != NULL)
        {
            object3d *temp_ptr = aux_obj_ptr->next;
            if(colision_check(current_obj_ptr,aux_obj_ptr)){
                current_obj_ptr->has_collided = 1;
                aux_obj_ptr->has_collided = 1;
                current_obj_ptr ->onCollideOther(aux_obj_ptr);
                current_obj_ptr -> onCollideSelf(current_obj_ptr);
                if(aux_obj_ptr == NULL){
                    continue;
                }
                aux_obj_ptr->onCollideSelf(aux_obj_ptr);
                if(current_obj_ptr == NULL){
                    break;
                }
                aux_obj_ptr->onCollideOther(current_obj_ptr);

            }
            aux_obj_ptr = temp_ptr;
        }
        current_obj_ptr = current_obj_ptr->next;
    }

    current_obj_ptr = _first_object;
    while (current_obj_ptr != NULL)
    {
        if(current_obj_ptr->has_collided){
            current_obj_ptr->has_collided = 0;
        }else
        {
            current_obj_ptr->onNoCollision(current_obj_ptr);
        }
        

        current_obj_ptr = current_obj_ptr->next;
    }
    

    // int has_collided = 0;
    // aux_obj_ptr = current_obj_ptr;
    // if(aux_obj_ptr->next == NULL)
    //         current_obj_ptr->onNoCollision(current_obj_ptr);
    // aux_obj_ptr = current_obj_ptr ->next;
    // while(aux_obj_ptr != NULL){
    //     if(colision_check(current_obj_ptr,aux_obj_ptr)){
    //         current_obj_ptr->onCollideOther(aux_obj_ptr);
    //         current_obj_ptr->onCollideSelf(current_obj_ptr);
    //         has_collided = 1;
    //     }
    //     aux_obj_ptr = aux_obj_ptr->next;
    // }
    // if (!has_collided)
    //     current_obj_ptr->onNoCollision(current_obj_ptr);
}

void basic_ttl(object3d *current_obj_ptr){
    if (current_obj_ptr->ttl > 0)
        current_obj_ptr->ttl--;
    else if (current_obj_ptr->ttl == 0)
    {
        printf("Objeto eliminado\n");
        eliminar_objeto(current_obj_ptr);
    }
}

void basic_OnUpdate(object3d *current_obj_ptr){
    basic_physics(current_obj_ptr);
}

void test_onNoColScale(object3d *obj_ptr){
    rotate_selected_object(obj_ptr,LOCAL_MODE,1,1,0,0);
}

void missile_DisposeSelf(object3d *obj_ptr){
    if (obj_ptr == _first_object)
    {
        if(_first_object == NULL)
            return;
        /*To remove the first object we just set the first as the current's next*/
        _first_object = _first_object->next;
        /*Once updated the pointer to the first object it is save to free the memory*/
        liberar_objeto(obj_ptr);
        /*Finally, set the selected to the new first one*/
    }
    else
    {
        /*In this case we need to getmissile_Update the previous element to the one we want to erase*/
        object3d *auxiliar_object;
        auxiliar_object = _first_object;
        while (auxiliar_object->next != obj_ptr)
            auxiliar_object = auxiliar_object->next;
        /*Now we bypass the element to erase*/
        auxiliar_object->next = obj_ptr->next;
        /*free the memory*/
        liberar_objeto(obj_ptr);
        /*and update the selection*/
    }
}

void missile_ttl(object3d *current_obj_ptr){
    if (current_obj_ptr->ttl > 0)
        current_obj_ptr->ttl--;
    else if (current_obj_ptr->ttl == 0)
    {
        printf("Objeto eliminado\n");
        missile_DisposeSelf(current_obj_ptr);
    }
}

void missile_Update(object3d *obj_ptr){
    displace_selected_object(obj_ptr,LOCAL_MODE,0,0,-0.5);
    missile_ttl(obj_ptr);
}

void missile_OnCollide(object3d * obj_ptr){
    object3d *aux = NULL ;
    if(obj_ptr != _selected_object)
        aux = _selected_object;
    eliminar_objeto(obj_ptr);
    if(aux != NULL)
        _selected_object = aux;
}
