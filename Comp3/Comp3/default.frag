#version 330 core

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material
{
	float shininess;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 colorOut;
in vec3 vertexOut;
in vec2 textureOut;
out vec4 fragColor;

uniform Light light;
uniform Material material;
uniform vec3 cameraPosition;

uniform bool isLit;
uniform bool isTextured;
uniform sampler2D textureImage;

void main()
{
	fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);

}