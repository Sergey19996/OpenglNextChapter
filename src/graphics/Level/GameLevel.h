#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include "../models/BrickObject.h"
#include <vector>
#include "../ResourceManager.h"
#include "../algorithms/indicesGenerator.h"

class GameLevel {
public:
	std::vector<GameObject> Bricks;
	GameLevel();
	~GameLevel();
	void Generate(const unsigned int&  SCREENWIDTH,const  unsigned int& SCREENHEIGHT,Texture* texture);

	void Draw(SpriteRenderer& reder);

	bool IsCompleted();


private:
	void init(const unsigned int& SCREENWIDTH, const unsigned int& SCREENHEIGHT, std::vector<uint8_t>& indices,const int& row, const int& line, Texture* texture );

	const int BRICK_SIZE = 32;



};


#endif // !GAMELEVEL_H
