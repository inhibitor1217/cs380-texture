#include <CubeMaterial.hpp>

void CubeMaterial::CreateMaterial(int unit)
{
	_program->AttachShader("Resources\\Materials\\CubeVertexShader.glsl", GL_VERTEX_SHADER);
	_program->AttachShader("Resources\\Materials\\CubeFragmentShader.glsl", GL_FRAGMENT_SHADER);
	_program->LinkShader();

	glUseProgram(_program->GetProgramId());
	GLuint location = glGetUniformLocation(_program->GetProgramId(), "skybox");
	glUniform1i(location, unit);
}

void CubeMaterial::IsSkybox(int is_skybox)
{
	glUseProgram(_program->GetProgramId());
	GLuint location = glGetUniformLocation(_program->GetProgramId(), "is_skybox");
	glUniform1i(location, is_skybox);
}

