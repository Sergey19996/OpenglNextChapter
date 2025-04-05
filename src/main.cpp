#include <iostream>

#include <glad/glad.h>   // всё остальное
#include <GLFW/glfw3.h>  // окна/вводы.эвенты
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>  // для текстур 

#include <string>   // for reading shaders
#include <fstream>  // for reading shaders
#include <sstream>  // for reading shaders
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"


#include <vector>
#include <random>

#define SCREENWIDTH 800
#define SCREENHEIGHT 608



Game BlocksDestroyer(SCREENWIDTH,SCREENHEIGHT);









void framebuffer_size_callback(GLFWwindow* window, int width, int height);







void keyChanged(GLFWwindow* window, int key, int scancode, int action, int mods) {

	switch (action)
	{
	case GLFW_RELEASE:
		std::cout << "Release code" << key << std::endl;
		break;
	case GLFW_PRESS:
		std::cout << "Press code" << key << std::endl;
		BlocksDestroyer.sceneEvent();
		break;
	case GLFW_REPEAT:
		std::cout << "Repeat code" << key << std::endl;
		break;

	};

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);


	
}


int main(int m) {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "Mine", NULL, NULL);
	if (window == NULL) {

		std::cout << "FAILED to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, Game::keyCallback);
	glfwSwapInterval(1);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {

		std::cout << "FAILED to initialize GLAD" << std::endl;
		return -1;

	}
	

	//   для текстур с альфа каналом
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//

	 BlocksDestroyer.Init();

	

		
	float timeValue = glfwGetTime(); // Получаем начальное время

	const float AnimationTimer = 0.2f;
	float timer = 0.0f;

	
	Game::keyCallbacks.push_back(keyChanged);

	while (!glfwWindowShouldClose(window)){
		float LastTime = timeValue;  // Сохраняем предыдущее время
		 timeValue = glfwGetTime(); // Получаем текущее время
		float felapsedTime = timeValue - LastTime;
		timer += felapsedTime;



		BlocksDestroyer.Update(felapsedTime);


		glfwSwapBuffers(window);
		glfwPollEvents();

		glClearColor(0.2, 0.3, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		

		BlocksDestroyer.Render();

	
	

	}
	glfwTerminate();
	ResourceManager::clear();
	return 0;


}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

	glViewport(0, 0, width, height);


}
