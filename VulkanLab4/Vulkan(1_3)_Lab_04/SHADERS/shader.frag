#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 lightPos;
    vec3 eyePos;
} ubo;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragWorldPos;
layout(location = 2) in vec3 fragWorldNormal;

layout(location = 0) out vec4 outColor;

void main()
{
    vec3 lightColor =
        vec3(1.0);

    vec3 ambientMaterial =
        vec3(0.4);

    vec3 normal =
        normalize(fragWorldNormal);

    vec3 lightDir =
        normalize(
            ubo.lightPos -
            fragWorldPos
        );

    float diff =
        max(
            dot(normal, lightDir),
            0.0
        );

    vec3 diffuse =
        diff * lightColor;

    vec3 result =
        ambientMaterial * lightColor;

    result +=
        fragColor * diffuse;

    outColor =
        vec4(result, 1.0);
}