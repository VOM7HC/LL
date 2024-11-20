#ifndef _BABL_LIST_HPP
#define _BABL_LIST_HPP

#ifndef _BABL_HPP
#error babl-list.h is only to be included after babl.h
#endif

struct _BablList {
    int32_t count;
    int32_t size;
    Babl** items;
};

BablList* babl_list_init(void);

BablList* babl_list_init_with_size(int initial_size);

int32_t babl_list_size(BablList* list);

void babl_list_insert_last(BablList* list, Babl* item);

void babl_list_remove_last(BablList* list);

#define babl_list_get_n(list, n) (list->items[(n)])
#define babl_list_get_first(list) (babl_list_get_n(list, 0))
#define babl_list_size(list) (list->count)
#define babl_list_get_last(list) (babl_list_get_n(list, babl_list_size(list) - 1))

void babl_list_copy(BablList* from,  BablList* to);

void babl_list_each(BablList* list, BablEachFunction each_fun, void* user_data);

#endif
