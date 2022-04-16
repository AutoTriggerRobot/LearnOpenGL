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
		//�Ƿ�����ECS
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
			cout << "����GLFW����ʧ��" << endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			cout << "��ʼ��GLADʧ��" << endl;
			return -1;
		}
		//��ʼ����
		glViewport(0, 0, 800, 600);
		//ע�ᴰ�ڴ�С����ص�
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		//��Ⱦѭ��
		while (!glfwWindowShouldClose(window))
		{
			//����
			processInput(window);

			//��Ⱦָ��
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			//����ˢ�£���������
			glfwSwapBuffers(window);
			//�¼�����
			glfwPollEvents();
		}

		//�ͷ�
		glfwTerminate();
		return 0;
	}

}