#ifndef _R_GL_SHADER_H_
#define _R_GL_SHADER_H_

#include <glad/glad.h>

#define GLM_FORCE_RADIANS
#include "dependencies/glm/glm.hpp"
#include "dependencies/glm/ext.hpp"

#include <string>

class Shader {
public:
	
	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();
	void Activate();
	GLuint Program() const;

	void SetViewProjMatrices(glm::mat4 view, glm::mat4 proj);
	void DrawWireframe(uint32_t yesOrNo);

private:
	bool CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);
	bool IsCompiled(GLuint shader);
	bool IsValidProgram();

	GLuint m_VertexShader;
	GLuint m_FragmentShader;
	GLuint m_ShaderProgram;

	GLuint m_ViewProjUniformIndex;
	GLuint m_SettingsUniformIndex;
	GLuint m_ViewProjUBO;
	GLuint m_SettingsUBO;
};


#endif