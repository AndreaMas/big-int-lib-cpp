#include "BigInt.h"
#include <iostream>
#include <deque>
#include <vector>
#include <limits>
#include <string>
#include <bitset>
#include <functional>

namespace bigint {

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
	value.push_back(firstHalf);
	if (secondHalf) value.push_back(secondHalf);
}

BigInt::BigInt(const BigInt& num) {
	*this = num;
}

BigInt::BigInt(const char* cArray)
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
* ALGEBRA OPERATOR OVERLOADS
* *******************************************************************
*/
#pragma region algebOperatorOverload

BigInt BigInt::operator + (const BigInt& other) const {
	// if signs differ call sub instead
	if (neg != other.neg)
		return *this - (-other);
	// sign
	BigInt result;
	result.neg = neg;
	// prepare for sum
	size_t a;
	uint32_t carry = 0;
	uint32_t res32 = 0;
	uint64_t res64 = 0;
	const uint32_t numCellsA = value.size();
	const uint32_t numCellsB = other.value.size();
	const uint32_t maxCells = std::max(numCellsA, numCellsB);
	// sum betw all cells
	for (uint32_t i = 0; i < maxCells || carry != 0; i++) {
		uint64_t a = (numCellsA > i) ? value[i] : 0;
		uint64_t b = (numCellsB > i) ? other.value[i] : 0;
		res64 = a + b + carry;
		// cut 64bit result into 32bit halves, left is carry
		res32 = (uint32_t)(res64);
		carry = (uint32_t)(res64 >> 32);
		result.value.push_back(res32);
	}
	return result;
}

BigInt BigInt::operator - (const BigInt& other) const {
	// if signs differ call sum instead
	if (neg != other.neg)
		return *this + (-other);
	// if left operand has less cells re-call, swapped, with !signs
	const uint32_t nCellsA = value.size();
	const uint32_t nCellsB = other.value.size();
	if (nCellsA < nCellsB)
		return -other - (-(*this));
	// sign
	BigInt result;
	result.neg = neg;
	// prepare for sub
	uint64_t carry = 0;
	uint64_t b2 = 0;
	uint32_t res32 = 0;
	const uint64_t twoPow32 = (uint64_t)UINT32_MAX + 1;
	// sub betw all cells
	for (uint32_t i = 0; i < nCellsA; i++) {
		uint64_t a = value[i];
		uint64_t b = (nCellsB > i) ? other.value[i] : 0;

		b2 = b + carry;
		if (b2 <= a) {
			res32 = a - b2;
			carry = 0;
		}
		else {
			res32 = a + twoPow32 - b2;
			carry = 1;
		}
		// if is first element and is 0 don't push
		//if (!(i == nCellsA - 1 && res32 == 0))
		result.value.push_back(res32);
	}
	result.RemoveZeroCells();
	if (result.value.size() == 0) result.value.push_back(0ll); // if bigint "empty" push 0
	return result;
}

BigInt BigInt::operator * (const BigInt& other) const {
	// sign
	BigInt result;
	result.neg = neg ^ other.neg;
	// zero pad result (dimension is sum of operand dimensions)
	for (int i = 0; i < value.size() + other.value.size() - 1; i++) {
		result.value.push_back(0); // TODO: is there more efficient way?
	}
	// product betw cells
	uint32_t carry = 0; // stores the MSBits that result from the 32bit x 32bit product
	for (uint32_t i = 0; i < value.size(); i++) {
		uint64_t a = value.at(i);
		for (uint32_t j = 0; j < other.value.size(); j++) {
			uint64_t b = other.value.at(j);
			uint64_t prod = a * b + carry;
			// split in two 32 bit cells
			result.value.at(i+j) += uint32_t(prod);
			carry = uint32_t(prod >> 32);
		}
	}
	if (carry != 0)
		result.value.push_back(carry);
	return result;
}

BigInt BigInt::operator / (const BigInt& other) const {
	BigInt result;
	BigInt remainder;
	result = this->DivideTemporary(other, remainder);
	return result;
}

BigInt BigInt::operator % (const BigInt& other) const {
	BigInt remainder;
	this->DivideTemporary(other, remainder);
	return remainder;
}

void BigInt::operator += (const BigInt& other) // TODO: avoid creating a new BigInt to assign to *this (huge refactor necessary)
{
	*this = *this + other;
}

void BigInt::operator -= (const BigInt& other)
{
	*this = *this - other;
}

void BigInt::operator*=(const BigInt& other)
{
	*this = *this * other;
}

void BigInt::operator/=(const BigInt& other)
{
	*this = *this / other;
}

void BigInt::operator %= (const BigInt& other)
{
	*this = *this % other;
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

BigInt BigInt::pow(int64_t exponent)
{
	BigInt one(1ll);
	if (exponent < 0) {
		if (exponent == -1 && *this == one) return one;
		std::cout << "If exponent negative always returns 1.\n";
		return BigInt(0ll);
	}

	BigInt result(1ll);
	for (int i = 0; i < exponent; i++) {
		result *= (* this);
	}
	return result;
}

BigInt BigInt::pow(const BigInt& exponent)
{
	BigInt zero(0ll);
	BigInt one(1ll);
	if (exponent < zero) {
		if (exponent == -one && *this == one) return one;
		std::cout << "If exponent negative always returns 1.\n";
		return one;
	}

	BigInt result(1ll);
	for (BigInt i = zero; i < exponent; i += one) {
		result *= (*this);
	}
	return result;
}

#pragma endregion

/*
* *******************************************************************
* ALGEBRIC FUNCTIONS
* *******************************************************************
*/
#pragma region algebFunctions

// Crappy division, by iterative subtraction
BigInt BigInt::DivideTemporary(const BigInt& divisor, BigInt& remainder) const {
	BigInt result(0ll);
	BigInt dividend = *this;

	while (dividend >= divisor) {
		dividend -= divisor;
		++result;

		std::cout <<
			"\n- dividend: " << dividend <<
			"\n- result: " << result << '\n';
	}
	remainder = dividend;
	return result;
}

BigInt BigInt::Divide(const BigInt& divisor, BigInt& remainder) const {
	// save sign
	bool resNeg = this->neg ^ divisor.neg;
	// A dividend, B divisor, C quotient, D remainder
	BigInt A = *this, B = divisor, C(0ll), D(0ll);
	B.neg = false; B.neg = false;

	// SPECIAL CASES
	
	// divisor == 0
	if (B == BigInt(0ll)) {
		std::cout << "ERROR: dividing for zero." << std::endl;
		return BigInt();
	}
	// divisor == 1 or -1
	if (B == BigInt(1ll)) {
		A.neg = resNeg;
		return A;
	}
	// divisor greater or equal wrt dividend
	if (A < B) return BigInt(0ll);
	if (A == B) {
		BigInt one(1ll);
		one.neg = resNeg;
		return one;
	}

	// DIVISION ALGORITHM
	
	// Copy highest part of A with length of B to D
	D = A;
	D.value.erase(D.value.begin() + B.value.size(), D.value.end());

	// Insert element 0 to the end of integer queue of D
	D.value.push_back(0ll);

	// Repeat this for all elements of A
	for (uint32_t i = 0; i < A.value.size(); i++) {

		// If D < B, then insert an element 0 to the lower pos of C, aka this quotient digital is 0;
		if (D < B) 
			C.value.push_front(0);
		// If D = B, then insert an element 1 to the lower pos of C, aka this quotient digital is 1, and do D = D - B;
		if (D == B) {
			C.value.push_front(1);
			D -= B;
		}
		// If D > B, then try to find the quotient as follows
		if (D > B) {
			// copy highest two digitals of D to variable d2
			uint64_t d2 = ((uint64_t)D.value[D.value.size() - 1] << 32) | (uint64_t)D.value[D.value.size() - 2];
			// copy highest one digital of B to variable b
			uint64_t b = (uint64_t)B.value[B.value.size() - 1];
			uint64_t q = 0; // tried quotient
			// min & max q values
			uint64_t qMin = std::min(d2 / (b + 1), b + 1); // TODO: try: uint64_t qMin = d2 / (b + 1);
			uint64_t qMax = std::min(d2 / (b + 1), uint64_t(UINT32_MAX));
			// BINARY SEARCH
			// q must satisfy D<=B*(q+1) && D>=B*q
			auto correctCondition = [&D, &B](uint64_t q) -> bool { return D <= B * BigInt(q + 1) && D >= B * BigInt(q); };
			// if  D<B*q  then q must be higher
			auto isHigherCondition = [&D, &B](uint64_t q) -> bool { return D < B* BigInt(q); };
			// if  D>B*(q+1)  then q must be lower
			auto isLowerCondition = [&D, &B](uint64_t q) -> bool { return D > B * (BigInt(q) + BigInt(1ll)); };
			q = BinarySearch(qMin, qMax, isHigherCondition, isLowerCondition, correctCondition);
			// q correct value should be q or q + 1
			q = (D < B* BigInt(q + 1)) ? q : q + 1;
			// insert it to the lower pos of C
			C.value.push_front(q);
			//  Subtract the product of B and the quotient from D.
			D -= B * BigInt(q);
			//  Temporally the highest digital of D must be 0, because
			//  D < B, and the length of D is 1 digital more than B;
			D.value.push_back(0ll);
		}

		// Insert the next digital from A to the lower position of D
		D.value.push_front(A.value.at(i));
		// delete the highest element of D (it is 0)
		D.value.pop_back();
	}

	// finally, D is the remainder.
	remainder = D;
	C.neg = resNeg;
	return C;
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

bool BigInt::operator <= (const BigInt& other) const {
	// infer by sign (if left negative true)
	if (this->neg != other.neg) {
		if (this->neg == true) return true;
		else return false;
	}
	// infer by number of cells (left and right)
	uint32_t sizeL = this->value.size();
	uint32_t sizeR = other.value.size();
	if (sizeL != sizeR) {
		if (sizeL < sizeR) return true;
		if (sizeL > sizeR) return false;
	}
	// compare cells
	for (uint32_t i = sizeL - 1; i < sizeL; --i) {
		uint32_t leftval = this->value[i];
		uint32_t rightval = other.value[i];
		if (leftval < rightval) return true;
		if (leftval > rightval) return false;
	}
	return true;
}

bool BigInt::operator >= (const BigInt& other) const {
	// infer by sign
	if (this->neg != other.neg) {
		if (this->neg == true) return false;
		else return true;
	}
	// infer by number of cells (left and right)
	uint32_t sizeL = this->value.size();
	uint32_t sizeR = other.value.size();
	if (sizeL != sizeR) {
		if (sizeL < sizeR) return false;
		if (sizeL > sizeR) return true;
	}
	// compare cells
	for (uint32_t i = sizeL - 1; i < sizeL; --i) {
		uint32_t leftval = this->value[i];
		uint32_t rightval = other.value[i];
		if (leftval < rightval) return false;
		if (leftval > rightval) return true;
	}
	return true;
}

bool BigInt::operator < (const BigInt& other) const {
	if (*this >= other) return false;
	return true;
}

bool BigInt::operator > (const BigInt& other) const {
	if (*this <= other) return false;
	return true;
}

#pragma endregion

/*
* *******************************************************************
* PRE/POST INCREMENT, DECREMENT
* *******************************************************************
*/
#pragma region increment

void BigInt::operator ++ ()
{
	BigInt one(1ll);
	*this += one;
}

void BigInt::operator -- ()
{
	BigInt one(1ll);
	*this -= one;
}

BigInt BigInt::operator ++ (int)
{
	BigInt tmp(*this);
	operator++();
	return tmp;
}

BigInt BigInt::operator -- (int)
{
	BigInt tmp(*this);
	operator--();
	return tmp;
}

#pragma endregion

/*
* *******************************************************************
* BITWISE OPERATORS
* *******************************************************************
*/
#pragma region bitwiseop

template<typename T>
void BigInt::ValueBitOps(const BigInt& left, const BigInt& right, T&& lambdaFunc) { // Are templates good practice for lambdas?
	this->value.clear();
	const uint32_t nCellsA = left.value.size();
	const uint32_t nCellsB = right.value.size();
	const uint32_t nMaxCells = std::max(nCellsA, nCellsB);
	for (int i = 0; i < nMaxCells; i++) {
		uint32_t a = (nCellsA > i) ? left.value[i] : 0;
		uint32_t b = (nCellsB > i) ? right.value[i] : 0;
		uint32_t r = lambdaFunc(a,b); // bitwise operation
		this->value.push_back(r);
	}
	this->RemoveZeroCells();
}

BigInt BigInt::operator&(const BigInt& other) const{
	BigInt result;
	auto funcBitwiseAnd = [](uint32_t a, uint32_t b) -> uint32_t { return a & b; }; // lambda &
	result.ValueBitOps(*this, other, funcBitwiseAnd);
	return result;
}

BigInt BigInt::operator|(const BigInt& other) const{
	BigInt result;
	auto funcBitwiseOr = [](uint32_t a, uint32_t b) -> uint32_t { return a | b; }; // lambda |
	result.ValueBitOps(*this, other, funcBitwiseOr);
	return result;
}

BigInt BigInt::operator^(const BigInt& other) const{
	BigInt result;
	auto funcBitwiseXor = [](uint32_t a, uint32_t b) -> uint32_t { return a ^ b; }; // lambda ^
	result.ValueBitOps(*this, other, funcBitwiseXor);
	return result;
}

BigInt BigInt::operator>>(const BigInt& shift) const{
	// check of how many cells and bits we need to shift
	BigInt result = *this;
	BigInt cellShift = shift / BigInt(32);
	uint32_t bitShift = BigInt( shift % BigInt(32) ).value.at(0); // TODO: ugly, implement cast BigInt to uint32_t
	if (cellShift >= value.size()) return BigInt(0ll);
	// shift whole cells (by removing LSB cells)
	for (BigInt i(0ll); i < cellShift; ++i) {
		result.value.pop_front();
	}
	// shift single bits
	uint32_t carry = 0; // stores the overflown bits of cell >> amount
	for (uint32_t i = result.value.size() - 1; i != UINT32_MAX; i--) {
		uint64_t shiftedCell = uint64_t( result.value[i] ) << ( 32 - bitShift);
		shiftedCell += uint64_t(carry) << 32;
		// split in two 32 bit cells
		result.value.at(i) = uint32_t(shiftedCell >> 32);
		carry = uint32_t(shiftedCell);
	}
	result.RemoveZeroCells();
	return result;
}

BigInt BigInt::operator<<(const BigInt& shift) const{
	// check of how many cells and bits we need to shift
	BigInt result = *this;
	BigInt nCells(value.size());
	BigInt cellShift = shift / BigInt(32);
	BigInt bitShift = shift % BigInt(32);
	if (cellShift >= nCells) return BigInt(0ll);
	// shift whole cells (by introducing zeros as LSB cells)
	for (BigInt i(0ll); i < cellShift; ++i) {
		result.value.push_front(0);
	}
	// shift single bits
	uint32_t carry = 0; // stores the overflown bits of cell << amount
	for (uint32_t i = 0; i < result.value.size(); i++) {
		uint64_t cell64( result.value[i] );
		uint64_t shifted = cell64 << bitShift.value.at(0);
		shifted += carry;
		// split in two 32 bit cells
		result.value.at(i) = uint32_t(shifted);
		carry = uint32_t(shifted >> 32);
	}
	if (carry != 0)
		result.value.push_back(carry);
	return result;
}

void BigInt::operator&=(const BigInt& other){
	return *this = *this & other;
}

void BigInt::operator |= (const BigInt& other) {
	return *this = *this | other;
}
void BigInt::operator ^= (const BigInt& other) {
	return *this = *this ^ other;
}
void BigInt::operator >>= (const BigInt& shift) {
	return *this = *this >> shift;
}
void BigInt::operator <<= (const BigInt& shift) {
	return *this = *this << shift;
}



#pragma endregion


/*
* *******************************************************************
* UTILITIES
* *******************************************************************
*/
#pragma region utilities
void BigInt::RemoveZeroCells()
{
	auto iter = value.rbegin(); // last element, aka MSBits

	while (!value.empty() && (*iter) == 0) {
		value.pop_back();
		iter = value.rbegin();
	}

	if (value.size() == 0) {
		neg = 0;
		value.push_back(0ll);
	}
}

uint64_t BinarySearch(
	uint64_t begin, 
	uint64_t end,
	const std::function<bool(uint64_t q)>& isHigher,
	const std::function<bool(uint64_t q)>& isLower,
	const std::function<bool(uint64_t q)>& isCorrect)
{
	uint64_t result;
	while (begin <= end) {
		result = (begin + end) / 2; // middle
		if (isCorrect(result)) return result;
		else if (isHigher(result)) end = result - 1;
		else if (isLower(result)) begin = result + 1;
		else std::cout << "Binary search is failing ... ";
	}
}
#pragma endregion

/*
* *******************************************************************
* PRINT & STRING OPS
* *******************************************************************
*/
#pragma region stringOps

std::ostream& operator << (std::ostream& os, const BigInt& bigint) {
	return os << "\n ~ " << bigint.BigIntToBinary() << " ~ \n";
}

std::string BigInt::BigIntToString() const
{
	if (value.size() == 0) return "Empty";
	std::string result = neg ? "-" : "";

	//int cellIndex = 0;
	for (auto iter = value.rbegin(); iter != value.rend(); iter++) {
		uint32_t cellValue = *iter;
		//printf("- cell %02d --> %010u\n", (int)cellIndex, cellValue);
		std::string s = std::to_string(cellValue);
		
		// cell zero-padding
		std::string zerostr = "0";
		//if (iter != value.rbegin()) {
			int numZeros = CELL_NUM_DIGITS - s.size();
			for (int i = 0; i < numZeros; i++) {
				s = zerostr + s; // TODO: refactor
			}
		//}

		result.append(s);
		result.append(".");
		//cellIndex++;
	}
	return result;
}

std::string BigInt::BigIntToBinary() const
{
	if (value.size() == 0) return "Empty";
	std::string result = neg ? "-" : "";

	for (auto iter = value.rbegin(); iter != value.rend(); iter++) {
		std::bitset<32> bitCell(*iter);
		//printf("- cell %02d --> %010u\n", (int)cellIndex, cellValue);
		std::string s = bitCell.to_string();

		result.append(s);
		result.append(".");
	}
	return result;
}

void BigInt::StringToBigint(const std::string& s)
{
	this->value.clear();
	uint32_t signOffset = (s[0] == '-' || s[0] == '+') ? 1 : 0;
	// for each character, multiply it for 10^position and sum it to this bigint
	for (int64_t i = s.size() - 1; i >= signOffset; i--) {
		uint32_t character = uint32_t(s[i]) - 48;
		BigInt charValue = BigInt(character) * BigInt(10).pow(s.size() - 1 - i);
		*this += charValue;
	}
	// sign
	this->neg = false;
	if (signOffset) {
		if (s[0] == '-') this->neg = true;
		if (s[0] == '+') this->neg = false;
	}
}

#pragma endregion


} // end of namespace bigint

