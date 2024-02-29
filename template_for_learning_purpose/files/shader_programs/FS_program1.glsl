#version 330 core

in vec3 Scolor; //must be the same name as in vertex shader to pass variable from vertex shader to fragment shader (variables can't be directly passed to fragment shader if aren't uniform);
in vec2 STexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

out vec4 FragColor;

void main()
{
    vec4 color0 = texture(texture0,STexCoord);
    vec4 color1 = texture(texture1,STexCoord);
    if(color1.w>0.4) FragColor = color1*vec4(Scolor.x,Scolor.y,Scolor.z,1.0); //line "if(color1.w>0.4)" is needed 'cause image "cat.png" that i used is made with a thought that should be transparent but isn't in this example;
    else FragColor = color0*vec4(Scolor.x,Scolor.y,Scolor.z,1.0);
}
