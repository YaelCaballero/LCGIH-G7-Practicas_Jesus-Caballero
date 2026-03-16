// Caballero Antunez Jesus Yael - 320231364
// Práctica #6: Carga de modelos
// 15 de marzo del 2026

// Std. Includes
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Camera.h"
#include "Model.h"
#include "Shader.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
// #include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main() {
  // Init GLFW
  glfwInit();
  // Set all the required options for GLFW
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  // Create a GLFWwindow object that we can use for GLFW's functions
  GLFWwindow *window = glfwCreateWindow(
      WIDTH, HEIGHT, "Práctica 6 Jesús Caballero", nullptr, nullptr);

  if (nullptr == window) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();

    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);

  glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

  // Set the required callback functions
  glfwSetKeyCallback(window, KeyCallback);
  glfwSetCursorPosCallback(window, MouseCallback);

  // GLFW Options
  // glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

  // Set this to true so GLEW knows to use a modern approach to retrieving
  // function pointers and extensions
  glewExperimental = GL_TRUE;
  // Initialize GLEW to setup the OpenGL Function pointers
  GLenum err = glewInit();
  if (GLEW_OK != err && err != 4) {
    std::cout << "Failed to initialize GLEW" << std::endl;
    return EXIT_FAILURE;
  }

  // Define the viewport dimensions
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  // OpenGL options
  glEnable(GL_DEPTH_TEST);

  // Setup and compile our shaders
  Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");

  // Load models

  Model couch((char *)"Models/Sillon/sillon.obj");
  Model cushions((char *)"Models/Sillon/cojines-1.obj");
  Model plants((char *)"Models/Planta/planta.obj");
  Model stool((char *)"Models/Banco/banco.obj");
  Model lamp((char *)"Models/Lampara/lampara.obj");
  Model carpet((char *)"Models/Tapete/tapete.obj");
  Model bed((char *)"Models/Cama/pet_bed.obj");
  Model dog((char *)"Models/Perro/RedDog.obj");
  Model chair((char *)"Models/Silla/Eames_OBJ.obj");

  glm::mat4 projection = glm::perspective(
      camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f,
      100.0f);

  // Game loop
  while (!glfwWindowShouldClose(window)) {
    // Set frame time
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // Check and call events
    glfwPollEvents();
    DoMovement();

    // Clear the colorbuffer
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.Use();

    glm::mat4 view = camera.GetViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1,
                       GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1,
                       GL_FALSE, glm::value_ptr(view));

    // Draw the loaded model
    // Sillón
    glm::mat4 model(1);
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1,
                       GL_FALSE, glm::value_ptr(model));
    couch.Draw(shader);

    // Cojines
    model = glm::mat4(1);
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1,
                       GL_FALSE, glm::value_ptr(model));
    cushions.Draw(shader);

    // Plantas
    model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(1.8672f, 0.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1,
                       GL_FALSE, glm::value_ptr(model));
    plants.Draw(shader);

    // Banco
    model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(-1.691f, 0.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1,
                       GL_FALSE, glm::value_ptr(model));
    stool.Draw(shader);

    // Lámpara
    model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(-1.691f, 0.4597f, 0.023f));
    model =
        glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(-0.004f, -0.004f, -0.004f));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1,
                       GL_FALSE, glm::value_ptr(model));
    lamp.Draw(shader);

    // Tapete
    model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(0.1424f, 0.0f, 1.422f));
    model =
        glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.0007f, 0.0007f, 0.0007f));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1,
                       GL_FALSE, glm::value_ptr(model));
    carpet.Draw(shader);

    // Silla
    model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(-1.521f, 0.0f, 1.181f));
    model =
        glm::rotate(model, glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1,
                       GL_FALSE, glm::value_ptr(model));
    chair.Draw(shader);

    // Cama
    model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(1.7091f, 0.0f, 1.188f));
    model =
        glm::rotate(model, glm::radians(43.74f), glm::vec3(0.0f, -1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.029f, 0.029f, 0.029f));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1,
                       GL_FALSE, glm::value_ptr(model));
    bed.Draw(shader);

    // Perro
    model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(1.6744f, 0.5058f, 1.214f));
    model =
        glm::rotate(model, glm::radians(50.73f), glm::vec3(0.0f, -1.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1,
                       GL_FALSE, glm::value_ptr(model));
    dog.Draw(shader);

    // Swap the buffers
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement() {
  // Camera controls
  if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
    camera.ProcessKeyboard(FORWARD, deltaTime);
  }

  if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  }

  if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
    camera.ProcessKeyboard(LEFT, deltaTime);
  }

  if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
    camera.ProcessKeyboard(RIGHT, deltaTime);
  }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mode) {
  if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }

  if (key >= 0 && key < 1024) {
    if (action == GLFW_PRESS) {
      keys[key] = true;
    } else if (action == GLFW_RELEASE) {
      keys[key] = false;
    }
  }
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos) {
  if (firstMouse) {
    lastX = xPos;
    lastY = yPos;
    firstMouse = false;
  }

  GLfloat xOffset = xPos - lastX;
  GLfloat yOffset =
      lastY - yPos; // Reversed since y-coordinates go from bottom to left

  lastX = xPos;
  lastY = yPos;

  camera.ProcessMouseMovement(xOffset, yOffset);
}
