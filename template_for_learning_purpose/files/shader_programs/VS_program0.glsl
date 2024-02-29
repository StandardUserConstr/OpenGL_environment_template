#version 330 core

layout (location = 0) in vec3 Vpos;
layout (location = 1) in vec3 Vcolor;

out vec3 Scolor;

void main()
{
	gl_Position =  vec4(Vpos.x, Vpos.y, Vpos.z, 1.0);
	Scolor = Vcolor;
}
