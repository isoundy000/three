﻿#include "Window.h"

#include <stdexcept>
#include <cassert>
#include <iostream>

Window::Window(const std::string& title, int width, int height)
    :
    width(width),
    height(height) {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        throw std::runtime_error("Window creation failed");
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    auto glewInitResult = glewInit();
    if (glewInitResult != GLEW_NO_ERROR) {
        std::cerr << glewGetErrorString(glewInitResult) << std::endl;
        throw std::runtime_error("GLEW initialization failed");
    }
    glGetError(); // GLEW has a long-existing bug where calling glewInit() always sets the GL_INVALID_ENUM error flag

    glfwSetKeyCallback(window, &Window::keyCallback);

    glViewport(0, 0, width, height);

    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    assert(glGetError() == GL_NO_ERROR);

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
}

Window::~Window() {
    glfwDestroyWindow(window);

    glfwTerminate();
}

bool Window::isRunning() const {
    return !glfwWindowShouldClose(window);
}

void Window::processEvents() {
    glfwPollEvents();
}

void Window::swapBuffers() {
    glfwSwapBuffers(window);
}

int Window::getWidth() const {
    return width;
}

int Window::getHeight() const {
    return height;
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}