#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in float aTintIndex;

uniform mat4 projection;
uniform mat4 model;

out vec2 uv;
out vec3 normal;
out float tintIndex;

void main()
{
    gl_Position = projection * model * vec4(aPosition, 1.0);
    uv = aUV;
    normal = normalize(mat3(model) * aNormal);
    tintIndex = aTintIndex;
}