#pragma once

#include <iostream>
#include <memory>
#include <map>

using namespace std;

namespace Render
{
	class ShaderProgram;
	class Texture2D;
	class Sprite;
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

	shared_ptr<Render::Texture2D> loadTexture(const string& textureName, const string& texturePath);
	shared_ptr<Render::Texture2D> getTexture(const string& textureName);

	shared_ptr<Render::Sprite> loadSprite(const string& spriteName, 
										const string& textureName, 
										const string& shaderName, 
										const unsigned int spriteW, 
										const const unsigned int spriteH);
	shared_ptr<Render::Sprite> getSprite(const string& spriteName);


private:
	string getFileString(const string& relativeFilePath) const;

	typedef map<const string, shared_ptr<Render::ShaderProgram>> ShaderProgramsMap;
	ShaderProgramsMap m_shaderPrograms;

	typedef map<const string, shared_ptr<Render::Texture2D>> Texture2DMap;
	Texture2DMap m_textures;

	typedef map<const string, shared_ptr<Render::Sprite>> SpritesMap;
	SpritesMap m_sprites;

	string m_path;
};

