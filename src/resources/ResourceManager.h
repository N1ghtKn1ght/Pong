#pragma once

#include <iostream>
#include <memory>
#include <map>

using namespace std;

namespace Render
{
	class ShaderProgram;
}

class ResourceManager
{
public:
	ResourceManager(const string& executablePath);
	~ResourceManager() = default;

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(const ResourceManager&&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&&) = delete;

	shared_ptr<Render::ShaderProgram> loadShaders(const string shaderName, const string& vertexPath, const string& fragmentPath);
	shared_ptr<Render::ShaderProgram> getShaderProgram(const string shaderName);

private:
	string getFileString(const string& relativeFilePath) const;

	typedef map<const string, shared_ptr<Render::ShaderProgram>> ShaderProgramsMap;
	ShaderProgramsMap m_shaderPrograms;
	string m_path;
};

