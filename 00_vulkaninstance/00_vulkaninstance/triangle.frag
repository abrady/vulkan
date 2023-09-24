//glsl version 4.5
#version 450

layout(set = 0, binding = 0) uniform vec4 inputColor;

//output write
layout (location = 0) out vec4 outFragColor;

void main()
{
	//return red
	outFragColor = vec4(1.f,0.f,0.f,1.0f);
}
