#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>
#include <camera.h>
#include <shapes.h>
#include <globals.h>

#define CPP_SHADER_INCLUDE
#include <position.vert>
#include <position.frag>

#include <iostream>

// camera
Camera camera{ glm::vec3{0.0f, 0.0f, 3.0f} };

unsigned int cubeVAO{};
unsigned int instanceVBO{};

void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouseCallback(GLFWwindow*, double xPos, double yPos);
void renderCube(int instanceAmount);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window{ glfwCreateWindow(GLOBALS::SCR_WIDTH, GLOBALS::SCR_HEIGHT, "Mathematical Surfaces", nullptr, nullptr) };
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialise GLAD\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // instance stuff
    constexpr int amount{ 160000 };
    glm::vec3* instanceData{ new glm::vec3[amount]{} };

    // build and compile shaders
    Shader shader{};
    shader.compile(positionVert, positionFrag);

    // configure shaders

    // render loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        GLOBALS::deltaTime = currentFrame - GLOBALS::lastFrame;
        GLOBALS::lastFrame = currentFrame;

        // input
        processInput(window);

        // projection and view matrices
        glm::mat4 projection{ glm::perspective(glm::radians(camera.getZoom()),
            static_cast<float>(GLOBALS::SCR_WIDTH) / static_cast<float>(GLOBALS::SCR_HEIGHT), 0.1f, 1000.0f) };
        glm::mat4 view{ camera.getViewMatrix() };

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render
        // -------------------------------------------------
        shader.use();
        shader.setMatrix4("projection", projection);
        shader.setMatrix4("view", view);

        int index = 0;
        for (int z = -200; z < 200; ++z) {
            for (int x = -200; x < 200; ++x) {
                instanceData[index] = glm::vec3{ x, currentFrame, z };
                ++index;
            }
        }

        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::vec3), &instanceData[0], GL_STATIC_DRAW);

        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glVertexAttribDivisor(3, 1);

        renderCube(amount);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        glDeleteBuffers(1, &instanceVBO);
    }
    delete[] instanceData;
    glfwTerminate();
    return 0;
}

void frameBufferSizeCallback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    glfwSetCursorPosCallback(window, mouseCallback);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    int movementSpeedMultiplier{ 1 };
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        movementSpeedMultiplier = 2;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.processKeyboard(FORWARD, GLOBALS::deltaTime * movementSpeedMultiplier);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processKeyboard(BACKWARD, GLOBALS::deltaTime * movementSpeedMultiplier);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.processKeyboard(LEFT, GLOBALS::deltaTime * movementSpeedMultiplier);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.processKeyboard(RIGHT, GLOBALS::deltaTime * movementSpeedMultiplier);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.processKeyboard(UP, GLOBALS::deltaTime * movementSpeedMultiplier);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera.processKeyboard(DOWN, GLOBALS::deltaTime * movementSpeedMultiplier);
    }
}

void mouseCallback(GLFWwindow*, double xPos, double yPos) {
    float xPosF{ static_cast<float>(xPos) };
    float yPosF{ static_cast<float>(yPos) };

    if (GLOBALS::firstMouse) {
        GLOBALS::lastX = xPosF;
        GLOBALS::lastY = yPosF;
        GLOBALS::firstMouse = false;
    }

    float xOffset{ xPosF - GLOBALS::lastX };
    float yOffset{ GLOBALS::lastY - yPosF };
    GLOBALS::lastX = xPosF;
    GLOBALS::lastY = yPosF;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

unsigned int cubeVBO{ 0 };
void renderCube(int instanceAmount) {

    if (cubeVAO == 0) {
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Shapes::cube), &Shapes::cube, GL_STATIC_DRAW);

        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    glBindVertexArray(cubeVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, instanceAmount);
    glBindVertexArray(0);
}