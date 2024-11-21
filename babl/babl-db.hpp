#ifndef _BABL_DB_HPP
#define _BABL_DB_HPP

#ifndef _BABL_HPP
#error babl-db.h is only to be included after babl.h
#endif

#include "babl-hash-table.hpp"
#include "babl-list.hpp"
#include "babl-memory.hpp"
#include "babl-mutex.hpp"

typedef struct _BablDb BablDb;

typedef struct _BablDb {
    BablHashTable* name_hash;
    BablHashTable* id_hash;
    BablList* babl_list;
    BablMutex* mutex;
} _BablDb;

#ifdef NEEDS_BABL_DB
static BablDb* db = NULL;
#endif

BablDb* babl_db_init(void);

void babl_db_each(BablDb* db, BablEachFunction each_fun, void* user_data);

int babl_db_count(BablDb* db);

Babl* babl_db_insert(BablDb* db, Babl* entry);

Babl* babl_db_exist(BablDb* db, int id, const char* name);

Babl* babl_db_exist_by_name(BablDb* db, const char* name);
Babl* babl_db_exist_by_id(BablDb* db, int id);

Babl* babl_db_find(BablDb* db, const char* name);

#endif
