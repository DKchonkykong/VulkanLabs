#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 lightPos;
    vec3 eyePos;
} ubo;

layout(push_constant) uniform PushConstants {
    mat4 model;
} pushConstants;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragWorldPos;
layout(location = 2) out vec3 fragWorldNormal;

void main()
{
    vec4 worldPosition =
        pushConstants.model *
        vec4(inPosition, 1.0);

    fragWorldPos =
        worldPosition.xyz;

    fragWorldNormal =
        mat3(
            transpose(
                inverse(pushConstants.model)
            )
        ) * inNormal;

    fragColor = inColor;

    gl_Position =
        ubo.proj *
        ubo.view *
        worldPosition;
}