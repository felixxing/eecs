# Eecs
This is an entity component system in plain c99, it required stb_ds.h

## Design principle
Focusing on simplisity and easy of use
Just like what you expect, *programmer should be trusted*. 

Thisfore, this library does not do any data hiding and it's your responsibilty to ensure that you understand what you are doing with it.

## Purpose
This is the ECS system for my own custom OpenGl engine. and it's inspired by [entt](https://github.com/skypjack/entt).

It lots of hash map to maintain almost constant time operation.

## Doc

### Data type
*EID* Id of entity. this is the ONLY identification you should use with this library;

*detach_func* A specify function pattern that use to free special custom component. PLEASE SEE DETAIL EXPLAINATION BELOW ABOUT HOW IT WORK!

*Ecs* The ECS registry data type, you shouldn't make any adjustment on it. DO NOT FREE THIS ON YOU OWN!

*Entity* The entity data type, you shouldn't make any adjustment on it. DO NOT FREE THIS ON YOU OWN!
- Members:
    - *id*: id of the entity
    - *comp_arr*: Array of entity component name
    - *father*: Father entity

*Entity_View* This is an type that holds some *Entity* pointer, you shouldn't make any adjustment on it. DO NOT FREE THIS ON YOU OWN!
- Members:
    - *entity_count*: How many entity in this view
    - *all_entity*: Array of entity id

*Comp_Item* This is the conponent struct pointer, you shouldn't make any adjustment on it. DO NOT FREE THIS ON YOU OWN!
- Members:
    - *key*: Entity id
    - *value*: pointer to the entity

REST THE STRUCT IS THE STRUCT THAT YOU SHOULD NEVER USE IN YOU RPROGRAM!

### *detach_func*
This function has following signature:
```c
void (*detach_func)(void* comp)
```
When a component is free/detach, registry will call this function and take the pointer of that component as ```comp``` pram.

Attention! This function is used to free any resouce that you created that been referenced by this component. NOT THE COMPONENT ITSELF.

Therefore, DO NOT do something like ```free(comp)``` in this function.

Example: 
``` c
struct Ff_Struct{float* ff;}; // the component

// when detach, ff member is allocate at some time
void defunc_ff(void* comp)
{
    struct Ff_Struct* tmp = comp;
    free(tmp->ff); // free the ff member

    // do not free tmp itself, Eecs will do that for you!
}
```

### *get_entity_view*
```comp_names``` is an arr of string;

Example:
``` c
char* patterns[] = {"Transform", "Mesh"};
Entity_View* view = get_entity_view(registry, patterns, 2);
```
This will return a list of EID that has both *Transform* and *Mesh* component. Use ```get_entity_comp``` to get the pointer to conponent.