#include <ShadowMaterial.hpp>

void ShadowMaterial::CreateMaterial()
{
	_program->AttachShader("Resources\\Materials\\ShadowVertexShader.glsl", GL_VERTEX_SHADER);
	_program->AttachShader("Resources\\Materials\\ShadowFragmentShader.glsl", GL_FRAGMENT_SHADER);
	_program->LinkShader();
}