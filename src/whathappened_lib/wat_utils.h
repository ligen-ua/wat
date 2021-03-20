#pragma once


namespace wat
{

class noncopyable
{
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
public:
    noncopyable()
    {
    }
};

class ptime
{
    std::chrono::system_clock::time_point m_std_time;
public:
    ptime() {}
    ptime(std::chrono::system_clock::time_point std_time);
    const std::chrono::system_clock::time_point & get() const { return m_std_time; }

    template<class Type>
    ptime & operator + (const Type & obj)
    {
        m_std_time += obj;
        return *this;
    }
};

ptime ptime_create(int dateval_year,
    int dateval_month,
    int dateval_day,
    int timeOfDay_hours,
    int timeOfDay_minutes,
    int timeOfDay_seconds,
    int timeOfDay_milliseconds);

void time_to_string(const ptime & timeval, std::string * pstring);
bool operator > (const ptime & time1, const ptime & time2);

template<class Other>
ptime operator + (const ptime & time1, const Other & time2)
{
    ptime res = time1;
    res += time2;
    return res;
}

typedef std::chrono::duration<std::chrono::system_clock::rep, std::chrono::system_clock::period>
def_duration_t;
class time_duration
{
    def_duration_t m_duration;
public:
    time_duration(def_duration_t duration);
    long long total_milliseconds();
};
time_duration operator - (const ptime & time1, const ptime & time2);

ptime get_local_time();

void split(std::vector<std::string> & ret,
    const std::string& str,
    const std::string& sep);

template<class T> class intrusive_ptr
{
private:
    typedef intrusive_ptr this_type;
public:
    typedef T element_type;

    intrusive_ptr() : p_(0)
    {
    }

    intrusive_ptr(T * p, bool add_ref = true) : p_(p)
    {
        if (p_ != 0 && add_ref) intrusive_ptr_add_ref(p_);
    }

    intrusive_ptr(intrusive_ptr const & rhs) : p_(rhs.p_)
    {
        if (p_ != 0) intrusive_ptr_add_ref(p_);
    }

    ~intrusive_ptr()
    {
        if (p_ != 0) intrusive_ptr_release(p_);
    }

    intrusive_ptr & operator=(intrusive_ptr const & rhs)
    {
        this_type(rhs).swap(*this);
        return *this;
    }

    intrusive_ptr & operator=(T * rhs)
    {
        this_type(rhs).swap(*this);
        return *this;
    }

    T * get() const
    {
        return p_;
    }

    T & operator*() const
    {
        return *p_;
    }

    T * operator->() const
    {
        return p_;
    }

    operator bool() const
    {
        return p_ != 0;
    }

    bool operator! () const
    {
        return p_ == 0;
    }

    void swap(intrusive_ptr & rhs)
    {
        T * tmp = p_;
        p_ = rhs.p_;
        rhs.p_ = tmp;
    }

private:
    T * p_;
};


typedef ptime ptime_t;
typedef time_duration time_duration_t;
typedef noncopyable noncopyable_t;

}