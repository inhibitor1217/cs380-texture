#pragma once

#include <glm/glm.hpp>

#include <Transform.hpp>

#define MAX_LIGHTS 3
// Feel free to increase maximum number of lights!

enum LightType : int
{
	PointLight = 1,
	DirectionalLight = 2,
	Spotlight = 3,
};

// Support point light and directional light.
struct Light
{
	LightType type;
	int enabled = 1;		// Your light should be able to turn on/off independently!

	Engine::Transform transform;
	glm::vec3 light_direction;

	glm::vec3 diffuse_illuminance;
};
