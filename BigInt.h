#pragma once
#include <iostream>
#include <deque>
#include <limits>
#include <string>

const int CELL_NUM_DIGITS = 10;
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
	// separate 64bit number into two 32bit
	uint32_t firstHalf = (uint32_t)(num & UINT32_MAX);
	uint32_t secondHalf = (uint32_t)(num >> 32);
	//std::cout << " -- first half: " << firstHalf << std::endl;
	//std::cout << " -- secon half: " << secondHalf << std::endl;
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

BigInt BigInt::operator + (const BigInt& other) const{
	// if signs differ call sub instead
	if (this->neg != other.neg)
		return *this - (-other);
	// prepare for sum
	BigInt result;
	result.neg = this->neg;
	uint32_t carry = 0;
	uint64_t a = 0;
	uint64_t b = 0;
	uint32_t res32 = 0;
	uint64_t res64 = 0;
	uint64_t numCellsA = value.size();
	uint64_t numCellsB = other.value.size();
	uint64_t maxCells = std::max(numCellsA, numCellsB);
	// sum betw all integer cells in vectors
	for (uint64_t i = 0; i < maxCells || carry != 0; i++) {
		a = (numCellsA > i) ? value.at(i) : 0;
		b = (numCellsB > i) ? other.value.at(i) : 0;
		res64 = a + b + carry;
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

BigInt BigInt::operator - (const BigInt& other) const {
	// if signs differ call sum instead
	if (this->neg != other.neg)
		return *this + (-other);
	// if left < right re-call, swapped, with !signs
	if (*this < other)
		return -other - (-(*this));
	// prepare for sub
	BigInt result;
	result.neg = this->neg;
	uint64_t carry = 0;
	uint64_t a = 0;
	uint64_t b = 0;
	uint64_t b2 = 0;
	uint64_t res64 = 0;
	uint32_t res32 = 0;
	uint64_t numCellsA = value.size();
	uint64_t numCellsB = other.value.size();
	// sum betw all integer cells in vectors
	for (uint64_t i = 0; i < numCellsA; i++) {
		a = value.at(i);
		b = (numCellsB > i) ? other.value.at(i) : 0;

		b2 = b + carry;
		if (b2 <= a) {
			res32 = a - b2;
			carry = 0;
		}
		else {
			res32 = a + ((uint64_t)UINT32_MAX + 1) - b2;
			carry = 1;
		}
		//res32 = (uint32_t)res64;
		// if first element 0 don't push
		if (i != numCellsA-1 || res64 != 0)
			result.value.push_back(res32);
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
	return os << "\n ~ " << bigint.BigintToString() << " ~ \n";
}

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
			int numZeros = CELL_NUM_DIGITS - s.size();
			for (int i = 0; i < numZeros; i++) {
				s = zerostr + s; // TODO: refactor
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
		value.push_front(cellValue); // TODO: efficient for deque? Should be, yet check
	}
}

#pragma endregion

