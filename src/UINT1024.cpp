#include "UINT1024.hpp"

UINT1024::UINT1024(uint64_t init) {
	bitset<1024> aux(init);
	num = aux;
}

UINT1024::UINT1024() {
	bitset<1024> aux(0x0);
	num = aux;
}

UINT1024 UINT1024::operator+(UINT1024 num2)
{
	bool carry = false;
	unsigned long long aux = 0;
	int sizeOfGroup = 0;
	int sz = 0;
	UINT1024 result = 0;
	uint64_t* r = (uint64_t*)&result.num;
	uint64_t* p = (uint64_t*)(&num);
	uint64_t* n = (uint64_t*)(&num2.num);


	for (int i = 0; i < size / 64; i++)
	{
		sz = sizeOfGroup + 63;
		if (!*p && !*n) {
			*r++ = carry;
			p++; n++;
			continue;
		}

		aux = (*p++ + *n++ + carry);
		carry = (num[sz] && num2.num[sz]);

		*r++ = aux;
		sizeOfGroup += 64;
	}
	return result;
}

void UINT1024::operator=(UINT1024 num2)
{
	this->num = num2.num;
}

ostream& operator<<(ostream& ss, UINT1024& s)
{
	return (ss << s.FastB2DConversion());
}

//ostream& operator<<(ostream& ss, UINT1024 s)
//{
//	return (ss << s.FastB2DConversion());
//}

string UINT1024::ToString()
{
	int max = getMax(this);

	//if (max < 64)
	//{
	//	if (max == -1) return "0";
	//	else { return to_string(this->num.to_ullong()); }
	//}

	int decDigits = max * log10(2) + 1;

	int* res;
	//res = new int[decDigits];

	int* aux;
	aux = new int[decDigits];

	res = (int*)calloc(decDigits, sizeof(int));

	aux[0] = 1;
	res[0] = 0;
	int resSize = 0;
	int auxSize = 0;

	for (int i = 0; i <= max; i++)
	{
		if(i) auxSize += MultBy2(aux, auxSize);

		if (num.test(i))
		{
			resSize += Sum2Arr(res, aux, auxSize, resSize);
		}
	}

	stringstream stream;

	ofstream file;

	file.open("pipo.txt", ios::trunc);

	for (int i = decDigits-1; i >= 0; i--)
	{
 		stream << res[i];
	}
	file << stream.str().c_str();
	file.close();


	return stream.str();
}

string UINT1024::BitsToDecimal()
{
	uint16_t max = getMax(this);				 //Pilla la posicion del bit mas importante.
	if (max == -1) return "0";
	uint16_t DigitsSize = max * log10(2) + 1; //Tama�o de la cadena de digitos.

	//vector<bool> BCD(sizeBCD); //Vector de los digitos en BCD
	//uint8_t sizeBCD = DigitsSize * 4 + (DigitsSize % 2 ? 4 : 0); // vector<bool>

	uint8_t sizeBCD = DigitsSize / 2 + (DigitsSize % 2 ? 1 : 0);

	//vector<uint8_t> BCD(sizeBCD, 0); //Vector de los digitos en BCD.

	uint8_t* BCD = new uint8_t[sizeBCD];

	for (int i = 0; i < sizeBCD; i++)
	{
		BCD[i] = 0;
	}

	uint8_t* PBCDDigit = new uint8_t;

	for (uint16_t i = 0; i < max; i++)
	{
		leftShift(BCD, sizeBCD);

		BCD[0] |= num[max - i];

		PBCDDigit = (uint8_t*)BCD;

		for (uint8_t j = 0; j < sizeBCD; j++)
		{
			if ((*PBCDDigit & 0b00001111) > 4)
				*PBCDDigit += 3;

			if (((*PBCDDigit & 0b11110000)>>4) > 4)
				*PBCDDigit += 48; //3 << 4

			PBCDDigit++;
		}

	}
	leftShift(BCD, sizeBCD);
	BCD[0] |= num[0];

	string Decimal = "";

	PBCDDigit = &BCD[sizeBCD - 1];

	for (uint8_t i = 0; i <= sizeBCD / 2; i++)
	{
		Decimal += ('0' + ((*PBCDDigit & 0b11110000)>>4));
		Decimal += ('0' + (*PBCDDigit & 0b00001111));
		PBCDDigit--;
	}

	while (Decimal[0] == '0')
	{
		Decimal.erase(0, 1);
	}

	return Decimal;
}

string UINT1024::FastB2DConversion()
{
	Base32 = (uint32_t*)(&num);
	uint32_t* Final		= (uint32_t*)calloc(32, sizeof(uint32_t));
	string Decimal		= "";
	uint32_t iterator	= 31;
	for (; iterator > 0 && Base32[iterator] == 0; --iterator);

	do {
		uint32_t r			= 0;
		int i				= iterator;

		for (; i >= 0; --i) { DivTwoWords(r, Base32[i] , 10 , &Final[i], &r); }

		memcpy(Base32, Final , 32 * sizeof(uint32_t));

		memset(Final, 0, 32 * sizeof(uint32_t));

		Decimal.insert(Decimal.begin(),'0' + r);

	} while (!IsSetToZero(iterator,Base32));

	Final = nullptr;

	free(Final);

	return Decimal;
}

void UINT1024::DivTwoWords(uint64_t a, uint64_t b, uint32_t c, uint32_t* r, uint32_t* rest) {
	uint64_t big = ((a << 32) | b);// EDX:EAX      q-> EAX, r-> EDX
	lldiv_t st = lldiv(big, c);
	*r = st.quot;
	*rest = st.rem;
	/*
	uint32_t r_;
	uint32_t rest_;
	r_ = big / c;
	rest_ = big % c;
	*r = r_;
	*rest = rest_;
	*/
}

bool UINT1024::IsSetToZero(uint32_t& iterator, uint32_t* lista) {
	int x = iterator;
	for (; x > 0 && lista[x] == 0; x--);
	iterator = x;
	if (!x && !lista[x]) { return true; }
	return false;
}

bool UINT1024::MultBy2(int* arr, int size)
{
	short aux, carry = 0;

	if (size == this->size-1) size--;

	for (int i = 0; i <= size; i++)
	{
		aux = arr[i]*2 + carry;

		arr[i] = aux % 10;
		carry = aux / 10;
	}

	if (carry)
	{
		arr[size + 1] = 1;
	}
	return carry;
}

bool UINT1024::Sum2Arr(int arr1[], int arr2[], int size, int resSize)
{
	uint8_t carry = 0;
	uint8_t aux = 0;
	int actualSize = 0;

	for(int x = 0; x <= size; x++){

		aux = (arr1[x] + arr2[x] + carry);
		arr1[x] = (aux) % 10;
	    carry = (aux)/10;
		actualSize++;
	}
	while (carry)
	{
		if (size == this->size) break;
		aux = arr1[actualSize] + carry;
		arr1[actualSize] = aux % 10;
		carry = aux / 10;
		actualSize++;
	}

	return actualSize - 1 > resSize;
}

long long UINT1024::value()
{
	return this->num.to_ullong();
}

uint64_t UINT1024::getMax(const UINT1024* u)
{
	uint64_t* BitGroup = (uint64_t*)&(u->num);
	BitGroup += 15;

	for (int i = size/64 - 1; i >= 0; i--)
	{
		if (*BitGroup)
		{
			return log2(*BitGroup) + 64*i; // Devuelve la posicion del primer uno.
		}
		BitGroup--;
	}

	return -1;
}

void UINT1024::leftShift(uint8_t BCD[], uint8_t sizeBCD)
{
	bool carry = false, nextCarry = false;

	for (int i = 0; i < sizeBCD; i++)
	{
		carry = nextCarry;

		nextCarry = BCD[i] & 10000000;

		BCD[i] <<= 1;

		BCD[i] |= carry;
	}
}

void UINT1024::set(long long pos)
{
	num.set(pos);
}

void UINT1024::set(long long pos, bool value)
{
	num.set(pos, value);
}

void UINT1024::reset()
{
	num.reset();
}

bool UINT1024::get(short pos)
{
	return num.test(pos);
}
