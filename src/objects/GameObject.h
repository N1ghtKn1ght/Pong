#pragma once

#include <ostream>
#include <memory>


#include <glm/vec2.hpp>

#include "../resources/ResourceManager.h"

using namespace std;

namespace Render
{
	class Sprite;
}

namespace Object
{
	class GameObject 
	{
	public:
		glm::vec2 m_position;
		glm::vec2 m_size;

		GameObject(ResourceManager& pResourceManager,
			const string& spriteName,
			const string& textureName,
			const string& shaderName,
			const glm::vec2& pPosition,
			const glm::vec2& pSize,
			const float rotate);
		void render();

	protected:
		shared_ptr<Render::Sprite> m_sprite;
		float m_rotate;
	};
}