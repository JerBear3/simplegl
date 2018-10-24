#include <GLFW/glfw3.h>

float __dtc_deltaTime = 0, __dtc_prvTime = 0;

void setVsync(bool enable)
{
	glfwSwapInterval(enable ? 1 : 0);
}

void updateDeltaTime()
{
	float curTime = glfwGetTime();
	__dtc_deltaTime = curTime - __dtc_prvTime;
	__dtc_prvTime = curTime;
}

float getDeltaTime()
{
	return __dtc_deltaTime;
}

float getDeltaSpeed(float originalSpeed, int targetFPS)
{
	return originalSpeed * __dtc_deltaTime * targetFPS;
}
