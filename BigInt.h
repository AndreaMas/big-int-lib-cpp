/*
* *******************************************************
* BIG INTEGER C++ LIBRARY
* 
* AUTHOR: ANDREA MASCIULLI
* 
* *******************************************************
*/


#pragma once
#include <iostream>
#include <deque>
#include <limits>
#include <string>

const int CELL_MAX = 1000000000;
const short CELL_NUM_DIGITS = 9;
//const int CELL_MAX = 2147483647;
//const int CELL_MAX = INT_MAX;
//const int CELL_MIN = INT_MIN;
const int CELL_MAX_PLUS_ONE = CELL_MAX + 1;

class BigInt {
private:
	std::deque<int> value;
	bool neg; // is negative
public:
	BigInt();
	BigInt(long long);
	BigInt(const BigInt&);
	void operator = (const BigInt&);
	BigInt operator - () const;
	BigInt operator + () const;
	BigInt operator + (const BigInt&) const;
	BigInt operator - (const BigInt&) const;
	BigInt operator / (const BigInt&) const;
	BigInt operator * (const BigInt&) const;
	friend std::ostream& operator << (std::ostream&, const BigInt&);
	std::string BigintToString() const;
	
	void StringToBigint(const std::string&);
};

std::ostream& operator << (std::ostream& os, const BigInt& bigint) {
	return os << bigint.BigintToString();
}

BigInt::BigInt() : neg(false) {
	value.clear();
}

BigInt::BigInt(long long num) : neg(false) {
	value.clear();
	if (num == 0) {
		value.push_back(0);
		return;
	}
	if (num < 0) {
		neg = true;
		num = -num;
	}
	while (num != 0){
		value.push_back(num % CELL_MAX_PLUS_ONE);
		num /= CELL_MAX_PLUS_ONE;
	}
}

BigInt::BigInt(const BigInt& num) {
	*this = num;
}

void BigInt::operator = (const BigInt& num) {
	this->neg = num.neg;
	this->value.clear();
	this->value = num.value;
}

//BigInt BigInt::operator = (const BigInt& num) {
//	BigInt r;
//	r.neg = num.neg;
//	r.value = num.value;
//	return r;
//}

BigInt BigInt::operator - () const {
	BigInt res = *this;
	res.neg = !neg;
	return res;
}

BigInt BigInt::operator + () const {
	BigInt res = *this;
	return res;
}

BigInt BigInt::operator + (const BigInt& num) const{
	// if signs differ call sub instead
	if (this->neg != num.neg)
		return *this - (-num);
	// prepare for sum
	BigInt result;
	result.neg = this->neg;
	int carry = 0;
	int a = 0;
	int b = 0;
	int r = 0;
	int numCellsA = value.size(); // TODO: best to make this long/bigint as well?
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
	int numCellsA = value.size();
	int numCellsB = num.value.size();
	int maxCells = std::max(numCellsA, numCellsB);
	// sub betw all integer cells in vectors
	for (int i = 0; i < maxCells || carry != 0; i++) {
		a = (numCellsA > i) ? value.at(i) : 0;
		b = (numCellsB > i) ? num.value.at(i) : 0;
		r = a + b + carry;
		// if cell overflows add 1 to next cell
		if (r < 0 || r > CELL_MAX) {
			carry = 1;
			r = (r + CELL_MAX) + 1;
		}
		else {
			carry = 0;
		}
		result.value.push_back(r);
	}
	return result;
}

std::string BigInt::BigintToString() const
{
	if (value.size() == 0) return "Empty";
	std::string result = neg ? "-" : "";

	//
	int cellIndex = 0;
	for (auto iter = value.rbegin(); iter != value.rend(); iter++) {
		int cellValue = *iter;
		printf("- cell %d --> %09d\n", cellIndex, cellValue);
		std::string s = std::to_string(cellValue);
		std::string zerostr = "0";

		// cell zero-padding
		if (iter != value.rbegin()) {
			for (int i = s.size(); i < CELL_NUM_DIGITS; i++) {
				s = zerostr + s;
			}
		}

		result.append(s);
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
		std::string cellstring = s.substr(signOffset + digitsFirstCell + i * CELL_NUM_DIGITS, CELL_NUM_DIGITS);
		int cellValue = std::stoi(cellstring);
		value.push_front(cellValue); // TODO: Use push_back
	}

	//short digitNum = 1;
	//for (char c : s) {
	//	if (digitNum == CELL_NUM_DIGITS) {
	//		// allocate new cell
	//		digitNum = 1;
	//	}
	//}
}





//BigInt BigInt::operator + (const BigInt& num) const {
//	// subtract if signs differ
//	if (this->neg != num.neg)
//		return *this - (-num);
//	// operands
//	BigInt a(*this);
//	BigInt b(num);
//	// padding cells
//	int maxCells = std::max<int>(value.size(), num.value.size());
//	while (a.value.size() < maxCells) {
//		a.value.push_back(0);
//	}
//	while (b.value.size() < maxCells) {
//		b.value.push_back(0);
//	}
//	// sum: sum betw cells, add 1 to next cell if overflown
//	int carry = 0;
//	for (int i = 0; i < maxCells; i++) {
//		a.value[i] = a.value[i] + b.value[i] + carry;
//		if (a.value[i] < 0) { // overflowed
//			carry = 1;
//			a.value[i] = (a.value[i] + CELL_MAX) + 1;
//		}
//		else {
//			carry = 0;
//		}
//	}
//	if (carry != 0)
//		a.value.push_back(1);
//	return a;
//}




//BigInt BigInt::operator + (const BigInt& num) {
//	//if (neg != num.neg) return *this - (-num); // if signs differ
//	int base = 1000000000;
//	BigInt res = num;
//	for (int i = 0, carry = 0; i < (int)std::max(value.size(), num.value.size()) || carry; ++i) {
//		if (i == (int)res.value.size())
//			res.value.push_back(0);
//		res.value[i] += carry + (i < (int)value.size() ? value[i] : 0);
//		carry = res.value[i] >= base;
//		if (carry)
//			res.value[i] -= base;
//	}
//	return res;
//}
