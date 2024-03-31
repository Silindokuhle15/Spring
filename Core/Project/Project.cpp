#include "Project.h"

Project::Project(std::string file_path)
{
	open_flags =  std::ios::in | std::ios::out | std::ios::binary;
	m_CmakeFilepath = file_path;
	auto created = Create();
}

bool Project::Open()
{
	auto path = std::filesystem::path(m_CmakeFilepath);
	if (std::filesystem::is_regular_file(path))
	{
		m_CmakeFile = std::fstream(m_CmakeFilepath, open_flags);
	}
	else
	{
		LogError("Failed to Open", "Project:Open()", m_CmakeFilepath);
		return false;
	}
}

bool Project::Close()
{
	return false;
}

void LogError(std::string error, std::string func_call, std::string param)
{
	std::cerr << error << "in" << func_call << "with parameters : " << param << std::endl;
	std::flush(std::cerr);
}

