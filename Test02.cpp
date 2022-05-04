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
        //是否按下了ECS
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

    // settings
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;

    //shader源代码
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


    //归一化的顶点
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
        //声明顶点着色器
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        //附加shader
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        //shader错误检测
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
            printf("Error:顶点着色器shader编译错误:\n %s", infoLog);
        }

        //声明片段着色器
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        //附加shader
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        //shader错误检测
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
            printf("Error:片段着色器shader编译错误:\n %s", infoLog);
        }

        //创建着色程序
        unsigned int shaderProgram = glCreateProgram();
        //连接顶点着色器与片段着色器
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        //错误检测
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
            printf("Error:着色程序连接错误:\n %s", infoLog);
        }

        //删除原始着色器对象
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return shaderProgram;
    }

    void Test02::InitVertexArrayObject()
    {
        //首先绑定顶点数组对象，然后绑定并设置顶点缓冲区，之后配置顶点属性
        //创建顶点数组对象
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        //将顶点数据写入顶点缓冲对象
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        //顶点数据解析规则
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);


        //创建顶点索引缓冲对象
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


        //创建颜色缓存对象
        glGenBuffers(1, &CBO);
        glBindBuffer(GL_ARRAY_BUFFER, CBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
        //数据解析规则
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);



        //取消绑定
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Test02::Draw(unsigned int& shaderProgram, unsigned int& VAO,size_t size)
    {
        //绘制
        //激活绘制shader
        glUseProgram(shaderProgram);

        float timeValue = glfwGetTime();
        float colorValue = sin(timeValue) / 2.0f + 0.5f;
        //获取属性位置
        int colorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        //变更属性
        glUniform4f(colorLocation, 0, colorValue, 0, 1);

        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
    }

    int Test02::CreateWindow()
    {

        glfwInit();
        //设定OpenGL 3.3 核心模式(Core-profile)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//大版本号
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//次版本号
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//使用核心模式(Core-profile)


        GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
        if (window == nullptr)
        {
            printf("创建GLFW窗口失败");
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            printf("初始化GLAD失败");
            return -1;
        }
        //初始窗口
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        //注册窗口大小变更回调
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        int nrAttributes;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
        printf("当前机器支持的最大顶点属性数量:%d", nrAttributes);

        //初始化渲染
        auto shaderProgram = InitShaderProgem();
        auto size = sizeof(indices);
        InitVertexArrayObject();
        size = size / 3;

        //渲染循环
        while (!glfwWindowShouldClose(window))
        {
            //输入
            processInput(window);

            //渲染指令
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            //绘制
            Draw(shaderProgram, VAO, size);

            //画面刷新，交换缓冲
            glfwSwapBuffers(window);
            //事件捕获
            glfwPollEvents();
        }
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteBuffers(1, &CBO);
        glDeleteProgram(shaderProgram);

        //释放
        glfwTerminate();
        return 0;
    }
}