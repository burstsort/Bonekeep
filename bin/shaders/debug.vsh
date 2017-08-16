#version 330 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec3 color;

out vec3 _color;

void main()
{
	_color = color;
	gl_Position = vec4(-1.0+pos.x, 1.0-pos.y, 0.0, 1.0);
}