#ifndef BRICKOBJECT_H
#define BRICKOBJECT_H
#include "GameObject.h"



class BrickObject : public GameObject
{ 
public:

	BrickObject();
	BrickObject(glm::vec2 pos, glm::vec2 size, float angle, Texture* texture, int TextureIdx);

	
	



};



#endif // !BRICKOBJECT_H
