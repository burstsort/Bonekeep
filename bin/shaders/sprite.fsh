#version 330 core

in vec2 _uv;
in vec4 _color;

uniform sampler2D TEXTURE;

out vec4 fragment;

void main()
{
	vec4 texel = texture(TEXTURE, _uv); // vec4(_color.rgb*_color.a, _color.a);
	
	if (texel.a < 0.5)
    	discard;

	fragment = texel;
}