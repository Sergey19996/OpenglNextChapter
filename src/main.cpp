#include <iostream>

#include <glad/glad.h>   // всё остальное
#include <GLFW/glfw3.h>  // окна/вводы.эвенты
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>  // для текстур 

#include <string>   // for reading shaders
#include <fstream>  // for reading shaders
#include <sstream>  // for reading shaders

#include "Shader.h"


#define WIDTH 800
#define HEIGHT 600



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
float x = 128, y = 128;
float wh = 128;

float u1 = 0;
float v1 = 0;
float u2 = 0;
float v2 = 0;

void UpdateUVData(unsigned int VAO, unsigned int VBO, float* vertices);

int main(int m) {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "FUCK", NULL, NULL);
	if (window == NULL) {

		std::cout << "FAILED to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, (GLFWkeyfun)processInput);


	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {

		std::cout << "FAILED to initialize GLAD" << std::endl;
		return -1;

	}
	


	//  SHADERS START
	Shader shader("Assets/shaders/vertexShader.glsl", "Assets/shaders/fragmentShader.glsl");


	//Texture one Start
	
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load("Assets/textures/dungeonMap.jpg", &width, &height, &nrChannels, 0);
	if (data) {

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);  //создаёт текстуру по ширине и высоте
	glGenerateMipmap(GL_TEXTURE_2D); // когда текстура очень далеко/маленькая, для семплирования(оптимизация)

	}
	else
	{
		std::cout << "ERROR::TEXTURE:failed to load" << std::endl;
	}

	stbi_image_free(data);


	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("Assets/textures/Plates.png", &width, &height, &nrChannels, 0);
	if (data) {

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);  //создаёт текстуру по ширине и высоте
		glGenerateMipmap(GL_TEXTURE_2D); // когда текстура очень далеко/маленькая, для семплирования(оптимизация)


	



	}
	else
	{
		std::cout << "ERROR::TEXTURE:failed to load" << std::endl;
	}

	stbi_image_free(data);

	 x = 0, y = 0;
	 wh = 128;

	 u1 = x / width;
	 v1 = y / height;
	 u2 = (x + wh) / width;
	 v2 = (y + wh) / height;


	////  VAO/VBO
	float vertices[] = {
		// first triangle      COLORS          UV
   0.5f,  0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	u2,v1,		 // top right    0
   0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	u2,v2,			// bottom right 1
  -0.5f,  0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	u1,v1,			// top left     2
  // second triangle
  -0.5f, -0.5f, 0.0f,	1.0f, 1.0f, 1.0f,	u1,v2,			// bottom left  3
	};
	unsigned int indices[] = {
		0,3,2,
		1,3,0
	};



	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	
	
	// // 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);  
	// 3. copy our index array in a element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // layout = 0  // говорим как читать данные с VBO
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // layout = 0  // говорим как читать данные с VBO
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // layout = 0  // говорим как читать данные с VBO
	glEnableVertexAttribArray(2);
	// Разбиндивание VAO (необязательно, но хорошая практика)
	glBindVertexArray(0);
	//
	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);
		
	float timeValue = glfwGetTime(); // Получаем начальное время

	while (!glfwWindowShouldClose(window)){
		float LastTime = timeValue;  // Сохраняем предыдущее время
		 timeValue = glfwGetTime(); // Получаем текущее время
		float felapsedTime = timeValue - LastTime;

		glfwSwapBuffers(window);
		glfwPollEvents();

		glClearColor(0.2, 0.3, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		std::cout << x <<"\t" << y << std::endl;
		shader.setFloat("x_offset", x/512);
		shader.setFloat("y_offset", y/512);
		shader.setFloat("timeValue", timeValue);
		//int timeValueLocation = glGetUniformLocation(shaderProgram, "timeValue");
		//glUniform1f(timeValueLocation, timeValue);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glBindVertexArray(VAO);
		
	

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);  // 6 indices
		glBindVertexArray(0);

		

	}
	glfwTerminate();
	return 0;


}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

	glViewport(0, 0, width, height);


}
void processInput(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		if (y <= 0.0f) {
		y = 512.0f-128.0f;

		}
		else
		{
			y -= 128.0f;
		}

	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		if (y >= 512.0f-128.0f) {
			y = 0.0f;

		}
		else
		{
			y += 128.0f;
		}

	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		if (x < 512.0f-128.0f) {
			x += 128.0f;

		}
		else
		{
			x = 0;
		}

	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		if (x > 0.0f) {
			x -= 128.0f;

		}
		else
		{
			x = 512.0f-128.0f;
		}

	}
}
