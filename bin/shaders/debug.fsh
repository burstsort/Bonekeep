#version 330 core

in vec3 _color;
out vec4 fragment;

void main()
{
	fragment = vec4(_color, 1.0);
}