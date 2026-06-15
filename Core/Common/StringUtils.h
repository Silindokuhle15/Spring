#ifndef _STRING_UTILS_H_
#define _STRING_UTILS_H_
#include <string>
#include <vector>
std::string GetDirectoryFromPath(const std::string& filepath);

std::string GetFilenameWithoutExtension(const std::string& path);

std::vector<std::string> getWords(const std::string& s, const char* delim);
#endif // !_STRING_UTILS_H_
