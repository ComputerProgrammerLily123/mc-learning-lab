#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D textureIn;
void main(){
    vec4 texColor = texture(textureIn,TexCoord);
    if(texColor.a < 0.1f)discard;
    FragColor = texColor;
}