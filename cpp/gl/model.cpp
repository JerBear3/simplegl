#include "../gl.hpp"

Model::Model() {}

Model::Model(MatMesh* matmesh)
{
	data.push_back(matmesh);
}
