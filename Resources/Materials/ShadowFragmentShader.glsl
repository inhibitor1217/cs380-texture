#version 330 core

in vec2 uv;

uniform sampler2D textureMap;

void main()
{             
    if (texture(textureMap, uv).a < 0.1)
		discard;
}  