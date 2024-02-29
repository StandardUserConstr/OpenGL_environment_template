#version 420 core

out vec4 FragColor;

struct Material
{
    sampler2D ambient[4];
    sampler2D diffuse[4];  // main texture;
    sampler2D specular[4]; // reflection texture;
    //sampler2D normals;
    //sampler2D height;
    float     shininess;
};

uniform int NR_OF_MATERIAL_AMBIENTS;
uniform int NR_OF_MATERIAL_DIFFUSE;
uniform int NR_OF_MATERIAL_SPECULAR;
//uniform int NR_OF_MATERIAL_NORMALS;
//uniform int NR_OF_MATERIAL_HEIGHT;

struct SpotLight // "flashlight" type of light;
{
    vec3 position;
    vec3 direction;
    float cutOff;       // angle (cos) of the lighting;
    float outerCutOff;  // soft outer angle (cos) of the lighting (it has to be: cutOff+some_value);

    vec3 ambient;  // default brightness of light that depends on the light distance;
    vec3 diffuse;  // color of the light (shoudl be from 0.0 to 1.0,but if u want stronger light then u can multiply color value like this: vec3(0.992,0.952,0.36)*2.5);
    vec3 specular; // visibility of the reflection;

    float constant; // calculation variable,typically just stays 1;
    float linear;   // these variables are responsible for light distance and when light is stronger and when weaker; help table to set recomended values to "linear" and "quadratic" variables is below:
    float quadratic;// https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
};

struct DirLight // "sun" type of light;
{
    vec3 direction;

    vec3 ambient;   // default brightness of light (darkest dark of the object);
    vec3 diffuse;
    vec3 specular;
};

struct PointLight // "glowing ball" type of light;
{
    vec3 position;

    vec3 ambient;   // default brightness of light that depends on the light distance;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform int NR_OF_POINT_LIGHTS;
uniform vec3 CameraPos;
uniform DirLight dir_light;
uniform PointLight point_light[1];
uniform SpotLight spot_light;
uniform Material material;

vec3 calc_dir_light(DirLight light,vec3 normal, vec3 view_dir);
vec3 calc_point_light(PointLight light,vec3 normal,vec3 frag_pos,vec3 view_dir);
vec3 calc_spot_light(SpotLight light,vec3 normal,vec3 frag_pos,vec3 view_dir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 view_dir = normalize(CameraPos-FragPos);

    // directional lighting;
    vec3 result = calc_dir_light(dir_light,norm,view_dir);
    // point lights;
    for(int i = 0; i!=NR_OF_POINT_LIGHTS; i++) result+=calc_point_light(point_light[i],norm,FragPos,view_dir);
    // flashlights;
    result+=calc_spot_light(spot_light,norm,FragPos,view_dir);

    FragColor = vec4(result, 1.0);

    return;
}


vec3 calc_dir_light(DirLight light,vec3 normal,vec3 view_dir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse (which side of the texture to illuminate and how much);
    float diff = max(dot(normal,lightDir),0.0);
    // specular (reflection);
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(view_dir,reflectDir),0.0),material.shininess);
    // combine results
    vec3 ambient = light.ambient;
    for(int i = 0; i!=NR_OF_MATERIAL_AMBIENTS; i++) ambient*=vec3(texture(material.ambient[i],TexCoords));
    vec3 diffuse = vec3(diff);
    for(int i = 0; i!=NR_OF_MATERIAL_DIFFUSE; i++) diffuse*=vec3(texture(material.diffuse[i],TexCoords));
    vec3 specular = light.specular*spec;
    for(int i = 0; i!=NR_OF_MATERIAL_SPECULAR; i++) specular*=vec3(texture(material.specular[i],TexCoords));

    ambient*=light.diffuse;
    diffuse*=light.diffuse;
    specular*=light.diffuse;
    return (ambient+diffuse+specular);
}

vec3 calc_point_light(PointLight light,vec3 normal,vec3 frag_pos,vec3 view_dir)
{
    vec3 lightDir = normalize(light.position-frag_pos);
    // diffuse (which side of the texture to illuminate and how much);
    float diff = max(dot(normal,lightDir),0.0);
    // specular (reflection);
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(view_dir,reflectDir),0.0),material.shininess);
    // attenuation (how strong is the light at actual distance);
    float distance = length(light.position-frag_pos);
    float attenuation = 1.0/(light.constant+light.linear*distance+light.quadratic*(distance*distance));
    // combine results
    vec3 ambient = light.ambient;
    for(int i = 0; i!=NR_OF_MATERIAL_AMBIENTS; i++) ambient*=vec3(texture(material.ambient[i],TexCoords));
    vec3 diffuse = vec3(diff);
    for(int i = 0; i!=NR_OF_MATERIAL_DIFFUSE; i++) diffuse*=vec3(texture(material.diffuse[i],TexCoords));
    vec3 specular = light.specular*spec;
    for(int i = 0; i!=NR_OF_MATERIAL_SPECULAR; i++) specular*=vec3(texture(material.specular[i],TexCoords));

    ambient*=attenuation;
    diffuse*=attenuation;
    specular*=attenuation;
    ambient*=light.diffuse;
    diffuse*=light.diffuse;
    specular*=light.diffuse;
    return (ambient+diffuse+specular);
}

vec3 calc_spot_light(SpotLight light,vec3 normal,vec3 frag_pos,vec3 view_dir)
{
    vec3 lightDir = normalize(light.position-frag_pos);
    // diffuse (which side of the texture to illuminate and how much);
    float diff = max(dot(normal,lightDir),0.0); //max returns y if x<y; otherwise returns x; //the dot product is (a.x*b.x)+(a.y*b.y)+(a.z*b.z);
    // specular (reflection);
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(view_dir,reflectDir),0.0),material.shininess);
    // attenuation (how strong is the light at actual distance);
    float distance = length(light.position-frag_pos);
    float attenuation = 1.0/(light.constant+light.linear*distance+light.quadratic*(distance*distance));
    // spotlight ("flashlight" light + soft edges);
    float theta = dot(lightDir,normalize(-light.direction));
    float epsilon = light.cutOff-light.outerCutOff;
    float intensity = clamp((theta-light.outerCutOff)/epsilon,0.0,1.0); //clamp is function that returns variable between 0.0 and 1.0 (if something is above 1.0 or below 0.0 then it returns 1.0 or 0.0);
    // combine results
    vec3 ambient = light.ambient;
    for(int i = 0; i!=NR_OF_MATERIAL_AMBIENTS; i++) ambient*=vec3(texture(material.ambient[i],TexCoords));
    vec3 diffuse = vec3(diff);
    for(int i = 0; i!=NR_OF_MATERIAL_DIFFUSE; i++) diffuse*=vec3(texture(material.diffuse[i],TexCoords));
    vec3 specular = light.specular*spec;
    for(int i = 0; i!=NR_OF_MATERIAL_SPECULAR; i++) specular*=vec3(texture(material.specular[i],TexCoords));

    ambient*=attenuation;
    diffuse*=attenuation*intensity;
    specular*=attenuation*intensity;
    ambient*=light.diffuse;     // color of the light;
    diffuse*=light.diffuse;     // color of the light;
    specular*=light.diffuse;    // color of the light reflection;
    return (ambient+diffuse+specular);
}
