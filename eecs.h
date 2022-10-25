#ifndef ECS_H
#define ECS_H

#include <stdio.h>
#include <stdlib.h>
#define nullptr NULL

typedef unsigned long long EID;
typedef void (*detach_func)(void* comp);

typedef struct Ecs Ecs;
typedef struct Entity Entity;
typedef struct Entity_View Entity_View;

typedef struct Comp_Map Comp_Map;
typedef struct Entity_Map* Entity_Map;
typedef struct Master_Map* Master_Map;
typedef struct Comp_Item* Comp_Item;
typedef struct de_func_map* De_Func_Map;

/**
 * @brief Create a ecs registry
 *
 * @return Ecs* The pointer that return, you should never use free() on this pointer
 */
Ecs* create_registry();

/**
 * @brief Free the registry
 *
 * @param registry
 */
void free_registry(Ecs* registry);

/**
 * @brief Add a detach function for certain component,
 *        this function should not free the comp itself
 *
 * @param registry the registry
 * @param name name of the component
 * @param func function
 */
void add_defunc(Ecs* registry, char* name, detach_func func);

/**
 * @brief Get the detach function of certain conpoment type
 *
 * @param registry the registry
 * @param name name of the component
 * @return detach_func, nullptr if 1)the component never been attatch 2)detach_func never added
 */
detach_func get_defunc(Ecs* registry, char* name);

/**
 * @brief Get the entity view of givern type pattern
 *
 * @param registry the registry
 * @param comp_names pattern, order does not matter
 * @param size sizeof of the comp_names
 * @return Entity_View* ids for all entity that follow partern, you should never use free() on this pointer
 */
Entity_View* get_entity_view(Ecs* registry, char** comp_names, size_t size);

/**
 * @brief free the entity view
 *
 * @param view view that need to be freed
 */
void free_entity_view(Entity_View* view);

/**
 * @brief add an entity to a registry
 *
 * @param registry the registry
 * @return EID id of that entity
 */
EID add_entity(Ecs* registry);

/**
 * @brief Change the name of an entity
 * 
 * @param registry 
 * @param entity 
 */
void change_entity_name(Ecs* registry, EID entity, const char* name);

/**
 * @brief Get the entity pointer
 *
 * @param registry the registry
 * @param id id of entity
 * @return Entity*, you should never use free() on this pointer
 */
Entity* get_entity(Ecs* registry, EID id);

/**
 * @brief remove a eneity base on it's id, this will also free all of it's component
 *
 * @param registry the registry
 * @param entity enity id
 */
void remove_entity(Ecs* registry, EID entity);

unsigned long long get_entity_comp_size(Ecs* registry, EID entity);

/**
 * @brief attatch a component to given entity
 *
 * @param registry the registry
 * @param entity entity id
 * @param name you should use name of the type, attatch with same name will overriten and be freed
 * @param comp_size
 * @return void*
 */
void* attach_comp(Ecs* registry, EID entity, char* name, size_t comp_size);

/**
 * @brief Detach a component from an entity, it's memory will be freed
 *
 * @param registry the registry
 * @param entity entity id
 * @param name name of the component
 */
void detach_comp(Ecs* registry, EID entity, char* name);

/**
 * @brief Get the entity comp
 *
 * @param registry the registry
 * @param entity entity id
 * @param name name of the component
 * @return Comp_Item, you should never use free() on this
 */
Comp_Item get_entity_comp(Ecs* registry, EID entity, char* name);

/**
 * @brief Get the number of entity in the registry
 *
 * @param registry the registry
 * @return unsigned long long, size
 */
unsigned long long get_registry_size(Ecs* registry);

/**
 * @brief Get the number of certain type of comp in registry
 *
 * @param registry the registry
 * @param name name of the component
 * @return unsigned long long, size
 */
unsigned long long get_comp_size(Ecs* registry, char* name);

// struct definitions
struct Entity
{
    char name[512];
    unsigned long long id;
    char** comp_arr;

    Entity* father;
};

struct Comp_Item
{
    unsigned long long key;
    void* value;
};

struct Comp_Map
{
    // size of the compobnent in bytes, important!
    size_t comp_size;

    Comp_Item map;
};

// comp map
struct Master_Map
{
    // name of the comp
    char* key;

    // comp map for each one
    Comp_Map* value;
};

struct Entity_Map
{
    unsigned long long key;
    Entity* value;
};

struct de_func_map
{
    char* key;
    detach_func value;
};

// ecs system
struct Ecs
{
    // map for different comp_map
    Entity_Map entity_map;

    Master_Map comp_map;

    De_Func_Map defunc_map;
};

struct Entity_View
{
    unsigned long long entity_count;
    EID* all_entity;
};

#endif // ECS_H
