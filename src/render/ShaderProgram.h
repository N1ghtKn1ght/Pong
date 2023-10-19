#pragma once

#include <glad/glad.h>
#include <iostream>


using namespace std;

namespace Render 
{
	class ShaderProgram
	{
	public:
		ShaderProgram(const string vertexShader, const string fragmentShader);
		~ShaderProgram();
		bool isCompiled() const { return m_isCompiled; }
		void use() const;

		ShaderProgram() = delete;
		ShaderProgram(ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram& operator=(ShaderProgram&& shaderProgram);
		ShaderProgram(ShaderProgram&& shaderProgram);


	private:
		bool m_isCompiled = false;
		GLuint m_ID = 0;
		bool createShader(const string& source, const GLenum shaderType, GLuint& shaderID);
	};
}

