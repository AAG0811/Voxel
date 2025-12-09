#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "camera.hpp"
#include "window.hpp"
#include "cube.hpp"
#include "aabb.hpp"

#define FNL_IMPL
#include "FastNoiseLite.h"

// #include "noise.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffest);

// settings
unsigned int SCR_WIDTH = 1200;
unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(0.0f, 20.0f, 3.0f));

float lastX = (float)SCR_WIDTH / 2;
float lastY = (float)SCR_HEIGHT / 2;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// SUN VARIABLES
glm::vec3 sundir = glm::normalize(glm::vec3(0.4, 1.0, 0.0));
// sun col = vec3(1.0, 0.95, 0.9)
// sky col = vec3(0.53, 0.81, 1.0)
// night col = vec3(0.6, 0.7, 0.8)
// // light pos
// glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    // camera.lookAt(sundir);
    glfwInit();
    // camera.setFPSCam();
    Window win("Voxel", SCR_WIDTH, SCR_HEIGHT);
    
    glfwSetFramebufferSizeCallback(win.window, framebuffer_size_callback);
    glfwSetCursorPosCallback(win.window, mouse_callback);
    glfwSetScrollCallback(win.window, scroll_callback);
    // capture mouse
    glfwSetInputMode(win.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);

    // define cube
    Material grass("shaders/textured.vert", "shaders/textured2.frag");
    grass.setTexture("assets/atlas.png");    
    Cube mycube;
    mycube.material = &grass;
    Shader skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");

    // world params
    int wwidth = 64;
    int wheight = 64;
    // setup noise
    fnl_state noise = fnlCreateState();
    float *noiseBuff = (float*)malloc(sizeof(float) * wwidth * wheight);
    noise.noise_type = FNL_NOISE_PERLIN;

    // load noise buffer
    for (int y = 0; y < wheight; y++) {
        for (int x = 0; x < wwidth; x++) {
            noiseBuff[y * wwidth + x] = fnlGetNoise2D(&noise, x, y);
            // std::cout << "noise val: " << noiseBuff[y*wwidth+x] << std::endl;
        }
    }

    // testing noise
    // PerlinNoise noise;
    // noise.amplitude = 1;
    // noise.frequency = 3;
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(win.window))
    {
        // delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        glfwPollEvents();
        processInput(win.window);

        // render
        // ------
        glClearColor(0.1f, 0.10f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model(1.0f);

        // collision
        vec3 pfeet = camera.Position - glm::vec3(0.0, 3.f, 0.0);

        // render world

        for (int cy = 0; cy < wheight; cy++) {
            for (int cx = 0; cx < wwidth; cx++) {
                int xpos = cx*2-(wwidth/2);
                int zpos = cy*2-(wheight/2);
                glm::vec3 cubePos = glm::vec3(xpos, (int)roundeven((noiseBuff[cy * wwidth + cx] * 40)) & ~1, zpos);
                model = glm::translate(glm::mat4(1.0), cubePos);
                mycube.render(model, view, projection);
                pfeet = aabbIntersect(pfeet, cubePos);
                camera.Position = pfeet + glm::vec3(0, 3, 0);
                
            }
        }
        
        // render skybox
        glDepthFunc(GL_LEQUAL);
        glDisable(GL_CULL_FACE);
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skyboxShader.use();
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        glBindVertexArray(mycube.getVAO());
        glDrawElements(GL_TRIANGLES, mycube.indices.size(), GL_UNSIGNED_INT, 0);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glfwSwapBuffers(win.window);
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffest)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffest));
}

