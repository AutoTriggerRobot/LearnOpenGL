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
        //�Ƿ�����ECS
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

    // settings
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;

    //��һ���Ķ���
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
    //��ͼuv
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

        Shader ourShader("TestTextureVertexShader.txt","TestTexturePixelShader.txt");
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
            ourShader.use();
            float timeValue = glfwGetTime();
            float colorValue = sin(timeValue) / 2.0f + 0.5f;
            //�������
            ourShader.setFloat("ourColor", colorValue);
            //����Texture
            ourShader.setInt("texture0", 0);
            ourShader.setInt("texture1", 1);

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

            //����ˢ�£���������
            glfwSwapBuffers(window);
            //�¼�����
            glfwPollEvents();
        }
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteBuffers(1, &CBO);
        glDeleteBuffers(1, &TBO);

        //�ͷ�
        glfwTerminate();
        return 0;
    }

    void TestTexture::InitVertexArrayObject()
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

        //������ͼ����
        glGenBuffers(1, &TBO);
        glBindBuffer(GL_ARRAY_BUFFER, TBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
        //���ݽ�������
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);

        //������1
        unsigned int texture;
        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        //�����������
        //STR ������ͼ���������ѭ����ʽ
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //�������ŷ�ʽ
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        //��������
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(false);
        unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            //����Mipmap
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            printf("��������ͼƬʧ��");
        }

        //�ͷ�����2
        stbi_image_free(data); 

        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture);
        //�����������
        //STR ������ͼ���������ѭ����ʽ
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //�������ŷ�ʽ
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        //��������
        stbi_set_flip_vertically_on_load(true);
        data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            //����Mipmap
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            printf("��������ͼƬʧ��");
        }

        //�ͷ�����
        stbi_image_free(data);

        //ȡ����
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

}