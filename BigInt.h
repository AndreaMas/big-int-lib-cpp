#pragma once
#include <iostream>
#include <deque>
#include <limits>

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
	BigInt pow(int64_t);
	BigInt pow(const BigInt&);
	// comparisons
	bool operator == (const BigInt&) const;
	bool operator != (const BigInt&) const;
	bool operator <= (const BigInt&) const;
	bool operator >= (const BigInt&) const;
	bool operator < (const BigInt&) const;
	bool operator > (const BigInt&) const;
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
	std::string BigintToString() const;
	void StringToBigint(const std::string&);
private:
	// helpers
	void TrimZeros();
	// algebra internal functions
	template<typename T> void ValueBitOps(const BigInt& left, const BigInt& right, T&& lambdaFunc);
	void ValueBitwiseOps(const BigInt& left, const BigInt& right);
	std::deque<uint32_t> MultiplyValues(const BigInt& other) const;
	BigInt DivideTemporary(const BigInt& other) const;
	BigInt RemainderTemporary(const BigInt& other) const;
	BigInt Divide(const BigInt& other) const;
};
