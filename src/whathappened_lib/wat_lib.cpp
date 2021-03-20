#include "wat_lib.h"
#include "wat_hub.h"
#include "wat_reporter.h"
#include "wat_log.h"
#include "wat_event_cache.h"
namespace wat
{
    class wat_lib:wat::noncopyable_t
    {
        WAT_SHARED_PTR<wat_event_cache> m_cache;
        WAT_SHARED_PTR<wat_reporter> m_reporter;
        WAT_SHARED_PTR<wat_hub> m_hub;
    public:
        wat_lib(const std::string & file_name)
        {
            m_cache.reset(new wat_event_cache());
            m_reporter.reset(new wat_reporter(file_name));
            m_hub.reset(new wat_hub(m_reporter));
        }
        ~wat_lib()
        {
            m_hub->sync_stop();
        }
        WAT_SHARED_PTR<wat_hub> get_hub()
        {
            return m_hub;
        }
        WAT_SHARED_PTR<wat_event_cache> get_cache()
        {
            return m_cache;
        }
    };
    static WAT_AUTO_PTR<wat_lib> g_watLib;

    //-- lib_*
    void lib_init(const std::string & file_name)
    {
#ifdef WAT_DISABLE_LIBRARY
        &file_name;
#else
        if (g_watLib.get())
            throw wat_error("Already initialized");

        g_watLib.reset(new wat_lib(file_name));
#endif
    }
    void lib_free()
    {
        g_watLib.reset(0);
    }
    void lib_release()
    {
        g_watLib.release();
    }
    wat_lib * lib_get()
    {
        wat_lib * p = g_watLib.get();
        return p;
    }

    // public_event
    public_event::~public_event()
    {
        if (m_started)
        {
            m_event->stop();
        }
    }
    void public_event::set_flags(int flags)
    {
        m_flags = flags;
    }
    bool public_event::no_library() const
    {
        return !lib_get();
    }
    void public_event::add_text(const def_funcname_string_type & function_name,
                                const wat::ptime_t & timeval, 
                                def_data_string_type & data)
    {
        wat_lib * plib = lib_get();
        if (!plib)
        {
            return;
        }
        WAT_SHARED_PTR<wat_hub> phub = plib->get_hub();
        if (!phub)
        {
            return;
        }

        m_event = plib->get_cache()->alloc(function_name, timeval, data);
        m_event->set_flags(m_flags, m_flags);
        m_event->start(phub);
        m_started = true;
    }

}