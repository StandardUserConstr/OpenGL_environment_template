#version 330 core

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;
uniform mat3 normal_model;  //this kind of model is for scaling operations; without it the scaling operations will distorts lighting of the model;

void main()
{
    FragPos = vec3(model*vec4(aPos, 1.0));
    Normal = normal_model*aNormal; //it should somehow fix distorts of lighting when scaling; //normal_model*aNormal is equal to: mat3(transpose(inverse(model)))*aNormal; but inverse() func is expensive so i did it in main program;
    TexCoords = aTexCoords;

    gl_Position = projection*camera*model*vec4(aPos, 1.0);

    return;
}
