#ifndef  INDICES_GENERATOR_H
#define  INDICES_GENERATOR_H
#include <vector>
#include <glm/glm.hpp>


// Represents the four possible (collision) directions


class indicesGenerator {
public:

	indicesGenerator();

	uint8_t GenerateGroundIndex(uint8_t escapingNumber = 0);
	std::vector <uint8_t>& AlgorithmBasicGround(unsigned int row, unsigned int line);
	std::vector <uint8_t>& AlgorithmLakeGround(unsigned int row, unsigned int line,unsigned int LakesCount);


private:
	std::vector <uint8_t> indices;


};


#endif // ! INDICESGENERATOR_H
