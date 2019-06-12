#version 330 core

in vec4 fragmentPosition;
in vec4 frag_color;
in vec4 fragmentNormal;
in vec2 uv;

uniform sampler2D ourTexture;
out vec4 output_color;

struct Light
{
	int type;
	vec3 diffuse_illuminance;
	vec3 pos;
	vec3 dir;
	int enabled;
};

uniform mat4 cameraTransform;
uniform int numLights;
uniform Light lights[3];

uniform vec3 diffuse_reflectance;

void main()
{
	vec3 intensity = vec3(0);
	vec3 normal = normalize(fragmentNormal.xyz);

	mat4 worldToCamera = inverse(cameraTransform);
	for (int i=0; i<numLights; i++)
	{
		if (lights[i].enabled == 0)
		{
			continue;
		}
		
		vec3 tolight = vec3(0);
		vec4 pos = worldToCamera * vec4(lights[i].pos, 1);
		vec3 dir = -normalize((worldToCamera * vec4(lights[i].dir, 0)).xyz);
		if (lights[i].type == 1)
		{
			tolight = normalize(pos.xyz - fragmentPosition.xyz);
		}
		else if (lights[i].type == 2)
		{
			tolight = dir;
		}
		else
		{
			continue;
		}
		
		float diffuse = max(0, dot(normal, tolight));
		
		intensity += diffuse_reflectance * lights[i].diffuse_illuminance * diffuse;
		}

	output_color = vec4(intensity, 1);
	
	// Get color from texture
	output_color = output_color * texture(ourTexture, uv);

	output_color.rgb = pow(output_color.rgb, vec3(1.0/2.2));

	
}
