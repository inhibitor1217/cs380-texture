#version 330 core

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 uv_in;

uniform mat4 worldTransform;
uniform mat4 cameraTransform;
uniform mat4 projectionMatrix;
uniform vec4 color;

out vec4 fragmentPosition;
out vec4 frag_color;
out vec4 fragmentNormal;
out vec2 uv;

mat4 NormalMatrix(mat4 MVM)
{
	mat4 invm = inverse(MVM);
	invm[3][0] = 0;
	invm[3][1] = 0;
	invm[3][2] = 0;

	return transpose(invm);
}


void main()
{
	uv =uv_in;
	// Output position of the vertex, in clip space : MVP * position
	mat4 MVM = inverse(cameraTransform) * worldTransform;
	mat4 NVM = NormalMatrix(MVM);

	vec4 wPosition = MVM * vec4(pos);
	fragmentPosition = wPosition;
	fragmentNormal = NVM * normal;
	gl_Position = projectionMatrix * wPosition;
	
	frag_color = color;
}
