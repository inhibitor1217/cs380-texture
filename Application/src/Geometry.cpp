#include <Geometry.hpp>
#include <stdlib.h>

Geometry::Geometry()
{
}


Geometry::~Geometry()
{
}

glm::vec4 vertices[8] = {
    glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f),
    glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f),
    glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
    glm::vec4(1.0f, -1.0f, 1.0f, 1.0f),
    glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f),
    glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f),
    glm::vec4(1.0f, 1.0f, -1.0f, 1.0f),
    glm::vec4(1.0f, -1.0f, -1.0f, 1.0f)
};

glm::vec3 Geometry::ComputeNormal(glm::vec4 a_a, glm::vec4 a_b, glm::vec4 a_c)
{
    glm::vec3 normal = glm::normalize(glm::cross(a_b.xyz() - a_a.xyz(), a_c.xyz() - a_a.xyz()));
    return normal;
}

void Geometry::Quad(Engine::Mesh* mesh, int a_a, int a_b, int a_c, int a_d)
{
	/* a -- d
	 * |    |
	 * b -- c
	 */
	 // Compute normal for quad
	glm::vec4 n = glm::vec4(ComputeNormal(vertices[a_a], vertices[a_b], vertices[a_c]), 0.0f);

	// Compute Tangent for quad
	glm::vec4 e1 = vertices[a_b] - vertices[a_a];
	glm::vec4 e2 = vertices[a_c] - vertices[a_a];
	glm::vec4 tangent = glm::vec4(0.0f);
	int du1, du2, dv1, dv2;
	du1 = 0, du2 = 1, dv1 = 1, dv2 = 1;

	float coef = 1.0f / (du1 * dv2 - du2 * dv1);

	tangent.x = coef * (dv2 * e1.x - dv1 * e2.x);
	tangent.y = coef * (dv2 * e1.y - dv1 * e2.y);
	tangent.z = coef * (dv2 * e1.z - dv1 * e2.z);
	tangent = glm::normalize(tangent);

	// Triangle 1, Vertex a
	mesh->AddVertexData(vertices[a_a]);
	mesh->AddVertexData(n);
	mesh->AddVertexData(glm::vec2(0.0f, 0.0f));
	mesh->AddVertexData(tangent);
	// Triangle 1, Vertex b
	mesh->AddVertexData(vertices[a_b]);
	mesh->AddVertexData(n);
	mesh->AddVertexData(glm::vec2(0.0f, 1.0f));
	mesh->AddVertexData(tangent);
	// Triangle 1, Vertex c
	mesh->AddVertexData(vertices[a_c]);
	mesh->AddVertexData(n);
	mesh->AddVertexData(glm::vec2(1.0f, 1.0f));
	mesh->AddVertexData(tangent);
	// Triangle 2, Vertex a
	mesh->AddVertexData(vertices[a_a]);
	mesh->AddVertexData(n);
	mesh->AddVertexData(glm::vec2(0.0f, 0.0f));
	mesh->AddVertexData(tangent);
	// Triangle 2, Vertex c
	mesh->AddVertexData(vertices[a_c]);
	mesh->AddVertexData(n);
	mesh->AddVertexData(glm::vec2(1.0f, 1.0f));
	mesh->AddVertexData(tangent);
	// Triangle 2, Vertex d
	mesh->AddVertexData(vertices[a_d]);
	mesh->AddVertexData(n);
	mesh->AddVertexData(glm::vec2(1.0f, 0.0f));
	mesh->AddVertexData(tangent);
}


void Geometry::GenerateCube(Engine::Mesh* mesh)
{
    /*   5 -- 6
     *  /|   /|
     * 1(4)-2 7
     * |    |/
     * 0 -- 3
     */
    mesh->AddAttribute(4); // for position
    mesh->AddAttribute(4); // for normal
	mesh->AddAttribute(2); // for texture coordinates
	mesh->AddAttribute(4); // for tangent

    // Add four quad for generating a cube
    Quad(mesh, 1, 0, 3, 2);
    Quad(mesh, 2, 3, 7, 6);
    Quad(mesh, 3, 0, 4, 7);
    Quad(mesh, 6, 5, 1, 2);
    Quad(mesh, 4, 5, 6, 7);
    Quad(mesh, 5, 4, 0, 1);

    mesh->SetNumElements(36);
    mesh->CreateMesh();
    return;
}

void Geometry::GenerateTriangleMesh(Engine::Mesh* mesh)
{
    mesh->AddAttribute(4);

    glm::vec3 position_data[] = {
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, -1.0f, 0.0f),
        glm::vec3(0.0f,1.0f, 0.0f)
    };

    for (int i = 0; i < 3; i++)
    {
        mesh->AddVertexData(position_data[i]);
        mesh->AddVertexData(1.0f);

    }
    mesh->SetNumElements(3);
    mesh->CreateMesh();
    return;
}

void Geometry::GenerateLine(Engine::Mesh* mesh)
{
    mesh->SetDrawMode(GL_LINES);
    mesh->AddAttribute(4);
    mesh->AddVertexData(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    mesh->AddVertexData(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    mesh->SetNumElements(2);
    mesh->CreateMesh();
    return;
}