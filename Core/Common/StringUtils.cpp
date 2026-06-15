#include "StringUtils.h"

std::string GetDirectoryFromPath(const std::string& filepath)
{
	size_t found = filepath.find_last_of("/\\");
	return (found != std::string::npos) ? filepath.substr(0, found + 1) : "";
}

std::string GetFilenameWithoutExtension(const std::string& path)
{
	auto lastOfIndex = path.find_last_of('/');
	auto fullFilename = path.substr(++lastOfIndex);
	lastOfIndex = fullFilename.find_last_of('.');
	auto filename = fullFilename.substr(0, lastOfIndex);
	return filename;
}

std::vector<std::string> getWords(const std::string& s, const char* delim) {
	std::vector<std::string> tokens;
	size_t start = 0, end;
	while ((end = s.find(delim, start)) != std::string::npos) {
		if (end != start)
			tokens.push_back(s.substr(start, end - start));
		start = end + strlen(delim);
	}
	if (start < s.length())
		tokens.push_back(s.substr(start));
	return tokens;
}