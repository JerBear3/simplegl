#include "../lib/glad.h" //glad needs to be included before glfw
#include "../gl.hpp"

using namespace std;

const char __ssc_vertexShaderSource[] =
"#version 330 core\n"
"\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 2) in vec2 aTexuv;\n"
"\n"
"out vec2 texuv;\n"
"\n"
"uniform mat4 camMat, traMat;\n"
"\n"
"void main()\n"
"{\n"
"	texuv = aTexuv;\n"
"	gl_Position = camMat * traMat * vec4(aPos, 1.f);\n"
"}";

const char __ssc_fragmentShaderSource[] =
"#version 330 core\n"
"\n"
"struct Material\n"
"{\n"
"	vec4 ambCol;\n"
"	sampler2D ambTex;\n"
"	bool ambTexEn;\n"
"};\n"
"\n"
"in vec2 texuv;\n"
"\n"
"out vec4 fragColor;\n"
"\n"
"uniform Material mat;\n"
"\n"
"void main()\n"
"{\n"
"	fragColor = mat.ambCol;\n"
"	if(mat.ambTexEn)\n"
"		fragColor *= texture(mat.ambTex, texuv);\n"
"}";

SimpleShader::SimpleShader() : Shader(__ssc_vertexShaderSource, __ssc_fragmentShaderSource)
{
	use();
	
	camMatLoc = getUniform("camMat");
	traMatLoc = getUniform("traMat");
	
	matAmbColLoc =   getUniform("mat.ambCol");
	matAmbTexLoc =   getUniform("mat.ambTex");
	matAmbTexEnLoc = getUniform("mat.ambTexEn");
}

void SimpleShader::render(const MatMesh& matmesh)
{
	setUniformMaterial(*matmesh.material);
	
	if(matmesh.material->ambientTextureEnable)
		matmesh.material->ambientTexture->bind(0);
	
	matmesh.mesh->bind();
	glDrawElements(GL_TRIANGLES, matmesh.mesh->getIndexLength(), GL_UNSIGNED_INT, 0);
}

void SimpleShader::render(const Model& model)
{
	setUniform4x4f(traMatLoc, model.transform);
	for(vector<MatMesh>::size_type i = 0; i < model.data.size(); i++)
		render(*model.data[i]);
}

void SimpleShader::setUniformCamera(Camera& camera)
{
	setUniform4x4f(camMatLoc, camera.calculateMatrix(camMat));
}

void SimpleShader::setUniformMaterial(const Material& material)
{
	setUniform4f(matAmbColLoc, material.ambientColor);
	setUniform1i(matAmbTexLoc, 0);
	setUniform1b(matAmbTexEnLoc, material.ambientTextureEnable);
}
