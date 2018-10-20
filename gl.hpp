#ifndef GL_HPP
#define GL_HPP

#include <vector>
#include <GLFW/glfw3.h>

#include "math.hpp"

#define COLOR_CLEAR   0.f, 0.f, 0.f, 0.f
#define COLOR_BLACK   0.f, 0.f, 0.f, 1.f
#define COLOR_RED     1.f, 0.f, 0.f, 1.f
#define COLOR_GREEN   0.f, 1.f, 0.f, 1.f
#define COLOR_BLUE    0.f, 0.f, 1.f, 1.f
#define COLOR_YELLOW  1.f, 1.f, 0.f, 1.f
#define COLOR_CYAN    0.f, 1.f, 1.f, 1.f
#define COLOR_MAGENTA 1.f, 0.f, 1.f, 1.f
#define COLOR_WHITE   1.f, 1.f, 1.f, 1.f

struct Color
{
	float r, g, b, a;
	
	Color() : r(0.f), g(0.f), b(0.f), a(1.f) {}
	Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
	
	Color& set(float r, float g, float b, float a);
};

class Camera
{
	public:
		float speed = 0.05f;
		float sensitivity = 0.0025f;
		
		int keyForward = GLFW_KEY_W;
		int keyBackward = GLFW_KEY_S;
		int keyLeft = GLFW_KEY_A;
		int keyRight = GLFW_KEY_D;
		int keyUp = GLFW_KEY_SPACE;
		int keyDown = GLFW_KEY_LEFT_SHIFT;
		
		Vector3 position, direction, up;
		Matrix4 projection;
		
		Camera(float near, float far, float fovy, float aspectRatio);
		Camera(float near, float far, float fovy, float aspectRatio,
				const Vector3& position, const Vector3& direction, const Vector3& up);
		
		Camera(float left, float right, float bottom, float top, float near, float far);
		Camera(float left, float right, float bottom, float top, float near, float far,
				const Vector3& position, const Vector3& direction, const Vector3& up);
		
		float getFPSYaw() const;
		float getFPSPitch() const;
		void setFPSPitch(float pitch);
		void setFPSYaw(float yaw);
		void setFPSLookAt(Vector3 at);
		
		void calculateFPSView(GLFWwindow* window);
		Matrix4& calculateMatrix(Matrix4& out);
	
	private:
		Matrix4 view;
		long prvx, prvy;
		float yaw, pitch;
		bool firstFrame = true;
};

struct Vertex
{
	Vertex() : u(0.f), v(0.f) {}
	Vertex(const Vector3& position, const Vector3& normal, float u, float v) :
			position(position), normal(normal), u(u), v(v) {}
	
	Vector3 position, normal;
	float u, v;
};

class Texture
{
	public:
		Texture(const char* file, unsigned int wrapS = GL_REPEAT, unsigned int wrapT = GL_REPEAT,
				unsigned int minFilter = GL_LINEAR, unsigned int magFilter = GL_LINEAR);
		~Texture();
		
		void bind(unsigned int active) const;
		
		unsigned int getID() const;
		int getWidth() const;
		int getHeight() const;
	
	private:
		unsigned int id;
		int width, height;
		
		Texture(const Texture&);
		Texture& operator=(const Texture&);
};

struct Material
{
	Material() : ambientColor(COLOR_WHITE),            ambientTexture(nullptr),  ambientTextureEnable(false),
	             diffuseColor(COLOR_WHITE),            diffuseTexture(nullptr),  diffuseTextureEnable(false),
	             specularColor(0.5f, 0.5f, 0.5f, 1.f), specularTexture(nullptr), specularTextureEnable(false), specularShine(32.f) {}
	
	Color ambientColor;
	const Texture* ambientTexture;
	bool ambientTextureEnable;
	
	Color diffuseColor;
	const Texture* diffuseTexture;
	bool diffuseTextureEnable;
	
	Color specularColor;
	const Texture* specularTexture;
	bool specularTextureEnable;
	float specularShine;
};

class Mesh
{
	public:
		Mesh(Vertex vertices[], size_t vlength, unsigned int indices[], size_t ilength);
		~Mesh();
		
		void bind() const;
		
		unsigned int getVAO() const;
		unsigned int getVBO() const;
		unsigned int getEBO() const;
		
		size_t getVertexLength() const;
		size_t getIndexLength() const;
	
	private:
		unsigned int VAO, VBO, EBO;
		size_t vlength, ilength;
		
		Mesh(const Mesh&);
		Mesh& operator=(const Mesh&);
};

struct MatMesh
{
	MatMesh();
	MatMesh(Material* material, Mesh* mesh) : material(material), mesh(mesh) {}
	
	Material* material;
	Mesh* mesh;
};

struct Model
{
	std::vector<MatMesh*> data;
	Matrix4 transform;
	
	Model();
	Model(MatMesh* matmesh);
	
	Matrix4& calculateNormalMatrix(Matrix4& out) const;
};

class Shader
{
	public:
		Shader(const char* vertex, const char* fragment, bool debug = false, int debugBufferLength = 1024);
		~Shader();
		
		void use() const;
		unsigned int getID() const;
		int getUniform(const char* name) const;
		
		void setUniform1b(const char* name, bool b);
		void setUniform1i(const char* name, int i);
		void setUniform1f(const char* name, float f);
		void setUniform3f(const char* name, const Vector3& vec);
		void setUniform4f(const char* name, const Color& col);
		void setUniform4x4f(const char* name, const Matrix4& mat);
		
		void setUniform1b(int loc, bool b);
		void setUniform1i(int loc, int i);
		void setUniform1f(int loc, float f);
		void setUniform3f(int loc, const Vector3& vec);
		void setUniform4f(int loc, const Color& col);
		void setUniform4x4f(int loc, const Matrix4& mat);
	
	private:
		unsigned int id;
		
		Shader(const Shader&);
		Shader& operator=(const Shader&);
};

struct DirectionalLight
{
	DirectionalLight() : direction(VNY), diffuse(V1) {}
	
	Vector3 direction, ambient, diffuse, specular;
};

struct PointLight
{
	PointLight() : diffuse(V1), attenuationConstant(1.f), attenuationLinear(0.f), attenuationQuadratic(0.f) {}
	
	Vector3 position, ambient, diffuse, specular;
	float attenuationConstant, attenuationLinear, attenuationQuadratic;
};

struct SpotLight
{
	SpotLight() : direction(VNZ), diffuse(V1), attenuationConstant(1.f), attenuationLinear(0.f),
			attenuationQuadratic(0.f), innerCutOff(cos(7.5f * DEGRAD)), outerCutOff(cos(15.f * DEGRAD)) {}
	
	Vector3 position, direction, ambient, diffuse, specular;
	float attenuationConstant, attenuationLinear, attenuationQuadratic;
	float innerCutOff, outerCutOff; //must be cosine values
};

class DefaultShader : public Shader
{
	public:
		DefaultShader(size_t maxPointLights = 8, size_t maxSpotLights = 8);
		~DefaultShader();
		
		void render(const MatMesh& mesh);
		void render(const Model& model);
		
		void setUniformCamera(Camera& camera);
		void setUniformMaterial(const Material& material);
		void setUniformDirectionalLight(const DirectionalLight& light);
		void setUniformPointLight(size_t i, const PointLight& light);
		void setUniformSpotLight(size_t i, const SpotLight& light);
		void setUniformDirectionalLightEnable(bool en);
		void setUniformPointLightEnable(size_t i, bool en);
		void setUniformSpotLightEnable(size_t i, bool en);
		
		size_t getMaxPointLights() const;
		size_t getMaxSpotLights() const;
	
	private:
		size_t maxPointLights, maxSpotLights;
		
		int camPosLoc, camMatLoc, norMatLoc, traMatLoc;
		
		int lightDirEnLoc, lightDirDirLoc, lightDirAmbLoc, lightDirDiffLoc, lightDirSpecLoc;
		int *lightPointEnLoc, *lightPointPosLoc, *lightPointAmbLoc, *lightPointDiffLoc, *lightPointSpecLoc,
				*lightPointAttConstLoc, *lightPointAttLinLoc, *lightPointAttQuadLoc;
		int *lightSpotEnLoc, *lightSpotPosLoc, *lightSpotDirLoc, *lightSpotAmbLoc, *lightSpotDiffLoc, *lightSpotSpecLoc,
				*lightSpotAttConstLoc, *lightSpotAttLinLoc, *lightSpotAttQuadLoc, *lightSpotInnerLoc, *lightSpotOuterLoc;
		
		int matAmbColLoc, matAmbTexLoc, matAmbTexEnLoc;
		int matDiffColLoc, matDiffTexLoc, matDiffTexEnLoc;
		int matSpecColLoc, matSpecTexLoc, matSpecTexEnLoc, matSpecShineLoc;
		
		Matrix4 camMat, norMat;
		
		DefaultShader(const DefaultShader&);
		DefaultShader& operator=(const DefaultShader&);
};

class SimpleShader : public Shader
{
	public:
		SimpleShader();
		
		void render(const MatMesh& mesh);
		void render(const Model& model);
		
		void setUniformCamera(Camera& camera);
		void setUniformMaterial(const Material& material);
	
	private:
		int camMatLoc, traMatLoc;
		int matAmbColLoc, matAmbTexLoc, matAmbTexEnLoc;
		
		Matrix4 camMat;
		
		SimpleShader(const SimpleShader&);
		SimpleShader& operator=(const SimpleShader&);
};

void setVsync(bool enable);
void updateDeltaTime();
float getDeltaTime();
float getDeltaSpeed(float originalSpeed, int targetFPS = 60);

#endif
