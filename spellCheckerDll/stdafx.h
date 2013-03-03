#pragma once
#ifndef STDAFX_H
#define STDAFX_H

// C++ standard header files:
#pragma warning(push)
#pragma warning(disable: 4242) // conversion from 'int' to 'char', possible loss of data
#pragma warning(disable: 4710) // function not inlined
#pragma warning(disable: 4711) // function ... selected for automatic inline expansion
#pragma warning(push, 3)
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <iterator>
#include <fstream>
#pragma warning(pop)

// Windows header files:
#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
#pragma warning(push)
#pragma warning(disable: 4514) // unreferenced inline function has been removed
#pragma warning(disable: 4820) // ... bytes padding added after data member
#pragma warning(push, 3)
#include <windows.h>
#include <tchar.h>
#include <Shlwapi.h> // PathIsRelative
#pragma warning(pop)

// Shared:
#include "../shared/tdefs.h"

#endif // STDAFX_H
