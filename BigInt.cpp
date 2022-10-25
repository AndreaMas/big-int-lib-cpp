#include "BigInt.h"
#include <iostream>
#include <deque>
#include <vector>
#include <limits>
#include <string>
#include <bitset>


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
* ALGEBRA OPERATIONS
* *******************************************************************
*/
#pragma region algebOps

BigInt BigInt::operator + (const BigInt& other) const {
	// if signs differ call sub instead
	if (neg != other.neg)
		return *this - (-other);
	// sign
	BigInt result;
	result.neg = neg;
	// prepare for sum
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
		if (!(i == nCellsA - 1 && res32 == 0))
			result.value.push_back(res32);
	}
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
	result = this->DivideTemporary(other);
	result.neg = neg ^ other.neg;
	return result;
}

BigInt BigInt::operator % (const BigInt& other) const {
	BigInt result;
	result = this->RemainderTemporary(other);
	result.neg = neg ^ other.neg;
	return result;
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

// ********************************************************

//Alternative multiplication call: result.value= this->MultiplyValues(other);
std::deque<uint32_t> BigInt::MultiplyValues(const BigInt& other) const {
	uint32_t resultLen = value.size() + other.value.size();
	std::deque<uint32_t> result(resultLen - 1);
	uint32_t carry = 0; // stores the MSBits that result from the 32bit x 32bit product
	for (uint32_t i = 0; i < value.size(); i++) {
		uint64_t a = value[i];
		for (uint32_t j = 0; j < other.value.size(); j++) {
			uint64_t b = other.value[j];
			uint64_t prod = a * b + carry;
			// split in two 32 bit cells
			result[i + j] += uint32_t(prod);
			carry = uint32_t(prod >> 32);
		}
	}
	if (carry != 0)
		result.push_back(carry);
	return result;
}

// Crappy division (by iterative subtraction)
BigInt BigInt::DivideTemporary(const BigInt& other) const {
	BigInt result(0ll);
	BigInt dividend = *this;
	BigInt divisor = other;

	while (dividend >= divisor) {
		dividend -= divisor;
		result += BigInt(1); // result++

		std::cout <<
			"\n- dividend: " << dividend <<
			"\n- result: " << result << '\n';
	}

	return result;
}

// Crappy % (by iterative subtraction)
BigInt BigInt::RemainderTemporary(const BigInt& other) const {
	BigInt dividend = *this;
	BigInt divisor = other;

	while (dividend >= divisor) {
		dividend -= divisor;

		std::cout <<
			"\n- dividend: " << dividend << '\n';
	}
	return dividend;
}


BigInt BigInt::Divide(const BigInt& other) const {
	BigInt result;

	//  Division is relatively complex because it must test
	//	quotients each time.The quotient test procedure must recur to
	//	multiplication and subtraction of unlimited integers.For
	//	example, there are unlimited integers A, B, C and D.A is the
	//	dividend, B(B != O and A > B) is the divisor, C will be the
	//	quotient and D will be the remainder.
	//  The division arithmetic can be described as following steps:
	//  1) Copy the highest part of A with the length of B to D, and 
	//     then insert an element 0 to the end ofinteger queue of D;

	//  2) 
	//     If D < B, then insert an element 0 to the lower position of C,
	//	             it means this quotient digital is 0;

	//     If D = B, then insert an element 1 to the lower position of C, it
	//               means this quotient digital is 1, and do D = D - B;

	//     If D > B, then try to find the quotient as following steps :

	//	     (1) Copying the highest two digital of D to variable d2
	//           (unsigned _int64), copy the highest one digital ofB to
	//           variable b(unsigned _int64), assuming q(unsigned_int64) 
	//           is the tried quotient;

	//       (2) Minimum of q is d2 / (b + 1), maximum of q is min(d2 / b+1),
	//           by dichotomy, it is possible to find one suitable
	//           q settle for D <= B * (q + 1) and D >= B * q.At this time, the
	//           right quotient must be q or q + 1, ifD < B* (q + 1) then the
	//           quotient is q, else the quotient is q + 1. The quotient
	//           would be inserted to the lower position of C;

	//       (3) Subtract the product ofBand the quotient from D.
	//           Temporally the highest digital ofD must be 0, because
	//           D < B, and the length ofD is 1 digital more than B;

	//  3) Insert the next digital from A to the lower position of D, and
	//     delete the highest element of D(it is 0), and repeat step 2
	//     until all elements of A has been treated

	//  4) D is the remainder.

	return result;
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
	for (uint32_t i = 0; i < sizeL; i++) {
		if (this->value[i] > other.value[i]) return false;
	}
	return true;
}

bool BigInt::operator >= (const BigInt& other) const {
	// infer by sign (if left negative false)
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
	for (uint32_t i = 0; i < sizeL; i++) {
		if (this->value[i] < other.value[i]) return false;
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
* INCREMENT, DECREMENT
* *******************************************************************
*/

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

/*
* *******************************************************************
* BITWISE OPERATORS
* *******************************************************************
*/
#pragma region bitwiseop

template<typename T>
void BigInt::ValueBitOps(const BigInt& left, const BigInt& right, T&& lambdaFunc) { // TODO: are templates good practice for lambdas?
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
	BigInt bitShift = shift % BigInt(32);
	if (cellShift >= value.size()) return BigInt(0ll);
	// shift whole cells (by removing LSB cells)
	for (BigInt i(0ll); i < cellShift; ++i) {
		result.value.pop_front();
	}
	// shift single bits
	uint32_t carry = 0; // stores the overflown bits of cell >> amount
	for (uint32_t i = result.value.size() - 1; i != UINT32_MAX; i--) {
		uint64_t a = uint64_t( result.value[i] ) << 32;
		uint64_t shifted = a >> bitShift.value.at(0); // TODO: refactor, use a cast BigInt to uint32_t
		shifted += uint64_t(carry) << 32;
		// split in two 32 bit cells
		result.value.at(i) = uint32_t(shifted >> 32);
		carry = uint32_t(shifted);
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
	auto iter = value.rbegin();

	while (!value.empty() && (*iter) == 0) {
		value.pop_back();
		iter = value.rbegin();
	}

	if (value.size() == 0) {
		neg = 0;
		value.push_back(0);
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
	return os << "\n ~ " << bigint.BigintToBinaryString() << " ~ \n";
}

std::string BigInt::BigintToString() const
{
	if (value.size() == 0) return "Empty";
	std::string result = neg ? "-" : "";

	//
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

std::string BigInt::BigintToBinaryString() const
{
	if (value.size() == 0) return "Empty";
	std::string result = neg ? "-" : "";

	//
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

