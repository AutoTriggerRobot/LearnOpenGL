#include "TestTexture.h"
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace TestTextureWin
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

    //归一化的顶点
    float vertices[] = {
       0.5f,  0.5f, 0.0f,
       0.5f, -0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,
       -0.5f,  0.5f, 0.0f,
    };
    unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
    };
    float colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f,
    };
    //贴图uv
    float texCoords[] = {
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
    };

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int CBO;
    unsigned int TBO;

    int TestTexture::CreateWindow()
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

        Shader ourShader("TestTextureVertexShader.txt","TestTexturePixelShader.txt");
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
            ourShader.use();
            float timeValue = glfwGetTime();
            float colorValue = sin(timeValue) / 2.0f + 0.5f;
            //变更属性
            ourShader.setFloat("ourColor", colorValue);
            //设置Texture
            ourShader.setInt("texture0", 0);
            ourShader.setInt("texture1", 1);

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

            //画面刷新，交换缓冲
            glfwSwapBuffers(window);
            //事件捕获
            glfwPollEvents();
        }
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteBuffers(1, &CBO);
        glDeleteBuffers(1, &TBO);

        //释放
        glfwTerminate();
        return 0;
    }

    void TestTexture::InitVertexArrayObject()
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

        //创建贴图缓存
        glGenBuffers(1, &TBO);
        glBindBuffer(GL_ARRAY_BUFFER, TBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
        //数据解析规则
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);

        //绑定纹理1
        unsigned int texture;
        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        //设置纹理参数
        //STR 设置贴图各个方向的循环方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //设置缩放方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        //加载纹理
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(false);
        unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            //生成Mipmap
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            printf("加载纹理图片失败");
        }

        //释放纹理2
        stbi_image_free(data); 

        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture);
        //设置纹理参数
        //STR 设置贴图各个方向的循环方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //设置缩放方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        //加载纹理
        stbi_set_flip_vertically_on_load(true);
        data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            //生成Mipmap
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            printf("加载纹理图片失败");
        }

        //释放纹理
        stbi_image_free(data);

        //取消绑定
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

}