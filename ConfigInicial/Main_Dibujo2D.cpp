// Caballero Antunez Jesús Yael - 32023164
// Práctica #2: Dibujo de primitivas en 2D
// 15 de febrero del 2026


#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;


int main() {
	glfwInit();
	//Verificaci�n de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Práctica 2 Jesús Caballero", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);
	
	//Verificaci�n de errores de creacion  ventana
	if (window== NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaci�n de errores de inicializaci�n de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos informacin de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	// Define las dimensiones del viewport
	//glViewport(0, 0, screenWidth, screenHeight);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	//float vertices[] = {
	// 	0.5f,  0.5f, 0.0f,    1.0f,1.0f,0.0f,  // top right
	// 	0.5f, -0.5f, 0.0f,    1.0f,1.0f,0.0f,  // bottom right
	// 	-0.5f, -0.5f, 0.0f,   1.0f,0.0f,1.0f,  // bottom left
	//	-0.5f,  0.5f, 0.0f,   1.0f,1.0f,0.0f, // top left 
	//};
	//unsigned int indices[] = {  // note that we start from 0!
	//	3,2,1,// second Triangle
	//	0,1,3,
		
	//};
	
	enum Vertice {
		C, D, E, F, G, H, I, J, K, L, M, N, O, P,
		Q, R, S, T, U, V, W, Z,
		A1, B1, C1, D1, E1, F1, G1, H1,
		I1, J1, K1, L1, M1, N1, O1, P1
	};


	float vertices[] = {

		0.08f,  0.04f, 0.0f, 0.0f,0.0f,0.0f,   // C 0
		0.04f, -0.01f, 0.0f, 0.0f,0.0f,0.0f,   // D 1
	   -0.01f, -0.02f, 0.0f, 0.0f,0.0f,0.0f,   // E 2
		0.03f, -0.05f, 0.0f, 0.0f,0.0f,0.0f,   // F 3
		0.14f, -0.04f, 0.0f, 0.0f,0.0f,0.0f,   // G 4
		0.24f,  0.05f, 0.0f, 0.0f,0.0f,0.0f,   // H 5
		0.37f, -0.01f, 0.0f, 0.0f,0.0f,0.0f,   // I 6
		0.48f, -0.06f, 0.0f, 0.0f,0.0f,0.0f,   // J 7
		0.50f, -0.01f, 0.0f, 0.0f,0.0f,0.0f,   // K 8
		0.45f,  0.07f, 0.0f, 0.0f,0.0f,0.0f,   // L 9
		0.46f,  0.14f, 0.0f, 0.0f,0.0f,0.0f,   // M 10
		0.34f,  0.26f, 0.0f, 0.0f,0.0f,0.0f,   // N 11
		0.20f,  0.15f, 0.0f, 0.0f,0.0f,0.0f,   // O 12
		0.10f,  0.30f, 0.0f, 0.0f,0.0f,0.0f,   // P 13
	   -0.07f,  0.36f, 0.0f, 0.0f,0.0f,0.0f,   // Q 14
	   -0.20f,  0.30f, 0.0f, 0.0f,0.0f,0.0f,   // R 15
	   -0.11f,  0.29f, 0.0f, 0.0f,0.0f,0.0f,   // S 16
	   -0.08f,  0.25f, 0.0f, 0.0f,0.0f,0.0f,   // T 17
	   -0.23f,  0.10f, 0.0f, 0.0f,0.0f,0.0f,   // U 18
	   -0.36f, -0.19f, 0.0f, 0.0f,0.0f,0.0f,   // V 19
	   -0.41f, -0.22f, 0.0f, 0.0f,0.0f,0.0f,   // W 20
	   -0.52f, -0.28f, 0.0f, 0.0f,0.0f,0.0f,   // Z 21
	   -0.53f, -0.38f, 0.0f, 0.0f,0.0f,0.0f,   // A₁ 22
	   -0.49f, -0.34f, 0.0f, 0.0f,0.0f,0.0f,   // B₁ 23
	   -0.38f, -0.33f, 0.0f, 0.0f,0.0f,0.0f,   // C₁ 24
	   -0.36f, -0.27f, 0.0f, 0.0f,0.0f,0.0f,   // D₁ 25
	   -0.35f, -0.34f, 0.0f, 0.0f,0.0f,0.0f,   // E₁ 26
	   -0.28f, -0.35f, 0.0f, 0.0f,0.0f,0.0f,   // F₁ 27
	   -0.26f, -0.40f, 0.0f, 0.0f,0.0f,0.0f,   // G₁ 28
	   -0.23f, -0.31f, 0.0f, 0.0f,0.0f,0.0f,   // H₁ 29
	   -0.30f, -0.30f, 0.0f, 0.0f,0.0f,0.0f,   // I₁ 30
	   -0.28f, -0.25f, 0.0f, 0.0f,0.0f,0.0f,   // J₁ 31
	   -0.30f, -0.22f, 0.0f, 0.0f,0.0f,0.0f,   // K₁ 32
	   -0.33f, -0.21f, 0.0f, 0.0f,0.0f,0.0f,   // L₁ 33
	   -0.14f, -0.03f, 0.0f, 0.0f,0.0f,0.0f,   // M₁ 34
	   -0.16f,  0.06f, 0.0f, 0.0f,0.0f,0.0f,   // N₁ 35
	   -0.02f,  0.13f, 0.0f, 0.0f,0.0f,0.0f,   // O₁ 36
		0.08f,  0.18f, 0.0f, 0.0f,0.0f,0.0f    // P₁ 37

	};

	unsigned int indices[] = { 
		// Contorno
		C, D,  D, E,  E, F,  F, G,  G, H,  H, I,  I, J,  J, K,  K, L,  L, M,  M, N,
		N, P,  P, Q,  Q, R,  R, S,  S, T,  T, U,  U, V,  V, W,  W, Z,
		Z, A1, A1, B1, B1, C1, C1, D1, D1, E1, E1, F1, F1, G1, G1, H1,
		H1, J1, J1, K1, K1, M1, M1, C,

		// Cola
		V,L1, L1,K1, D1,W, D1,L1, D1,J1, D1,Z, D1,I1, I1,H1,
		Z,B1, Z,C1, I1,E1, F1,H1,

		// Dorsal
		Q,S, Q,T, T,P,

		// Aleta
		D,F, D,G, C,G, C,H,

		// Cabeza
		H,N, H,M, H,L, L,I,

		// Torso
		L1,N1, U,N1, M1,N1, N1,T, N1,C,
		O1,N1, O1,T, O1,C, O1,P1, T,P1, C,P1, P1,P,
		O,P, O,N, O,C, O,H
	};

	unsigned int fillIndices[] = {
		O1, T, P1,
		O1, N1, C,
		A1, B1, Z,
		Z, B1, C1,
		F1, G1, H1,
		E1, H1, F1
	};



	GLuint VBO, VAO,EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	GLuint EBO_fill;
	glGenBuffers(1, &EBO_fill);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_fill);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(fillIndices),
		fillIndices,
		GL_STATIC_DRAW);


	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw our first triangle
        ourShader.Use();
        glBindVertexArray(VAO);

        //glPointSize(10);
        //glDrawArrays(GL_POINTS,0,4);
        
        //glDrawArrays(GL_LINES,0,4);
        //glDrawArrays(GL_LINE_LOOP,0,4);
        
		//glDrawArrays(GL_TRIANGLES,0,3);
        //glDrawElements(GL_TRIANGLES, 3,GL_UNSIGNED_INT,0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_fill);
		glDrawElements(GL_TRIANGLES,
			sizeof(fillIndices) / sizeof(unsigned int),
			GL_UNSIGNED_INT,
			0);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glLineWidth(2.5f);
		glDrawElements(GL_LINES,
			sizeof(indices) / sizeof(unsigned int),
			GL_UNSIGNED_INT,
			0);


        
        glBindVertexArray(0);
    
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}



	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
	//glViewport(0, 0, screenWidth, screenHeight);
}