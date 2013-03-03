#pragma once
#ifndef TDEFS_H
#define TDEFS_H

#pragma warning(push)
#pragma warning(disable: 4986) // exception specification does not match previous declaration
#pragma warning(disable: 4710) // function not inlined
#pragma warning(push, 3)
#include <string>
#pragma warning(pop)

// Shared defines and typedefs for missing "t"-things.

#ifdef _UNICODE 
#define tcout std::wcout 
typedef std::wstring tstring;
#else 
#define tcout std::cout
typedef std::string tstring;
#endif // _UNICODE

#endif // TDEFS_H
