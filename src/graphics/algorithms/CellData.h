#ifndef CELLDATA_H
#define CELLDATA_H
#include <unordered_map>
struct CellData
{
	char flags;
	char mask;
	uint8_t idx;


	CellData(float flags, float mask, uint8_t idx) :flags(flags), mask(mask), idx(idx) {};

};


class CellChecker {

public:

	CellChecker();

bool findIndex(char& flags,uint8_t& idx);


private:


 std::vector<CellData> Data;


};

#endif // !CELLDATA_H
