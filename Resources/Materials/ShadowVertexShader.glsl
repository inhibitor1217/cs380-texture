#version 330 core

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 uv_in;
layout(location = 3) in vec4 tangent;

uniform mat4 worldTransform;
uniform mat4 cameraTransform;
uniform mat4 projectionMatrix;

out vec2 uv;

void main()
{
	uv = uv_in;
	gl_Position = projectionMatrix * inverse(cameraTransform) * worldTransform * pos;
}
