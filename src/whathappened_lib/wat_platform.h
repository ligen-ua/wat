#pragma once
#include "memory"
#include "stdexcept"

#ifdef _WIN32
#include "wat_platform_win32.h"
#else
#include "wat_platform_gcc.h"
#endif

