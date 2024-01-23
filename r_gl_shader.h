#ifndef _R_GL_SHADER_H_
#define _R_GL_SHADER_H_

#include <glad/glad.h>

#include <string>

class Shader {
public:
	
	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();
	void Activate();
	GLuint Program() const;

private:
	bool CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);
	bool IsCompiled(GLuint shader);
	bool IsValidProgram();

	GLuint m_VertexShader;
	GLuint m_FragmentShader;
	GLuint m_ShaderProgram;
};


#endif