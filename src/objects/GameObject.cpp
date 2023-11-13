#include "GameObject.h"



#include "../render/Sprite.h"

Object::GameObject::GameObject(ResourceManager& pResourceManager, const string& spriteName, const string& textureName, const string& shaderName, const glm::vec2& pPosition, const glm::vec2& pSize, const float rotate)
	: m_sprite(NULL),
	m_position(pPosition),
	m_size(pSize),
	m_rotate(rotate)
{
	m_sprite = pResourceManager.loadSprite(spriteName, textureName, shaderName, m_size.x, m_size.y);
	m_sprite->setPosition(m_position);
	m_sprite->setRotation(m_rotate);
}

void Object::GameObject::render()
{
	m_sprite->setPosition(m_position);
	m_sprite->render();
}

