#include "../lib/glad.h" //glad needs to be included before glfw
#include "../gl.hpp"

Mesh::Mesh(Vertex vertices[], size_t vlength, unsigned int indices[], size_t ilength)
{
	glGenVertexArrays(1, &VAO); //vertex attribute layout
	glGenBuffers(1, &VBO); //vertices
	glGenBuffers(1, &EBO); //indices of vertices
	
	size_t vertexSize = sizeof(Vertex); //32
	
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexSize * vlength, vertices, GL_STATIC_DRAW);
	this->vlength = vlength;
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * ilength, indices, GL_STATIC_DRAW);
	this->ilength = ilength;
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*) (3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*) (6 * sizeof(float)));
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void Mesh::bind() const
{
	glBindVertexArray(VAO);
}

unsigned int Mesh::getVAO() const
{
	return VAO;
}

unsigned int Mesh::getVBO() const
{
	return VBO;
}

unsigned int Mesh::getEBO() const
{
	return EBO;
}

size_t Mesh::getVertexLength() const
{
	return vlength;
}

size_t Mesh::getIndexLength() const
{
	return ilength;
}
