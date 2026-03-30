// Caballero Antunez Jesus Yael - 320231364
// Práctica #8: Materiales e Iluminación
// 29 de marzo del 2026

// Std. Includes
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <ostream>

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
#include "SOIL2/SOIL2.h"
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
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(0.0f, 3.0f, 0.0f);
float movelightPos = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
bool activanim = false;

struct Material {
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;
};

enum Modelo {
  COUCH,
  CUSHIONS,
  PLANTS,
  STOOL,
  LAMP,
  CARPET,
  CHAIR,
  BED,
  DOG,
  NUM_MODELOS
};

#define SET_MAT(m)                                                             \
  glUniform3fv(                                                                \
      glGetUniformLocation(lightingShader.Program, "material.ambient"), 1,     \
      glm::value_ptr(mat[m].ambient));                                         \
  glUniform3fv(                                                                \
      glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1,     \
      glm::value_ptr(mat[m].diffuse));                                         \
  glUniform3fv(                                                                \
      glGetUniformLocation(lightingShader.Program, "material.specular"), 1,    \
      glm::value_ptr(mat[m].specular));                                        \
  glUniform1f(                                                                 \
      glGetUniformLocation(lightingShader.Program, "material.shininess"),      \
      mat[m].shininess);

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
      WIDTH, HEIGHT, "Práctica 8 Jesús Caballero", nullptr, nullptr);

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
  Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
  Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");

  // Load models
  Model sun((char *)"Models/Sol/sol.obj");
  Model moon((char *)"Models/Luna/luna.obj");
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

  float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, //
      0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, //
      0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, //
      0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, //
      -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, //
      -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, //

      -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f, //
      0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f, //
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //
      -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //
      -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f, //

      -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f, //
      -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f, //
      -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f, //
      -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f, //
      -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f, //
      -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f, //

      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, //
      0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f, //
      0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, //
      0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, //
      0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f, //
      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, //

      -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f, //
      0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f, //
      0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f, //
      0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f, //
      -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f, //
      -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f, //

      -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f, //
      0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f, //
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //
      -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //
      -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f  //
  };

  // First, set the container's VAO (and VBO)
  GLuint VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                        (GLvoid *)0);
  glEnableVertexAttribArray(0);
  // normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Load textures

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  int textureWidth, textureHeight, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *image;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                  GL_NEAREST_MIPMAP_NEAREST);

  // image = stbi_load("Models/Perro/Texture_albedo.jpg", &textureWidth,
  //                   &textureHeight, &nrChannels, 0);
  // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0,
  // GL_RGB,
  //              GL_UNSIGNED_BYTE, image);
  // glGenerateMipmap(GL_TEXTURE_2D);

  // if (image) {
  //   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0,
  //                GL_RGB, GL_UNSIGNED_BYTE, image);
  //   glGenerateMipmap(GL_TEXTURE_2D);
  // } else {
  //   std::cout << "Failed to load texture" << std::endl;
  // }
  // stbi_image_free(image);

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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lightingShader.Use();
    GLint lightPosLoc =
        glGetUniformLocation(lightingShader.Program, "light.position");
    GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");

    glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y,
                camera.GetPosition().z);

    glm::mat4 view = camera.GetViewMatrix();
    glUniformMatrix4fv(
        glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE,
        glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1,
                       GL_FALSE, glm::value_ptr(view));

    glm::mat4 model;

    Material mat[NUM_MODELOS];

    if (((int)(abs(movelightPos) + 90) / 180) % 2 == 0) {
      // Luz de Sol
      glUniform3f(lightPosLoc, -lightPos.y * sin(glm::radians(movelightPos)),
                  lightPos.y * cos(glm::radians(movelightPos)) + 0.5,
                  lightPos.z);
      glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"),
                  0.45f, 0.40f, 0.30f);
      glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"),
                  0.95f, 0.85f, 0.65f);
      glUniform3f(
          glGetUniformLocation(lightingShader.Program, "light.specular"), 1.00f,
          0.95f, 0.80f);

      mat[COUCH] = {{0.40f, 0.35f, 0.30f},
                    {0.70f, 0.60f, 0.50f},
                    {0.10f, 0.10f, 0.10f},
                    8.0f};

      mat[CUSHIONS] = {{0.45f, 0.35f, 0.35f},
                       {0.75f, 0.55f, 0.55f},
                       {0.05f, 0.05f, 0.05f},
                       4.0f};

      mat[PLANTS] = {{0.50f, 0.50f, 0.50f},
                     {0.80f, 0.80f, 0.80f},
                     {0.05f, 0.05f, 0.05f},
                     16.0f};

      mat[STOOL] = {{0.35f, 0.25f, 0.15f},
                    {0.60f, 0.45f, 0.25f},
                    {0.15f, 0.10f, 0.08f},
                    32.0f};

      mat[LAMP] = {{0.30f, 0.30f, 0.30f},
                   {0.50f, 0.50f, 0.50f},
                   {0.80f, 0.80f, 0.80f},
                   128.0f};

      mat[CARPET] = {{0.35f, 0.25f, 0.20f},
                     {0.60f, 0.45f, 0.35f},
                     {0.05f, 0.04f, 0.03f},
                     2.0f};

      mat[CHAIR] = {{0.30f, 0.30f, 0.30f},
                    {0.55f, 0.55f, 0.55f},
                    {0.40f, 0.40f, 0.40f},
                    64.0f};

      mat[BED] = {{0.40f, 0.30f, 0.25f},
                  {0.65f, 0.50f, 0.40f},
                  {0.05f, 0.04f, 0.03f},
                  4.0f};

      mat[DOG] = {{0.40f, 0.25f, 0.20f},
                  {0.70f, 0.40f, 0.30f},
                  {0.08f, 0.06f, 0.05f},
                  8.0f};

    } else {
      // Luz de Luna
      glUniform3f(
          lightPosLoc, -lightPos.y * sin(glm::radians(movelightPos + 180)),
          lightPos.y * cos(glm::radians(movelightPos + 180)) + 0.5, lightPos.z);
      glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"),
                  0.15f, 0.15f, 0.22f);
      glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"),
                  0.30f, 0.32f, 0.50f);
      glUniform3f(
          glGetUniformLocation(lightingShader.Program, "light.specular"), 0.35f,
          0.38f, 0.55f);

      mat[COUCH] = {{0.30f, 0.28f, 0.35f},
                    {0.50f, 0.46f, 0.55f},
                    {0.05f, 0.05f, 0.08f},
                    8.0f};

      mat[CUSHIONS] = {{0.28f, 0.24f, 0.32f},
                       {0.45f, 0.40f, 0.52f},
                       {0.03f, 0.03f, 0.05f},
                       4.0f};

      mat[PLANTS] = {{0.30f, 0.30f, 0.35f},
                     {0.45f, 0.45f, 0.52f},
                     {0.02f, 0.02f, 0.04f},
                     16.0f};

      mat[STOOL] = {{0.22f, 0.20f, 0.25f},
                    {0.38f, 0.34f, 0.42f},
                    {0.08f, 0.06f, 0.10f},
                    32.0f};

      mat[LAMP] = {{0.25f, 0.25f, 0.35f},
                   {0.35f, 0.35f, 0.50f},
                   {0.60f, 0.60f, 0.80f},
                   128.0f};

      mat[CARPET] = {{0.20f, 0.18f, 0.24f},
                     {0.35f, 0.30f, 0.40f},
                     {0.02f, 0.02f, 0.03f},
                     2.0f};

      mat[CHAIR] = {{0.25f, 0.25f, 0.32f},
                    {0.40f, 0.40f, 0.52f},
                    {0.25f, 0.25f, 0.38f},
                    64.0f};

      mat[BED] = {{0.22f, 0.20f, 0.26f},
                  {0.38f, 0.32f, 0.44f},
                  {0.03f, 0.02f, 0.04f},
                  4.0f};

      mat[DOG] = {{0.22f, 0.18f, 0.20f},
                  {0.38f, 0.28f, 0.32f},
                  {0.05f, 0.04f, 0.05f},
                  8.0f};
    }

    SET_MAT(COUCH);
    model = glm::mat4(1);
    glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1,
                       GL_FALSE, glm::value_ptr(model));
    couch.Draw(lightingShader);

    SET_MAT(CUSHIONS);
    model = glm::mat4(1);
    glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1,
                       GL_FALSE, glm::value_ptr(model));
    cushions.Draw(lightingShader);

    SET_MAT(PLANTS);
    model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(1.8672f, 0.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1,
                       GL_FALSE, glm::value_ptr(model));
    plants.Draw(lightingShader);

    SET_MAT(STOOL);
    model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(-1.691f, 0.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1,
                       GL_FALSE, glm::value_ptr(model));
    stool.Draw(lightingShader);

    SET_MAT(LAMP);
    model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(-1.691f, 0.4597f, 0.023f));
    model =
        glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(-0.004f, -0.004f, -0.004f));
    glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1,
                       GL_FALSE, glm::value_ptr(model));
    lamp.Draw(lightingShader);

    SET_MAT(CARPET);
    model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(0.1424f, 0.0f, 1.422f));
    model =
        glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.0007f, 0.0007f, 0.0007f));
    glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1,
                       GL_FALSE, glm::value_ptr(model));
    carpet.Draw(lightingShader);

    SET_MAT(CHAIR);
    model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(-1.521f, 0.0f, 1.181f));
    model =
        glm::rotate(model, glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1,
                       GL_FALSE, glm::value_ptr(model));
    chair.Draw(lightingShader);

    SET_MAT(BED);
    model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(1.7091f, 0.0f, 1.188f));
    model =
        glm::rotate(model, glm::radians(43.74f), glm::vec3(0.0f, -1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.029f, 0.029f, 0.029f));
    glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1,
                       GL_FALSE, glm::value_ptr(model));
    bed.Draw(lightingShader);

    SET_MAT(DOG);
    model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(1.6744f, 0.5058f, 1.214f));
    model =
        glm::rotate(model, glm::radians(50.73f), glm::vec3(0.0f, -1.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1,
                       GL_FALSE, glm::value_ptr(model));
    dog.Draw(lightingShader);

    glBindVertexArray(0);

    lampshader.Use();
    glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"),
                       1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1,
                       GL_FALSE, glm::value_ptr(view));

    if (((int)(abs(movelightPos) + 90) / 180) % 2 == 0) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
      model = glm::rotate(model, glm::radians(movelightPos),
                          glm::vec3(0.0f, 0.0f, 1.0f));
      model = glm::translate(model, lightPos);
      model = glm::scale(model, glm::vec3(0.3f));
      glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1,
                         GL_FALSE, glm::value_ptr(model));
      sun.Draw(lampshader);
    } else {
      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
      model = glm::rotate(model, glm::radians(movelightPos),
                          glm::vec3(0.0f, 0.0f, 1.0f));
      model = glm::translate(model, -lightPos);
      model = glm::scale(model, glm::vec3(0.2f));
      glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1,
                         GL_FALSE, glm::value_ptr(model));
      moon.Draw(lampshader);
    }

    glBindVertexArray(0);

    // Swap the buffers
    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

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

  if (activanim) {
    if (rot > -90.0f)
      rot -= 0.1f;
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

  if (keys[GLFW_KEY_O]) {

    movelightPos = (int)(movelightPos + 1) % 360;
  }

  if (keys[GLFW_KEY_L]) {

    movelightPos = (int)(movelightPos - 1) % 360;
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
