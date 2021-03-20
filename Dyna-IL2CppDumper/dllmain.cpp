#include <windows.h>
#include "il2cpp_dump.h"
#include "log.h"
#include "define.h"

//#define CONSOLE

DWORD WINAPI Run(LPVOID lpThreadParameter) {
#ifdef CONSOLE
    AllocConsole();
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
#endif

    HMODULE module;
    while ((module = GetModuleHandleA("GameAssembly.dll")) == nullptr) {
        Sleep(1);
    }
    il2cpp_dump(module, kOutputDir);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Run, NULL, 0, NULL);
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}
