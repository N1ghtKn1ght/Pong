#pragma once 

#include <memory>
#include <ostream>

#include "GameObject.h"
#include "../resources/ResourceManager.h"

using namespace std;

namespace Render 
{
	class ResourceManager;
}

namespace Object
{
	class BallObject : public GameObject
	{
	public:
		glm::vec2 m_velocity;

		BallObject(ResourceManager& pResourceManager,
			const string& spriteName,
			const string& textureName,
			const string& shaderName,
			const glm::vec2& pPosition,
			const glm::vec2& pSize,
			const float rotate, glm::vec2& velocity = glm::vec2(5,5)) :
			GameObject(pResourceManager, spriteName, textureName, shaderName, pPosition, pSize, rotate),
			m_velocity(velocity)
		{
			
		}
		void move(const glm::vec2& pWindowSize);
		void setVelocity(const glm::vec2& velocity);

	};

}