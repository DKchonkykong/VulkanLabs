#version 450

layout(binding = 0) uniform UniformBufferObject
{
    vec4 eye;
    vec4 target;
    vec4 up;
    vec4 perspective;
} ubo;

layout(push_constant) uniform PushConstants
{
    mat4 model;
} pushConstants;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

void main()
{
    
    vec3 eye = ubo.eye.xyz;
    vec3 target = ubo.target.xyz;
    vec3 up = ubo.up.xyz;

    vec3 forward =
        normalize(target - eye);

    vec3 right =
        normalize(cross(forward, up));

    vec3 cameraUp =
        cross(right, forward);

    mat4 view = mat4(
        vec4(right.x, cameraUp.x, -forward.x, 0.0),
        vec4(right.y, cameraUp.y, -forward.y, 0.0),
        vec4(right.z, cameraUp.z, -forward.z, 0.0),
        vec4(
            -dot(right, eye),
            -dot(cameraUp, eye),
            dot(forward, eye),
            1.0
        )
    );

    
    float fov = ubo.perspective.x;
    float aspect = ubo.perspective.y;
    float nearPlane = ubo.perspective.z;
    float farPlane = ubo.perspective.w;

    float tanHalfFov =
        tan(fov / 2.0);

    mat4 proj = mat4(0.0);

    proj[0][0] =
        1.0 / (aspect * tanHalfFov);

    proj[1][1] =
        -1.0 / tanHalfFov;

    proj[2][2] =
        farPlane /
        (nearPlane - farPlane);

    proj[2][3] = -1.0;

    proj[3][2] =
        (farPlane * nearPlane) /
        (nearPlane - farPlane);

    gl_Position =
        proj *
        view *
        pushConstants.model *
        vec4(inPosition, 1.0);

    fragColor = inColor;
}