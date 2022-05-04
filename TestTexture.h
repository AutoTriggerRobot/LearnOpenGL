#pragma once
#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
using namespace std;

namespace TestTextureWin
{
	class TestTexture
	{
		void InitVertexArrayObject();
    public:
		int CreateWindow();
	};
}
