#pragma once
#include "wat_internal_base.h"

namespace wat
{

class wat_hub;
class wat_event
{
    def_funcname_string_type m_function_name;
    WAT_SHARED_PTR<wat_hub> m_phub;
    wat::ptime_t m_start_time;
    wat::ptime_t m_stop_time;
    WAT_ATOMIC<bool> m_stopped;
    WAT_ATOMIC<int> m_stop_resolve_count;
    WAT_ATOMIC<bool> m_start_reported;
    int m_flags;
    def_data_string_type m_data;
    long long m_message_id;
    thread_id_type m_thread_id;

    WAT_ATOMIC<long> m_counter;
    WAT_FUNCTION<void(wat_event*)> m_deleter;
public:
    // list node bases
    list_base_node<wat_event> m_node_start;
    list_base_node<wat_event> m_node_stop;

public:
    wat_event(const def_funcname_string_type & function_name,
              const wat::ptime_t & start_time, 
              def_data_string_type & data);
    virtual ~wat_event();

    void cache_reinit(const def_funcname_string_type & function_name,
              const wat::ptime_t & start_time, 
              def_data_string_type & data);
    void set_deleter(WAT_FUNCTION<void(wat_event*)> deleter);
    void start(WAT_SHARED_PTR<wat_hub> phub);
    void stop();
    void set_flags(int flag, int mask);
    bool is_flag_set(int flag) const;
    void init(long long message_id);
    bool is_stopped(wat::ptime_t * pstop_time) const;
    bool resolve_stop();
    void clear();
    bool is_start_reported() const;
    const def_data_string_type & get_data() const;
    def_data_string_type & get_data();
    long long get_message_id() const;
    thread_id_type get_thread_id() const;
    const wat::ptime_t & get_start_time() const;
    const wat::ptime_t & get_stop_time() const;
    const def_funcname_string_type & get_function_name() const;
    void add_ref();
    void release();
};

void event_default_deleter(wat_event * pwat_event);
void intrusive_ptr_add_ref(wat_event * p);
void intrusive_ptr_release(wat_event * p);


class wat_reporter;
class wat_hub:wat::noncopyable_t
{
    WAT_SHARED_PTR<wat_reporter> m_pwat_reporter;
    long long m_last_message_id;
    typedef wat::intrusive_list<wat_event> event_list_type;
    event_list_type m_events;

#ifdef WAT_BOOST_BASED
    boost::thread_group m_threads;
#else
    std::thread m_thread;
#endif
    WAT_ATOMIC<int> m_stopped;
    void process_event(WAT_INTRUSIVE_PTR<wat_event> pevent);
public:
    wat_hub(WAT_SHARED_PTR<wat_reporter> pwat_reporter);
    ~wat_hub();
    void wat_register_event(WAT_INTRUSIVE_PTR<wat_event> pevent,
        list_base_node<wat_event> & node);
    void sync_stop();
    void run();
};

void SetPoolWaitMilliseconds(int count);
}