#pragma once
#include <unordered_set>
#include <string>

#define WHITE_LIST
extern const char* kOutputDir;
extern const char* kLogFile;

#ifdef WHITE_LIST
extern const std::unordered_set<std::string> kDumpAssemblies;
#endif
