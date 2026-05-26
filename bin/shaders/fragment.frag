#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec3 Normal;
in float BlockId;

uniform sampler2D textureIn;
void main(){
    vec4 additionalColor = vec4(1.0,1.0,1.0,1.0);
    if(Normal.y > 0.9 && abs(BlockId - 1.0) < 0.1f) additionalColor = vec4(0.5,0.8,0.4,1.0);
    if(abs(BlockId - 10.0) < 0.1f) 
    {
        if(texture(textureIn,TexCoord).a < 0.1) discard;
        additionalColor = vec4(0.5,0.8,0.4,1.0);
    }
    FragColor = texture(textureIn,TexCoord) * additionalColor;
}