#pragma once

#include "memory"
#include "stdexcept"
#include "wat_platform.h"

namespace wat
{
template<class lock_type>
class scoped_lock
{
    scoped_lock(const scoped_lock&);
    scoped_lock & operator = (const scoped_lock&);

    lock_type * m_pLock;
public:
    scoped_lock(lock_type * pLock)
       : m_pLock(pLock)
    {
       m_pLock->lock();
    }
    ~scoped_lock()
    {  
       m_pLock->unlock();
    }
};

}
