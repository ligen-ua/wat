#include "wat_hub.h"
#include "wat_reporter.h"
#include "wat_warns_off.h"
namespace wat
{

WAT_ATOMIC<int> g_poolMsCount(WAT_DEFAULT_POLL_MILLISECONDS_COUNT);

void event_default_deleter(wat_event * pwat_event)
{
    delete pwat_event;
}

void SetPoolWaitMilliseconds(int count)
{
    g_poolMsCount = count;
}
// wat_event
wat_event::wat_event(const def_funcname_string_type & function_name,
                     const wat::ptime_t & start_time, 
                     def_data_string_type & data)
    :
        m_function_name(function_name),
        m_start_time(start_time),
        m_message_id(0),
        m_stopped(false),
        m_stop_resolve_count(0),
        m_start_reported(false),
        m_thread_id(get_current_thread_id()),
        m_flags(0),
        m_counter(0),
        m_node_start(this),
        m_node_stop(this),
        m_deleter(event_default_deleter)
{
    WAT_MOVE(m_data, data);
}
wat_event::~wat_event()
{
    stop();
}
void wat_event::cache_reinit(const def_funcname_string_type & function_name,
                             const wat::ptime_t & start_time, 
                             def_data_string_type & data)
{
    m_function_name = function_name;
    m_start_time = start_time;
    WAT_MOVE(m_data, data);
    m_message_id = 0;
    m_stopped = false;
    m_stop_resolve_count = 0;
    m_start_reported = false;
    m_thread_id = get_current_thread_id(),
    m_flags = 0;
    m_counter = 0;
}
void wat_event::set_deleter(WAT_FUNCTION<void(wat_event*)> deleter)
{
    m_deleter = deleter;
}
void wat_event::start(WAT_SHARED_PTR<wat_hub> phub)
{
    m_phub = phub;
    m_phub->wat_register_event(this, m_node_start);
}
bool wat_event::is_flag_set(int flag) const
{
    return (m_flags & flag) != 0;
}
void wat_event::set_flags(int flag, int mask)
{
    int cleared_flags = m_flags & ~mask;
    m_flags = cleared_flags | (flag & mask);
}
void wat_event::stop()
{
    if (m_flags & event_flag_just_info)
        return;
    if (!m_phub.get())
        return;
    try
    {
        m_stop_time = wat::get_local_time();
        m_stopped = true;
        m_phub->wat_register_event(this, m_node_stop);
    }
    catch(const std::exception & ex)
    {
        WAT_LIB_LOG(ex);
    }
    m_phub.reset();
}
void wat_event::init(long long message_id)
{
    m_message_id = message_id;
}
bool wat_event::is_stopped(wat::ptime_t * pstop_time) const
{
    if (!m_stopped)
        return false;
    *pstop_time = m_stop_time;
    return true;
}
bool wat_event::resolve_stop()
{
    return ++m_stop_resolve_count == 2;
}
void wat_event::clear()
{
    m_data.clear();
    m_start_reported = true;
}
bool wat_event::is_start_reported() const
{
    return m_start_reported;
}
const def_data_string_type & wat_event::get_data() const
{
    return m_data;
}
def_data_string_type & wat_event::get_data()
{
    return m_data;
}
long long wat_event::get_message_id() const
{
    return m_message_id;
}
thread_id_type wat_event::get_thread_id() const
{
    return m_thread_id;
}
const wat::ptime_t & wat_event::get_start_time() const
{
    return m_start_time;
}
const wat::ptime_t & wat_event::get_stop_time() const
{
    return m_stop_time;
}
const def_funcname_string_type & wat_event::get_function_name() const
{
    return m_function_name;
}
void wat_event::add_ref()
{
    ++m_counter;
}
void wat_event::release()
{
    if (--m_counter == 0)
    {
        m_deleter(this);
    }
}

void intrusive_ptr_add_ref(wat_event * p)
{
    p->add_ref();
}
void intrusive_ptr_release(wat_event * p)
{
    p->release();
}
// wat_hub
wat_hub::wat_hub(WAT_SHARED_PTR<wat_reporter> pwat_reporter)
    :
        m_pwat_reporter(pwat_reporter),
        m_last_message_id(0),
        m_stopped(0)
{

#ifdef WAT_BOOST_BASED
    m_threads.create_thread(boost::bind<void, 
                                        wat_hub, 
                                        wat_hub*>
                                (
                                    &wat_hub::run, 
                                    this
                                )
                            );
#else
    m_thread = std::thread([=] { this->run();  });
#endif
}
wat_hub::~wat_hub()
{
    if (!m_stopped)
    {
        sync_stop();
    }
}

void wat_hub::run()
{
    int was_stopped = 0;
    for(;;)
    {
        if (wat_event* pevent = m_events.pop_front())
        {
            WAT_INTRUSIVE_PTR<wat_event> ptr(pevent, false);
            // init id
            if (ptr->get_message_id() == 0)
            {
                long long id = ++m_last_message_id;
                if (id == 0)
                {
                    id = ++m_last_message_id;
                }
                ptr->init(id);
            }

            process_event(ptr);
            continue;
        }
        else
        {
            if (was_stopped)
            {
                break;
            }
            wat::sleep_for_milliseconds(g_poolMsCount);
        }
        was_stopped = m_stopped;
    }
}
void wat_hub::sync_stop()
{
    m_stopped = 1;

#ifdef WAT_BOOST_BASED
    m_threads.join_all();
#else
    if (m_thread.joinable())
    {
        m_thread.join();
    }
#endif
}

void wat_hub::process_event(WAT_INTRUSIVE_PTR<wat_event> pevent)
{
    // report start
    if (pevent->is_flag_set(event_flag_just_info))
    {
        if (pevent->is_flag_set(event_flag_set_thread_tag))
        {
            m_pwat_reporter->report_thread_tag(pevent);
            return;
        }
        m_pwat_reporter->report_info(pevent);
        return;
    }
    if (!pevent->is_start_reported())
    {
        m_pwat_reporter->report_start(pevent);
        pevent->clear();
    }
    if (pevent->resolve_stop())
    {
        // report stop
        m_pwat_reporter->report_stop(pevent);
    }
}
void wat_hub::wat_register_event(WAT_INTRUSIVE_PTR<wat_event> pevent,
                                 list_base_node<wat_event> & node)
{
    if (m_stopped)
    {
        return;
    }
    pevent->add_ref();
    // list do not throw:
    m_events.push_back(&node);
}

}