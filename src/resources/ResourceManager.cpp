#include "ResourceManager.h"
#include "../render/ShaderProgram.h"
#include "../render/Texture2D.h"
#include "../render/Sprite.h"


#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

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
	cerr << "Cant compiled shader " << shaderName << endl;

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

shared_ptr<Render::Texture2D> ResourceManager::loadTexture(const string& textureName, const string& texturePath)
{
	int channels = 0;
	int width = 0;
	int height = 0;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* pixels = stbi_load(string(m_path + "/" + texturePath).c_str(), &width, &height, &channels, 0);

	if (!pixels)
	{
		cerr << "Cant load image: " << texturePath << endl;
		return nullptr;
	}


	shared_ptr<Render::Texture2D> newTexture = m_textures.emplace(textureName, 
		make_shared<Render::Texture2D>(width, height, pixels, channels, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;
	stbi_image_free(pixels);
	return newTexture;

}

shared_ptr<Render::Texture2D> ResourceManager::getTexture(const string& textureName)
{
	Texture2DMap::const_iterator it = m_textures.find(textureName);
	if (it != m_textures.end())
	{
		return it->second;
	}
	cerr << "Cant find the texture by name: " << textureName << endl;
	return nullptr;
}

shared_ptr<Render::Sprite> ResourceManager::loadSprite(const string& spriteName, 
														const string& textureName, 
														const string& shaderName, 
														const unsigned int spriteW, 
														const unsigned int spriteH)
{
	auto pTextute = getTexture(textureName);
	if (!pTextute)
	{
		cerr << "Cant find the texture by name: " << textureName << "for the sprite: " << spriteName << endl;
	}
	auto pShader = getShaderProgram(shaderName);
	if (!pShader)
	{
		cerr << "Cant find the shader by name: " << shaderName << "for the sprite: " << spriteName << endl;
	}

	shared_ptr<Render::Sprite> newSprite = m_sprites.emplace(textureName,
		make_shared<Render::Sprite>(pTextute, pShader, glm::vec2(0.f, 0.f), glm::vec2(spriteW, spriteH))).first->second;

	return newSprite;
}

shared_ptr<Render::Sprite> ResourceManager::getSprite(const string& spriteName)
{
	SpritesMap::const_iterator it = m_sprites.find(spriteName);
	if (it != m_sprites.end())
	{
		return it->second;
	}
	cerr << "Cant find the shader by name: " << spriteName << endl;
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
