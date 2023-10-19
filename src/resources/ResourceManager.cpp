#include "ResourceManager.h"
#include "../render/ShaderProgram.h"


#include <sstream>
#include <fstream>

ResourceManager::ResourceManager(const string& executablePath)
{
	size_t found = executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, found);
}

shared_ptr<Render::ShaderProgram> ResourceManager::loadShaders(const string shaderName, const string& vertexPath, const string& fragmentPath)
{
	string vertexString = getFileString(vertexPath);
	if (vertexString.empty())
	{
		cerr << "No vertex shader" << endl;
		return nullptr;
	}

	string fragmentString = getFileString(fragmentPath);
	if (fragmentString.empty())
	{
		cerr << "No fragment shader" << endl;
		return nullptr;
	}

	shared_ptr<Render::ShaderProgram>& newShader = m_shaderPrograms.emplace(shaderName, make_shared<Render::ShaderProgram>(vertexString, fragmentString)).first->second;
	if (newShader->isCompiled())
	{
		return newShader;
	}
	cerr << "Cant compiled shader " << endl;

	return nullptr;
}

shared_ptr<Render::ShaderProgram> ResourceManager::getShaderProgram(const string shaderName)
{
	ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
	if (it != m_shaderPrograms.end())
	{
		return it->second;
	}
	cerr << "Cant find the shader by name: " << shaderName << endl;
	return nullptr;
}

string ResourceManager::getFileString(const string& relativeFilePath) const
{
	ifstream f;
	f.open(m_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open())
	{
		cerr << "Faild to open file: " << relativeFilePath << endl;
		return string{};
	}

	stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();

}
