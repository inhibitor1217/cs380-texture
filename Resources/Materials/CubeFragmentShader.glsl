#version 330 core

in vec4 fragmentPosition;
in vec4 fragmentNormal;
in vec3 Texcoords;

uniform samplerCube skybox;
out vec4 output_color;

uniform mat4 cameraTransform;
uniform bool is_skybox;

void main()
{
	output_color = vec4(0);
	if(is_skybox)
	{
		output_color = texture(skybox, Texcoords);	
	}
	else
	{
		vec3 V = normalize(vec3(fragmentPosition));
		vec3 R = reflect(V, normalize(vec3(fragmentNormal)));
		R = vec3(cameraTransform*vec4(R, 0.0));
		output_color = texture(skybox, R);
		
	}
	
}
