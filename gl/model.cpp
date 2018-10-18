#include "../gl.hpp"

Model::Model() {}

Model::Model(MatMesh* matmesh)
{
	data.push_back(matmesh);
}

Matrix4& Model::calculateNormalMatrix(Matrix4& out) const
{
	out = transform;
	out[M03] = 0.f;
	out[M13] = 0.f;
	out[M23] = 0.f;
	return out.inv().tra();
}
