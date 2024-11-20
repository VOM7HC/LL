#ifndef _BABL_HASH_TABLE_HPP
#define _BABL_HASH_TABLE_HPP

#include <cinttypes>

#ifndef _BABL_HPP
#error babl-hash-table.h is only to be included after babl.h
#endif

typedef struct _BablHashTable BablHashTable;

typedef int32_t (*BablHashValFunction)(BablHashTable* htab, Babl* item);
typedef int32_t (*BablHashFindFunction)(Babl* item, void* data);

typedef struct _BablHashTable {
    Babl** data_table;
    int32_t* chain_table;
    int32_t mask;
    int32_t count;
    BablHashValFunction hash_func;
    BablHashFindFunction find_func;
} _BablHashTable;

BablHashTable* babl_hash_table_init(BablHashValFunction hfunc, BablHashFindFunction ffunc);

int32_t babl_hash_by_str(BablHashTable* htab, const char* str);

int32_t babl_hash_by_int(BablHashTable* htab, int32_t id);

int32_t babl_hash_table_size(BablHashTable* htab);

int32_t babl_hash_table_insert(BablHashTable* htab, Babl* item);

Babl* babl_hash_table_find(BablHashTable* htab, int32_t hash, BablHashFindFunction find_func, void* data);

#endif
