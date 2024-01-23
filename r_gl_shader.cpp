#include "r_gl_shader.h"

#include <glad/glad.h>

#include <stdio.h>

#define GLM_FORCE_RADIANS
#include "dependencies/glm/glm.hpp"
#include "dependencies/glm/ext.hpp"

#include "platform.h"

bool Shader::Load(const std::string& vertName, const std::string& fragName)
{
	if (!CompileShader(vertName, GL_VERTEX_SHADER, m_VertexShader)
		|| !CompileShader(fragName, GL_FRAGMENT_SHADER, m_FragmentShader)) {

		return false;
	}

	m_ShaderProgram = glCreateProgram();
	glAttachShader(m_ShaderProgram, m_VertexShader);
	glAttachShader(m_ShaderProgram, m_FragmentShader);
	glLinkProgram(m_ShaderProgram);

	if (!IsValidProgram()) return false;

	// Uniforms
	GLuint bindingPoint = 0;
	m_ViewProjUniformIndex  = glGetUniformBlockIndex(m_ShaderProgram, "ViewProjMatrices");
	if (m_ViewProjUniformIndex == GL_INVALID_INDEX) {
		printf("Not able to get index for UBO in shader program.\nShaders:\n %s\n %s\n", vertName.c_str(), fragName.c_str());
	}
	glUniformBlockBinding(m_ShaderProgram, m_ViewProjUniformIndex, bindingPoint);

	glGenBuffers(1, &m_ViewProjUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, m_ViewProjUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, m_ViewProjUBO, 0, 2 * sizeof(glm::mat4));	

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return true;
}

void Shader::Unload()
{
	glDeleteProgram(m_ShaderProgram);
	glDeleteShader(m_VertexShader);
	glDeleteShader(m_FragmentShader);
}

void Shader::Activate()
{
	glUseProgram(m_ShaderProgram);
}

GLuint Shader::Program() const
{
	return m_ShaderProgram;
}

void Shader::SetViewProjMatrices(glm::mat4 view, glm::mat4 proj)
{	
	glBindBuffer(GL_UNIFORM_BUFFER, m_ViewProjUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(proj));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

bool Shader::CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader)
{
	HKD_File shaderCode;
	if (hkd_read_file(fileName.c_str(), &shaderCode) != HKD_FILE_SUCCESS) {
		printf("Could not read file: %s!\n", fileName.c_str());
		return false;
	}

	outShader = glCreateShader(shaderType);
	glShaderSource(outShader, 1, (GLchar**)(&shaderCode.data), nullptr);
	glCompileShader(outShader);

	if (!IsCompiled(outShader)) {
		printf("Failed to compile shader: %s\n", fileName.c_str());
		return false;
	}

	return true;
}

bool Shader::IsCompiled(GLuint shader)
{
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, buffer);
		printf("GLSL compile error:\n%s\n", buffer);

		return false;
	}

	return true;
}

bool Shader::IsValidProgram()
{
	GLint status;
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(m_ShaderProgram, 511, nullptr, buffer);
		printf("GLSL compile error:\n%s\n", buffer);

		return false;
	}

	return true;
}
