#ifndef UINT1024_HPP
#define UINT1024_HPP

#include <algorithm>
#include <bitset>
#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
#include <sstream>
#include <fstream>
#include <array>

using namespace std;

class UINT1024 {

private:
	const int size = 1024;
	uint32_t* Base32 = nullptr;

public:
	bitset<1024> num{ 0x0 };
	UINT1024(uint64_t init);
	UINT1024();

	UINT1024 operator+(UINT1024 num2);
	UINT1024 operator+(int num2);
	UINT1024 operator+(long long num2);

	UINT1024 operator-(UINT1024 num2);
	UINT1024 operator-(int num2);
	UINT1024 operator-(long long num2);

	UINT1024 operator/(UINT1024 num2);
	UINT1024 operator/(int num2);
	UINT1024 operator/(long long num2);

	UINT1024 operator*(UINT1024 num2);
	UINT1024 operator*(int num2);
	UINT1024 operator*(long long num2);

	UINT1024 operator%(UINT1024 num2);
	UINT1024 operator%(int num2);
	UINT1024 operator%(long long num2);

	void operator=(UINT1024 num2);
	friend ostream& operator<<(ostream& ss, UINT1024& s);

	//Deprecated
	string ToString();
	string BitsToDecimal();
	void leftShift(uint8_t BCD[], uint8_t sizeBCD);
	bool MultBy2(int arr[], int size);
	bool Sum2Arr(int arr1[], int arr2[], int size, int resSize);
	//Deprecated

	string FastB2DConversion();


	void DivTwoWords(uint64_t a, uint64_t b, uint32_t c, uint32_t* r, uint32_t* rest);
	bool IsSetToZero(uint32_t& iterator, uint32_t* lista);
	uint64_t getMax(const UINT1024* u);
	long long value();


	void set(long long);
	void set(long long,bool);
	void reset();
	bool get(short);
};
#endif // UINT1024_HPP

