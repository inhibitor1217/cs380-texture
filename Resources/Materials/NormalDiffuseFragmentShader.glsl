#version 330 core

in vec4 fragmentPosition;
in vec4 fragmentNormal;
in vec4 fragmentTangent;
in vec2 uv;

uniform sampler2D textureMap;
uniform sampler2D normalMap;
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

const vec3 ambient_illuminance = vec3(0.2, 0.2, 0.2);

void main()
{
	vec3 intensity = vec3(0);

	// Tweak normals using textures
	vec3 mappedNormal = 2.0 * texture(normalMap, uv).xyz - 1.0;
	vec3 N = normalize(fragmentNormal.xyz);
	vec3 T = normalize(fragmentTangent.xyz);
	vec3 B = cross(N, T);

	mat3 tangentToCamera;
	tangentToCamera[0] = T;
	tangentToCamera[1] = B;
	tangentToCamera[2] = N;
	vec3 normal = tangentToCamera * mappedNormal;

	vec3 n_normal = normalize(normal);
	// output_color = vec4(n_normal * 0.5 + 0.5, 1);

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
			vec3 n_half_angle      = normalize( n_light_direction - normalize(fragmentPosition.xyz) );
			
			float diffuse_factor   = clamp( dot(n_normal, n_light_direction), 0, 1);
			float specular_factor  = pow( dot(n_normal, n_half_angle), 50.0);

			intensity += (
				   diffuse_factor  * lights[i].diffuse_illuminance  * diffuse_reflectance 
			//	 + specular_factor * vec3(1, 1, 1)
			) / pow(light_length, 2);
		}
		else if(lights[i].type == 2)
		{
			// Directional Light
			vec3 n_light_direction = normalize((worldToCamera * vec4(lights[i].light_direction, 0)).xyz);
			vec3 n_half_angle      = normalize( n_light_direction - normalize(fragmentPosition.xyz) );
			
			float diffuse_factor   = clamp( dot(n_normal, n_light_direction), 0, 1);
			float specular_factor  = pow( dot(n_normal, n_half_angle), 50.0);

			intensity += (
				   diffuse_factor  * lights[i].diffuse_illuminance  * diffuse_reflectance 
			//	 + specular_factor * vec3(1, 1, 1)
			);
		}
	}
	intensity += (lights[1].enabled == 1 ? 1.5 : 1) * ambient_illuminance * diffuse_reflectance;

	output_color = vec4(intensity, 1);

	// Get color from texture
	output_color = output_color * texture(textureMap, uv);

	
}
