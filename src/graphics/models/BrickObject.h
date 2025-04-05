#ifndef BRICKOBJECT_H
#define BRICKOBJECT_H
#include "GameObject.h"



class BrickObject : public GameObject
{ 
public:

	BrickObject();
	BrickObject(glm::vec2 pos, glm::vec2 size, float angle, Texture* texture);

	
	



};



#endif // !BRICKOBJECT_H
