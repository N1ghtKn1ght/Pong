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

#define WIDTH 640
#define HEIGHT 480
#define SPEED 40

glm::ivec2 g_windowSize(WIDTH, HEIGHT);
glm::vec2 g_blockPosition(10.f, HEIGHT / 2 - 50);
glm::vec2 g_blockSize(30, 125);
glm::vec2 g_ballPosition(WIDTH / 2, HEIGHT / 2);
glm::vec2 g_ballSize(25, 25);
glm::vec2 velocity(-5, -5);

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
    if (key == GLFW_KEY_W)
    {
        GLfloat value = g_blockPosition.y + SPEED + g_blockSize.y;
        if(g_windowSize.y > value)
            g_blockPosition += glm::vec2(0, SPEED);
    }
    if (key == GLFW_KEY_S)
    {
        GLfloat value = g_blockPosition.y - SPEED;
        if (0 <= value)
            g_blockPosition += glm::vec2(0, -SPEED);
        else
            g_blockPosition += glm::vec2(0, 0);
    }
}

void MoveBall()
{
    g_ballPosition += velocity;
    auto value = g_blockPosition.y + g_blockSize.y;
    if (g_ballPosition.x <= 0.f)
    {
        velocity.x = -velocity.x;
        g_ballPosition.x = g_windowSize.x - g_ballSize.x;
    }
    else if (g_ballPosition.x <= g_blockPosition.x + g_blockSize.x && (g_ballPosition.y >= g_blockPosition.y && g_ballPosition.y <= value))
    {
        velocity.x = -velocity.x;
        g_ballPosition.x = g_blockPosition.x + g_blockSize.x;
    }
    else if (g_ballPosition.x + g_ballSize.x >= g_windowSize.x)
    {
        velocity.x = -velocity.x;
        g_ballPosition.x = g_windowSize.x - g_ballSize.x;
    }
    else if (g_ballPosition.y <= 0.f)
    {
        velocity.y = -velocity.y;
        g_ballPosition.y = 0;
    }
    else if (g_ballPosition.y + g_ballSize.y >= g_windowSize.y)
    {
        velocity.y = -velocity.y;
        g_ballPosition.y = g_windowSize.y - g_ballSize.y;
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

        shared_ptr<Render::ShaderProgram> pSpriteShaderProgram = resourceManager.loadShaders("SpriteShader", "resources/shaders/vSprite.txt", "resources/shaders/fSprite.txt");
        if (!pSpriteShaderProgram)
        {
            cerr << "cant create shader program" << endl;
            return -1;
        }

        auto tex = resourceManager.loadTexture("Default", "/resources/textures/error.png");

        auto block_1 = resourceManager.loadSprite("Block_1", "Default", "SpriteShader", g_blockSize.x, g_blockSize.y);
        block_1->setPosition(g_blockPosition);
        auto ball = resourceManager.loadSprite("ball", "Default", "SpriteShader", 25, 25);
        ball->setPosition(glm::vec2(g_windowSize.x / 2, g_windowSize.y / 2));

        glm::mat4 modelMatrix = glm::mat4(1.f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(50.f, 200.f, 0.f));

        glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_windowSize.x), 0.f, static_cast<float>(g_windowSize.y), -100.f, 100.f);

        pSpriteShaderProgram->use();
        pSpriteShaderProgram->setMat4("projectionMat", projectionMatrix);

        while (!glfwWindowShouldClose(pWindow))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);
            tex->bind();
           
            ball->render();
            MoveBall();
            ball->setPosition(g_ballPosition);
            block_1->render();
            block_1->setPosition(g_blockPosition);

            glfwSwapBuffers(pWindow);

            glfwPollEvents();

            Sleep(8);
        }
    }

    glfwTerminate();
    return 0;
}