// Caballero Antunez Jesus Yael - 320231364
// Practica #3: Proyecciones, transformaciones y Shaders
// 22 de febrero del 2026

#include <cmath>
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

const GLint WIDTH = 800, HEIGHT = 600;

int main() {
  glfwInit();
  // Verificacion de compatibilidad
  //  Set all the required options for GLFW
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow *window = glfwCreateWindow(
      WIDTH, HEIGHT, "Práctica 3 Jesús Caballero", nullptr, nullptr);

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
    return -1;
  }

  glGetError();

  // if (GLEW_OK != glewInit()) {
  // 	std::cout << "Failed to initialise GLEW" << std::endl;
  // 	return EXIT_FAILURE;
  // }

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
  // use with Orthographic Projection

  // GLfloat vertices[] = {
  //      -0.5f*500, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,//Front
  //	0.5f * 500, -0.5f * 500, 0.5f * 500,  1.0f, 0.0f,0.0f,
  //	0.5f * 500,  0.5f * 500, 0.5f * 500,  1.0f, 0.0f,0.0f,
  //	0.5f * 500,  0.5f * 500, 0.5f * 500,  1.0f, 0.0f,0.0f,
  //	-0.5f * 500,  0.5f * 500, 0.5f * 500, 1.0f, 0.0f,0.0f,
  //	-0.5f * 500, -0.5f * 500, 0.5f * 500, 1.0f, 0.0f,0.0f,
  //
  //    -0.5f * 500, -0.5f * 500,-0.5f * 500, 0.0f, 1.0f,0.0f,//Back
  //	 0.5f * 500, -0.5f * 500,-0.5f * 500, 0.0f, 1.0f,0.0f,
  //	 0.5f * 500,  0.5f * 500,-0.5f * 500, 0.0f, 1.0f,0.0f,
  //	 0.5f * 500,  0.5f * 500,-0.5f * 500, 0.0f, 1.0f,0.0f,
  //    -0.5f * 500,  0.5f * 500,-0.5f * 500, 0.0f, 1.0f,0.0f,
  //    -0.5f * 500, -0.5f * 500,-0.5f * 500, 0.0f, 1.0f,0.0f,
  //
  //	 0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 0.0f,1.0f,
  //	 0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 0.0f,1.0f,
  //	 0.5f * 500,  0.5f * 500, -0.5f * 500,  0.0f, 0.0f,1.0f,
  //	 0.5f * 500,  0.5f * 500, -0.5f * 500,  0.0f, 0.0f,1.0f,
  //	 0.5f * 500,  0.5f * 500,  0.5f * 500,  0.0f, 0.0f,1.0f,
  //	 0.5f * 500,  -0.5f * 500, 0.5f * 500, 0.0f, 0.0f,1.0f,
  //
  //	-0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 1.0f,0.0f,
  //	-0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 1.0f,0.0f,
  //	-0.5f * 500, -0.5f * 500, -0.5f * 500,  1.0f, 1.0f,0.0f,
  //	-0.5f * 500, -0.5f * 500, -0.5f * 500,  1.0f, 1.0f,0.0f,
  //	-0.5f * 500, -0.5f * 500,  0.5f * 500,  1.0f, 1.0f,0.0f,
  //	-0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 1.0f,0.0f,
  //
  //	-0.5f * 500, -0.5f * 500, -0.5f * 500, 0.0f, 1.0f,1.0f,
  //	0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,1.0f,
  //	0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 1.0f,1.0f,
  //	0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 1.0f,1.0f,
  //	-0.5f * 500, -0.5f * 500,  0.5f * 500, 0.0f, 1.0f,1.0f,
  //	-0.5f * 500, -0.5f * 500, -0.5f * 500, 0.0f, 1.0f,1.0f,
  //
  //	-0.5f * 500,  0.5f * 500, -0.5f * 500, 1.0f, 0.2f,0.5f,
  //	0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 0.2f,0.5f,
  //	0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.2f,0.5f,
  //	0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.2f,0.5f,
  //	-0.5f * 500,  0.5f * 500,  0.5f * 500, 1.0f, 0.2f,0.5f,
  //	-0.5f * 500,  0.5f * 500, -0.5f * 500, 1.0f, 0.2f,0.5f,
  //};

  // use with Perspective Projection
  float vertices[] = {
      -0.5f, -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, 0.5f,
      1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,
      1.0f,  0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  -0.5f, -0.5f,
      0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f,
      0.0f,  1.0f,  0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  0.0f,

      0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, -0.5f,
      0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  1.0f,
      0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,
      0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,

      -0.5f, 0.5f,  0.5f,  1.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f,
      1.0f,  1.0f,  0.0f,  -0.5f, -0.5f, -0.5f, 1.0f,  1.0f,  0.0f,
      -0.5f, -0.5f, -0.5f, 1.0f,  1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,
      1.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  1.0f,  1.0f,  0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, -0.5f,
      0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  1.0f,  1.0f,
      0.5f,  -0.5f, 0.5f,  0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, 0.5f,
      0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  1.0f,

      -0.5f, 0.5f,  -0.5f, 1.0f,  0.2f,  0.5f,  0.5f,  0.5f,  -0.5f,
      1.0f,  0.2f,  0.5f,  0.5f,  0.5f,  0.5f,  1.0f,  0.2f,  0.5f,
      0.5f,  0.5f,  0.5f,  1.0f,  0.2f,  0.5f,  -0.5f, 0.5f,  0.5f,
      1.0f,  0.2f,  0.5f,  -0.5f, 0.5f,  -0.5f, 1.0f,  0.2f,  0.5f,
  };

  GLuint VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // glGenBuffers(1, &EBO);

  // Enlazar  Vertex Array Object
  glBindVertexArray(VAO);

  // 2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que
  // OpenGL lo use
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que
  // OpenGL lo use
  /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
  GL_STATIC_DRAW);*/

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

  glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any
                        // buffer/array to prevent strange bugs)

  glm::mat4 projection = glm::mat4(1);

  projection =
      glm::perspective(45.0f, (GLfloat)screenWidth / (GLfloat)screenHeight,
                       0.1f, 100.0f); // FOV, Radio de aspecto,znear,zfar
  // projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f,
  // (GLfloat)screenHeight, 0.1f, 1000.0f);//Izq,Der,Fondo,Alto,Cercania,Lejania

  while (!glfwWindowShouldClose(window)) {
    // Check if any events have been activiated (key pressed, mouse moved etc.)
    // and call corresponding response functions
    glfwPollEvents();

    // Render
    // Clear the colorbuffer
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw our first triangle
    ourShader.Use();
    glBindVertexArray(VAO);

    GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
    GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
    GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");

    // Generarcion de la vista
    glm::mat4 view = glm::mat4(1);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -7.5f));

    // view = glm::rotate(view, glm::radians(90.0f),
    //                    glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
    // view = glm::rotate(view, glm::radians(60.0f),
    //                    glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));

    // Primer cubo
    glm::mat4 model = glm::mat4(1);

    glm::vec3 c1_pos, c1_dim;

    c1_dim.x = 1.5f;
    c1_dim.y = 0.75f;
    c1_dim.z = 3.0f;

    c1_pos.x = 0.0f;
    c1_pos.y = -2.5f;
    c1_pos.z = 0.0f;

    model = glm::translate(model, c1_pos);

    model = glm::rotate(model, glm::radians(20.0f),
                        glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f)));

    model = glm::scale(model, c1_dim);

    glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Segundo cubo
    model = glm::mat4(1);

    glm::vec3 c2_pos, c2_dim;

    c2_dim.x = 1.5f;
    c2_dim.y = 0.5f;
    c2_dim.z = 2.5f;

    c2_pos.x = 0.7f;
    c2_pos.y = (c2_dim.y + c1_dim.y) / 2 + c1_pos.y;
    c2_pos.z = -0.5f;

    model = glm::translate(model, c2_pos);

    model = glm::rotate(model, glm::radians(180.0f),
                        glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f)));
    model = glm::rotate(model, glm::radians(70.0f),
                        glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));

    model = glm::scale(model, c2_dim);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Tercer cubo
    model = glm::mat4(1);

    glm::vec3 c3_pos, c3_dim;

    c3_dim.x = 3.0f;
    c3_dim.y = 0.3f;
    c3_dim.z = 0.5f;

    c3_pos.x = 1.5f;
    c3_pos.y = c3_dim.x / 2 + c2_dim.y / 2 + c2_pos.y;
    c3_pos.z = -0.35f;

    model = glm::translate(model, c3_pos);

    model = glm::rotate(model, glm::radians(170.0f),
                        glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f)));
    model = glm::rotate(model, glm::radians(90.0f),
                        glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f)));

    model = glm::scale(model, c3_dim);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Cuarto cubo
    model = glm::mat4(1);

    glm::vec3 c4_pos, c4_dim;

    c4_dim.x = 0.6f;
    c4_dim.y = 3.5f;
    c4_dim.z = 0.4f;

    float mag =
        sqrt(pow(c1_dim.x - c4_dim.z, 2) + pow(c1_dim.z - c4_dim.x, 2)) / 2;
    float angle =
        atan2(c1_dim.x - c4_dim.z, c1_dim.z - c4_dim.x) + glm::radians(20.0f);

    c4_pos.x = -mag * sin(angle);
    c4_pos.y = (c4_dim.y + c1_dim.y) / 2 + c1_pos.y;
    c4_pos.z = mag * cos(angle);

    model = glm::translate(model, c4_pos);

    model = glm::rotate(model, glm::radians(200.0f),
                        glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f)));
    model = glm::rotate(model, glm::radians(180.0f),
                        glm::normalize(glm::vec3(-1.0f, 0.0f, -1.0f)));

    model = glm::scale(model, c4_dim);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Quinto cubo
    model = glm::mat4(1);

    glm::vec3 c5_pos, c5_dim;

    c5_dim.x = 2.5f;
    c5_dim.y =
        sqrt(pow(c4_pos.x - c3_pos.x, 2) + pow(c4_pos.z - c3_pos.z, 2)) + 1;
    c5_dim.z = 0.1f;

    c5_pos.x = (c4_pos.x + c3_pos.x) / 2;
    c5_pos.y = (c5_dim.z + c3_dim.x) / 2 + c3_pos.y;
    c5_pos.z = (c4_pos.z + c3_pos.z) / 2;

    angle =
        atan2(c4_pos.x - c3_pos.x, c4_pos.z - c3_pos.z) + glm::radians(180.0f);

    model = glm::translate(model, c5_pos);

    model =
        glm::rotate(model, angle, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
    model = glm::rotate(model, glm::radians(90.0f),
                        glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));

    model = glm::scale(model, c5_dim);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Sexto cubo
    model = glm::mat4(1);

    glm::vec3 c6_pos, c6_dim;

    c6_dim.x = 1.0f;
    c6_dim.y = 1.0f;
    c6_dim.z = 1.0f;

    float diag = sqrt(pow(c6_dim.x, 2) + pow(c6_dim.y, 2) + pow(c6_dim.z, 2));

    c6_pos.x = -0.6f;
    c6_pos.y = (diag + c5_dim.z) / 2 + c5_pos.y;
    c6_pos.z = 0.8f;

    model = glm::translate(model, c6_pos);

    model = glm::rotate(model, glm::radians(45.0f),
                        glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
    model = glm::rotate(model, glm::radians(45.0f),
                        glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
    model = glm::rotate(model, glm::radians(90.0f),
                        glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));

    model = glm::scale(model, c6_dim);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);

    // Swap the screen buffers
    glfwSwapBuffers(window);
  }
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  glfwTerminate();
  return EXIT_SUCCESS;
}
