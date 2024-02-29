#version 330 core

layout (location = 0) in vec3 Vpos;
layout (location = 1) in vec3 Vcolor;
layout (location = 2) in vec2 VTexCoord;

out vec3 Scolor;
out vec2 STexCoord;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

void main()
{
	gl_Position =  projection*camera*model*vec4(Vpos.x, Vpos.y, Vpos.z, 1.0);
	Scolor = Vcolor;
	STexCoord = VTexCoord;
}
