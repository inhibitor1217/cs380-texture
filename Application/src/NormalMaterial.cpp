#include <NormalMaterial.hpp>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define BUF_SIZE 50

//TODO
void NormalMaterial::CreateMaterial(int unit, int normal, int depth)
{
	_program->AttachShader("Resources\\Materials\\NormalVertexShader.glsl", GL_VERTEX_SHADER);
	_program->AttachShader("Resources\\Materials\\NormalDiffuseFragmentShader.glsl", GL_FRAGMENT_SHADER);
	_program->LinkShader();

	glUseProgram(_program->GetProgramId());
	GLuint location = glGetUniformLocation(_program->GetProgramId(), "textureMap");
	glUniform1i(location, unit);

	location = glGetUniformLocation(_program->GetProgramId(), "normalMap");
	glUniform1i(location, normal);

	location = glGetUniformLocation(_program->GetProgramId(), "depthMap");
	glUniform1i(location, depth);
}

void NormalMaterial::UpdateDiffuseReflectance(glm::vec3 color)
{
	glUseProgram(_program->GetProgramId());
	GLuint location = glGetUniformLocation(_program->GetProgramId(), "diffuse_reflectance");
	glUniform3fv(location, 1, (float*)&color);
}

void NormalMaterial::UpdateLight(std::vector<Light> &lights)
{
	GLuint pid = _program->GetProgramId();
	glUseProgram(pid);

	int numLights = MIN(lights.size(), MAX_LIGHTS);
	GLuint location = glGetUniformLocation(pid, "numLights");
	glUniform1i(location, numLights);

	char buf[BUF_SIZE];
	for (int i = 0; i < numLights; i++)
	{
		snprintf(buf, BUF_SIZE, "lights[%d].diffuse_illuminance", i);
		location = glGetUniformLocation(pid, buf);
		glUniform3fv(location, 1, (float*)&(lights[i].diffuse_illuminance));

		snprintf(buf, BUF_SIZE, "lights[%d].light_direction", i);
		location = glGetUniformLocation(pid, buf);
		glUniform3fv(location, 1, (float*)&(lights[i].light_direction));

		snprintf(buf, BUF_SIZE, "lights[%d].type", i);
		location = glGetUniformLocation(pid, buf);
		glUniform1iv(location, 1, (int*)&(lights[i].type));

		glm::mat4 world_transform = lights[i].transform->GetWorldTransform();
		glm::vec4 local_pos = glm::vec4(0.0, 0.0, 0.0, 1.0);
		glm::vec4 world_pos = world_transform * local_pos;
		snprintf(buf, BUF_SIZE, "lights[%d].pos", i);
		location = glGetUniformLocation(pid, buf);
		glUniform3fv(location, 1, (float*)&(world_pos));

		snprintf(buf, BUF_SIZE, "lights[%d].enabled", i);
		location = glGetUniformLocation(pid, buf);
		glUniform1iv(location, 1, (int*)&(lights[i].enabled));

		if (lights[i].type == LightType::DirectionalLight)
		{
			glm::mat4 lightTransform = glm::inverse(glm::lookAt(
				10.0f * lights[i].light_direction,
				glm::vec3(0),
				glm::vec3(0, 1, 0)
			));
			glUniformMatrix4fv(glGetUniformLocation(pid, "lightTransform"), 1, GL_FALSE, (float *)&lightTransform);

			glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 1.0f, 100.0f);
			glUniformMatrix4fv(glGetUniformLocation(pid, "lightProjection"), 1, GL_FALSE, (float *)&lightProjection);
		}

	}
}

