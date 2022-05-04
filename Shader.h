#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

class Shader
{
public:
	// 程序id
	unsigned int ID;

	//读取并构建着色器
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		string vertexCode;
		string fragmentCode;
		ifstream vShaderFile;
		ifstream fShaderFile;
		// 保证ifstream对象可以抛出异常：
		vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
		fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
		try
		{
			// 打开文件
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			// 读取文件的缓冲内容到数据流中
			stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// 关闭文件处理器
			vShaderFile.close();
			fShaderFile.close();
			// 转换数据流到string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (ifstream::failure e)
		{
			printf("shader文件读取错误: %s",e.what());
		}
		const char* vertexShaderSource = vertexCode.c_str();
		const char* fragmentShaderSource = fragmentCode.c_str();

		//声明顶点着色器
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		//附加shader
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		checkComplieErrors(vertexShader,"Vertex");

		//声明片段着色器
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		//附加shader
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		checkComplieErrors(fragmentShader, "Fragment");

		//创建着色程序
		ID = glCreateProgram();
		//连接顶点着色器与片段着色器
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);

		checkComplieErrors(ID, "Program");

		//删除原始着色器对象
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	~Shader()
	{
		glDeleteProgram(ID);
	}

	//使用着色器
	void use()
	{
		glUseProgram(ID);
	}

	//uniform参数设置
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}


private:
	//shader错误检测
	void checkComplieErrors(unsigned int shader, string type)
	{
		int success;
		char infoLog[1024];
		if(type == "Program")
		{
			glGetProgramiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, sizeof(infoLog), NULL, infoLog);
				printf("Error:着色器shader编译错误:%s\n %s", type.c_str(), infoLog);
			}
		}
		else
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
				printf("Error:着色器shader编译错误:%s\n %s", type.c_str(), infoLog);
			}
		}
	}

};
#endif

