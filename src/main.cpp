#include <iostream>

#include <glad/glad.h>   // всё остальное
#include <GLFW/glfw3.h>  // окна/вводы.эвенты
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>  // для текстур 

#include <string>   // for reading shaders
#include <fstream>  // for reading shaders
#include <sstream>  // for reading shaders

#include "graphics/rendering/Shader.h"
#include "graphics/rendering/Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/SpriteRenderer.h"
#include "graphics/ResourceManager.h"

#include "graphics/models/BrickObject.h"
#include <vector>
#include <random>

#define SCREENWIDTH 800
#define SCREENHEIGHT 608

#define SQUERE_X  384
#define SQUERE_Y  256

const int  BRICK_SIZE = 32;


std::vector<BrickObject> bricks;



void initBricks(Texture* texture);


enum Direction
{
	NORTH = 0,
	WESST = 1,
	SOUTH = 2,
	IEAST = 3
};


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
	GLFWwindow* window = glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "FUCK", NULL, NULL);
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
	


	ResourceManager::LoadShader("Assets/shaders/vertexShader.glsl", "Assets/shaders/fragmentShader.glsl", nullptr, "sprite");
	ResourceManager::LoadTexture("Assets/textures/Plates_Clear.png", true, "plates");
	ResourceManager::GetShader("sprite").use().setInt("texture0", 0);
	SpriteRenderer renderer(ResourceManager::GetShader("sprite"), ResourceManager::GetTexture("plates").Width, ResourceManager::GetTexture("plates").Height,128);  // инициализирутся после текстуры и шейдера 

	initBricks(&ResourceManager::GetTexture("plates"));
		
	float timeValue = glfwGetTime(); // Получаем начальное время

	const float animClamp = 0.2f;
	float timer = 0.0f;

	glm::mat4 ortoMatrix = glm::ortho(0.0f, static_cast<float>(SCREENWIDTH), static_cast<float>(SCREENHEIGHT),0.0f, -1.0f, 1.0f);

	ResourceManager::GetShader("sprite").setMat4("projection", ortoMatrix);

	

	while (!glfwWindowShouldClose(window)){
		float LastTime = timeValue;  // Сохраняем предыдущее время
		 timeValue = glfwGetTime(); // Получаем текущее время
		float felapsedTime = timeValue - LastTime;
		timer += felapsedTime;


		//
		/*if (timer > animClamp) {

			timer -= timer;
			if (x < 512.0f - 128.0f) {
				x += 128.0f;

			}
			else
			{
				x = 0;
			}

		}*/
		//


		glfwSwapBuffers(window);
		glfwPollEvents();

		glClearColor(0.2, 0.3, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ResourceManager::GetShader("sprite").use();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		std::cout << x <<"\t" << y << std::endl;
		ResourceManager::GetShader("sprite").setFloat("x_offset", x/512);
		ResourceManager::GetShader("sprite").setFloat("y_offset", y/512);
		ResourceManager::GetShader("sprite").setFloat("timeValue", timeValue);
		

		
		for (BrickObject& brick : bricks) {
			ResourceManager::GetShader("sprite").setInt("tileIndex", brick.TextureIdx);
			brick.Draw(renderer);
		};
	

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

void initBricks(Texture* texture) {


	int line = SQUERE_Y / BRICK_SIZE;
	int row = SQUERE_X / BRICK_SIZE;

	unsigned int offsetX = SCREENWIDTH / 2 - SQUERE_X / 2;
	unsigned int offsetY = SCREENHEIGHT / 2 - SQUERE_Y / 2;

	//unsigned int SQUERE_CenterX = SQUERE_X / 2;
	//unsigned int SQUERE_CenterY = SQUERE_Y / 2;


	for (unsigned int i = 0; i < row; i++)
	{
		for (unsigned int k = 0; k < line; k++)
		{
			int idx = rand() % 15;
			float rotate = rand() % 4;
			bricks.push_back(BrickObject(glm::vec2(i * BRICK_SIZE + offsetX, k * BRICK_SIZE + offsetY), glm::vec2(BRICK_SIZE, BRICK_SIZE), rotate, texture, idx));
		}
	}



}