#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif 
#include "intrin.h"
#include "winsock2.h"

namespace wat
{

// win32.exchange
template<class Type>
Type * exchange(Type ** ppDestination, Type * pNewData)
{
    // returns old value from *ppDestination
    return (Type *)InterlockedExchangePointer((void**)ppDestination, pNewData);
}
inline long exchange(long * pDestination, long newData)
{
    // returns old value from *pDestination
    return  InterlockedExchange(pDestination, newData);
}

// win32.compare_and_swap
#ifdef _M_X64
template<class Type>
Type * compare_and_swap(Type ** ppDestination, Type * pNewData, Type * pComparand)
{
    // returns old value from *ppDestination
    return (Type *)_InterlockedCompareExchangePointer((void **)ppDestination, (Type *)pNewData, (Type *)pComparand);
}
#else
template<class Type>
Type * compare_and_swap(Type ** ppDestination, Type * pNewData, Type * pComparand)
{
    // returns old value from *ppDestination
    return (Type *)InterlockedCompareExchangePointer((void **)ppDestination, (Type *)pNewData, (Type *)pComparand);
}
#endif 
inline long compare_and_swap(long * pDestination, long newData, long comparand)
{
    // returns old value from *pDestination
    return InterlockedCompareExchange(pDestination, newData, comparand);
}

}
