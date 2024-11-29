#define _BABL_DB_C

#include "babl-internal.hpp"
#include "config.h"
#include <cstring>

static int
_babl_hash_by_str(BablHashTable* htab,
    const char* str)
{
    int hash = 0;

    while (*str) {
        hash += *str++;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return (hash & htab->mask);
}

int babl_hash_by_str(BablHashTable* htab,
    const char* str)
{
    return _babl_hash_by_str(htab, str);
}

static int
_babl_hash_by_int(BablHashTable* htab,
    int id)
{
    int hash = 0;
    hash += id & 0xFF;
    hash += (hash << 10);
    hash ^= (hash >> 6);
    id >>= 8;
    hash += id & 0xFF;
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return (hash & htab->mask);
}

int babl_hash_by_int(BablHashTable* htab,
    int id)
{
    return _babl_hash_by_int(htab, id);
}

static int
db_find_by_name(Babl* item,
    void* data)
{
    if (!strcmp(item->instance.name, (char*)data))
        return 1;
    return 0;
}

static int
db_find_by_id(Babl* item,
    void* data)
{
    if (item->instance.id == *((int*)data))
        return 1;
    return 0;
}

static int
db_hash_by_name(BablHashTable* htab,
    Babl* item)
{
    return _babl_hash_by_str(htab, item->instance.name);
}

static int
db_hash_by_id(BablHashTable* htab,
    Babl* item)
{
    return _babl_hash_by_int(htab, item->instance.id);
}

static int
each_free(Babl* data,
    void* foo)
{
    babl_free(data);
    return 0;
}

static int
babl_db_destroy(void* data)
{
    BablDb* db = data;
    babl_assert(db);

    babl_db_each(db, each_free, NULL);
    babl_mutex_destroy(db->mutex);
    babl_free(db->name_hash);
    babl_free(db->id_hash);
    babl_free(db->babl_list);
    return 0;
}

BablDb* babl_db_init(void)
{
    BablDb* db = babl_calloc(sizeof(BablDb), 1);
    babl_set_destructor(db, babl_db_destroy);

    db->name_hash = babl_hash_table_init(db_hash_by_name, db_find_by_name);
    db->id_hash = babl_hash_table_init(db_hash_by_id, db_find_by_id);
    db->babl_list = babl_list_init_with_size(512);
    db->mutex = babl_mutex_new();

    return db;
}

Babl* babl_db_find(BablDb* db,
    const char* name)
{
    return babl_hash_table_find(db->name_hash, _babl_hash_by_str(db->name_hash, name),
        NULL, (void*)name);
}

int babl_db_count(BablDb* db)
{
    return db->babl_list->count;
}

Babl* babl_db_insert(BablDb* db,
    Babl* item)
{
    babl_mutex_lock(db->mutex);
    if (item->instance.id)
        babl_hash_table_insert(db->id_hash, item);
    babl_hash_table_insert(db->name_hash, item);
    babl_list_insert_last(db->babl_list, item);

    item->instance.creator = babl_extender();
    babl_mutex_unlock(db->mutex);
    return item;
}

void babl_db_each(BablDb* db,
    BablEachFunction each_fun,
    void* user_data)
{
    babl_list_each(db->babl_list, each_fun, user_data);
}

Babl* babl_db_exist(BablDb* db,
    int id,
    const char* name)
{
    Babl* ret;
    if (id)
        ret = babl_hash_table_find(db->id_hash, _babl_hash_by_int(db->id_hash, id), NULL, &id);
    else
        ret = babl_hash_table_find(db->name_hash, _babl_hash_by_str(db->name_hash, name), NULL, (void*)name);
    return ret;
}

Babl* babl_db_exist_by_id(BablDb* db,
    int id)
{
    Babl* ret;
    ret = babl_hash_table_find(db->id_hash, _babl_hash_by_int(db->id_hash, id), NULL, &id);
    return ret;
}

Babl* babl_db_exist_by_name(BablDb* db,
    const char* name)
{
    Babl* ret;
    ret = babl_hash_table_find(db->name_hash, _babl_hash_by_str(db->name_hash, name),
        NULL, (void*)name);
    return ret;
}
