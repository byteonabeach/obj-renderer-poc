// src/main.cpp
#include <GLFW/glfw3.h>
#include <filesystem>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
import Camera;
import Mesh;
import Shader;
import Texture;

GLFWwindow *window = nullptr;
Graphics::Camera camera(glm::vec3(0.0f, 0.0f,
                                  10.0f));
float lastX = 800.0f / 2.0;
float lastY = 600.0f / 2.0;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void RenderCube();

const unsigned int scr_width = 800;
const unsigned int scr_height = 600;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " <model.obj> <texture.png>"
              << std::endl;
    return -1;
  }

  std::string objPath = argv[1];
  std::string texPath = argv[2];

  if (!std::filesystem::exists(objPath)) {
    std::cout << "Error: Model file does not exist: " << objPath << std::endl;
    return -1;
  }
  if (!std::filesystem::exists(texPath)) {
    std::cout << "Error: Texture file does not exist: " << texPath << std::endl;
    return -1;
  }

  if (!glfwInit()) {
    std::cout << "GLFW initialization failed\n";
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(scr_width, scr_height, "Model in Ice Cube", nullptr,
                            nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  try {
    Graphics::Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    Graphics::Shader iceShader(
        "shaders/ice_vertex.glsl",
        "shaders/ice_fragment.glsl");
    Graphics::Mesh mesh(objPath);
    const unsigned int tex = Graphics::loadTexture(texPath, false);

    while (!glfwWindowShouldClose(window)) {
      float currentFrame = static_cast<float>(glfwGetTime());
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      processInput(window);

      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glEnable(GL_DEPTH_TEST);

      shader.use();
      shader.setInt("textureSampler", 0);
      shader.setVec3("lightPos", glm::vec3(3.0f, 4.0f, 5.0f));
      shader.setVec3("viewPos", camera.Position);
      shader.setVec3("lightColor", {1.0f, 1.0f, 1.0f});
      shader.setFloat("ambientStrength", 0.2f);
      shader.setFloat("specularStrength", 0.8f);
      shader.setFloat("shininess", 32.0f);

      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model,
                             glm::vec3(0.0f, 0.0f, 0.0f));
      model = glm::rotate(model, currentFrame * glm::radians(30.0f),
                          glm::vec3(0.0f, 1.0f, 0.0f));

      glm::mat4 view = camera.GetViewMatrix();
      glm::mat4 projection = glm::perspective(
          glm::radians(camera.Zoom),
          static_cast<float>(scr_width) / static_cast<float>(scr_height), 0.1f,
          100.0f);

      shader.setMat4("model", model);
      shader.setMat4("view", view);
      shader.setMat4("projection", projection);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, tex);
      mesh.Draw(shader);

      glDisable(GL_DEPTH_TEST);

      iceShader.use();
      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
      model = glm::rotate(model, currentFrame,
                          glm::vec3(0.4f, 1.0f, 0.2f));
      model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));

      iceShader.setMat4("model", model);
      iceShader.setMat4("view", view);
      iceShader.setMat4("projection", projection);

      RenderCube();

      glEnable(GL_DEPTH_TEST);

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  } catch (const std::exception &e) {
    std::cout << "Exception occurred during rendering setup or loop: "
              << e.what() << std::endl;
    return -1;
  }

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {
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

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  if (firstMouse) {
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

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(yoffset);
}

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;

void RenderCube() {
  if (cubeVAO == 0) {
    float vertices[] = {
        -1.0f,
        -1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        -1.0f,
        -1.0f,
        1.0f,

        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,

        -1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,

        1.0f,
        1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        -1.0f,
        -1.0f,
        1.0f,
        -1.0f,
        -1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        1.0f,

        -1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        -1.0f,

        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
    };
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
  glBindVertexArray(cubeVAO);
  glDrawArrays(
      GL_TRIANGLES, 0,
      36);
  glBindVertexArray(0);
}
