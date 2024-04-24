#version 330 core
out vec4 FinalColor;  

//in vec3 OurColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
   // FinalColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
   FinalColor = vec4(ourColor, 1.0);
}