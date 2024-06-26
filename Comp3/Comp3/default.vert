#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

//out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
  
uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
    gl_Position = camMatrix * model * vec4(aPos, 1.0);
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = vec2(aTexCoords.x, aTexCoords.y);
} 


//Used code we did in class with Ali (for .vert and .frag as well as cube), so it is very similar to https://github.com/AA-A93/3Dprog24-lighting-class.git