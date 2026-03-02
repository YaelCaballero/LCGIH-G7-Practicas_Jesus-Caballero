// Caballero Antunez Jesus Yael - 320231364
// Practica #4: Modelado Geométrico
// 1 de marzo del 2026

#include <cmath>
#include <cstdio>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>

// #define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
#include "Shader.h"

void Inputs(GLFWwindow *window);

const GLint WIDTH = 800, HEIGHT = 600;
float movX = 0.0f;
float movY = 0.0f;
float movZ = -5.0f;
float rot = 0.0f;

int main() {
  glfwInit();
  // Verificacion de compatibilidad
  // Set all the required options for GLFW
  /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);*/

  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow *window = glfwCreateWindow(
      WIDTH, HEIGHT, "Práctica 4 Jesús Caballero", nullptr, nullptr);

  int screenWidth, screenHeight;

  glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

  // Verificacion de errores de creacion ventana
  if (nullptr == window) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();

    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);
  glewExperimental = GL_TRUE;

  // Verificacion de errores de inicializacion de glew
  GLenum err = glewInit();
  if (GLEW_OK != err && err != 4) {
    std::cout << "Failed to initialise GLEW" << std::endl;
    return EXIT_FAILURE;
  }

  // Define las dimensiones del viewport
  glViewport(0, 0, screenWidth, screenHeight);

  // Setup OpenGL options
  glEnable(GL_DEPTH_TEST);

  // enable alpha support
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Build and compile our shader program
  Shader ourShader("Shader/core.vs", "Shader/core.frag");

  // Set up vertex data (and buffer(s)) and attribute pointers

  // use with Perspective Projection
  float vertices[] = {
      -0.5f, -0.5f, 0.5f,  1.0f,  0.0f,  0.0f, // Front
      0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,
      0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  -0.5f, 0.5f,
      0.5f,  1.0f,  0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  0.0f, // Back
      0.5f,  -0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,
      1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,
      -0.5f, 0.0f,  1.0f,  0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  0.0f,

      0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 0.0f,
      0.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  1.0f,  0.5f,  0.5f,
      -0.5f, 0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
      0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,

      -0.5f, 0.5f,  0.5f,  1.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 1.0f,
      1.0f,  0.0f,  -0.5f, -0.5f, -0.5f, 1.0f,  1.0f,  0.0f,  -0.5f, -0.5f,
      -0.5f, 1.0f,  1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  1.0f,  1.0f,  0.0f,
      -0.5f, 0.5f,  0.5f,  1.0f,  1.0f,  0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 0.0f,
      1.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  1.0f,  1.0f,  0.5f,  -0.5f,
      0.5f,  0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  1.0f,  1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  1.0f,

      -0.5f, 0.5f,  -0.5f, 1.0f,  0.2f,  0.5f,  0.5f,  0.5f,  -0.5f, 1.0f,
      0.2f,  0.5f,  0.5f,  0.5f,  0.5f,  1.0f,  0.2f,  0.5f,  0.5f,  0.5f,
      0.5f,  1.0f,  0.2f,  0.5f,  -0.5f, 0.5f,  0.5f,  1.0f,  0.2f,  0.5f,
      -0.5f, 0.5f,  -0.5f, 1.0f,  0.2f,  0.5f,
  };

  GLuint VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  // Enlazar Vertex Array Object
  glBindVertexArray(VAO);

  // 2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que
  // OpenGL lo use
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // 4. Despues colocamos las caracteristicas de los vertices
  // Posicion
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                        (GLvoid *)0);
  glEnableVertexAttribArray(0);

  // Color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                        (GLvoid *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glm::mat4 projection = glm::mat4(1);
  projection = glm::perspective(glm::radians(45.0f),
                                (GLfloat)screenWidth / (GLfloat)screenHeight,
                                0.1f, 100.0f);

  while (!glfwWindowShouldClose(window)) {
    Inputs(window);
    glfwPollEvents();

    // Render
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ourShader.Use();
    glm::mat4 model = glm::mat4(1);
    glm::mat4 view = glm::mat4(1);

    view = glm::translate(view, glm::vec3(movX, movY, movZ));
    view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

    GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
    GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
    GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
    GLint colorLoc = glGetUniformLocation(ourShader.Program, "overrideColor");
    GLint useOverrideLoc =
        glGetUniformLocation(ourShader.Program, "useOverride");

    glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);

    // Suelo
    glUniform1i(useOverrideLoc, GL_TRUE);
    glUniform3f(colorLoc, 0.0f, 0.5f, 0.0f);
  
    model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
    model = glm::scale(model, glm::vec3(40.0f, 1.0f, 40.0f));


    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Base
    model = glm::mat4(1.0f);

    glm::vec3 base_dim = glm::vec3(10.0f, 1.0f, 10.0f);
    glm::vec3 base_pos = glm::vec3(0.0f, base_dim.y / 2, 0.0f);
    float reduccion = base_dim.x / 9;
    int pisos = 7;

    // Pisos
    for (int i = 0; i < pisos; i++) {
      float tonoPiso = 0.5f + (i * 0.05f);
      glUniform3f(colorLoc, tonoPiso, tonoPiso, tonoPiso);

      model = glm::mat4(1.0f);

      model = glm::translate(
          model,
          glm::vec3(base_pos.x, base_pos.y + i * base_dim.y, base_pos.z));
      model =
          glm::scale(model, glm::vec3(base_dim.x - reduccion * i, base_dim.y,
                                      base_dim.z - reduccion * i));

      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    float altura = base_dim.y * pisos;
    float angle;

    // Escaleras
    glUniform3f(colorLoc, 0.4f, 0.4f, 0.4f);
    glm::vec3 escalera_dim = glm::vec3();
    glm::vec3 escalera_pos = glm::vec3();

    angle = atan2(reduccion / 2.0f, base_dim.y);

    escalera_dim.x = base_dim.x - reduccion * (pisos + 1);
    escalera_dim.y = altura / cos(angle);
    escalera_dim.z = 0.5f;

    escalera_pos.x = 0;
    escalera_pos.y =
        (escalera_dim.y * cos(angle) / 2 -
         escalera_dim.z *
             (base_dim.z + reduccion - escalera_dim.y * cos(angle)) /
             (2 * escalera_dim.y));
    escalera_pos.z =
        ((base_dim.z + reduccion - escalera_dim.y * sin(angle)) / 2 -
         escalera_dim.z * escalera_pos.y / escalera_dim.y);

    for (int i = 0; i < 4; i++) {
      model = glm::mat4(1.0f);

      model = glm::rotate(model, glm::radians(90.0f * i),
                          glm::vec3(0.0f, 1.0f, 0.0f));
      model = glm::translate(model, escalera_pos);
      model = glm::rotate(model, angle, glm::vec3(-1.0f, 0.0f, 0.0f));
      model = glm::scale(model, escalera_dim);

      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // Columnas - Coords polares
    glUniform3f(colorLoc, 0.85f, 0.85f, 0.85f);
    glm::vec3 columna_dim1 = glm::vec3(0.75f, 1.25f, 0.2f);
    glm::vec3 columna_dim2 = glm::vec3(columna_dim1.z, columna_dim1.y,
                                       columna_dim1.x - columna_dim1.z);

    glm::vec3 columna_pos1 = glm::vec3();
    glm::vec3 columna_pos2 = glm::vec3();

    columna_pos1.y = altura + columna_dim1.y / 2;
    columna_pos2.y = columna_pos1.y;

    float mag1 = sqrt(pow(base_dim.x - reduccion * pisos - columna_dim1.x, 2) +
                      pow(base_dim.z - reduccion * pisos - columna_dim1.z, 2)) /
                 2;
    float angle1 = atan2(base_dim.z - reduccion * pisos - columna_dim1.z,
                         base_dim.x - reduccion * pisos - columna_dim1.x);

    float mag2 =
        sqrt(pow((base_dim.x - reduccion * pisos - columna_dim2.x) / 2, 2) +
             pow((base_dim.z - reduccion * pisos - columna_dim2.z) / 2 -
                     columna_dim1.z,
                 2));
    float angle2 = atan2((base_dim.z - reduccion * pisos - columna_dim2.z) / 2 -
                             columna_dim1.z,
                         (base_dim.x - reduccion * pisos - columna_dim2.x) / 2);

    for (int i = 0; i < 4; i++) {
      // Columnas
      model = glm::mat4(1.0f);

      columna_pos1.x = pow(-1, i / 2) * mag1 * cos(angle1);
      columna_pos1.z = pow(-1, i) * mag1 * sin(angle1);

      model = glm::translate(model, columna_pos1);
      model = glm::scale(model, columna_dim1);

      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
      glDrawArrays(GL_TRIANGLES, 0, 36);

      model = glm::mat4(1.0f);

      columna_pos2.x = pow(-1, i / 2) * mag2 * cos(angle2);
      columna_pos2.z = pow(-1, i) * mag2 * sin(angle2);

      model = glm::translate(model, columna_pos2);
      model = glm::scale(model, columna_dim2);

      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    altura += columna_dim1.y;

    // Techo
    glUniform3f(colorLoc, 0.55f, 0.55f, 0.55f);
    model = glm::mat4(1.0f);

    model =
        glm::translate(model, glm::vec3(0.0f, altura + base_dim.y / 6, 0.0f));
    model = glm::scale(model, glm::vec3(base_dim.x - reduccion * pisos + 0.3f,
                                        base_dim.y / 3,
                                        base_dim.x - reduccion * pisos + 0.3f));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    altura += base_dim.y / 3;

    glUniform3f(colorLoc, 0.85f, 0.85f, 0.85f);
    model = glm::mat4(1.0f);

    model =
        glm::translate(model, glm::vec3(0.0f, altura + base_dim.y / 4, 0.0f));
    model = glm::scale(model,
                       glm::vec3(base_dim.x - reduccion * pisos, base_dim.y / 2,
                                 base_dim.x - reduccion * pisos));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    altura += base_dim.y / 2;

    glUniform3f(colorLoc, 0.55f, 0.55f, 0.55f);
    model = glm::mat4(1.0f);

    model =
        glm::translate(model, glm::vec3(0.0f, altura + base_dim.y / 6, 0.0f));
    model = glm::scale(model, glm::vec3(base_dim.x - reduccion * pisos + 0.3f,
                                        base_dim.y / 3,
                                        base_dim.x - reduccion * pisos + 0.3f));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);

    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  glfwTerminate();
  return EXIT_SUCCESS;
}

void Inputs(GLFWwindow *window) {
  // if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    movX -= 0.08f;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    movX += 0.08f;
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    movY -= 0.08f;
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    movY += 0.08f;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    movZ += 0.08f;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    movZ -= 0.08f;
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    rot -= 0.8f;
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    rot += 0.8f;
}
