#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include <memory>


using namespace std;

namespace Render
{
	class Texture2D;
	class ShaderProgram;
	class Sprite
	{
	public:
		Sprite(const shared_ptr<Texture2D> pTexture, const shared_ptr<ShaderProgram> pShaderProgram, 
			const glm::vec2& position = glm::vec2(0.f), const glm::vec2& size = glm::vec2(1.f), const float rotate = 0.f);
		~Sprite();

		Sprite(const Sprite&) = delete;
		Sprite& operator =(const Sprite&) = delete;

		void render() const;
		void setPosition(const glm::vec2& position);
		void setSize(const glm::vec2& size);
		void setRotation(const float rotate);
		glm::vec2 getPosition();

	private:
		shared_ptr<Texture2D> m_Texture;
		shared_ptr<ShaderProgram> m_ShaderProgram;
		glm::vec2 m_position;
		glm::vec2 m_size;
		float m_rotate;
		GLuint m_vertexCoordsVBO;
		GLuint m_VAO;
		GLuint m_texutureCoordsVBO;
	};
}