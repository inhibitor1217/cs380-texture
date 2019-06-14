#include <ShadowCastingObject.hpp>
#include <glm/glm.hpp>

ShadowCastingObject::ShadowCastingObject(Engine::Mesh* mesh, Engine::Material* material) : Engine::RenderObject(mesh, material)
{

}

void ShadowCastingObject::RenderShadow(Light light)
{
	GLuint shadowShaderProgramID = _shadowMat->GetProgram()->GetProgramId();
	glUseProgram(shadowShaderProgramID);

	if (light.type == LightType::DirectionalLight)
	{
		glm::mat4 objectWorldTransform = GetTransform()->GetWorldTransform();
		glUniformMatrix4fv(glGetUniformLocation(shadowShaderProgramID, "worldTransform"), 1, GL_FALSE, (float *)&objectWorldTransform);

		glm::mat4 lightTransform = glm::inverse(glm::lookAt(
			10.0f * light.light_direction,
			glm::vec3(0),
			glm::vec3(0, 1, 0)
		));
		glUniformMatrix4fv(glGetUniformLocation(shadowShaderProgramID, "cameraTransform"), 1, GL_FALSE, (float *)&lightTransform);

		glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 1.0f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(shadowShaderProgramID, "projectionMatrix"), 1, GL_FALSE, (float *)&lightProjection);
	}

	GLenum drawMode = _mesh->GetDrawMode();
	int numElements = _mesh->GetNumElements();

	if (drawMode == GL_FALSE)
	{
		std::cout << "Cannot render this object with undefined behaviour\n";
		throw std::exception();
	}

	if (numElements == 0)
	{
		std::cout << "Cannot render this object with zero elements\n";
		throw std::exception();
	}

	glBindVertexArray(_mesh->GetVao());

	if (_mesh->GetHasIndex())
	{
		glBindBuffer(GL_ARRAY_BUFFER, _mesh->GetVbo());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->GetIbo());
		glDrawElements(drawMode, numElements, GL_UNSIGNED_INT, 0);
	}
	else
	{

		glBindBuffer(GL_ARRAY_BUFFER, _mesh->GetVbo());
		glDrawArrays(drawMode, 0, numElements);
	}

	glBindVertexArray(0);
}