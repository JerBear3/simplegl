#include "../lib/glad.h" //glad needs to be included before glfw
#include "../gl.hpp"

Mesh::Mesh(Vertex vertices[], unsigned long vlength, unsigned int indices[], unsigned long ilength)
{
	glGenVertexArrays(1, &VAO); //vertex attributes
	glGenBuffers(1, &VBO); //vertices
	glGenBuffers(1, &EBO); //indices of vertices
	
	unsigned long vertexSize = sizeof(Vertex); //32
	unsigned long vertexAttCnt = sizeof(Vertex) / sizeof(float); //8
	
	float* vertexData = new float[vertexAttCnt * vlength];
	for(unsigned long i = 0; i < vlength; i++)
	{
		unsigned long vi = i * vertexAttCnt;
		vertexData[vi    ] = vertices[i].position.x;
		vertexData[vi + 1] = vertices[i].position.y;
		vertexData[vi + 2] = vertices[i].position.z;
		vertexData[vi + 3] = vertices[i].normal.x;
		vertexData[vi + 4] = vertices[i].normal.y;
		vertexData[vi + 5] = vertices[i].normal.z;
		vertexData[vi + 6] = vertices[i].u;
		vertexData[vi + 7] = vertices[i].v;
	}
	
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexSize * vlength, vertexData, GL_STATIC_DRAW);
	delete[] vertexData;
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

unsigned long Mesh::getVertexLength() const
{
	return vlength;
}

unsigned long Mesh::getIndexLength() const
{
	return ilength;
}
