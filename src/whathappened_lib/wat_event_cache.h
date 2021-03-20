#pragma once
#include "wat_strings.h"
#include "wat_list_and_locks.h"

namespace wat
{
class wat_event;
class wat_event_cache
{
    typedef wat::intrusive_list<wat_event> event_list_type;
    event_list_type m_events;

    WAT_ATOMIC<long long> m_cache_size;
    int get_current_size() const;
    void on_event_free(wat_event * pevent);
public:
    wat_event_cache();
    ~wat_event_cache();

    WAT_INTRUSIVE_PTR<wat_event> alloc(const def_funcname_string_type & function_name,
              const wat::ptime_t & start_time, 
              def_data_string_type & data);
};


void SetMaxCacheCountItems(int count);

}