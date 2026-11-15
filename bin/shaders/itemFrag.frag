#version 330 core
in vec2 uv;
in vec3 normal;
in float tintIndex;
uniform sampler2D textureIn;
out vec4 FragColor;

void main()
{
    vec4 color = texture(textureIn, uv);

    if (color.a < 0.1)
        discard;

    float light = 0.65 + 0.35 * max(dot(normalize(normal), normalize(vec3(-1.0, 1.0, 0.5))), 0.0);

    if (tintIndex == 0.0)
        color.rgb *= vec3(0.45, 0.75, 0.30);

    FragColor = vec4(color.rgb * light, color.a);
}