#pragma once
#ifndef STDAFX_H
#define STDAFX_H

// C++ standard header files:
#pragma warning(push)
#pragma warning(disable: 4710) // function not inlined
#pragma warning(disable: 4514) // unreferenced inline function has been removed
#pragma warning(disable: 4711) // function ... selected for automatic inline expansion
#pragma warning(disable: 4820) // 'x' bytes padding added after data member
#pragma warning(disable: 4986) // exception specification does not match previous declaration
#include <iostream>
#include <memory> // std::unique_ptr
#pragma warning(pop)

// Windows header files:
#include <tchar.h>

// Shared:
#include "../shared/tdefs.h"

#endif // STDAFX_H
