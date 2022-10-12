#pragma once
#include <iostream>
#include <deque>
#include <limits>
#include <string>

//const int CELL_MAX = 1000000000;
const int CELL_NUM_DIGITS = 10;
//const int CELL_MAX = 2147483647;
//const int CELL_MAX = INT_MAX;
//const int CELL_MIN = INT_MIN;
//const int CELL_MAX_PLUS_ONE = CELL_MAX + 1;
const uint32_t CELL_MAX = UINT32_MAX;

class BigInt {
private:
	std::deque<uint32_t> value;
	bool neg; // is negative
public:
	// constructors & copy
	BigInt();
	BigInt(int64_t);
	BigInt(const BigInt&);
	BigInt(const char* cArray);
	void operator = (const BigInt&);
	// algebra ops
	BigInt operator + (const BigInt&) const;
	BigInt operator - (const BigInt&) const;
	BigInt operator / (const BigInt&) const;
	BigInt operator * (const BigInt&) const;
	BigInt operator % (const BigInt&) const;
	void operator += (const BigInt&);
	void operator -= (const BigInt&);
	void operator *= (const BigInt&);
	void operator /= (const BigInt&);
	void operator %= (const BigInt&);
	BigInt operator - () const;
	BigInt operator + () const;
	BigInt pow(const BigInt&, int);
	// comparisons
	bool operator == (const BigInt&) const;
	bool operator != (const BigInt&) const;
	bool operator <= (const BigInt&) const;
	bool operator >= (const BigInt&) const;
	bool operator < (const BigInt&) const;
	bool operator > (const BigInt&) const;
	// print & string ops
	friend std::ostream& operator << (std::ostream&, const BigInt&);
	std::string BigintToString() const;
	void StringToBigint(const std::string&);
};


/*
* *******************************************************************
* CONSTRUCTORS & COPY
* *******************************************************************
*/
#pragma region constructors

BigInt::BigInt() : neg(false) {
	value.clear();
}

BigInt::BigInt(int64_t num) : neg(false) {
	value.clear();
	if (num == 0) {
		value.push_back(0);
		return;
	}
	if (num < 0) {
		neg = true;
		num = -num;
	}
	uint32_t firstHalf = (uint32_t)(num & UINT32_MAX);
	uint32_t secondHalf = (uint32_t)(num >> 32);
	std::cout << "\n -- first half: " << firstHalf << std::endl;
	std::cout << " -- secon half: " << secondHalf << std::endl;
	value.push_back(firstHalf);
	if (secondHalf) value.push_back(secondHalf);
}

BigInt::BigInt(const BigInt& num) {
	*this = num;
}

BigInt::BigInt(const char *cArray)
{
	std::string s(cArray); // TODO: avoid using string
	this->StringToBigint(s);
}

void BigInt::operator = (const BigInt& num) {
	this->neg = num.neg;
	this->value.clear();
	this->value = num.value; // deep copy
}

#pragma endregion

/*
* *******************************************************************
* ALGEBRA OPERATIONS
* *******************************************************************
*/
#pragma region algebOps

BigInt BigInt::operator + (const BigInt& num) const{
	// if signs differ call sub instead
	if (this->neg != num.neg)
		return *this - (-num);
	// prepare for sum
	BigInt result;
	result.neg = this->neg;
	uint32_t carry = 0;
	uint32_t a = 0;
	uint32_t b = 0;
	uint32_t res32 = 0;
	uint64_t res64 = 0;
	uint64_t numCellsA = value.size(); // TODO: check size() is 64 bit
	uint64_t numCellsB = num.value.size();
	uint64_t maxCells = std::max(numCellsA, numCellsB);
	// sum betw all integer cells in vectors
	for (uint64_t i = 0; i < maxCells || carry != 0; i++) {
		a = (numCellsA > i) ? value.at(i) : 0;
		b = (numCellsB > i) ? num.value.at(i) : 0;
		res64 = (uint64_t)a + (uint64_t)b + carry;
		// cut into 32bit halves, left is carry
		res32 = (uint32_t)(res64);
		carry = (uint32_t)(res64 >> 32);
		//std::cout << res64 << "\n";
		//std::cout << res32 << "\n";
		//std::cout << carry << "\n";
		result.value.push_back(res32);
	}
	return result;
}

BigInt BigInt::operator - (const BigInt& num) const {
	// if signs differ call sum instead
	if (this->neg != num.neg)
		return *this + (-num);
	// prepare for sub
	BigInt result;
	result.neg = this->neg;
	int carry = 0;
	int a = 0;
	int b = 0;
	int r = 0;
	int numCellsA = value.size(); // TODO: make this long/bigint?
	int numCellsB = num.value.size();
	int maxCells = std::max(numCellsA, numCellsB);
	// sum betw all integer cells in vectors
	for (int i = 0; i < maxCells || carry != 0; i++) {
		a = (numCellsA > i) ? value.at(i) : 0;
		b = (numCellsB > i) ? num.value.at(i) : 0;
		r = a + b + carry;
		// if cell overflows add 1 to next cell
		// if (r < 0)
		if (r > CELL_MAX) {
			carry = 1;
			//r = (r + CELL_MAX) + 1;
			r = r - CELL_MAX;
		}
		else {
			carry = 0;
		}
		result.value.push_back(r);
	}
	return result;
}

void BigInt::operator+=(const BigInt& other)
{
	*this = *this + other;
}

void BigInt::operator-=(const BigInt& other)
{
	*this = *this - other;
}

// change sign
BigInt BigInt::operator - () const {
	BigInt res = *this;
	res.neg = !neg;
	return res;
}

BigInt BigInt::operator + () const {
	BigInt res = *this;
	return res;
}

#pragma endregion

/*
* *******************************************************************
* COMPARISONS
* *******************************************************************
*/
#pragma region comparisons

bool BigInt::operator != (const BigInt& other) const {
	if (this->neg != other.neg || this->value != other.value) 
		return true;
	return false;
}

bool BigInt::operator == (const BigInt& other) const {
	if (*this != other) return false;
	return true;
}

bool BigInt::operator < (const BigInt& other) const {
	// infer by sign (if left negative true)
	if (this->neg != other.neg) {
		if (this->neg == true) return true;
		else return false;
	}
	// infer by number of cells (left and right)
	uint64_t sizeL = this->value.size();
	uint64_t sizeR = other.value.size();
	if (sizeL != sizeR) {
		if (sizeL < sizeR) return true;
		if (sizeL > sizeR) return false;
	}
	// compare cells
	for (uint64_t i = 0; i < sizeL; i++) {
		if (this->value.at(i) > other.value.at(i)) return false;
	}
	return true;
}

//bool BigInt::operator > (const BigInt& other) const {
//	if (!(*this < other) && *this != other) return true;
//}

bool BigInt::operator > (const BigInt& other) const {
	// infer by sign (if left negative false)
	if (this->neg != other.neg) {
		if (this->neg == true) return false;
		else return true;
	}
	// infer by number of cells (left and right)
	uint64_t sizeL = this->value.size();
	uint64_t sizeR = other.value.size();
	if (sizeL != sizeR) {
		if (sizeL < sizeR) return false;
		if (sizeL > sizeR) return true;
	}
	// compare cells
	for (uint64_t i = 0; i < sizeL; i++) {
		if (this->value.at(i) < other.value.at(i)) return false;
	}
	return true;
}

bool BigInt::operator <= (const BigInt& other) const {
	if (*this > other) return false;
	return true;
}

bool BigInt::operator >= (const BigInt& other) const {
	if (*this < other) return false;
	return true;
}





#pragma endregion

/*
* *******************************************************************
* PRINT & STRING OPS
* *******************************************************************
*/
#pragma region stringOps

std::ostream& operator << (std::ostream& os, const BigInt& bigint) {
	return os << "\n ~~~ BI: ~~~\n" << bigint.BigintToString() << "\n ~~~ end BI ~~~ \n";
}

//std::string BigInt::BigintToString() const
//{
//	if (value.size() == 0) return "Empty";
//	std::string result = neg ? "-" : "";
//
//	uint64_t index = 0;
//	for (auto iter = value.rbegin(); iter != value.rend(); iter++) {
//		uint32_t cell = *iter;
//		printf("- cell %lu --> %010u\n", index, cell);
//		//std::cout << index << " cell --> " << cell << std::endl;
//		std::string s = std::to_string(cell);
//		std::string zerostr = "0";
//
//		// cell zero-padding
//		if (iter != value.rbegin()) {
//			for (int i = s.size(); i < CELL_NUM_DIGITS; i++) {
//				s = zerostr + s; // horrible
//			}
//		}
//
//		result.append(s);
//		result.append(".");
//		index++;
//	}	
//	return result;
//}

std::string BigInt::BigintToString() const
{
	if (value.size() == 0) return "Empty";
	std::string result = neg ? "-" : "";

	//
	int cellIndex = 0;
	for (auto iter = value.rbegin(); iter != value.rend(); iter++) {
		uint32_t cellValue = *iter;
		//printf("- cell %d --> %010u\n", (int)cellIndex, cellValue);
		std::string s = std::to_string(cellValue);
		std::string zerostr = "0";

		// cell zero-padding
		if (iter != value.rbegin()) {
			for (int i = s.size(); i < CELL_NUM_DIGITS; i++) {
				s = zerostr + s;
			}
		}

		result.append(s);
		result.append(".");
		cellIndex++;
	}
	return result;
}

void BigInt::StringToBigint(const std::string& s)
{
	int signOffset = 0;
	this->value.clear();
	this->neg = false;
	if (s == "Empty") return;
	if (s[0] == '-') {
		neg = true;
		signOffset = 1;
	}
	if (s[0] == '+') {
		signOffset = 1;
	}
	if (s == "0") {
		neg = false;
		value.push_back(0);
		return;
	}
	
	// amount of cells
	int necessaryCells = (s.size() - signOffset) / CELL_NUM_DIGITS;
	int digitsFirstCell = (s.size() - signOffset) % CELL_NUM_DIGITS;

	// push first cell
	if (digitsFirstCell != 0) {
		int cellValue = std::stoi(s.substr(signOffset, digitsFirstCell)); // TODO: Use string_view instead
		value.push_back(cellValue);
	}

	// subsequent cells
	for (int i = 0; i < necessaryCells; i++) {
		std::string cellstring = s.substr(signOffset + digitsFirstCell + (i * CELL_NUM_DIGITS), CELL_NUM_DIGITS);
		int cellValue = std::stoi(cellstring);
		value.push_front(cellValue); // TODO: efficient for deque? It should be, yet check
	}
}

#pragma endregion

