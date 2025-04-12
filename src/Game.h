#ifndef  GAME_H
#define GAME_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "graphics/Level/GameLevel.h"
#include "graphics/models/Character.h"

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};



struct SaveData
{
	
	
	 unsigned int Life;
	 unsigned int currLife;
	 unsigned int Level;
	 SaveData() : Life(3), currLife(3),Level(1) {
		 std::cout << "Hello" << std::endl;
	 };

};


template<typename T>
void saveGame(const T Data, const std::string& fileName) {
	std::ofstream outFile(fileName, std::ios::binary);
	if (outFile) {
		outFile.write(reinterpret_cast<const char*>(&Data), sizeof(T));
		outFile.close();
		std::cout << "GAME::SAVED " << std::endl;
	}
	else
	{
		std::cout << "ERROR::SAVED:can't find file for save " << std::endl;
	}


}


template<typename T>
bool loadGame(T& Data, const std::string& fileName) {
	std::ifstream inFile(fileName, std::ios::binary);
	if (inFile) {
		inFile.read(reinterpret_cast<char*>(&Data), sizeof(T));
		inFile.close();

		std::cout << "PROGRESS LOADED" << std::endl;
		return true;

	}
	else
	{
		std::cout << "ERROR::LOAD: can't open file for reading" << std::endl;
	}



	return false;
}


class Game {
public:
	static std::vector<void(*)(GLFWwindow* windiw, int key, int scancode, int action, int mods)>keyCallbacks;
	GameState State;

	unsigned int Width, Height;
	static unsigned int RECORD;
	static unsigned int POINTS;

	Game(unsigned int width, unsigned int height);
	~Game();

	void Init();

	void ProcessInput(float dt);
	void Update(float dt);
	void Render();


	void CharDeath();


	void ResetLevel();
	void ResetPlayer();
	void sceneEvent();
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int modes);

	SaveData& getSaveData() { return saveData; };


private:

	glm::vec2 offset;
	float timer = 0.0f;
	SaveData saveData;


	static bool Keys[];
	static bool KeysProcessed[];
	void AnimationTimer(float dt);
	void DoCollisions();
	bool checkCollision(character& player, GameObject& box);


	

};


#endif // ! GAME_H
