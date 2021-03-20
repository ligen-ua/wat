#include "wat_event_cache.h"
#include "wat_hub.h"

namespace wat
{
WAT_ATOMIC<int> g_maxCacheCount(WAT_DEFAULT_EVENT_CACHE_COUNT);

void SetMaxCacheCountItems(int count)
{
    g_maxCacheCount = count;
}

// wat_event_cache
wat_event_cache::wat_event_cache()
    :
        m_cache_size(0)
{
}
wat_event_cache::~wat_event_cache()
{
    int was_stopped = 0;
    for(;;)
    {
        wat_event* pevent = m_events.pop_front();
        if (!pevent)
        {
            break;
        }
        event_default_deleter(pevent);
    }
}
void wat_event_cache::on_event_free(wat_event * pevent)
{
    if (m_cache_size >= g_maxCacheCount)
    {
        // delete it
        event_default_deleter(pevent);
        return;
    }
    m_events.push_back(&pevent->m_node_start);
    ++m_cache_size;
}
WAT_INTRUSIVE_PTR<wat_event> wat_event_cache::alloc(const def_funcname_string_type & function_name,
              const wat::ptime_t & start_time, 
              def_data_string_type & data)
{
    if (wat_event* pevent = m_events.pop_front())
    {
        // got something
        --m_cache_size;
        WAT_INTRUSIVE_PTR<wat_event> ptr(pevent);
        ptr->cache_reinit(function_name, start_time, data);
        return ptr;
    }

    WAT_INTRUSIVE_PTR<wat_event>  pevent(new wat_event(function_name, start_time, data));
    pevent->set_deleter(WAT_EXBOOST_NS::bind(&wat_event_cache::on_event_free, this, pevent.get()));
    return pevent;
}

}