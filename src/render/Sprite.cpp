#include "Sprite.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Render
{
	Sprite::Sprite(const shared_ptr<Texture2D> pTexture, const shared_ptr<ShaderProgram> pShaderProgram, const glm::vec2& position, const glm::vec2& size, const float rotate)
		: m_Texture(pTexture),
		m_ShaderProgram(pShaderProgram),
		m_position(position),
		m_size(size),
		m_rotate(rotate)
	{
		const GLfloat vertexCoords[] =
		{
			 0.f, 0.f,
			 0.f, 1.f,
			 1.f, 1.f,

			 1.f, 1.f,
			 1.f, 0.f,
			 0.f, 0.f,
		};

		const GLfloat textureCoords[] =
		{
			// U  V
			 0.f, 0.f,
			 0.f, 1.f,
			 1.f, 1.f,

			 1.f, 1.f,
			 1.f, 0.f,
			 0.f, 0.f,
		};

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_vertexCoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoords), &vertexCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		glGenBuffers(1, &m_texutureCoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_texutureCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), &textureCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	Sprite::~Sprite()
	{
		glDeleteBuffers(1, &m_vertexCoordsVBO);
		glDeleteBuffers(1, &m_texutureCoordsVBO);
		glDeleteVertexArrays(1, &m_VAO);
	}

	void Sprite::render() const
	{
		m_ShaderProgram->use();

		glm::mat4 model(1.f);

		model = glm::translate(model, glm::vec3(m_position, 0.f));
		model = glm::translate(model, glm::vec3(0.5f * m_size.x, 0.5f * m_size.y, 0.f));
		model = glm::rotate(model, glm::radians(m_rotate), glm::vec3(0.f, 0.f, 1.f));
		model = glm::translate(model, glm::vec3(-0.5f * m_size.x, -0.5f * m_size.y, 0.f));
		model = glm::scale(model, glm::vec3(m_size, 1.f));

		glBindVertexArray(m_VAO);
		m_ShaderProgram->setMat4("modelMat", model);
		glActiveTexture(GL_TEXTURE0);
		m_Texture->bind();

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	void Sprite::setPosition(const glm::vec2& position)
	{
		m_position = position;
	}

	void Sprite::setSize(const glm::vec2& size)
	{
		m_size = size;
	}

	void Sprite::setRotation(const float rotate)
	{
		m_rotate = rotate;
	}
}