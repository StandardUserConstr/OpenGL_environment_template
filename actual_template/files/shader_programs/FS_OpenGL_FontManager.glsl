#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D texture0;
uniform vec4 textcolor;
uniform vec4 background;
void main()
{
    float text_visibility = texture(texture0, TexCoords).x;
    vec4 text = vec4(textcolor.x,textcolor.y,textcolor.z,textcolor.w*text_visibility);

    float visibility = (text_visibility*text.w)+((1.0-text_visibility)*background.w);
    vec4 result;
    result.xyz = vec3(text_visibility*text.x,text_visibility*text.y,text_visibility*text.z)+vec3((1.0-text_visibility)*background.x,(1.0-text_visibility)*background.y,(1.0-text_visibility)*background.z);
    result.w = visibility;
    color = result;
    return;
}
