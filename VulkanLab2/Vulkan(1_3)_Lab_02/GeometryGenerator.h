#pragma once

#include <vector>
#include <cstdint>
#include <glm/glm.hpp>
#include <array>
#include <vulkan/vulkan.h>
#include <string>
struct Vertex
{
    glm::vec3 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription{};

        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2>
        getAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 2>
            attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format =
            VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset =
            offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format =
            VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset =
            offsetof(Vertex, color);

        return attributeDescriptions;
    }
};

class Q4ReuseGeo
{
public:

    struct MeshData
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    };

    static MeshData CreateGrid(int width, int depth);

    static MeshData CreateTerrain(int width, int depth);

    static MeshData CreateCylinder(
        float radius,
        float height,
        int segments
    );

    static MeshData CreateSphere(
        float radius,
        int segments
    );
    static MeshData CreateModel(const std::string& path);
};