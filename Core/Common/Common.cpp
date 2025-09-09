#include "Common.h"

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