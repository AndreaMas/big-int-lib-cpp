#pragma once
#include <cstdlib>
#include <iostream>
#include <deque>
#include <limits>
#include <functional>

const int CELL_NUM_DIGITS = 10;
const uint32_t CELL_MAX = UINT32_MAX;

namespace bigint {

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
	BigInt pow(int64_t);
	BigInt pow(const BigInt&);
	BigInt Divide(const BigInt& divisor, BigInt& remainder) const;
	BigInt DivideTemporary(const BigInt& divisor, BigInt& remainder) const;
	// comparisons
	bool operator == (const BigInt&) const;
	bool operator != (const BigInt&) const;
	bool operator <= (const BigInt&) const;
	bool operator >= (const BigInt&) const;
	bool operator < (const BigInt&) const;
	bool operator > (const BigInt&) const;
	// increment decrement
	void operator ++ ();
	void operator -- ();
	BigInt operator ++ (int);
	BigInt operator -- (int);
	// bitwise ops
	BigInt operator & (const BigInt&) const;
	BigInt operator | (const BigInt&) const;
	BigInt operator ^ (const BigInt&) const;
	BigInt operator >> (const BigInt&) const;
	BigInt operator << (const BigInt&) const;
	void operator &= (const BigInt&);
	void operator |= (const BigInt&);
	void operator ^= (const BigInt&);
	void operator >>= (const BigInt&);
	void operator <<= (const BigInt&);
	// print & string ops
	friend std::ostream& operator << (std::ostream&, const BigInt&);
	std::string BigIntToString() const;
	std::string BigIntToBinary() const;
	void StringToBigint(const std::string&);
private:
	// helpers
	void RemoveZeroCells();
	template<typename T>void ValueBitOps(const BigInt& left, const BigInt& right, T&& lambdaFunc);
public:

};

std::ostream& operator << (std::ostream& os, const BigInt& bigint);

uint64_t BinarySearch(uint64_t begin, uint64_t end, const std::function<bool(uint64_t)>& isHigher, 
	const std::function<bool(uint64_t)>& isLower, const std::function<bool(uint64_t)>& isCorrect);


} // end of namespace bigint
