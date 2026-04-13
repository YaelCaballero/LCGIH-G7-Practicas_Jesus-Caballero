// Caballero Antunez Jesus Yael - 320231364
// Reporte #9: Fuentes de Luz
// 12 de abril del 2026

#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Load Models
#include "SOIL2/SOIL2.h"

// Other includes
#include "Camera.h"
#include "Model.h"
#include "Shader.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(-1.691f, 0.68f, 0.023f), glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)};

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

glm::vec3 Light1 = glm::vec3(0);

// Deltatime
GLfloat deltaTime = 0.0f; // Time between current frame and last frame
GLfloat lastFrame = 0.0f; // Time of last frame

struct Material {
  float shininess;
  float specularStrength;
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
  PANTALLA_BASE,
  LAMPARA_PISO,
  NUM_MODELOS
};

#define SET_MAT(m)                                                             \
  glUniform1f(                                                                 \
      glGetUniformLocation(lightingShader.Program, "material.shininess"),      \
      mat[m].shininess);                                                       \
  glUniform1f(glGetUniformLocation(lightingShader.Program,                     \
                                   "material.specularStrength"),               \
              mat[m].specularStrength);

int main() {
  // Init GLFW
  glfwInit();
  // Set all the required options for GLFW
  /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

  // Create a GLFWwindow object that we can use for GLFW's functions
  GLFWwindow *window = glfwCreateWindow(
      WIDTH, HEIGHT, "Práctica 9 Jesús Caballero", nullptr, nullptr);

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
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

  Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
  Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

  Model couch((char *)"Models/Sillon/sillon.obj");
  Model cushions((char *)"Models/Sillon/cojines-1.obj");
  Model plants((char *)"Models/Planta/planta.obj");
  Model stool((char *)"Models/Banco/banco.obj");
  Model lamp((char *)"Models/Lampara/lampara.obj");
  Model carpet((char *)"Models/Tapete/tapete.obj");
  Model bed((char *)"Models/Cama/pet_bed.obj");
  Model dog((char *)"Models/Perro/RedDog.obj");
  Model chair((char *)"Models/Silla/Eames_OBJ.obj");

  Model pantallaScreen((char *)"Models/Pantalla/pantalla.obj");
  Model pantallaBase((char *)"Models/Pantalla/base.obj");

  Model lampPiso((char *)"Models/LamparaPiso/lamp.obj");
  Model foco((char *)"Models/LamparaPiso/foco.obj");

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

  // Set texture units
  lightingShader.Use();
  glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"),
              0);
  glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"),
              1);

  glm::mat4 projection = glm::perspective(
      camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f,
      100.0f);

  // Game loop
  while (!glfwWindowShouldClose(window)) {

    // Calculate deltatime of current frame
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // Check if any events have been activiated (key pressed, mouse moved etc.)
    // and call corresponding response functions
    glfwPollEvents();
    DoMovement();

    // Clear the colorbuffer
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Load Model

    // Use cooresponding shader when setting uniforms/drawing objects
    lightingShader.Use();

    glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
    // glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

    GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
    glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y,
                camera.GetPosition().z);

    // Point Light
    // Lámpara de mesa
    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "pointLights[0].position"),
        pointLightPositions[0].x, pointLightPositions[0].y,
        pointLightPositions[0].z);
    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"),
        0.15f, 0.1f, 0.05f);
    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"),
        1.0f, 0.85f, 0.6f);
    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"),
        1.0f, 0.9f, 0.7f);
    glUniform1f(
        glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"),
        1.0f);
    glUniform1f(
        glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"),
        0.045f);
    glUniform1f(glGetUniformLocation(lightingShader.Program,
                                     "pointLights[0].quadratic"),
                0.075f);

    // SpotLight
    // Lámpara de piso
    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "spotLights[0].position"),
        1.0, 2.0f, 0.0f);
    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "spotLights[0].direction"),
        0.0f, -1.0f, 0.0f);
    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "spotLights[0].ambient"),
        0.15f, 0.1f, 0.05f);
    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "spotLights[0].diffuse"),
        1.0f, 0.85f, 0.6f);
    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "spotLights[0].specular"),
        1.0f, 0.9f, 0.7f);
    glUniform1f(
        glGetUniformLocation(lightingShader.Program, "spotLights[0].constant"),
        1.0f);
    glUniform1f(
        glGetUniformLocation(lightingShader.Program, "spotLights[0].linear"),
        0.09f);
    glUniform1f(
        glGetUniformLocation(lightingShader.Program, "spotLights[0].quadratic"),
        0.032f);
    glUniform1f(
        glGetUniformLocation(lightingShader.Program, "spotLights[0].cutOff"),
        glm::cos(glm::radians(12.0f)));
    glUniform1f(glGetUniformLocation(lightingShader.Program,
                                     "spotLights[0].outerCutOff"),
                glm::cos(glm::radians(30.0f)));
    // SpotLight
    // Pantalla

    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "spotLights[1].position"),
        0.0f, 0.0f, 2.4f);
    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "spotLights[1].direction"),
        0.0f, 0.0f, -1.0f);
    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "spotLights[1].ambient"),
        0.02f, 0.05f, 0.08f); // tinte azul frío muy tenue
    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "spotLights[1].diffuse"),
        0.5f, 0.75f, 1.0f); // azul cielo dominante
    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "spotLights[1].specular"),
        0.4f, 0.6f, 0.8f);
    glUniform1f(
        glGetUniformLocation(lightingShader.Program, "spotLights[1].constant"),
        1.0f);
    glUniform1f(
        glGetUniformLocation(lightingShader.Program, "spotLights[1].linear"),
        0.14f); // se desvanece en rango medio
    glUniform1f(
        glGetUniformLocation(lightingShader.Program, "spotLights[1].quadratic"),
        0.07f);
    glUniform1f(
        glGetUniformLocation(lightingShader.Program, "spotLights[1].cutOff"),
        glm::cos(glm::radians(170.0f)));
    glUniform1f(glGetUniformLocation(lightingShader.Program,
                                     "spotLights[1].outerCutOff"),
                glm::cos(glm::radians(180.0f)));

    // Set material properties
    Material mat[NUM_MODELOS];
    mat[COUCH] = {1.0f, 0.02f};
    mat[CUSHIONS] = {1.0f, 0.02f};
    mat[PLANTS] = {25.0f, 1.0f};
    mat[STOOL] = {32.0f, 0.70f};
    mat[LAMP] = {128.0f, 0.80f};
    mat[CARPET] = {2.0f, 0.03f};
    mat[CHAIR] = {64.0f, 0.8f};
    mat[BED] = {4.0f, 0.05f};
    mat[DOG] = {8.0f, 0.10f};
    mat[PANTALLA_BASE] = {64.0f, 0.60f};
    mat[LAMPARA_PISO] = {96.0f, 0.80f};

    // Create camera transformations
    glm::mat4 view;
    view = camera.GetViewMatrix();

    // Get the uniform locations
    GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
    GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
    GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

    // Pass the matrices to the shader
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glm::mat4 model(1);

    // Carga de modelo
    view = camera.GetViewMatrix();
    model = glm::mat4(1);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

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

    SET_MAT(PANTALLA_BASE);
    model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.5f));
    model =
        glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1,
                       GL_FALSE, glm::value_ptr(model));
    pantallaBase.Draw(lightingShader);

    SET_MAT(LAMPARA_PISO);
    model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(0.8f, 0.0f, 0.0f));
    model =
        glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1,
                       GL_FALSE, glm::value_ptr(model));
    lampPiso.Draw(lightingShader);

    glBindVertexArray(0);

    // Also draw the lamp object, again binding the appropriate shader
    lampShader.Use();
    // Get location objects for the matrices on the lamp shader (these could be
    // different on a different shader)
    modelLoc = glGetUniformLocation(lampShader.Program, "model");
    viewLoc = glGetUniformLocation(lampShader.Program, "view");
    projLoc = glGetUniformLocation(lampShader.Program, "projection");

    // Set matrices
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glUniform1i(glGetUniformLocation(lampShader.Program, "isTex"), 0);

    model = glm::mat4(1);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    model = glm::mat4(1);
    model = glm::translate(model, pointLightPositions[0]);
    model = glm::scale(model, glm::vec3(0.03f)); // Make it a smaller cube
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.5f));
    model =
        glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1,
                       GL_FALSE, glm::value_ptr(model));
    glUniform1i(glGetUniformLocation(lampShader.Program, "isTex"), 1);
    pantallaScreen.Draw(lampShader);

    model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(0.8f, 0.0f, 0.0f));
    model =
        glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1,
                       GL_FALSE, glm::value_ptr(model));
    glUniform1i(glGetUniformLocation(lampShader.Program, "isTex"), 0);
    foco.Draw(lampShader);

    glBindVertexArray(0);

    // Swap the screen buffers
    glfwSwapBuffers(window);
  }

  // Terminate GLFW, clearing any resources allocated by GLFW.
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

  if (keys[GLFW_KEY_T]) {
    pointLightPositions[0].x += 0.01f;
  }
  if (keys[GLFW_KEY_G]) {
    pointLightPositions[0].x -= 0.01f;
  }

  if (keys[GLFW_KEY_Y]) {
    pointLightPositions[0].y += 0.01f;
  }

  if (keys[GLFW_KEY_H]) {
    pointLightPositions[0].y -= 0.01f;
  }
  if (keys[GLFW_KEY_U]) {
    pointLightPositions[0].z -= 0.1f;
  }
  if (keys[GLFW_KEY_J]) {
    pointLightPositions[0].z += 0.01f;
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

  if (keys[GLFW_KEY_SPACE]) {
    active = !active;
    if (active) {
      Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
    } else {
      Light1 = glm::vec3(0); // Cuado es solo un valor en los 3 vectores pueden
                             // dejar solo una componente
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
