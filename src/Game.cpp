#include "Game.h"
#include "graphics/ResourceManager.h"
#include "graphics/SpriteRenderer.h"
#include "graphics/models/Character.h"
#include "graphics/Direction.hpp"

const int BRICK_SIZE = 32;
const glm::ivec2 SQUERE_SIZE = { 384,256 };

SpriteRenderer* renderer;
character* player;
 std::vector<void(*)(GLFWwindow* windiw, int key, int scancode, int action, int mods)>Game::keyCallbacks;

 bool::Game::Keys[GLFW_KEY_LAST] = { 0 };  //GLFW_KEY_LAST это константа, которая задаёт максимальное количество клавиш, распознаваемых GLFW.
 bool Game::KeysProcessed[GLFW_KEY_LAST] = { 0 };

 // Initial velocity of the player paddle
 const glm::vec2 INITIAL_PLAYER_VELOCITY(50.0f,0.0f);

 const float timeClamp = 0.05f;




Game::Game(unsigned int width, unsigned int height) :  Width(width), Height(height),State(GAME_ACTIVE)
{ 
}

Game::~Game()
{
	delete renderer;

}

void Game::Init()
{
	offset = {Width / 2 - SQUERE_SIZE.x / 2,Height / 2 - SQUERE_SIZE.y / 2 };
	


	ResourceManager::LoadShader("Assets/shaders/vertexShader.glsl", "Assets/shaders/fragmentShader.glsl", nullptr, "sprite");
	ResourceManager::LoadTexture("Assets/textures/Plates_Clear.png", true, "plates");
	ResourceManager::LoadTexture("Assets/textures/Character.png", true, "Char");
	ResourceManager::GetShader("sprite").use().setInt("texture0", 0);
	 renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"), ResourceManager::GetTexture("plates").Width, ResourceManager::GetTexture("plates").Height, 128);  // передаем для плейна, который берёт тайл в 128
	 gameLevel.Generate(&ResourceManager::GetTexture("plates"), BRICK_SIZE, offset, SQUERE_SIZE);

	 glm::mat4 ortoMatrix = glm::ortho(0.0f, static_cast<float>(Width), static_cast<float>(Height), 0.0f, -1.0f, 1.0f);

	 ResourceManager::GetShader("sprite").setMat4("projection", ortoMatrix);

	 glm::vec2 CharSize = { 48.0f,48.0f };
	 player = new character(offset-CharSize / 4.0f, CharSize,INITIAL_PLAYER_VELOCITY, 0, &ResourceManager::GetTexture("Char"),charDirection::Right);


}



void Game::ProcessInput(float dt)
{
	//float velocity = INITIAL_PLAYER_VELOCITY * dt;
				//move playerboard
				
		

}
void Game::Update(float dt){

	AnimationTimer(dt);



	ProcessInput(dt);

	player->update(dt, SQUERE_SIZE, offset);
	

}

void Game::Render(){
	if (this->State == GAME_ACTIVE || this->State == GAME_MENU) {


		

		gameLevel.Draw(*renderer);

		ResourceManager::GetShader("sprite").use();
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			//std::cout << x <<"\t" << y << std::endl;
		
		


		
		player->Draw(*renderer);




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

					gameLevel.Generate(&ResourceManager::GetTexture("plates"), BRICK_SIZE, offset, SQUERE_SIZE);

				}
				

			}

		}


		if (this->Keys[GLFW_KEY_A])
		{
			if (player->pos.x >= 0.0f) {
				//		player->pos.x -= velocity;
				player->setDirection(charDirection::Left, INITIAL_PLAYER_VELOCITY);

			}



		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (player->pos.x <= this->Width - player->size.x)
			{
				//	player->pos.x += velocity;
				player->setDirection(charDirection::Right, INITIAL_PLAYER_VELOCITY);
				std::cout << "HELLO" << std::endl;
			}


		}

		if (this->Keys[GLFW_KEY_W])
		{
			if (player->pos.y >= 0.0f) {
				//		player->pos.y -= velocity;
				player->setDirection(charDirection::Up, INITIAL_PLAYER_VELOCITY);

			}



		}
		if (this->Keys[GLFW_KEY_S])
		{
			if (player->pos.y <= this->Height - player->size.y)
			{
				//		player->pos.y += velocity;
				player->setDirection(charDirection::Down,INITIAL_PLAYER_VELOCITY);

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

	void Game::AnimationTimer(float dt){


		timer += dt;   

		if (timer > timeClamp) {

			timer -= timer;
			player->UVPulse();
			
		}


	}
