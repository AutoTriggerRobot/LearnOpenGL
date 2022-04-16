#pragma once

namespace LearnTest02
{
	class Test02
	{
		unsigned int InitShaderProgem();
		void InitVertexArrayObject();
		void Draw(unsigned int& shaderProgram, unsigned int& VAO, size_t size);
	public:
		int CreateWindow();
	};
}
