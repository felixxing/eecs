#define STB_DS_IMPLEMENTATION
#include "eecs.h"

#include <stb/stb_ds.h>

Ecs* create_registry()
{
    Ecs* tmp_ecs = malloc(sizeof(Ecs));
    memset(tmp_ecs, 0x0, sizeof(Ecs));

    sh_new_arena(tmp_ecs->comp_map);
    sh_new_arena(tmp_ecs->defunc_map);

    return tmp_ecs;
}

EID add_entity(Ecs* registry)
{
    static unsigned long long entity_order = 1;

    Entity* tmp_entity = malloc(sizeof(Entity));
    memset(tmp_entity, 0x0, sizeof(Entity));
    tmp_entity->id = entity_order;

    sprintf(tmp_entity->name, "EID_%lld", entity_order);

    hmput(registry->entity_map, entity_order, tmp_entity);
    entity_order++;

    return hmgetp_null(registry->entity_map, entity_order - 1)->value->id;
}

void change_entity_name(Ecs* registry, EID entity, const char* name)
{
    Entity* tmp = get_entity(registry, entity);

    sprintf(tmp->name, "%s", name);
}

Entity* get_entity(Ecs* registry, EID id)
{
    Entity_Map result = hmgetp_null(registry->entity_map, id);
    if (result == nullptr)
    {
        return nullptr;
    }
    return result->value;
}

void remove_entity(Ecs* registry, EID entity)
{
    Entity* tmp_ent = get_entity(registry, entity);
    if (tmp_ent == nullptr)
    {
        return;
    }

    for (unsigned long long i = 0; i < arrlen(tmp_ent->comp_arr); i++)
    {
        detach_comp(registry, entity, tmp_ent->comp_arr[i]);
    }

    if (tmp_ent->comp_arr != nullptr)
    {
        arrfree(tmp_ent->comp_arr);
    }
    hmdel(registry->entity_map, entity);
    free(tmp_ent);
}

unsigned long long get_entity_comp_size(Ecs* registry, EID entity)
{
    Entity* tmp = get_entity(registry, entity);
    if (tmp->comp_arr == nullptr)
    {
        return 0;
    }
    return arrlenu(tmp->comp_arr);
}

void* attach_comp(Ecs* registry, EID entity, char* name, size_t comp_size)
{
    if (hmgetp_null(registry->entity_map, entity) == nullptr)
    {
        return nullptr;
    }

    Master_Map result = shgetp_null(registry->comp_map, name);
    Comp_Map* map = nullptr;

    if (result == nullptr)
    {
        Comp_Map* tmp_map = malloc(sizeof(Comp_Map));

        shput(registry->comp_map, name, tmp_map);

        map = tmp_map;
        map->comp_size = comp_size;
        map->map = nullptr;
    }
    else
    {
        map = shgetp_null(registry->comp_map, name)->value;
    }

    Entity* tmp_ent = get_entity(registry, entity);
    arrput(tmp_ent->comp_arr, name);
    void* tmp_comp = malloc(comp_size);
    memset(tmp_comp, 0x0, comp_size);

    if (get_entity_comp(registry, entity, name) != nullptr)
    {
        detach_comp(registry, entity, name);
    }
    hmput(map->map, entity, tmp_comp);

    return hmgetp_null(map->map, entity)->value;
}

void detach_comp(Ecs* registry, EID entity, char* name)
{
    Comp_Item comp = get_entity_comp(registry, entity, name);
    detach_func func = get_defunc(registry, name);

    if (comp == nullptr)
    {
        return;
    }

    if (func != nullptr)
    {
        func(comp->value);
    }

    free(comp->value);

    Comp_Map* map = shgetp_null(registry->comp_map, name)->value;
    hmdel(map->map, entity);
}

Comp_Item get_entity_comp(Ecs* registry, EID entity, char* name)
{
    Master_Map result = shgetp_null(registry->comp_map, name);
    if (result == nullptr)
    {
        return nullptr;
    }

    Comp_Map* map = shgetp_null(registry->comp_map, name)->value;
    if (map == nullptr)
    {
        return nullptr;
    }

    return hmgetp_null(map->map, entity);
}

unsigned long long get_registry_size(Ecs* registry)
{
    return hmlenu(registry->entity_map);
}

unsigned long long get_comp_size(Ecs* registry, char* name)
{
    Master_Map result = shgetp_null(registry->comp_map, name);
    if (result == nullptr)
    {
        return -1;
    }

    return hmlenu(result->value->map);
}

Entity_View* get_entity_view(Ecs* registry, char** comp_names, size_t size)
{
    Entity_View* result = malloc(sizeof(Entity_View));
    memset(result, 0x0, sizeof(Entity_View));

    for (unsigned long long i = 0; i < hmlenu(registry->entity_map); i++)
    {
        char** comps = registry->entity_map[i].value->comp_arr;
        int present = 0;

        for (unsigned long long j = 0; j < size; j++)
        {
            for (unsigned long long k = 0; k < arrlen(comps); k++)
            {
                if (!strcmp(comps[k], comp_names[j]))
                {
                    present++;
                }
            }
        }

        if (present == size)
        {
            arrput(result->all_entity, registry->entity_map[i].value->id);
        }
    }

    result->entity_count = arrlen(result->all_entity);

    return result;
}

void free_entity_view(Entity_View* view)
{
    arrfree(view->all_entity);
    free(view);
}

void free_registry(Ecs* registry)
{

    for (unsigned long long i = 0; i < hmlenu(registry->entity_map); i++)
    {
        remove_entity(registry, registry->entity_map[i].value->id);
    }
    hmfree(registry->entity_map);

    for (unsigned long long k = 0; k < shlen(registry->comp_map); k++)
    {
        Comp_Map* cc = registry->comp_map[k].value;
        hmfree(cc->map);
        free(cc);
    }
    shfree(registry->comp_map);

    shfree(registry->defunc_map);

    free(registry);
}

void add_defunc(Ecs* registry, char* name, detach_func func)
{
    shput(registry->defunc_map, name, func);
}

detach_func get_defunc(Ecs* registry, char* name)
{
    De_Func_Map tmp = shgetp_null(registry->defunc_map, name);
    if (tmp == nullptr)
    {
        return nullptr;
    }

    return tmp->value;
}
