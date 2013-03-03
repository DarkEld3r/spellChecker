#pragma once
#ifndef STDAFX_H
#define STDAFX_H

// C++ standard header files:
#pragma warning(push)
#pragma warning(disable: 4710) // function not inlined
#pragma warning(disable: 4711) // function ... selected for automatic inline expansion
#pragma warning(push, 3)
#include <fstream>
#include <string>
#pragma warning(pop)

// Boost headers:
#pragma warning(push)
#pragma warning(disable: 4619) // there is no warning number ...
#pragma warning(disable: 4548) // expression before comma has no effect
#pragma warning(disable: 4265) // lass has virtual functions, but destructor is not virtual
#pragma warning(disable: 4640) // construction of local static object is not thread-safe
#pragma warning(disable: 4710) // function not inlined
#pragma warning(push, 3)
#define BOOST_TEST_MODULE SPELL_CHECKER_TEST
#include <boost/test/unit_test.hpp>
#include <boost/format.hpp>
#pragma warning(pop)

// Windows header files:
#pragma warning(push)
#pragma warning(disable: 4514) // unreferenced inline function has been removed
#pragma warning(disable: 4820) // ... bytes padding added after data member
#pragma warning(disable: 4191) // 'type cast' : unsafe conversion...
#pragma warning(push, 3)
#include <tchar.h>
#include <AtlBase.h>
#include <AtlConv.h> // String conversion classes
#pragma warning(pop)

#endif // STDAFX_H
