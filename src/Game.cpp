#include "Game.h"
#include "graphics/ResourceManager.h"
#include "graphics/SpriteRenderer.h"
#include "graphics/Effects/Particle.h"

#include "graphics/Direction.hpp"

#include "graphics/Text/TextRenderer.h"
#include "sstream"

#include "audio/SoundSource.h"
#include "audio/SoundDevice.h"
#include "audio/SoundBuffer.h"
#include "audio/MusicBuffer.h"





unsigned int Game::RECORD = 0;
unsigned int Game::POINTS = 0;
 std::vector<void(*)(GLFWwindow* windiw, int key, int scancode, int action, int mods)>Game::keyCallbacks;

const int BRICK_SIZE = 32;
const glm::ivec2 SQUERE_SIZE = { 384,256 };

SpriteRenderer* renderer;
character* player;
ParticleGenerator* particles;
TextRenderer* text;
GameLevel* gameLevel;

SoundDevice* soundDevice; //singleton
SoundSource Speaker;
MusicBuffer* music;

 bool::Game::Keys[GLFW_KEY_LAST] = { 0 };  //GLFW_KEY_LAST это константа, которая задаёт максимальное количество клавиш, распознаваемых GLFW.
 bool Game::KeysProcessed[GLFW_KEY_LAST] = { 0 };

 // Initial velocity of the player paddle
 const glm::vec2 INITIAL_PLAYER_VELOCITY(50.0f,0.0f);
 glm::vec2 CURRENT_PLAYER_VELOCITY(0.0f, 0.0f);

 const float timeClamp = 0.05f;
 const unsigned int TEXTSIZE = 24;



Game::Game(unsigned int width, unsigned int height) :  Width(width), Height(height),State(GAME_ACTIVE)
{ 
}

Game::~Game()
{
	delete renderer;
	delete particles;
	delete player;
	delete gameLevel;
	delete text;
	delete music;

}

void Game::Init()
{

	offset = {Width / 2 - SQUERE_SIZE.x / 2,Height / 2 - SQUERE_SIZE.y / 2 };
	CURRENT_PLAYER_VELOCITY = INITIAL_PLAYER_VELOCITY * 2.0f;
	if (!loadGame(saveData, "save/saveGame.bin")) // when we go out from the game in some progress
		saveGame(saveData, "save/saveGame.bin");

	if (!loadGame(RECORD, "save/saveRecord.bin"))
		saveGame(RECORD, "save/saveRecord.bin");

	soundDevice = SoundDevice::get();
	//uint32_t Music = SoundBuffer::get()->addSoundEffect("Assets/audio/Main_Music.mp3");
	//Speaker.generate();
	//Speaker.Play(Music);*/
	music = new MusicBuffer("Assets/audio/Main_Music.mp3");
	music->Play();

	ResourceManager::LoadShader("Assets/shaders/vertexShader.glsl", "Assets/shaders/fragmentShader.glsl", nullptr, "sprite");
	ResourceManager::LoadShader("Assets/shaders/particleVS.glsl", "Assets/shaders/particleFS.glsl", nullptr, "particle");
	ResourceManager::LoadShader("Assets/shaders/particleVS.glsl", "Assets/shaders/particleFS.glsl", nullptr, "particle");


	ResourceManager::LoadTexture("Assets/textures/Plates_Clear.png", true, "plates");
	ResourceManager::LoadTexture("Assets/textures/Character.png", true, "Char");
	ResourceManager::LoadTexture("Assets/textures/particle.png", true, "Stones");
	ResourceManager::LoadTexture("Assets/textures/Plates_Cracking.png", true, "Cracks");
	ResourceManager::LoadTexture("Assets/textures/BackGround.png", true, "background");

	 renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"), ResourceManager::GetTexture("plates").Width, ResourceManager::GetTexture("plates").Height, 128);  // передаем для плейна, который берёт тайл в 128
	gameLevel = new GameLevel();
	 gameLevel->Generate(&ResourceManager::GetTexture("plates"), BRICK_SIZE, offset, SQUERE_SIZE,saveData.Level);
	 particles = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("Stones"), 100);
	

	 glm::mat4 ortoMatrix = glm::ortho(0.0f, static_cast<float>(Width), static_cast<float>(Height), 0.0f, -1.0f, 1.0f);


	 //configure shaders
	 ResourceManager::GetShader("sprite").use().setInt("texture0", 0);
	 ResourceManager::GetShader("sprite").setMat4("projection", ortoMatrix);
	 ResourceManager::GetShader("particle").use().setInt("texture0", 0);
	 ResourceManager::GetShader("particle").setMat4("projection", ortoMatrix);

	 glm::vec2 CharSize = {48.0f,48.0f };

	 player = new character(offset-CharSize / 4.0f, CharSize, CURRENT_PLAYER_VELOCITY , 0, &ResourceManager::GetTexture("Char"),charDirection::Right);

	 text = new TextRenderer(this->Width, this->Height);
	 text->Load("Assets/fonts/Rubik-ExtraBold.ttf", TEXTSIZE);
	
	
}



void Game::ProcessInput(float dt)
{
	//float velocity = INITIAL_PLAYER_VELOCITY * dt;
				//move playerboard
				
		

}
void Game::Update(float dt){

	if (this->State == GAME_ACTIVE){

	DoCollisions();

	AnimationTimer(dt);
	ProcessInput(dt);
	player->update(dt, SQUERE_SIZE, offset);
	particles->Update(dt);
	gameLevel->Update(dt);

	
	if (gameLevel->IsCompleted()) {
		this->ResetLevel();
		this->ResetPlayer();
		this->State = GAME_WIN;
		saveData.Level++;
		
		gameLevel->Generate(&ResourceManager::GetTexture("plates"), BRICK_SIZE, offset, SQUERE_SIZE, saveData.Level);

	}

	if (POINTS > RECORD)
		RECORD = POINTS;

	}
}

void Game::Render(){
	


	music->UpdateBufferStream();


		ResourceManager::GetShader("sprite").use();
		ResourceManager::GetShader("sprite").setvec2("spriteScale", { 1.0f,1.0f });
		ResourceManager::GetShader("sprite").setvec2("uv", { 0.0f ,0.0f });
		ResourceManager::GetShader("sprite").setvec4("color", {1.0f,1.0f,1.0f,1.0f});
		renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);


		gameLevel->Draw(*renderer);

		particles->Draw();
		player->Draw(*renderer);
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			//std::cout << x <<"\t" << y << std::endl;
		
		


		if (this->State == GAME_MENU) {
			std::string sMenu = "MENU";
			text->RenderText(sMenu, Width / 2.0f - (sMenu.size() * TEXTSIZE / 2.0f), Height / 2.0f - sMenu.size() * TEXTSIZE / 2.0f , 2.0f);

		}
		else  if (this->State == GAME_WIN) {
			std::string sWind = "WIN";
			text->RenderText(sWind, Width / 2.0f - sWind.size() * TEXTSIZE / 2.0, Height / 2.0f - sWind.size() * TEXTSIZE / 2.0, 1.5f);

		}
		else{
		std::stringstream ss, sp,sr, sh; ss << saveData.Level;
		text->RenderText("Level: " + ss.str(), Width / 2.0f - SQUERE_SIZE.x / 2.0f, Height / 2.0f-SQUERE_SIZE.y / 2.0f - TEXTSIZE * 2.0f, 0.8f);
		sp << POINTS;
		text->RenderText("Points : " + sp.str(), Width / 2.0f + sp.str().size() * TEXTSIZE, Height / 2.0f - SQUERE_SIZE.y / 2.0f - TEXTSIZE * 2.0f, 0.8f);
		sr << RECORD;
		text->RenderText("RECORD : " + sr.str(), Width / 2.0f + sp.str().size() * TEXTSIZE, Height / 2.0f + SQUERE_SIZE.y / 2.0f + TEXTSIZE * 2.0f, 1.0f);
		sh << saveData.currLife;
		text->RenderText("LIFE : " + sh.str(), Width / 2.0f - sh.str().size() * TEXTSIZE, Height / 2.0f - SQUERE_SIZE.y / 2.0f - TEXTSIZE * 2.0f, 0.8f);

		

		
		}




	


	


}

void Game::CharDeath(){
	//dead
	if (saveData.currLife > 0) {
	saveData.currLife--;
	}
	else
	{

		saveData.Level = 1;
		saveData.currLife = saveData.Life;
		unsigned int LoadedRecord = 0;

		if (loadGame(LoadedRecord, "save/saveRecord.bin")) // если файл есть то мы подгружаем старый прогресс
		{
			if (RECORD > LoadedRecord) // если текущий рекорд больше подгруженого
			{
				saveGame(RECORD, "save/saveRecord.bin");  // то мы 

			}
		}

	
	}

	this->ResetLevel();
	this->ResetPlayer();
	this->State = GAME_MENU;
	POINTS = 0;

	gameLevel->Generate(&ResourceManager::GetTexture("plates"), BRICK_SIZE, offset, SQUERE_SIZE, saveData.Level);
}

void Game::ResetLevel()
{
	particles->reset(true);
}

void Game::ResetPlayer()
{
	offset = { Width / 2 - SQUERE_SIZE.x / 2,Height / 2 - SQUERE_SIZE.y / 2 };
	glm::vec2 CharSize = { 48.0f,48.0f };
	player->pos = offset - CharSize / 4.0f;
	player->velocity = { CURRENT_PLAYER_VELOCITY.x * saveData.Level,CURRENT_PLAYER_VELOCITY.y * saveData.Level };
	player->rotate = 0;
	player->setDirection(charDirection::Right, CURRENT_PLAYER_VELOCITY);
}

void Game::sceneEvent(){


	if (this->State == GAME_MENU) {

		if (this->Keys[GLFW_KEY_SPACE]) {

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
	}

		if (this->State == GAME_WIN) {

			if( this->Keys[GLFW_KEY_SPACE])
			{
				//	this->KeysProcessed[GLFW_KEY_ENTER] = true;
				//	Effects->Chaos = false;
 				this->State = GAME_MENU;

			}

		}

	

			if (this->State == GAME_ACTIVE) {


				if (this->Keys[GLFW_KEY_V] && !this->KeysProcessed[GLFW_KEY_V])
				{

				

				}
				

			}



		if (this->Keys[GLFW_KEY_A])
		{
			if (player->pos.x >= 0.0f) {
				//		player->pos.x -= velocity;
				player->setDirection(charDirection::Left, CURRENT_PLAYER_VELOCITY);

			}



		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (player->pos.x <= this->Width - player->size.x)
			{
				//	player->pos.x += velocity;
				player->setDirection(charDirection::Right, CURRENT_PLAYER_VELOCITY);
				std::cout << "HELLO" << std::endl;
			}


		}

		if (this->Keys[GLFW_KEY_W])
		{
			if (player->pos.y >= 0.0f) {
				//		player->pos.y -= velocity;
				player->setDirection(charDirection::Up, CURRENT_PLAYER_VELOCITY);

			}



		}
		if (this->Keys[GLFW_KEY_S])
		{
			if (player->pos.y <= this->Height - player->size.y)
			{
				//		player->pos.y += velocity;
				player->setDirection(charDirection::Down, CURRENT_PLAYER_VELOCITY);

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
			particles->Spawn(*player, 2, glm::vec2(player->size.x / 2.0f - 5, player->size.y / 2.0f - 5));
		
		}


	}

	void Game::DoCollisions()
	{
		for (GameObject& box : gameLevel->Bricks) {

				bool bCollision = checkCollision(*player, box); // checking on collision with box

			if (!box.bDestroyed) {   // when box not bDestroyed


				if (bCollision == true){   



					if (!box.IsSolid) {   

						//box.bDestroyed = true;
						if (!box.prepared)
						{

						gameLevel->PreparateDestroy(&box);
						POINTS += 100;
						
						}
					}

					if (box.IsDanger) 
						CharDeath();


				}





			}
			else
			{
				//dead
				if (bCollision == true) {
					CharDeath();
					


				}
			}


		}

	}

	bool Game::checkCollision(character& player, GameObject& box)
	{


		//get center point circle first
		glm::vec2 center(player.pos + player.size / 2.0f);
		//Calculate AABB info
		glm::vec2 aabb_half_extens(box.size / 2.0f);
		glm::vec2 aabb_center(box.pos + aabb_half_extens);
		glm::vec2 difference = center - aabb_center;

		glm::vec2 clmaped = glm::clamp(difference, -aabb_half_extens, aabb_half_extens); //nearest point in local space  -aabb, +aabb
		glm::vec2 closest = aabb_center + clmaped; // nearest point in world

		difference = closest - center; // вектор от сентра к ближайшей точк

		if (glm::length(difference) <= 0.01) {
			return true;



		}
		else
		{
			return false;
		}




		return false;
	}

	

	