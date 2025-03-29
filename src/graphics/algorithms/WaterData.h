#ifndef  WATER_DATA_H
#define WATER_DATA_H
#include <unordered_map>

#define WATERIDX 7
enum Direction {
	LEFTUP,
	UP,
	RIGHTUP,
	RIGHT,
	RIGHTDOWN,
	DOWN,
	LEFTDOWN,
	LEFT

};



struct WaterData
{
	                 //byte    number
	static std::unordered_map<uint8_t, uint8_t> waterTiles;
	
	static uint8_t computeWaterMask(int x, int y, int width, const std::vector<uint8_t>& indices);





};

#endif // ! WATER_DATA_H
