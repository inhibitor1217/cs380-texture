#pragma once

#include <Light.hpp>
#include <Material.hpp>

class NormalMaterial : public Engine::Material
{
public:
	void CreateMaterial(int unit, int normal, int depth);
	void UpdateDiffuseReflectance(glm::vec3 color);
	void UpdateLight(std::vector<Light> &lights);
};
