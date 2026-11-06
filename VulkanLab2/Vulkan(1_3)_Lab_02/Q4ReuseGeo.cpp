#include "GeometryGenerator.h"
#include <cmath>
#include <assimp/importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <string>

//q1 plane
Q4ReuseGeo::MeshData
Q4ReuseGeo::CreateGrid(int width, int depth)
{
	MeshData mesh;
   
        //vertices
        for (int z = 0; z <= depth; ++z) {
            for (int x = 0; x <= width; ++x) {
                Vertex vertex{};
                float xPos = static_cast<float>(x) - width / 2.0f;
                float zPos = static_cast<float>(z) - depth / 2.0f;
                vertex.pos = glm::vec3(xPos, 0.0f, zPos);
                vertex.color = glm::vec3(1.0f, 0.0f, 0.0f);
                mesh.vertices.push_back(vertex);
            }
        }

        //indices
        for (int z = 0; z < depth; ++z) {
            for (int x = 0; x < width; ++x)
                //two triangles
            {
                uint32_t topLeft = z * (width + 1) + x;
                uint32_t topRight = topLeft + 1;
                uint32_t bottomLeft = (z + 1) * (width + 1) + x;
                uint32_t bottomRight = bottomLeft + 1;
                mesh.indices.push_back(topLeft);
                mesh.indices.push_back(bottomLeft);
                mesh.indices.push_back(topRight);
                mesh.indices.push_back(topRight);
                mesh.indices.push_back(bottomLeft);
                mesh.indices.push_back(bottomRight);
            }
        }
        return mesh;
    }



//q2 procedural wavey terrain 
Q4ReuseGeo::MeshData
Q4ReuseGeo::CreateTerrain(int width, int depth)
{
    MeshData mesh;
    //vertices
    for (int z = 0; z <= depth; ++z) {
        for (int x = 0; x <= width; ++x) {
            Vertex vertex{};
            float xPos = static_cast<float>(x) - width / 2.0f;
            float zPos = static_cast<float>(z) - depth / 2.0f;
            float yPos = std::sin(xPos) * std::cos(zPos);
            vertex.pos = glm::vec3(xPos, yPos, zPos);
            vertex.color = glm::vec3(1.0f, 0.0f, 0.0f);
            mesh.vertices.push_back(vertex);
        }
    }

    //indicies loops
    for (int z = 0; z < depth; ++z)
    {
        for (int x = 0; x < width; ++x)
        {
            uint32_t topLeft =
                z * (width + 1) + x;

            uint32_t topRight =
                topLeft + 1;

            uint32_t bottomLeft =
                (z + 1) * (width + 1) + x;

            uint32_t bottomRight =
                bottomLeft + 1;

            mesh.indices.push_back(topLeft);
            mesh.indices.push_back(bottomLeft);
            mesh.indices.push_back(topRight);

            mesh.indices.push_back(topRight);
            mesh.indices.push_back(bottomLeft);
            mesh.indices.push_back(bottomRight);
        }
    }

    return mesh;
}
//q3 cylinder need to redo
//bug in here related to the top and bottom face of cylinder stray line moves out of mesh? 
Q4ReuseGeo::MeshData
Q4ReuseGeo::CreateCylinder(float radius, float height, int segments)
{
    MeshData mesh;
    {
        float halfHeight = height / 2.0f;

        //bottom ring
        for (int i = 0; i < segments; i++)
        {
            float tetha = (2.0f * 3.14159265359f * i) / segments;

            float x = radius * std::cos(tetha);
            float z = radius * std::sin(tetha);

            Vertex v{};
            v.pos = glm::vec3(x, -halfHeight, z);
            v.color = glm::vec3(1.0f, 0.0f, 0.0f);
            //mesh.indices.push_back(mesh.vertices.size()); // this is the reason 
            mesh.vertices.push_back(v);
        }

        //top ring

        for (int i = 0; i < segments; i++)
        {
            float theta = (2.0f * 3.14159265359f * i) / segments;


            float x = radius * std::cos(theta);
            float z = radius * std::sin(theta);

            Vertex v{};
            v.pos = glm::vec3(x, halfHeight, z);
            v.color = glm::vec3(0.0f, 1.0f, 0.0f);
            //mesh.indices.push_back(mesh.vertices.size());
            mesh.vertices.push_back(v);
        }

        //indices
        for (int i = 0; i < segments; i++)
        {
            uint32_t bottomCurrent = i;
            uint32_t bottomNext = (i + 1) % segments;

            uint32_t topCurrent = i + segments;
            uint32_t topNext = (i + 1) % segments + segments;

            mesh.indices.push_back(bottomCurrent);
            mesh.indices.push_back(topCurrent);
            mesh.indices.push_back(bottomNext);

            mesh.indices.push_back(bottomNext);
            mesh.indices.push_back(topCurrent);
            mesh.indices.push_back(topNext);
        }

        uint32_t bottomCenter = static_cast<uint32_t>(mesh.vertices.size());

        mesh.vertices.push_back({ glm::vec3(0.0f, -halfHeight, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f) });

        uint32_t topCenter = static_cast<uint32_t>(mesh.vertices.size());

        mesh.vertices.push_back({ glm::vec3(0.0f, halfHeight, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f) });

        for (int i = 0; i < segments; ++i)
        {
            uint32_t next = (i + 1) % segments;
            // Bottom face
            mesh.indices.push_back(bottomCenter);
            mesh.indices.push_back(i);
            mesh.indices.push_back(next);
        }

        for (int i = 0; i < segments; ++i)
        {
            uint32_t current = i + segments;
            uint32_t next = (i + 1) % segments;
            // Top face
            mesh.indices.push_back(topCenter);
            mesh.indices.push_back(segments + next);
            mesh.indices.push_back(segments + i);
        }

    }
 
    return mesh;
}


//q4 create sphere

Q4ReuseGeo::MeshData
Q4ReuseGeo::CreateSphere(float radius, int segments)
{
	MeshData mesh;
	for (int y = 0; y <= segments; ++y)
	{
		for (int x = 0; x <= segments; ++x)
		{
			float xSegment = static_cast<float>(x) / static_cast<float>(segments);
			float ySegment = static_cast<float>(y) / static_cast<float>(segments);
			float xPos = radius * std::cos(xSegment * 2.0f * 3.14159265359f) * std::sin(ySegment * 3.14159265359f);
			float yPos = radius * std::cos(ySegment * 3.14159265359f);
			float zPos = radius * std::sin(xSegment * 2.0f * 3.14159265359f) * std::sin(ySegment * 3.14159265359f);
			Vertex vertex{};
			vertex.pos = glm::vec3(xPos, yPos, zPos);
			vertex.color = glm::vec3(1.0f, 1.0f, 1.0f);
			mesh.vertices.push_back(vertex);
		}
	}
	for (int y = 0; y < segments; ++y)
	{
		for (int x = 0; x < segments; ++x)
		{
			uint32_t topLeft = y * (segments + 1) + x;
			uint32_t topRight = topLeft + 1;
			uint32_t bottomLeft = (y + 1) * (segments + 1) + x;
			uint32_t bottomRight = bottomLeft + 1;
			mesh.indices.push_back(topLeft);
			mesh.indices.push_back(bottomLeft);
			mesh.indices.push_back(topRight);
			mesh.indices.push_back(topRight);
			mesh.indices.push_back(bottomLeft);
			mesh.indices.push_back(bottomRight);
		}
	}
	return mesh;
}

//q5 loading model
Q4ReuseGeo::MeshData
Q4ReuseGeo::CreateModel(const std::string& path)
{
    MeshData mesh;

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate
    );

    if (!scene)
    {
        std::cerr << "ASSIMP ERROR: "
            << importer.GetErrorString()
            << std::endl;

        return mesh;
    }

    if (!scene->HasMeshes())
    {
        std::cerr << "Model loaded, but contains no meshes."
            << std::endl;

        return mesh;
    }

    aiMesh* ai_mesh = scene->mMeshes[0];

    std::cout << "Model loaded successfully\n";
    std::cout << "Vertices: "
        << ai_mesh->mNumVertices << "\n";
    std::cout << "Faces: "
        << ai_mesh->mNumFaces << "\n";

    for (unsigned int i = 0;
        i < ai_mesh->mNumVertices;
        ++i)
    {
        Vertex vertex{};

        vertex.pos = glm::vec3(
            ai_mesh->mVertices[i].x,
            ai_mesh->mVertices[i].y,
            ai_mesh->mVertices[i].z
        );

        vertex.color =
            glm::vec3(1.0f, 1.0f, 1.0f);

        mesh.vertices.push_back(vertex);
    }

    for (unsigned int i = 0;
        i < ai_mesh->mNumFaces;
        ++i)
    {
        const aiFace& face =
            ai_mesh->mFaces[i];

        for (unsigned int j = 0;
            j < face.mNumIndices;
            ++j)
        {
            mesh.indices.push_back(
                face.mIndices[j]
            );
        }
    }

    std::cout << "Imported vertices: "
        << mesh.vertices.size()
        << "\n";

    std::cout << "Imported indices: "
        << mesh.indices.size()
        << "\n";

    return mesh;
}
