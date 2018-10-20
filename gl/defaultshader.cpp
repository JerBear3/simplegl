#include <string>
#include <sstream>

#include "../lib/glad.h" //glad needs to be included before glfw
#include "../gl.hpp"

using namespace std;

const char __dsc_vertexShaderSource[] =
"#version 330 core\n"
"\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aNormal;\n"
"layout (location = 2) in vec2 aTexuv;\n"
"\n"
"out vec3 vpos;\n"
"out vec3 normal;\n"
"out vec2 texuv;\n"
"\n"
"uniform mat4 camMat, norMat, traMat;\n"
"\n"
"void main()\n"
"{\n"
"	vec4 vpos4 = traMat * vec4(aPos, 1.f);\n"
"	\n"
"	vpos = vpos4.xyz;\n"
"	normal = normalize(mat3(norMat) * aNormal);\n"
"	texuv = aTexuv;\n"
"	\n"
"	gl_Position = camMat * vpos4;\n"
"}";

string __dsc_fragmentShaderSourceA =
"#version 330 core\n"
"\n"
"#define POINTLIGHTS ";
string __dsc_fragmentShaderSourceB = "\n#define SPOTLIGHTS ";
string __dsc_fragmentShaderSourceC = "\n\n"
"struct Material\n"
"{\n"
"	vec4 ambCol;\n"
"	sampler2D ambTex;\n"
"	bool ambTexEn;\n"
"	\n"
"	vec4 diffCol;\n"
"	sampler2D diffTex;\n"
"	bool diffTexEn;\n"
"	\n"
"	vec4 specCol;\n"
"	sampler2D specTex;\n"
"	bool specTexEn;\n"
"	float specShine;\n"
"};\n"
"\n"
"struct DirectionalLight\n"
"{\n"
"	bool en;\n"
"	vec3 dir;\n"
"	\n"
"	vec3 amb;\n"
"	vec3 diff;\n"
"	vec3 spec;\n"
"};\n"
"\n"
"struct PointLight\n"
"{\n"
"	bool en;\n"
"	vec3 pos;\n"
"	\n"
"	vec3 amb;\n"
"	vec3 diff;\n"
"	vec3 spec;\n"
"	\n"
"	float attConst;\n"
"	float attLin;\n"
"	float attQuad;\n"
"};\n"
"\n"
"struct SpotLight\n"
"{\n"
"	bool en;\n"
"	vec3 pos;\n"
"	vec3 dir;\n"
"	\n"
"	vec3 amb;\n"
"	vec3 diff;\n"
"	vec3 spec;\n"
"	\n"
"	float attConst;\n"
"	float attLin;\n"
"	float attQuad;\n"
"	\n"
"	float inner;\n"
"	float outer;\n"
"};\n"
"\n"
"in vec3 vpos;\n"
"in vec3 normal;\n"
"in vec2 texuv;\n"
"\n"
"out vec4 fragColor;\n"
"\n"
"uniform Material mat;\n"
"uniform DirectionalLight dirLight;\n"
"uniform PointLight pointLights[POINTLIGHTS];\n"
"uniform SpotLight spotLights[SPOTLIGHTS];\n"
"uniform vec3 camPos;\n"
"\n"
"vec4 calcAmbient(vec3 vamb)\n"
"{\n"
"	vec4 ret = vec4(vamb, 1.f) * mat.ambCol;\n"
"	if(mat.ambTexEn)\n"
"		ret *= texture(mat.ambTex, texuv);\n"
"	\n"
"	return ret;\n"
"}\n"
"\n"
"vec4 calcDiffuse(vec3 vdiff, vec3 lightDir)\n"
"{\n"
"	float fdiff = max(dot(normal, lightDir), 0.f);\n"
"	vec4 ret = vec4(vdiff, 1.f) * mat.diffCol * fdiff;\n"
"	if(mat.diffTexEn)\n"
"		ret *= texture(mat.diffTex, texuv);\n"
"	\n"
"	return ret;\n"
"}\n"
"\n"
"vec4 calcSpecular(vec3 vspec, vec3 lightDir)\n"
"{\n"
"	vec3 camDir = normalize(camPos - vpos);\n"
"	vec3 reflectDir = reflect(-lightDir, normal);\n"
"	float fspec = pow(max(dot(camDir, reflectDir), 0.f), mat.specShine);\n"
"	vec4 ret = vec4(vspec, 1.f) * mat.specCol * fspec;\n"
"	if(mat.specTexEn)\n"
"		ret *= texture(mat.specTex, texuv);\n"
"	\n"
"	return ret;\n"
"}\n"
"\n"
"float calcAttenuation(vec3 lightPos, float attConst, float attLin, float attQuad)\n"
"{\n"
"	float distance = length(lightPos - vpos);\n"
"	return 1.f / (attConst + attLin * distance + attQuad * distance * distance);\n"
"}\n"
"\n"
"vec4 calcDirectionalLight()\n"
"{\n"
"	vec3 lightDir = normalize(-dirLight.dir);\n"
"	\n"
"	vec4 ambient = calcAmbient(dirLight.amb);\n"
"	vec4 diffuse = calcDiffuse(dirLight.diff, lightDir);\n"
"	vec4 specular = calcSpecular(dirLight.spec, lightDir);\n"
"	\n"
"	return ambient + diffuse + specular;\n"
"}\n"
"\n"
"vec4 calcPointLight(int i)\n"
"{\n"
"	PointLight light = pointLights[i];\n"
"	vec3 lightDir = normalize(light.pos - vpos);\n"
"	\n"
"	vec4 ambient = calcAmbient(light.amb);\n"
"	vec4 diffuse = calcDiffuse(light.diff, lightDir);\n"
"	vec4 specular = calcSpecular(light.spec, lightDir);\n"
"	\n"
"	float attenuation = calcAttenuation(light.pos, light.attConst, light.attLin, light.attQuad);\n"
"	ambient *= attenuation;\n"
"	diffuse *= attenuation;\n"
"	specular *= attenuation;\n"
"	\n"
"	return ambient + diffuse + specular;\n"
"}\n"
"\n"
"vec4 calcSpotLight(int i)\n"
"{\n"
"	SpotLight light = spotLights[i];\n"
"	vec3 lightDir = normalize(light.pos - vpos);\n"
"	\n"
"	vec4 ambient = calcAmbient(light.amb);\n"
"	vec4 diffuse = calcDiffuse(light.diff, lightDir);\n"
"	vec4 specular = calcSpecular(light.spec, lightDir);\n"
"	\n"
"	float attenuation = calcAttenuation(light.pos, light.attConst, light.attLin, light.attQuad);\n"
"	diffuse *= attenuation;\n"
"	specular *= attenuation;\n"
"	\n"
"	float theta = dot(lightDir, normalize(-light.dir));\n"
"	float epsilon = light.inner - light.outer;\n"
"	float intensity = clamp((theta - light.outer) / epsilon, 0.f, 1.f);\n"
"	\n"
"	diffuse *= intensity;\n"
"	specular *= intensity;\n"
"	\n"
"	return ambient + diffuse + specular;\n"
"}\n"
"\n"
"void main()\n"
"{\n"
"	fragColor = vec4(0.f);\n"
"	\n"
"	if(dirLight.en)\n"
"		fragColor += calcDirectionalLight();\n"
"	\n"
"	for(int i = 0; i < POINTLIGHTS; i++)\n"
"	{\n"
"		if(pointLights[i].en)\n"
"			fragColor += calcPointLight(i);\n"
"	}\n"
"	\n"
"	for(int i = 0; i < SPOTLIGHTS; i++)\n"
"	{\n"
"		if(spotLights[i].en)\n"
"			fragColor += calcSpotLight(i);\n"
"	}\n"
"}";

string scati(const string& str, int i)
{
	stringstream ret;
	ret << str << i;
	return ret.str();
}

DefaultShader::DefaultShader(size_t maxPointLights, size_t maxSpotLights)
		: Shader(__dsc_vertexShaderSource, (scati(__dsc_fragmentShaderSourceA, maxPointLights) +
		scati(__dsc_fragmentShaderSourceB, maxSpotLights) + __dsc_fragmentShaderSourceC).c_str())
{
	this->maxPointLights = maxPointLights;
	this->maxSpotLights = maxSpotLights;
	
	camPosLoc = getUniform("camPos");
	camMatLoc = getUniform("camMat");
	norMatLoc = getUniform("norMat");
	traMatLoc = getUniform("traMat");
	
	lightPointEnLoc = new int[maxPointLights];
	lightPointPosLoc = new int[maxPointLights];
	lightPointAmbLoc = new int[maxPointLights];
	lightPointDiffLoc = new int[maxPointLights];
	lightPointSpecLoc = new int[maxPointLights];
	lightPointAttConstLoc = new int[maxPointLights];
	lightPointAttLinLoc = new int[maxPointLights];
	lightPointAttQuadLoc = new int[maxPointLights];
	
	lightSpotEnLoc = new int[maxSpotLights];
	lightSpotPosLoc = new int[maxSpotLights];
	lightSpotDirLoc = new int[maxSpotLights];
	lightSpotAmbLoc = new int[maxSpotLights];
	lightSpotDiffLoc = new int[maxSpotLights];
	lightSpotSpecLoc = new int[maxSpotLights];
	lightSpotAttConstLoc = new int[maxSpotLights];
	lightSpotAttLinLoc = new int[maxSpotLights];
	lightSpotAttQuadLoc = new int[maxSpotLights];
	lightSpotInnerLoc = new int[maxSpotLights];
	lightSpotOuterLoc = new int[maxSpotLights];
	
	size_t i;
	
	lightDirEnLoc =   getUniform("dirLight.en");
	lightDirDirLoc =  getUniform("dirLight.dir");
	lightDirAmbLoc =  getUniform("dirLight.amb");
	lightDirDiffLoc = getUniform("dirLight.diff");
	lightDirSpecLoc = getUniform("dirLight.spec");
	setUniformDirectionalLightEnable(false);
	
	for(i = 0; i < maxPointLights; i++)
	{
		string stri = scati("pointLights[", i);
		lightPointEnLoc[i] =       getUniform((stri + "].en").c_str());
		lightPointPosLoc[i] =      getUniform((stri + "].pos").c_str());
		lightPointAmbLoc[i] =      getUniform((stri + "].amb").c_str());
		lightPointDiffLoc[i] =     getUniform((stri + "].diff").c_str());
		lightPointSpecLoc[i] =     getUniform((stri + "].spec").c_str());
		lightPointAttConstLoc[i] = getUniform((stri + "].attConst").c_str());
		lightPointAttLinLoc[i] =   getUniform((stri + "].attLin").c_str());
		lightPointAttQuadLoc[i] =  getUniform((stri + "].attQuad").c_str());
		setUniformPointLightEnable(i, false);
	}
	
	for(i = 0; i < maxSpotLights; i++)
	{
		string stri = scati("spotLights[", i);
		lightSpotEnLoc[i] =       getUniform((stri + "].en").c_str());
		lightSpotPosLoc[i] =      getUniform((stri + "].pos").c_str());
		lightSpotDirLoc[i] =      getUniform((stri + "].dir").c_str());
		lightSpotAmbLoc[i] =      getUniform((stri + "].amb").c_str());
		lightSpotDiffLoc[i] =     getUniform((stri + "].diff").c_str());
		lightSpotSpecLoc[i] =     getUniform((stri + "].spec").c_str());
		lightSpotAttConstLoc[i] = getUniform((stri + "].attConst").c_str());
		lightSpotAttLinLoc[i] =   getUniform((stri + "].attLin").c_str());
		lightSpotAttQuadLoc[i] =  getUniform((stri + "].attQuad").c_str());
		lightSpotInnerLoc[i] =    getUniform((stri + "].inner").c_str());
		lightSpotOuterLoc[i] =    getUniform((stri + "].outer").c_str());
		setUniformSpotLightEnable(i, false);
	}
	
	matAmbColLoc = getUniform("mat.ambCol");
	matAmbTexLoc = getUniform("mat.ambTex");
	matAmbTexEnLoc = getUniform("mat.ambTexEn");
	matDiffColLoc = getUniform("mat.diffCol");
	matDiffTexLoc = getUniform("mat.diffTex");
	matDiffTexEnLoc = getUniform("mat.diffTexEn");
	matSpecColLoc = getUniform("mat.specCol");
	matSpecTexLoc = getUniform("mat.specTex");
	matSpecTexEnLoc = getUniform("mat.specTexEn");
	matSpecShineLoc = getUniform("mat.specShine");
}

DefaultShader::~DefaultShader()
{
	delete[] lightPointEnLoc;
	delete[] lightPointPosLoc;
	delete[] lightPointAmbLoc;
	delete[] lightPointDiffLoc;
	delete[] lightPointSpecLoc;
	delete[] lightPointAttConstLoc;
	delete[] lightPointAttLinLoc;
	delete[] lightPointAttQuadLoc;
	
	delete[] lightSpotEnLoc;
	delete[] lightSpotPosLoc;
	delete[] lightSpotDirLoc;
	delete[] lightSpotAmbLoc;
	delete[] lightSpotDiffLoc;
	delete[] lightSpotSpecLoc;
	delete[] lightSpotAttConstLoc;
	delete[] lightSpotAttLinLoc;
	delete[] lightSpotAttQuadLoc;
	delete[] lightSpotInnerLoc;
	delete[] lightSpotOuterLoc;
}

void DefaultShader::render(const MatMesh& matmesh)
{
	setUniformMaterial(*matmesh.material);
	
	if(matmesh.material->ambientTextureEnable)
		matmesh.material->ambientTexture->bind(0);
	if(matmesh.material->diffuseTextureEnable)
		matmesh.material->diffuseTexture->bind(1);
	if(matmesh.material->specularTextureEnable)
		matmesh.material->specularTexture->bind(2);
	
	matmesh.mesh->bind();
	glDrawElements(GL_TRIANGLES, matmesh.mesh->getIndexLength(), GL_UNSIGNED_INT, 0);
}

void DefaultShader::render(const Model& model)
{
	setUniform4x4f(traMatLoc, model.transform);
	setUniform4x4f(norMatLoc, model.transform.calculateNormal(norMat));
	for(vector<MatMesh>::size_type i = 0; i < model.data.size(); i++)
		render(*model.data[i]);
}

void DefaultShader::setUniformCamera(Camera& camera)
{
	setUniform3f(camPosLoc, camera.position);
	setUniform4x4f(camMatLoc, camera.calculateMatrix(camMat));
}

void DefaultShader::setUniformMaterial(const Material& material)
{
	setUniform4f(matAmbColLoc, material.ambientColor);
	setUniform1i(matAmbTexLoc, 0);
	setUniform1b(matAmbTexEnLoc, material.ambientTextureEnable);
	setUniform4f(matDiffColLoc, material.diffuseColor);
	setUniform1i(matDiffTexLoc, 1);
	setUniform1b(matDiffTexEnLoc, material.diffuseTextureEnable);
	setUniform4f(matSpecColLoc, material.specularColor);
	setUniform1b(matSpecTexLoc, 2);
	setUniform1b(matSpecTexEnLoc, material.specularTextureEnable);
	setUniform1f(matSpecShineLoc, material.specularShine);
}

void DefaultShader::setUniformDirectionalLight(const DirectionalLight& light)
{
	setUniform3f(lightDirDirLoc,  light.direction);
	setUniform3f(lightDirAmbLoc,  light.ambient);
	setUniform3f(lightDirDiffLoc, light.diffuse);
	setUniform3f(lightDirSpecLoc, light.specular);
}

void DefaultShader::setUniformPointLight(size_t i, const PointLight& light)
{
	setUniform3f(lightPointPosLoc[i],      light.position);
	setUniform3f(lightPointAmbLoc[i],      light.ambient);
	setUniform3f(lightPointDiffLoc[i],     light.diffuse);
	setUniform3f(lightPointSpecLoc[i],     light.specular);
	setUniform1f(lightPointAttConstLoc[i], light.attenuationConstant);
	setUniform1f(lightPointAttLinLoc[i],   light.attenuationLinear);
	setUniform1f(lightPointAttQuadLoc[i],  light.attenuationQuadratic);
}

void DefaultShader::setUniformSpotLight(size_t i, const SpotLight& light)
{
	setUniform3f(lightSpotPosLoc[i],      light.position);
	setUniform3f(lightSpotDirLoc[i],      light.direction);
	setUniform3f(lightSpotAmbLoc[i],      light.ambient);
	setUniform3f(lightSpotDiffLoc[i],     light.diffuse);
	setUniform3f(lightSpotSpecLoc[i],     light.specular);
	setUniform1f(lightSpotAttConstLoc[i], light.attenuationConstant);
	setUniform1f(lightSpotAttLinLoc[i],   light.attenuationLinear);
	setUniform1f(lightSpotAttQuadLoc[i],  light.attenuationQuadratic);
	setUniform1f(lightSpotInnerLoc[i],    light.innerCutOff);
	setUniform1f(lightSpotOuterLoc[i],    light.outerCutOff);
}

void DefaultShader::setUniformDirectionalLightEnable(bool en)
{
	setUniform1b(lightDirEnLoc, en);
}

void DefaultShader::setUniformPointLightEnable(size_t i, bool en)
{
	setUniform1b(lightPointEnLoc[i], en);
}

void DefaultShader::setUniformSpotLightEnable(size_t i, bool en)
{
	setUniform1b(lightSpotEnLoc[i], en);
}

size_t DefaultShader::getMaxPointLights() const
{
	return maxPointLights;
}

size_t DefaultShader::getMaxSpotLights() const
{
	return maxSpotLights;
}
