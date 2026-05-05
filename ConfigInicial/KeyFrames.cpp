// Caballero Antunez Jesus Yael - 320231364
// Adicional: Skybox en OpenGL
// 5 de mayo del 2026

#include <algorithm>
#include <cctype>
#include <cmath>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/quaternion.hpp>
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
#include <iterator>
#include <nlohmann/detail/macro_scope.hpp>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include <fstream>
#include <string>
#include <vector>

// Load Models
#include "SOIL2/SOIL2.h"

// Other includes
#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "Texture.h"

using json = nlohmann::json;

#if defined(_WIN32)
#define dir "Animations\\"
#else
#define dir "Animations/"
#endif // defined (_WIN32)

#define rot(v)                                                                 \
  model = glm::rotate(model, v[0], glm::vec3(1.0f, 0.0f, 0.0f));               \
  model = glm::rotate(model, v[1], glm::vec3(0.0f, 1.0f, 0.0f));               \
  model = glm::rotate(model, v[2], glm::vec3(0.0f, 0.0f, 1.0f));

#define interpol(v, inc)                                                       \
  for (int i = 0; i < v.size(); i++) {                                         \
    KeyFrame[playIndex].inc[i] =                                               \
        (KeyFrame[playIndex + 1].v[i] - KeyFrame[playIndex].v[i]) /            \
        i_max_steps;                                                           \
  }

#define draw(v, inc)                                                           \
  for (int i = 0; i < v.size(); i++) {                                         \
    v[i] += KeyFrame[playIndex].inc[i];                                        \
  }

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void Animation();
void resetAnimation(bool hard = false);

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
bool saving;
fstream animation;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)};

float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  -0.5f, -0.5f,
    0.0f,  0.0f,  -1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
    0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, 0.5f,  -0.5f,
    0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,

    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,
    0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, 0.5f,  0.5f,
    0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f,
    -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,
    -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
    1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, 0.5f,
    1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, -0.5f,
    0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, 0.5f,
    0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
    0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,
    0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f};

glm::vec3 Light1 = glm::vec3(0);
// Anim
float rotBall = 0.0f;
int dogAnim = 0;

vector<float> rotDog(3, 0);
vector<float> posDog(3, 0);
vector<float> head(3, 0);
vector<float> tail(3, 0);
vector<float> fl_leg(3, 0);
vector<float> fr_leg(3, 0);
vector<float> bl_leg(3, 0);
vector<float> br_leg(3, 0);

// KeyFrames
#define MAX_FRAMES 100
int i_max_steps = 190;
int i_curr_steps = 0;
typedef struct _frame {
  // Valores
  vector<float> rotDog;
  vector<float> posDog;
  vector<float> head;
  vector<float> tail;
  vector<float> fl_leg;
  vector<float> fr_leg;
  vector<float> bl_leg;
  vector<float> br_leg;

  // Incrementos
  vector<float> rotDogInc;
  vector<float> posDogInc;
  vector<float> headInc;
  vector<float> tailInc;
  vector<float> fl_legInc;
  vector<float> fr_legInc;
  vector<float> bl_legInc;
  vector<float> br_legInc;
} FRAME;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(_frame, rotDog, posDog, head, tail, fl_leg,
                                   fr_leg, bl_leg, br_leg, rotDogInc, posDogInc,
                                   headInc, tailInc, fl_legInc, fr_legInc,
                                   bl_legInc, br_legInc)

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0; // introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void) {
  printf("frameindex %d\n", FrameIndex);
  KeyFrame[FrameIndex].rotDog = rotDog;
  KeyFrame[FrameIndex].posDog = posDog;
  KeyFrame[FrameIndex].head = head;
  KeyFrame[FrameIndex].tail = tail;
  KeyFrame[FrameIndex].fl_leg = fl_leg;
  KeyFrame[FrameIndex].fr_leg = fr_leg;
  KeyFrame[FrameIndex].bl_leg = bl_leg;
  KeyFrame[FrameIndex].br_leg = br_leg;

  FrameIndex++;
}

void resetElements(void) {
  posDog = KeyFrame[0].posDog;
  rotDog = KeyFrame[0].rotDog;
  head = KeyFrame[0].head;
  tail = KeyFrame[0].tail;
  fl_leg = KeyFrame[0].fl_leg;
  fr_leg = KeyFrame[0].fr_leg;
  bl_leg = KeyFrame[0].bl_leg;
  br_leg = KeyFrame[0].br_leg;
}

void interpolation(void) {
  interpol(rotDog, rotDogInc);
  interpol(posDog, posDogInc);
  interpol(head, headInc);
  interpol(tail, tailInc);
  interpol(fl_leg, fl_legInc);
  interpol(fr_leg, fr_legInc);
  interpol(bl_leg, bl_legInc);
  interpol(br_leg, br_legInc);
}

void resetAnimation(bool hard) {
  std::fill(std::begin(KeyFrame), std::end(KeyFrame), FRAME());
  FrameIndex = 0;

  if (hard) {
    rotDog = vector<float>(3, 0);
    posDog = vector<float>(3, 0);
    head = vector<float>(3, 0);
    tail = vector<float>(3, 0);
    fl_leg = vector<float>(3, 0);
    fr_leg = vector<float>(3, 0);
    bl_leg = vector<float>(3, 0);
    br_leg = vector<float>(3, 0);
  }
}

// Deltatime
GLfloat deltaTime = 0.0f; // Time between current frame and last frame
GLfloat lastFrame = 0.0f; // Time of last frame

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
      WIDTH, HEIGHT, "Skybox Jesús Caballero", nullptr, nullptr);

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
  Shader skyboxshader("Shader/SkyBox.vs", "Shader/SkyBox.frag");

  // models
  Model DogBody((char *)"Models/Perro/DogBody.obj");
  Model HeadDog((char *)"Models/Perro/HeadDog.obj");
  Model DogTail((char *)"Models/Perro/TailDog.obj");
  Model F_RightLeg((char *)"Models/Perro/F_RightLegDog.obj");
  Model F_LeftLeg((char *)"Models/Perro/F_LeftLegDog.obj");
  Model B_RightLeg((char *)"Models/Perro/B_RightLegDog.obj");
  Model B_LeftLeg((char *)"Models/Perro/B_LeftLegDog.obj");
  Model Piso((char *)"Models/Piso/piso.obj");
  Model Ball((char *)"Models/Pelota/ball.obj");

  // KeyFrames
  for (int i = 0; i < MAX_FRAMES; i++) {
    // Valores
    KeyFrame[i].rotDog = vector<float>(3, 0);
    KeyFrame[i].posDog = vector<float>(3, 0);
    KeyFrame[i].head = vector<float>(3, 0);
    KeyFrame[i].tail = vector<float>(3, 0);
    KeyFrame[i].fl_leg = vector<float>(3, 0);
    KeyFrame[i].fr_leg = vector<float>(3, 0);
    KeyFrame[i].bl_leg = vector<float>(3, 0);
    KeyFrame[i].br_leg = vector<float>(3, 0);

    // Incrementos
    KeyFrame[i].rotDogInc = vector<float>(3, 0);
    KeyFrame[i].posDogInc = vector<float>(3, 0);
    KeyFrame[i].headInc = vector<float>(3, 0);
    KeyFrame[i].tailInc = vector<float>(3, 0);
    KeyFrame[i].fl_legInc = vector<float>(3, 0);
    KeyFrame[i].fr_legInc = vector<float>(3, 0);
    KeyFrame[i].bl_legInc = vector<float>(3, 0);
    KeyFrame[i].br_legInc = vector<float>(3, 0);
  }

  GLfloat skyboxVertices[] = {
      // Positions
      -1.0f, 1.0f,  -1.0f, //
      -1.0f, -1.0f, -1.0f, //
      1.0f,  -1.0f, -1.0f, //
      1.0f,  -1.0f, -1.0f, //
      1.0f,  1.0f,  -1.0f, //
      -1.0f, 1.0f,  -1.0f, //

      -1.0f, -1.0f, 1.0f,  //
      -1.0f, -1.0f, -1.0f, //
      -1.0f, 1.0f,  -1.0f, //
      -1.0f, 1.0f,  -1.0f, //
      -1.0f, 1.0f,  1.0f,  //
      -1.0f, -1.0f, 1.0f,  //

      1.0f,  -1.0f, -1.0f, //
      1.0f,  -1.0f, 1.0f,  //
      1.0f,  1.0f,  1.0f,  //
      1.0f,  1.0f,  1.0f,  //
      1.0f,  1.0f,  -1.0f, //
      1.0f,  -1.0f, -1.0f, //

      -1.0f, -1.0f, 1.0f, //
      -1.0f, 1.0f,  1.0f, //
      1.0f,  1.0f,  1.0f, //
      1.0f,  1.0f,  1.0f, //
      1.0f,  -1.0f, 1.0f, //
      -1.0f, -1.0f, 1.0f, //

      -1.0f, 1.0f,  -1.0f, //
      1.0f,  1.0f,  -1.0f, //
      1.0f,  1.0f,  1.0f,  //
      1.0f,  1.0f,  1.0f,  //
      -1.0f, 1.0f,  1.0f,  //
      -1.0f, 1.0f,  -1.0f, //

      -1.0f, -1.0f, -1.0f, //
      -1.0f, -1.0f, 1.0f,  //
      1.0f,  -1.0f, -1.0f, //
      1.0f,  -1.0f, -1.0f, //
      -1.0f, -1.0f, 1.0f,  //
      1.0f,  -1.0f, 1.0f   //
  };

  GLuint indices[] = {
      // Note that we start from 0!
      0,  1,  2,  3,  //
      4,  5,  6,  7,  //
      8,  9,  10, 11, //
      12, 13, 14, 15, //
      16, 17, 18, 19, //
      20, 21, 22, 23, //
      24, 25, 26, 27, //
      28, 29, 30, 31, //
      32, 33, 34, 35  //
  };

  // First, set the container's VAO (and VBO)
  GLuint VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

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

  // Skybox
  GLuint skyboxVBO, skyboxVAO;
  glGenVertexArrays(1, &skyboxVAO);
  glGenBuffers(1, &skyboxVBO);
  glBindVertexArray(skyboxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        (GLvoid *)0);

  // Load textures
  vector<const GLchar *> faces;
  faces.push_back("SkyBox/right.jpg");
  faces.push_back("SkyBox/left.jpg");
  faces.push_back("SkyBox/top.jpg");
  faces.push_back("SkyBox/bottom.jpg");
  faces.push_back("SkyBox/back.jpg");
  faces.push_back("SkyBox/front.jpg");
  GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

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
    Animation();

    // Clear the colorbuffer
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    glm::mat4 modelTemp = glm::mat4(1.0f); // Temp

    // Use cooresponding shader when setting uniforms/drawing objects
    lightingShader.Use();

    glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
    // glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

    GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
    glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y,
                camera.GetPosition().z);

    // Directional light
    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "dirLight.direction"),
        -0.2f, -1.0f, -0.3f);
    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f,
        0.6f, 0.6f);
    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f,
        0.6f, 0.6f);
    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f,
        0.3f, 0.3f);

    // Point light 1
    glm::vec3 lightColor;
    lightColor.x = abs(sin(glfwGetTime() * Light1.x));
    lightColor.y = abs(sin(glfwGetTime() * Light1.y));
    lightColor.z = sin(glfwGetTime() * Light1.z);

    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "pointLights[0].position"),
        pointLightPositions[0].x, pointLightPositions[0].y,
        pointLightPositions[0].z);
    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"),
        lightColor.x, lightColor.y, lightColor.z);
    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"),
        lightColor.x, lightColor.y, lightColor.z);
    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"),
        1.0f, 0.2f, 0.2f);
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
    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "spotLight.position"),
        camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "spotLight.direction"),
        camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f,
        0.2f, 0.8f);
    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f,
        0.2f, 0.8f);
    glUniform3f(
        glGetUniformLocation(lightingShader.Program, "spotLight.specular"),
        0.0f, 0.0f, 0.0f);
    glUniform1f(
        glGetUniformLocation(lightingShader.Program, "spotLight.constant"),
        1.0f);
    glUniform1f(
        glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
    glUniform1f(
        glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"),
        0.7f);
    glUniform1f(
        glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"),
        glm::cos(glm::radians(12.0f)));
    glUniform1f(
        glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"),
        glm::cos(glm::radians(18.0f)));

    // Set material properties
    glUniform1f(
        glGetUniformLocation(lightingShader.Program, "material.shininess"),
        5.0f);

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
    Piso.Draw(lightingShader);

    model = glm::mat4(1);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"),
                0);
    // Body
    modelTemp = model =
        glm::translate(model, glm::vec3(posDog[0], posDog[1], posDog[2]));
    modelTemp = model =
        glm::rotate(model, rotDog[0], glm::vec3(1.0f, 0.0f, 0.0f));
    modelTemp = model =
        glm::rotate(model, rotDog[1], glm::vec3(0.0f, 1.0f, 0.0f));
    modelTemp = model =
        glm::rotate(model, rotDog[2], glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    DogBody.Draw(lightingShader);

    // Head
    model = modelTemp;
    model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
    rot(head);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    HeadDog.Draw(lightingShader);

    // Tail
    model = modelTemp;
    model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
    rot(tail);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    DogTail.Draw(lightingShader);

    // Front Left Leg
    model = modelTemp;
    model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
    rot(fl_leg);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    F_LeftLeg.Draw(lightingShader);

    // Front Right Leg
    model = modelTemp;
    model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
    rot(fr_leg);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    F_RightLeg.Draw(lightingShader);

    // Back Left Leg
    model = modelTemp;
    model = glm::translate(model, glm::vec3(0.082f, -0.046, -0.218));
    rot(bl_leg);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    B_LeftLeg.Draw(lightingShader);

    // Back Right Leg
    model = modelTemp;
    model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
    rot(br_leg);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    B_RightLeg.Draw(lightingShader);

    model = glm::mat4(1);
    glEnable(GL_BLEND); // Avtiva la funcionalidad para trabajar el canal
    // alfa
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"),
                1);
    model =
        glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    Ball.Draw(lightingShader);
    glDisable(GL_BLEND); // Desactiva el canal alfa
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
    model = glm::mat4(1);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    // Draw the light object (using light's vertex attributes)

    model = glm::mat4(1);
    model = glm::translate(model, pointLightPositions[0]);
    model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);

    // Draw SkyBox
    glDepthFunc(GL_LEQUAL); // Renderizado a distancia máxima
    skyboxshader.Use();

    view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "view"), 1,
                       GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "projection"),
                       1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthFunc(GL_LESS); // Se desactiva la función de profundidad

    // Swap the screen buffers
    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteVertexArrays(1, &skyboxVAO);
  glDeleteBuffers(1, &skyboxVBO);

  // Terminate GLFW, clearing any resources allocated by GLFW.
  glfwTerminate();

  return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement() {

  bool ctrl = keys[GLFW_KEY_LEFT_CONTROL] || keys[GLFW_KEY_RIGHT_CONTROL];
  float delta = ctrl ? -0.01f : 0.01f;

  if (keys[GLFW_KEY_1])
    rotDog[0] += delta;
  if (keys[GLFW_KEY_2])
    rotDog[1] += delta;
  if (keys[GLFW_KEY_3])
    rotDog[2] += delta;

  if (keys[GLFW_KEY_4])
    head[0] += delta;
  if (keys[GLFW_KEY_5])
    head[1] += delta;
  if (keys[GLFW_KEY_6])
    head[2] += delta;

  if (keys[GLFW_KEY_7])
    tail[0] += delta;
  if (keys[GLFW_KEY_8])
    tail[1] += delta;
  if (keys[GLFW_KEY_9])
    tail[2] += delta;

  if (keys[GLFW_KEY_E])
    fl_leg[0] += delta;
  if (keys[GLFW_KEY_R])
    fl_leg[1] += delta;
  if (keys[GLFW_KEY_T])
    fl_leg[2] += delta;

  if (keys[GLFW_KEY_U])
    fr_leg[0] += delta;
  if (keys[GLFW_KEY_I])
    fr_leg[1] += delta;
  if (keys[GLFW_KEY_O])
    fr_leg[2] += delta;

  if (keys[GLFW_KEY_Z])
    bl_leg[0] += delta;
  if (keys[GLFW_KEY_X])
    bl_leg[1] += delta;
  if (keys[GLFW_KEY_C])
    bl_leg[2] += delta;

  if (keys[GLFW_KEY_B])
    br_leg[0] += delta;
  if (keys[GLFW_KEY_N])
    br_leg[1] += delta;
  if (keys[GLFW_KEY_M])
    br_leg[2] += delta;

  if (keys[GLFW_KEY_H])
    posDog[2] += 0.01;
  if (keys[GLFW_KEY_Y])
    posDog[2] -= 0.01;
  if (keys[GLFW_KEY_V])
    posDog[1] += delta;
  if (keys[GLFW_KEY_G])
    posDog[0] -= 0.01;
  if (keys[GLFW_KEY_J])
    posDog[0] += 0.01;

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

  if (!animation.is_open() && keys[GLFW_KEY_Q]) {
    string nombre;
    std::cout << "Nombre de la animación a guardar: ";
    std::cin >> nombre;
    nombre.erase(std::remove_if(nombre.begin(), nombre.end(), ::isspace),
                 nombre.end());

    nombre = dir + nombre + ".json";

    animation.open(nombre, ios::app);
  } else if (animation.is_open() && keys[GLFW_KEY_Q]) {
    std::cout << "Animación guardada" << std::endl;
    animation.close();
  }

  if (keys[GLFW_KEY_P]) {
    string nombre;
    std::cout << "Nombre de la animación a leer: ";
    std::cin >> nombre;
    nombre.erase(std::remove_if(nombre.begin(), nombre.end(), ::isspace),
                 nombre.end());

    nombre = dir + nombre + ".json";

    animation.open(nombre, ios::in);

    if (!animation.is_open()) {
      std::cout << "No se abrió el archivo a leer" << std::endl;
      return;
    }

    resetAnimation();

    string line;
    string frame = "";

    while (std::getline(animation, line)) {
      frame += line;
      if (line == "}") {
        json j = json::parse(frame);
        KeyFrame[FrameIndex++] = j.get<FRAME>();
        frame = "";
      }
    }

    std::cout << FrameIndex << std::endl;
    animation.close();
  }

  if (keys[GLFW_KEY_F]) {
    resetAnimation(true);
  }

  if (keys[GLFW_KEY_L]) {
    if (play == false && (FrameIndex > 1)) {

      resetElements();
      // First Interpolation
      interpolation();

      play = true;
      playIndex = 0;
      i_curr_steps = 0;
    } else {
      play = false;
    }
  }

  if (keys[GLFW_KEY_K]) {
    if (FrameIndex < MAX_FRAMES) {
      saveFrame();
      if (animation.is_open()) {
        json j = KeyFrame[FrameIndex - 1];
        animation << j.dump(4) << std::endl;
      }
    }
  }

  if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
    if (animation.is_open())
      animation.close();
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
      Light1 = glm::vec3(0.2f, 0.8f, 1.0f);

    } else {
      Light1 = glm::vec3(0); // Cuado es solo un valor en los 3 vectores pueden
                             // dejar solo una componente
    }
  }
}
void Animation() {

  if (play) {
    if (i_curr_steps >= i_max_steps) // end of animation between frames?
    {
      playIndex++;
      if (playIndex > FrameIndex - 2) // end of total animation?
      {
        printf("termina anim\n");
        playIndex = 0;
        play = false;
      } else // Next frame interpolations
      {
        i_curr_steps = 0; // Reset counter
        // Interpolation
        interpolation();
      }
    } else {

      draw(posDog, posDogInc);
      draw(rotDog, rotDogInc);
      draw(head, headInc);
      draw(tail, tailInc);
      draw(fl_leg, fl_legInc);
      draw(fr_leg, fr_legInc);
      draw(bl_leg, bl_legInc);
      draw(br_leg, br_legInc);

      i_curr_steps++;
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
