#include <iostream>

#include "../lib/glad.h" //glad needs to be included before glfw
#include "../gl.hpp"

using namespace std;

Shader::Shader(const char* vertex, const char* fragment, bool debug, int debugBufferLength)
{
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertex, NULL);
	glCompileShader(vertexShader);
	
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragment, NULL);
	glCompileShader(fragmentShader);
	
	if(debug)
	{
		char debug[debugBufferLength];
		
		glGetShaderInfoLog(vertexShader, debugBufferLength, NULL, debug);
		cout << "VERTEX SHADER COMPILATION LOG:" << endl;
		cout << debug << endl;
		
		glGetShaderInfoLog(fragmentShader, debugBufferLength, NULL, debug);
		cout << "FRAGMENT SHADER COMPILATION LOG:" << endl;
		cout << debug << endl;
	}
	
	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(id);
}

void Shader::use() const
{
	glUseProgram(id);
}

unsigned int Shader::getID() const
{
	return id;
}

int Shader::getUniform(const char* name) const
{
	return glGetUniformLocation(id, name);
}

void Shader::setUniform1b(const char* name, bool b)
{
	glUniform1i(getUniform(name), b ? 1 : 0);
}

void Shader::setUniform1i(const char* name, int i)
{
	glUniform1i(getUniform(name), i);
}

void Shader::setUniform1f(const char* name, float f)
{
	glUniform1f(getUniform(name), f);
}

void Shader::setUniform3f(const char* name, const Vector3& vec)
{
	glUniform3f(getUniform(name), vec.x, vec.y, vec.z);
}

void Shader::setUniform4f(const char* name, const Color& col)
{
	glUniform4f(getUniform(name), col.r, col.g, col.b, col.a);
}

void Shader::setUniform4x4f(const char* name, const Matrix4& mat)
{
	glUniformMatrix4fv(getUniform(name), 1, false, mat.val);
}

void Shader::setUniform1b(int loc, bool b)
{
	glUniform1i(loc, b ? 1 : 0);
}

void Shader::setUniform1i(int loc, int i)
{
	glUniform1i(loc, i);
}

void Shader::setUniform1f(int loc, float f)
{
	glUniform1f(loc, f);
}

void Shader::setUniform3f(int loc, const Vector3& vec)
{
	glUniform3f(loc, vec.x, vec.y, vec.z);
}

void Shader::setUniform4f(int loc, const Color& col)
{
	glUniform4f(loc, col.r, col.g, col.b, col.a);
}

void Shader::setUniform4x4f(int loc, const Matrix4& mat)
{
	glUniformMatrix4fv(loc, 1, false, mat.val);
}
