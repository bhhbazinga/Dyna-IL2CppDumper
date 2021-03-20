#include "log.h"
extern const char* kLogFile;

void WriteLog(const char* fmt, ...) {
    HANDLE hfile = CreateFileA(kLogFile, FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hfile == INVALID_HANDLE_VALUE) {
        return;
    }

    char buffer[65535] = {0};
    va_list list;
    va_start(list, fmt);
    vsprintf_s(buffer, fmt, list);
    va_end(list);

    DWORD written;
    WriteFile(hfile, buffer, (DWORD)strlen(buffer), &written, NULL);
    WriteFile(hfile, "\r\n", 2, &written, NULL);

    CloseHandle(hfile);
}