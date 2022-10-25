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

*Entity_View* This is an type that holds some *Entity* pointer. 
- Members:
    - *entity_count* How many entity in this view
    - *all_entity* Array of entity id