#ifndef  GAME_H
#define GAME_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "graphics/Level/GameLevel.h"


enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};



class Game {
public:
	static std::vector<void(*)(GLFWwindow* windiw, int key, int scancode, int action, int mods)>keyCallbacks;
	GameState State;
	
	unsigned int Width, Height;

	Game(unsigned int width, unsigned int height);
	~Game();

	void Init();

	void ProcessInput(float dt);
	void Update(float dt);
	void Render();

	GameLevel gameLevel;
	unsigned int Level;


	void ResetLevel();
	void ResetPlayer();
	void sceneEvent();
	static void keyCallback(GLFWwindow* window, int key, int scancode,int action, int modes);


private:

	float timer = 0.0f;

	glm::vec2 offset;

	static bool Keys[];
	static bool KeysProcessed[];
	void AnimationTimer(float dt);
};



#endif // ! GAME_H
