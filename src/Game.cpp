#include "Game.h"
#include "graphics/ResourceManager.h"
#include "graphics/SpriteRenderer.h"
SpriteRenderer* renderer;

 std::vector<void(*)(GLFWwindow* windiw, int key, int scancode, int action, int mods)>Game::keyCallbacks;

 bool::Game::Keys[GLFW_KEY_LAST] = { 0 };  //GLFW_KEY_LAST это константа, которая задаёт максимальное количество клавиш, распознаваемых GLFW.
 bool Game::KeysProcessed[GLFW_KEY_LAST] = { 0 };



Game::Game(unsigned int width, unsigned int height) :  Width(width), Height(height),State(GAME_ACTIVE)
{ 
}

Game::~Game()
{
	delete renderer;

}

void Game::Init()
{

	ResourceManager::LoadShader("Assets/shaders/vertexShader.glsl", "Assets/shaders/fragmentShader.glsl", nullptr, "sprite");
	ResourceManager::LoadTexture("Assets/textures/Plates_Clear.png", true, "plates");
	ResourceManager::GetShader("sprite").use().setInt("texture0", 0);
	 renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"), ResourceManager::GetTexture("plates").Width, ResourceManager::GetTexture("plates").Height, 128);  // передаем для плейна, который берёт тайл в 128
	 gameLevel.Generate(Width, Height, &ResourceManager::GetTexture("plates"));

	 glm::mat4 ortoMatrix = glm::ortho(0.0f, static_cast<float>(Width), static_cast<float>(Height), 0.0f, -1.0f, 1.0f);

	 ResourceManager::GetShader("sprite").setMat4("projection", ortoMatrix);


}



void Game::ProcessInput(float dt)
{
	//float velocity = PLAYER_VELOCITY * dt;
				////move playerboard
				//if (this->Keys[GLFW_KEY_A])
				//{
				//	if (Player->Position.x >= 0.0f) {
				//		Player->Position.x -= velocity;
				//		if (Ball->Stuck)
				//			Ball->Position.x -= velocity;

				//	}



				//}
				//if (this->Keys[GLFW_KEY_D])
				//{
				//	if (Player->Position.x <= this->Width - Player->Size.x)
				//	{
				//		Player->Position.x += velocity;
				//		if (Ball->Stuck)
				//			Ball->Position.x += velocity;

				//	}


				//}
				//if (this->Keys[GLFW_KEY_SPACE]) {
				//	Ball->Stuck = false;


				//}

}
void Game::Update(float dt){


}

void Game::Render(){
	if (this->State == GAME_ACTIVE || this->State == GAME_MENU) {




		gameLevel.Draw(*renderer);

		ResourceManager::GetShader("sprite").use();
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			//std::cout << x <<"\t" << y << std::endl;
		
		ResourceManager::GetShader("sprite").setFloat("timeValue", glfwGetTime());







	}



}

void Game::sceneEvent(){

		if (this->State == GAME_WIN) {

			if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
			{
				//	this->KeysProcessed[GLFW_KEY_ENTER] = true;
				//	Effects->Chaos = false;
				this->State = GAME_MENU;

			}

		}

		if (this->State == GAME_MENU) {

			if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER]) {

				this->State = GAME_ACTIVE;
				//	this->KeysProcessed[GLFW_KEY_ENTER] = true;
			/*	}
				if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W])
				{
					this->Level = (this->Level + 1) % 4;
					this->KeysProcessed[GLFW_KEY_W] = true;
				}
				if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S])
				{
					if (this->Level > 0)
						--this->Level;
					else
						this->Level = 3;
					this->KeysProcessed[GLFW_KEY_S] = true;


				}*/

			}

			if (this->State == GAME_ACTIVE) {


				if (this->Keys[GLFW_KEY_V] && !this->KeysProcessed[GLFW_KEY_V])
				{

					gameLevel.Generate(Width, Height, &ResourceManager::GetTexture("plates"));

				}
				

			}

		}


	}

	void Game::keyCallback(GLFWwindow* window, int key, int scancode,int action, int modes){
		if (action != GLFW_RELEASE)   //если кнопка не релиз	
		{
			if (!Keys[key]) {    //если эта кнока false
				Keys[key] = true; //ставим true
			}

		}
		else
		{
			Keys[key] = false;
		}

		KeysProcessed[key] = action != GLFW_REPEAT; //Если событие не повторное (GLFW_PRESS или GLFW_RELEASE), то выражение вернёт true.

		for (void(*func)(GLFWwindow*, int, int, int, int) : Game::keyCallbacks) {
			func(window, key, scancode, action, modes);
		}

	}
