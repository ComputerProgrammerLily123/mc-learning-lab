#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec3 Normal;
in float Grass;

uniform sampler2D textureIn;
void main(){
    vec4 topColor = vec4(1.0,1.0,1.0,1.0);
    if(Normal.y > 0.9 && Grass > 0) topColor = vec4(0.5,0.8,0.4,1.0);
    FragColor = texture(textureIn,TexCoord) * topColor;
}