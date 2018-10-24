#include "lib/glad.h"

#include <iostream>
#include <GLFW/glfw3.h>

#include "math.hpp"
#include "gl.hpp"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720
#define CAM_NEAR 0.1f
#define CAM_FAR 100.f
#define CAM_FOVY 67.f * DEGRAD

using namespace std;

Camera camera(CAM_NEAR, CAM_FAR, CAM_FOVY, (float) WIN_WIDTH / WIN_HEIGHT);

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

int main(int argc, char* argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "simplegl demo", NULL, NULL);
	
	if(window == NULL)
	{
		cerr << "Failed to create window" << endl;
		glfwTerminate();
		return 1;
	}
	
	glfwMakeContextCurrent(window);
	
	if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		cerr << "Failed to initialize GLAD" << endl;
		glfwTerminate();
		return 1;
	}
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	setVsync(true);
	
	//opengl scope
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
		
		DefaultShader shader(1, 1);
		
		DirectionalLight dlight;
		dlight.direction.set(-1.f, -0.8f, -0.2f);
		dlight.ambient.set(0.4f, 0.4f, 0.4f);
		dlight.specular.set(V1);
		shader.setUniformDirectionalLightEnable(true);
		shader.setUniformDirectionalLight(dlight);
		
		Material mat;
		Texture texture("bonzi.png");
		mat.ambientTexture = &texture;
		mat.diffuseTexture = &texture;
		mat.ambientTextureEnable = true;
		mat.diffuseTextureEnable = true;
		
		Vertex* vbuf = new Vertex[24];
		unsigned int* ibuf = new unsigned int[36];
		
		float hw = 0.5f;
		float hh = 0.5f;
		float hd = 0.5f;
		
		vbuf[ 0] = Vertex(Vector3(-hw, -hh,  hd), Vector3(VZ),  0.f, 1.f); //BL  front
		vbuf[ 1] = Vertex(Vector3( hw, -hh,  hd), Vector3(VZ),  1.f, 1.f); //BR
		vbuf[ 2] = Vertex(Vector3(-hw,  hh,  hd), Vector3(VZ),  0.f, 0.f); //TL
		vbuf[ 3] = Vertex(Vector3( hw,  hh,  hd), Vector3(VZ),  1.f, 0.f); //TR
		
		vbuf[ 4] = Vertex(Vector3(-hw, -hh, -hd), Vector3(VNZ), 1.f, 1.f); //BR  back
		vbuf[ 5] = Vertex(Vector3( hw, -hh, -hd), Vector3(VNZ), 0.f, 1.f); //BL
		vbuf[ 6] = Vertex(Vector3(-hw,  hh, -hd), Vector3(VNZ), 1.f, 0.f); //TR
		vbuf[ 7] = Vertex(Vector3( hw,  hh, -hd), Vector3(VNZ), 0.f, 0.f); //TL
		
		vbuf[ 8] = Vertex(Vector3(-hw, -hh, -hd), Vector3(VNX), 0.f, 1.f); //BL  left
		vbuf[ 9] = Vertex(Vector3(-hw, -hh,  hd), Vector3(VNX), 1.f, 1.f); //BR
		vbuf[10] = Vertex(Vector3(-hw,  hh, -hd), Vector3(VNX), 0.f, 0.f); //TL
		vbuf[11] = Vertex(Vector3(-hw,  hh,  hd), Vector3(VNX), 1.f, 0.f); //TR
		
		vbuf[12] = Vertex(Vector3( hw, -hh, -hd), Vector3(VX),  1.f, 1.f); //BR  right
		vbuf[13] = Vertex(Vector3( hw, -hh,  hd), Vector3(VX),  0.f, 1.f); //BL
		vbuf[14] = Vertex(Vector3( hw,  hh, -hd), Vector3(VX),  1.f, 0.f); //TR
		vbuf[15] = Vertex(Vector3( hw,  hh,  hd), Vector3(VX),  0.f, 0.f); //TL
		
		vbuf[16] = Vertex(Vector3(-hw,  hh,  hd), Vector3(VY),  0.f, 1.f); //BL  top
		vbuf[17] = Vertex(Vector3( hw,  hh,  hd), Vector3(VY),  1.f, 1.f); //BR
		vbuf[18] = Vertex(Vector3(-hw,  hh, -hd), Vector3(VY),  0.f, 0.f); //TL
		vbuf[19] = Vertex(Vector3( hw,  hh, -hd), Vector3(VY),  1.f, 0.f); //TR
		
		vbuf[20] = Vertex(Vector3(-hw, -hh,  hd), Vector3(VNY), 0.f, 0.f); //TL  bottom
		vbuf[21] = Vertex(Vector3( hw, -hh,  hd), Vector3(VNY), 1.f, 0.f); //TR
		vbuf[22] = Vertex(Vector3(-hw, -hh, -hd), Vector3(VNY), 0.f, 1.f); //BL
		vbuf[23] = Vertex(Vector3( hw, -hh, -hd), Vector3(VNY), 1.f, 1.f); //BR
		
		ibuf[ 0] = 0;  ibuf[ 1] = 1;  ibuf[ 2] = 2; //front
		ibuf[ 3] = 3;  ibuf[ 4] = 2;  ibuf[ 5] = 1;
		ibuf[ 6] = 6;  ibuf[ 7] = 5;  ibuf[ 8] = 4; //back
		ibuf[ 9] = 5;  ibuf[10] = 6;  ibuf[11] = 7;
		ibuf[12] = 8;  ibuf[13] = 9;  ibuf[14] = 10; //left
		ibuf[15] = 11; ibuf[16] = 10; ibuf[17] = 9;
		ibuf[18] = 14; ibuf[19] = 13; ibuf[20] = 12; //right
		ibuf[21] = 13; ibuf[22] = 14; ibuf[23] = 15;
		ibuf[24] = 16; ibuf[25] = 17; ibuf[26] = 18; //top
		ibuf[27] = 19; ibuf[28] = 18; ibuf[29] = 17;
		ibuf[30] = 22; ibuf[31] = 21; ibuf[32] = 20; //bottom
		ibuf[33] = 21; ibuf[34] = 22; ibuf[35] = 23;
		
		Mesh mesh(vbuf, 24, ibuf, 36);
		delete[] vbuf;
		delete[] ibuf;
		
		MatMesh matmesh(&mat, &mesh);
		
		Model model(&matmesh);
		model.transform.setTranslation(Vector3(0.f, 0.f, -3.f));
		
		//render loop
		while(!glfwWindowShouldClose(window))
		{
			//---begin drawing---//
			glClearColor(COLOR_BLACK);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				break;
			
			camera.calculateFPSView(window);
			shader.setUniformCamera(camera);
			
			model.transform.rotate(Vector3(VY), getDeltaSpeed(1.f) * DEGRAD);
			shader.render(model);
			//---end drawing---//
			
			glfwSwapBuffers(window);
			glfwPollEvents();
			updateDeltaTime();
		}
	}
	
	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	camera.projection.setPerspective(CAM_NEAR, CAM_FAR, CAM_FOVY, (float) width / height);
}
