#pragma once

#include <Light.hpp>
#include <Material.hpp>

class CubeMaterial : public Engine::Material
{
public:
    void CreateMaterial(int unit);
	void IsSkybox(int is_skybox);
};
