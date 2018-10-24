#include <math.h>
#include "../gl.hpp"

Camera::Camera(float near, float far, float fovy, float aspectRatio)
{
	projection.setPerspective(near, far, fovy, aspectRatio);
	direction.set(VNZ);
	up.set(VY);
}

Camera::Camera(float near, float far, float fovy, float aspectRatio,
		const Vector3& position, const Vector3& direction, const Vector3& up)
{
	projection.setPerspective(near, far, fovy, aspectRatio);
	this->position = position;
	this->direction = direction;
	this->up = up;
}

Camera::Camera(float left, float right, float bottom, float top, float near, float far)
{
	projection.setOrthographic(left, right, bottom, top, near, far);
	direction.set(VNZ);
	up.set(VY);
}

Camera::Camera(float left, float right, float bottom, float top, float near, float far,
		const Vector3& position, const Vector3& direction, const Vector3& up)
{
	projection.setOrthographic(left, right, bottom, top, near, far);
	this->position = position;
	this->direction = direction;
	this->up = up;
}

float Camera::getFPSYaw() const
{
	return yaw;
}

float Camera::getFPSPitch() const
{
	return pitch;
}

void Camera::setFPSYaw(float yaw)
{
	while(yaw >= PI2) yaw -= PI2;
	while(yaw < 0.f)  yaw += PI2;
	this->yaw = yaw;
}

void Camera::setFPSPitch(float pitch)
{
	float min = -PIH + 0.1f;
	float max = PIH - 0.1f;
	
	if(pitch < min)
		this->pitch = min;
	else if(pitch > max)
		this->pitch = max;
	else
		this->pitch = pitch;
}

void Camera::setFPSLookAt(Vector3 at)
{
	at -= position;
	at.nor();
	
	setFPSYaw(-PIH + atan2(-at.z, at.x));
	setFPSPitch(atan2(at.y, sqrt(at.x * at.x + at.z * at.z)));
}

void Camera::calculateFPSView(GLFWwindow* window)
{
	double dmx, dmy;
	glfwGetCursorPos(window, &dmx, &dmy);
	
	long mx, my;
	mx = dmx;
	my = dmy;
	
	if(!firstFrame)
	{
		float dx = (mx - prvx) * sensitivity;
		float dy = (my - prvy) * sensitivity;
		
		setFPSYaw(yaw - dx);
		setFPSPitch(pitch - dy);
		
		up.set(VY);
		direction.set(VNZ);
		direction.rotate(up, yaw);
		direction.rotate(Vector3(direction).crs(up), pitch);
	}
	else
	{
		yaw = 0;
		pitch = 0;
		firstFrame = false;
	}
	
	prvx = mx;
	prvy = my;
	
	float deltaSpeed = getDeltaSpeed(speed);
	
	if(glfwGetKey(window, keyForward) == GLFW_PRESS)
		position += (Vector3(direction.x, 0.f, direction.z).nor() *= deltaSpeed);
	
	if(glfwGetKey(window, keyBackward) == GLFW_PRESS)
		position -= (Vector3(direction.x, 0.f, direction.z).nor() *= deltaSpeed);
	
	if(glfwGetKey(window, keyLeft) == GLFW_PRESS)
		position -= (Vector3(direction).crs(Vector3(VY)).nor() *= deltaSpeed);
	
	if(glfwGetKey(window, keyRight) == GLFW_PRESS)
		position += (Vector3(direction).crs(Vector3(VY)).nor() *= deltaSpeed);
	
	if(glfwGetKey(window, keyUp) == GLFW_PRESS)
		position.y += deltaSpeed;
	
	if(glfwGetKey(window, keyDown) == GLFW_PRESS)
		position.y -= deltaSpeed;
}

Matrix4& Camera::calculateMatrix(Matrix4& out)
{
	view.setView(position, direction, up);
	out = projection;
	return out *= view;
}
