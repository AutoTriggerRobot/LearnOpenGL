#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include "Test01.h"
#include <iostream>
using namespace std;

namespace LearnTest01
{

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void processInput(GLFWwindow* window)
	{
		//是否按下了ECS
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}

	int Test01::Learn01()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL01", nullptr, nullptr);
		if (window == nullptr)
		{
			cout << "创建GLFW窗口失败" << endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			cout << "初始化GLAD失败" << endl;
			return -1;
		}
		//初始窗口
		glViewport(0, 0, 800, 600);
		//注册窗口大小变更回调
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		//渲染循环
		while (!glfwWindowShouldClose(window))
		{
			//输入
			processInput(window);

			//渲染指令
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			//画面刷新，交换缓冲
			glfwSwapBuffers(window);
			//事件捕获
			glfwPollEvents();
		}

		//释放
		glfwTerminate();
		return 0;
	}

}