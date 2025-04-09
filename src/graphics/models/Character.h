#ifndef CHARACTER_H
#define CHARACTER_h
#include "GameObject.h"

class character :public GameObject
{
public:
	character();
	character(glm::vec2 pos, glm::vec2 size,glm::vec2 velocity, float angle, Texture* texture,uint8_t charDirection);

	//character

	void setDirection(uint8_t direction, const glm::vec2& INITIAL_PLAYER_VELOCITY);
	void Draw(SpriteRenderer& renderer) override;
	void UVPulse();
	void update(float dt,const glm::ivec2& Squere,const glm::ivec2& offset);

private:

	void Calc_UV_Direction(const uint8_t& direction, const glm::vec2& initialVelocity);

	uint8_t charDirection;
	glm::vec2 UVCoords;

};


#endif // !CHARACTER_H
