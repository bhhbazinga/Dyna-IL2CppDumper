#pragma once
#include <windows.h>
#include <Shlwapi.h>
#include <cstdio>
#include <string>

void WriteLog(const char* fmt, ...);

#define LOG(level, fmt, ...) \
do { \
SYSTEMTIME sys_time; \
GetLocalTime(&sys_time); \
char time[128] = {0}; \
snprintf(time, 128, "%04ld-%02ld-%02ld %02ld:%02ld:%02ld:%03ld", \
                    sys_time.wYear, \
                    sys_time.wMonth, \
                    sys_time.wDay, \
                    sys_time.wHour, \
                    sys_time.wMinute, \
                    sys_time.wSecond, \
                    sys_time.wMilliseconds); \
std::string file = PathFindFileNameA(__FILE__); \
WriteLog("[%s][%s]%s<%s>+%d: "  \
                   fmt, \
                   level, \
                   time, \
                   file.c_str(), \
                   __FUNCTION__, \
                   __LINE__, \
                   __VA_ARGS__); \
} while (0)

#define LOG_DBG(fmt, ...) LOG("DEBUG", fmt, __VA_ARGS__)
#define LOG_INFO(fmt, ...) LOG("INFO", fmt, __VA_ARGS__)
#define LOG_WAR(fmt, ...) LOG("WAR", fmt, __VA_ARGS__)
#define LOG_ERR(fmt, ...) LOG("ERR", fmt, __VA_ARGS__)
