#pragma once
#include <windows.h>
#include "game.h"

#define STR(x) #x
#define STRINGIFY_MACRO(x) STR(x)
#define EXPAND(x) x
#define IL2CPPHEADER(a, b, c) STRINGIFY_MACRO(EXPAND(a)EXPAND(b)EXPAND(c))
#define IL2CPPAPIDIR il2cppapi/
#define IL2CPPCLASS IL2CPPHEADER(IL2CPPAPIDIR, UnityVersion, /il2cpp-class.h)
#define IL2CPPAPI IL2CPPHEADER(IL2CPPAPIDIR, UnityVersion, /il2cpp-api-functions.h)

void il2cpp_dump(HMODULE module, const char* outDir);
