#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D texture0;
uniform float visibility;

void main()
{
    vec4 main_texture = texture(texture0,TexCoords);
    main_texture.w*=visibility;

    color = main_texture;
    return;
}
