#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <RenderObject.hpp>
#include <Mesh.hpp>
#include <Material.hpp>
#include <ShadowMaterial.hpp>
#include <Light.hpp>

class ShadowCastingObject : public Engine::RenderObject
{
private:
	ShadowMaterial* _shadowMat;

public:
	ShadowCastingObject(Engine::Mesh* mesh, Engine::Material* material);
	void RenderShadow(Light light);
	void SetShadowMat(ShadowMaterial* mat) { _shadowMat = mat; };
};