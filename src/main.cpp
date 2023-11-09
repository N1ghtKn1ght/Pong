#include <iostream>
#include <Windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "render/ShaderProgram.h"
#include "render/Texture2D.h"
#include "resources/ResourceManager.h"
#include "render/Sprite.h"

using namespace std; 

GLfloat point[] = {
     0.0f,  50.f, 0.0f,
     50.f,  -50.f, 0.0f,
    -50.f, -50.f, 0.0f,
};

GLfloat colors[] = {
    0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
};

GLfloat textureCoord[] = {
    0.5f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
};

glm::ivec2 g_windowSize(640, 480);

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSize.x = width;
    g_windowSize.y = height;
    glViewport(0, 0, g_windowSize.x, g_windowSize.y);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(int argc, char** argv)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "Pong", NULL, NULL);
    if (!pWindow)
    {
        glfwTerminate();
        cout << "glfwCreateWindow failed!" << endl;
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGL())
    {
        cout << "cant load glad" << endl;
        return -1;
    }

    cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << endl;

    {
        ResourceManager resourceManager(argv[0]);
        shared_ptr<Render::ShaderProgram> pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "resources/shaders/vertex.txt", "resources/shaders/fragment.txt");
        if (!pDefaultShaderProgram)
        {
            cerr << "cant create shader program" << endl;
            return -1;
        }

        shared_ptr<Render::ShaderProgram> pSpriteShaderProgram = resourceManager.loadShaders("SpriteShader", "resources/shaders/vSprite.txt", "resources/shaders/fSprite.txt");
        if (!pDefaultShaderProgram)
        {
            cerr << "cant create shader program" << endl;
            return -1;
        }

        auto tex = resourceManager.loadTexture("Default", "/resources/textures/error.png");
        auto sprite = resourceManager.loadSprite("DefaulfSprite", "Default", "SpriteShader", 100, 200);
        sprite->setPosition(glm::vec2(300.f, 200.f));
        sprite->setRotation(45);


        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

        GLuint color_vbo = 0;
        glGenBuffers(1, &color_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        GLuint textureCoord_vbo = 0;
        glGenBuffers(1, &textureCoord_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, textureCoord_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoord), textureCoord, GL_STATIC_DRAW);

        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, textureCoord_vbo);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
          
        pDefaultShaderProgram->use();
        pDefaultShaderProgram->setInt("tex", 0);

        glm::mat4 modelMatrix = glm::mat4(1.f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(50.f, 200.f, 0.f));

        glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_windowSize.x), 0.f, static_cast<float>(g_windowSize.y), -100.f, 100.f);
        pDefaultShaderProgram->setMat4("projectionMat", projectionMatrix);

        pSpriteShaderProgram->use();
        pSpriteShaderProgram->setMat4("projectionMat", projectionMatrix);
        float value = 0;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);
            pDefaultShaderProgram->use();
            glBindVertexArray(vao);
            tex->bind();
                
            /*      modelMatrix = glm::mat4(1.f);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(value, 200.f, 0.f));
            value += 10.f;
            if (g_windowSize.x < value - 50.f)
                value = 0.f;*/
            pDefaultShaderProgram->setMat4("modelMat", modelMatrix);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            value += 1.f;
           
            sprite->setRotation(value);
            sprite->render();


            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);
            /* Poll for and process events */
            glfwPollEvents();

            Sleep(8);
        }
    }

    glfwTerminate();
    return 0;
}