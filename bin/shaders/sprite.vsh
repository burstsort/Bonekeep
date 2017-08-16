#version 330 core

layout(location = 0) in vec2 quad;

layout(location = 1) in vec2 offset;
layout(location = 2) in vec2 pivot;
layout(location = 3) in vec2 size;

layout(location = 4) in vec3 position;
layout(location = 5) in vec2 scale;
layout(location = 6) in float angle;

layout(location = 7) in vec4 color;

uniform vec2 textureSize;

out vec2 _uv;
out vec4 _color;

void main()
{
	_uv = vec2(0.0, 1.0) + vec2(1.0, -1.0)*(quad*size + offset)/(textureSize);
	_color = color;
	
	mat4 T = mat4(1.0);

	T = mat4(
		size.x, 0.0, 0.0, 0.0,
		0.0, -size.y, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0 
	) * T;

	T = mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		-pivot.x, +pivot.y, 0.0, 1.0 
	) * T;

	T = mat4(
		scale.x, 0.0, 0.0, 0.0,
		0.0, scale.y, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	) * T;

	T = mat4(
		cos(angle), -sin(angle), 0.0, 0.0,
		sin(angle), cos(angle), 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	) * T;

	T = mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		-1.0+position.x, 1.0-position.y, position.z, 1.0 
	) * T;
	
	gl_Position = mat4(T)*vec4(quad, 0.0, 1.0);
}