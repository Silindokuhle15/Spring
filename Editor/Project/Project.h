#pragma once
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>

void LogError(std::string error, std::string func_call, std::string param);
template<class ProjectType>
class ProjectTemplate
{
public:
	bool Create()
	{
		m_CmakeFile = std::fstream(m_CmakeFilepath, std::ios::out | std::ios::binary);
		if (!m_CmakeFile.is_open())
		{
			LogError("Failed To Create", "Project:Create()", m_CmakeFilepath);
			return false;
		}
		return true;
	}
	virtual bool Open() = 0;
	virtual bool Close() = 0;

protected:

	std::ios::openmode open_flags;
	std::string m_CmakeFilepath;
	std::fstream m_CmakeFile;
};


class Project : public ProjectTemplate<Project>
{
public:
	Project(std::string file_path);

	bool Open() override;
	bool Close() override;
};