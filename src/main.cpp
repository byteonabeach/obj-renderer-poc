#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

import Camera;
import Mesh;
import Shader;
import Texture;

unsigned int scr_width = 1200, scr_height = 800;
Graphics::Camera camera{glm::vec3(0.0f, 0.0f, 5.0f)};
float lastX = static_cast<float>(scr_width) / 2.0f,
      lastY = static_cast<float>(scr_height) / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f, lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow *w, int width, int height) noexcept {
  scr_width = static_cast<unsigned int>(width);
  scr_height = static_cast<unsigned int>(height);

  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *w, double xpos, double ypos) noexcept {
  if (firstMouse) {
    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);
    firstMouse = false;
  }

  const float xoffset = static_cast<float>(xpos) - lastX;
  const float yoffset = lastY - static_cast<float>(ypos);
  lastX = static_cast<float>(xpos);
  lastY = static_cast<float>(ypos);
  camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *w, double xoffset, double yoffset) noexcept {
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow *window) noexcept {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(Graphics::CameraMovement::Forward, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(Graphics::CameraMovement::Backward, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(Graphics::CameraMovement::Left, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(Graphics::CameraMovement::Right, deltaTime);
}

int main() {
  if (!glfwInit()) {
    std::cout << "GLFW initialization failed\n";
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(
      static_cast<int>(scr_width), static_cast<int>(scr_height),
      "Homework - Phong Renderer", nullptr, nullptr);

  if (!window) {
    std::cout << "GLFW window creation failed\n";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "GLAD initialization failed\n";
    glfwTerminate();
    return -1;
  }

  glEnable(GL_DEPTH_TEST);

  try {
    Graphics::Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    Graphics::Mesh mesh("assets/models/head.obj");
    const unsigned int tex =
        Graphics::loadTexture("assets/textures/head.png", false);

    while (!glfwWindowShouldClose(window)) {
      const float currentFrame = static_cast<float>(glfwGetTime());
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      processInput(window);

      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      shader.use();
      shader.setInt("textureSampler", 0);
      shader.setVec3("lightPos", glm::vec3(3.0f, 4.0f, 5.0f));
      shader.setVec3("viewPos", camera.Position);
      shader.setVec3("lightColor", {1.0f, 1.0f, 1.0f});
      shader.setFloat("ambientStrength", 0.2f);
      shader.setFloat("specularStrength", 0.8f);
      shader.setFloat("shininess", 32.0f);

      glm::mat4 model = glm::mat4(1.0f);
      model = glm::rotate(model, currentFrame * glm::radians(30.0f),
                          glm::vec3(0.0f, 1.0f, 0.0f));
      glm::mat4 view = camera.GetViewMatrix();
      glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom),
                                        static_cast<float>(scr_width) /
                                            static_cast<float>(scr_height),
                                        0.1f, 100.0f);

      shader.setMat4("model", model);
      shader.setMat4("view", view);
      shader.setMat4("projection", proj);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, tex);
      mesh.Draw(shader);

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  } catch (const std::exception &e) {
    std::cout << "Exception occurred: " << e.what() << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwTerminate();
  return 0;
}
