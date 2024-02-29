#version 330 core

in vec3 Scolor; //must be the same name as in vertex shader to pass variable from vertex shader to fragment shader (variables can't be directly passed to fragment shader);

out vec4 FragColor;

void main()
{
    FragColor = vec4(Scolor.x,Scolor.y,Scolor.z,1.0);
}
