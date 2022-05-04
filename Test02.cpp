#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include "Test02.h"
#include <iostream>
using namespace std;

namespace LearnTest02
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

    // settings
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;

    //shaderԴ����
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 ourColor;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos, 1.0);\n"
        "   ourColor = aColor;\n"
        "}\0";
    const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0);\n"
        "}\n\0";


    //��һ���Ķ���
    float vertices[] = {
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f,
    };
    float colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    unsigned int indices[] = { 
    0, 1, 2,
    };
    unsigned int VAO, VBO, EBO,CBO;

    unsigned int Test02::InitShaderProgem()
    {
        //����������ɫ��
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        //����shader
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        //shader������
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
            printf("Error:������ɫ��shader�������:\n %s", infoLog);
        }

        //����Ƭ����ɫ��
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        //����shader
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        //shader������
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
            printf("Error:Ƭ����ɫ��shader�������:\n %s", infoLog);
        }

        //������ɫ����
        unsigned int shaderProgram = glCreateProgram();
        //���Ӷ�����ɫ����Ƭ����ɫ��
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        //������
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
            printf("Error:��ɫ�������Ӵ���:\n %s", infoLog);
        }

        //ɾ��ԭʼ��ɫ������
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return shaderProgram;
    }

    void Test02::InitVertexArrayObject()
    {
        //���Ȱ󶨶����������Ȼ��󶨲����ö��㻺������֮�����ö�������
        //���������������
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        //����������д�붥�㻺�����
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        //�������ݽ�������
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);


        //�������������������
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


        //������ɫ�������
        glGenBuffers(1, &CBO);
        glBindBuffer(GL_ARRAY_BUFFER, CBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
        //���ݽ�������
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);



        //ȡ����
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Test02::Draw(unsigned int& shaderProgram, unsigned int& VAO,size_t size)
    {
        //����
        //�������shader
        glUseProgram(shaderProgram);

        float timeValue = glfwGetTime();
        float colorValue = sin(timeValue) / 2.0f + 0.5f;
        //��ȡ����λ��
        int colorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        //�������
        glUniform4f(colorLocation, 0, colorValue, 0, 1);

        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
    }

    int Test02::CreateWindow()
    {

        glfwInit();
        //�趨OpenGL 3.3 ����ģʽ(Core-profile)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//��汾��
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//�ΰ汾��
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//ʹ�ú���ģʽ(Core-profile)


        GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
        if (window == nullptr)
        {
            printf("����GLFW����ʧ��");
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            printf("��ʼ��GLADʧ��");
            return -1;
        }
        //��ʼ����
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        //ע�ᴰ�ڴ�С����ص�
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        int nrAttributes;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
        printf("��ǰ����֧�ֵ���󶥵���������:%d", nrAttributes);

        //��ʼ����Ⱦ
        auto shaderProgram = InitShaderProgem();
        auto size = sizeof(indices);
        InitVertexArrayObject();
        size = size / 3;

        //��Ⱦѭ��
        while (!glfwWindowShouldClose(window))
        {
            //����
            processInput(window);

            //��Ⱦָ��
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            //����
            Draw(shaderProgram, VAO, size);

            //����ˢ�£���������
            glfwSwapBuffers(window);
            //�¼�����
            glfwPollEvents();
        }
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteBuffers(1, &CBO);
        glDeleteProgram(shaderProgram);

        //�ͷ�
        glfwTerminate();
        return 0;
    }
}