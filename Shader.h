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
	// ����id
	unsigned int ID;

	//��ȡ��������ɫ��
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		string vertexCode;
		string fragmentCode;
		ifstream vShaderFile;
		ifstream fShaderFile;
		// ��֤ifstream��������׳��쳣��
		vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
		fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
		try
		{
			// ���ļ�
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			// ��ȡ�ļ��Ļ������ݵ���������
			stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// �ر��ļ�������
			vShaderFile.close();
			fShaderFile.close();
			// ת����������string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (ifstream::failure e)
		{
			printf("shader�ļ���ȡ����: %s",e.what());
		}
		const char* vertexShaderSource = vertexCode.c_str();
		const char* fragmentShaderSource = fragmentCode.c_str();

		//����������ɫ��
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		//����shader
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		checkComplieErrors(vertexShader,"Vertex");

		//����Ƭ����ɫ��
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		//����shader
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		checkComplieErrors(fragmentShader, "Fragment");

		//������ɫ����
		ID = glCreateProgram();
		//���Ӷ�����ɫ����Ƭ����ɫ��
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);

		checkComplieErrors(ID, "Program");

		//ɾ��ԭʼ��ɫ������
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	~Shader()
	{
		glDeleteProgram(ID);
	}

	//ʹ����ɫ��
	void use()
	{
		glUseProgram(ID);
	}

	//uniform��������
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
	//shader������
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
				printf("Error:��ɫ��shader�������:%s\n %s", type.c_str(), infoLog);
			}
		}
		else
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
				printf("Error:��ɫ��shader�������:%s\n %s", type.c_str(), infoLog);
			}
		}
	}

};
#endif

