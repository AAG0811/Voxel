#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
    public:
    Window(const char* title, int w, int h) : title(title), width(w), height(h) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        // glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

        #ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif
        // glfw window creation
        // --------------------
        window =
            glfwCreateWindow(w, h, "LearnOpenGL", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return;
        }
        glViewport(0, 0, w, h);
    }
    const char* title;
    int width;
    int height;
    GLFWwindow *window;
};