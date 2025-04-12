#include "CellData.h"


//
//uint8_t WaterData::computeWaterMask(int x, int y, int width, const std::vector<uint8_t>& indices)
//{
//
//		auto r = [&](int dx, int dy) {
//			int nx = x + dx, ny = y + dy;
//			return (nx >= 0 && nx < width && ny >= 0 && ny < indices.size() / width) && indices[ny * width + nx] == WATERIDX;
//			};
//
//		return(r(-1, -1) << 0) | (r(0, -1) << 1) | (r(1, -1) << 2) |  // ���������� � ������ ��� �������� 
//			(r(-1, 0) << 3) |                      (r(1, 0) << 4) |
//			(r(-1, 1) << 5) |    (r(0, 1) << 6) |  (r(1, 1) << 7);
//
//
//	};

	CellChecker::CellChecker()
	{
		Data = {

			//               
			CellData{0b01000010,
					 0b01011010,29},
			CellData{0b01001000,
					 0b01011110,38},
			CellData{0b01010000,
					 0b01011011,46},
			CellData{0b00001010,
					 0b11011010,62},
			CellData{0b01010010,
					 0b01011010,53},
			CellData{0b00010010,
					 0b01111010,54},
			CellData{0b00011000,
					 0b01011010,12},
			CellData{0b00010000,
					 0b01111011,11},
			CellData{0b01000000,
					 0b01011111,25},
			CellData{0b00001000,
					 0b11011110,8},
			CellData{0b00000010,
					 0b11111010,1},
			CellData{0b10100010,
					 0b11111010,44},
			CellData{0b10001100,
					 0b11011110,52},
			CellData{0b00110001,
					 0b01111011,60},
			CellData{0b00100010,
					 0b11111010,9},
			CellData{0b01000100,
					 0b01011111,10},
			CellData{0b00110000,
					 0b01111011,18},
			CellData{0b01000001,
					 0b01011111,20},
			CellData{0b10000010,
					 0b11111010,22},
			CellData{0b00010001,
					 0b01111011,14},
			CellData{0b10001000,
					 0b11011110,16},
			CellData{0b00110010,
					 0b01111010,28},
			CellData{0b01010001,
					 0b01011011,4},
			CellData{0b01001100,
					 0b01011110,6},
			CellData{0b10001010,
					 0b11011010,30},
			CellData{0b00011010,
					 0b01011010,37},
			CellData{0b01011000,
					 0b01011010,45},
			CellData{0b01101011,
					 0b01111011,61},
			CellData{0b00001100,
					 0b11011110,17},
			CellData{0b01001010,
					 0b01011010,31},
			CellData{0b01000101,
					 0b01011111,36},
			CellData{0b01011010,
					 0b01011010,21},
			CellData{0b10000001,
					 0b11111111,15},
			CellData{0b00100100,
					 0b11111111,23},
			CellData{0b10100000,
					 0b11111111,43},
			CellData{0b00000101,
					 0b11111111,35},
			CellData{0b10000100,
					 0b11111111,51},
			CellData{0b00100001,
					 0b11111111,59},
			CellData{0b00000001,
					 0b11111111,0},
			CellData{0b10000000,
					 0b11111111,27},
			CellData{0b00100000,
					 0b11111111,24},
			CellData{0b00000100,
					 0b11111111,3},
			CellData{0b00100101,
					 0b11111111,34},
			CellData{0b10000101,
					 0b11111111,42},
			CellData{0b10100100,
					 0b11111111,50},
			CellData{0b10100001,
					 0b11111111,58},
			CellData{0b10100101,
					 0b11111111,13},
		};

	}

	bool CellChecker::findIndex(char& flags,uint8_t& idx)
	{
		
		bool isExist = false;
		// �������� �� ���� ��������� ������� Data
		for (size_t i = 0; i < Data.size(); ++i) {
			// ��������� ����� � ������ � ������� ������
			if ((flags & Data[i].mask) == (Data[i].flags & Data[i].mask)) { // �� ��������� �� ����� �������� ������ ������ ��� ����, �� ��������� ������ 0
				// ���������� ������, ���� ���� ������� �� �����
				idx = Data[i].idx;
				isExist = true;
				return true;
			}
		}

		// ���� ������ �� �����, ���������� 0 ��� ������ ���������
		return isExist;
	}
