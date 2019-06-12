#version 330 core

in vec4 fragmentPosition;
in vec4 fragmentNormal;
in vec2 uv;

// uniform sampler2D ourTexture;
out vec4 output_color;

struct Light
{
	int type;
	int enabled;
	vec3 diffuse_illuminance;
	vec3 pos;
	vec3 light_direction;
};

uniform mat4 cameraTransform;
uniform int numLights;
uniform Light lights[3];

uniform vec3 diffuse_reflectance;

const vec3 ambient_illuminance = vec3(0.1, 0.1, 0.1);

void main()
{
	vec3 intensity = vec3(0);
	vec3 n_normal = normalize(fragmentNormal.xyz);

	mat4 worldToCamera = inverse(cameraTransform);
	for (int i=0; i<numLights; i++)
	{
		if (lights[i].enabled == 0)
		{
			continue;
		}

		if (lights[i].type == 1)
		{
			// Point Light
			vec3 light_pos         = (worldToCamera * vec4(lights[i].pos, 1)).xyz;
			vec3 light_direction   = light_pos - fragmentPosition.xyz;
			float light_length     = length(light_direction);
			vec3 n_light_direction = normalize(light_direction);
			
			float diffuse_factor   = clamp( dot(n_normal, n_light_direction), 0, 1);

			intensity += (
				   diffuse_factor  * lights[i].diffuse_illuminance  * diffuse_reflectance 
			) / pow(light_length, 2);
		}
		else if(lights[i].type == 2)
		{
			// Directional Light
			vec3 n_light_direction = normalize((worldToCamera * vec4(lights[i].light_direction, 0)).xyz);

			float diffuse_factor   = clamp( dot(n_normal, n_light_direction), 0, 1);

			intensity += (
				   diffuse_factor  * lights[i].diffuse_illuminance  * diffuse_reflectance 
			);
		}
		else if(lights[i].type == 3)
		{
			// Spotlight
			vec3 light_pos         = (worldToCamera * vec4(lights[i].pos, 1)).xyz;
			vec3 n_light_vector    = normalize(light_pos - fragmentPosition.xyz);
			vec3 n_light_direction = normalize((worldToCamera * vec4(lights[i].light_direction, 0)).xyz);

			float diffuse_factor   = clamp( dot(n_normal, n_light_vector), 0, 1);
			
			float attenuation      = pow( dot(n_light_vector, n_light_direction), 50.0 );
			intensity += (
				   diffuse_factor  * lights[i].diffuse_illuminance  * diffuse_reflectance 
			) * attenuation;
		}
	}
	intensity += ambient_illuminance * diffuse_reflectance;

	output_color = vec4(intensity, 1);

	// Get color from texture
	// output_color = output_color * texture(ourTexture, uv);

	
}
