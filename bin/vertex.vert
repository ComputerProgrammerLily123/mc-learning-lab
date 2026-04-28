#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in float aGrass;

out vec2 TexCoord;
out vec3 Normal;
out float Grass;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
void main(){
    gl_Position = projection * view * model * vec4(aPos,1.0f);
    TexCoord = vec2(aTexCoord.x,aTexCoord.y);
    Normal = aNormal;
    Grass = aGrass;
}