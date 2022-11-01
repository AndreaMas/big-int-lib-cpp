// BigIntAssignment.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <assert.h>
#include "BigInt.h"

using namespace bigint;

/*
* **********************************************
* Asserts
* ********************************************** 
*/
#pragma region asserts
void CheckConstructors() {
	BigInt a(int64_t(UINT32_MAX) + 1);
	BigInt b("4294967296");
	assert(a == b);
}
void CheckBigIntToString() { // NOT WORKING [TODO] BigIntToString yet to implement
    // string -> bigint
	BigInt a("-122333444455555");
    // bigint -> string
	std::string str = a.BigIntToString();
	BigInt b( str.c_str() ); // c_str -> string to char*
    assert(a == b);
}
void CheckSum() {
	BigInt        a(uint64_t(UINT32_MAX) - 100);
	BigInt        b(200);
	BigInt expected(uint64_t(UINT32_MAX) + 100);
	BigInt res;
	res = a + b;
	assert(res == expected);
	a += b;
	assert(a == expected);
}
void CheckSub() {
	BigInt        a(int64_t(UINT32_MAX) + 100);
	BigInt        b(200);
	BigInt expected(int64_t(UINT32_MAX) - 100);
	BigInt res;
	res = a - b;
	assert(res == expected);
	a -= b;
	assert(a == expected);
}
void CheckMul() {
    BigInt a(500000000);
    BigInt b(500000000);
    BigInt expected(uint64_t(500000000) * uint64_t(500000000));
    BigInt res;
    res = a * b;
    assert(res == expected);
}
void CheckTempDiv() {
	BigInt a("50000000000000");
	BigInt b("5000000000000");
	BigInt expected("10");
	BigInt res;
	BigInt modulo;
	res = a / b;
	modulo = a % b;
    std::cout <<
        "\nA\n" << a <<
        "\nB\n" << b <<
        "\nExpected division result\n" << expected <<
        "\nDivision result\n" << res << 
        "\nExpected module result\n" << BigInt(0ll) <<
        "\nModule result\n" << modulo << std::endl;
	assert(res == expected);
	assert(modulo == BigInt(0ll));
}
void CheckTrueDiv() { // NOT WORKING [TODO] make division work
	BigInt a("5000000000000");
	BigInt b("50000000000");
	BigInt expected("100");
	BigInt res;
	BigInt modulo;
	res = a.Divide(b, modulo);
	std::cout <<
		"\nA\n" << a <<
		"\nB\n" << b <<
		"\nExpected division result\n" << expected <<
		"\nDivision result\n" << res <<
		"\nExpected module result\n" << BigInt(0ll) <<
		"\nModule result\n" << modulo << std::endl;
	assert(res == expected);
	assert(modulo == BigInt(0ll));
}
void CheckBitOps() {
	BigInt a(500000000);
	BigInt b(500000000);
	BigInt expectedAnd(500000000);
	BigInt expectedOr(500000000);
	BigInt expectedXor(0ll);
	BigInt res;
	res = a & b;
	assert(res == expectedAnd);
	res = a | b;
	assert(res == expectedOr);
	res = a ^ b;
	assert(res == expectedXor);
}
void CheckBitShifts() {
	BigInt a(9000000000);
	std::cout << "A:" << a << '\n';
	BigInt b = a >> BigInt(33);
	BigInt c = a << BigInt(33);
	std::cout << "A shifted right:" << b << '\n';
	std::cout << "A shifted left:" << c << '\n';
}
#pragma endregion


int main()
{
    
    {
        std::cout << "Runtime checks!\n";
		//CheckConstructors();
        //CheckBigIntToString();
        //CheckSum();
        //CheckSub();
        //CheckMul();
		CheckTrueDiv();
        //CheckTempDiv();
        //CheckBitOps();
        //CheckBitShifts();
    }
    
    // Constructors!
	BigInt x("199999999999999999999999");
	BigInt y(1999999999999);
	BigInt z;

	std::cout << "CONSTRUCTORS\n";
	std::cout << "X : " << x;
	std::cout << "Y : " << y;
	std::cout << "Z : " << z;

	// Sum!
    z = x + y;
	std::cout << "SUM \n X + Y : " << z << std::endl;

	// Sub!
	z = x - y;
	std::cout << "SUB \n X - Y : " << z << std::endl;

	// Multiplication!
	z = x * y;
	std::cout << "MUL \n X * Y : " << z << std::endl;

	// Power!
	z = x.pow(10);
	std::cout << "POW \n X ^ 10 : " << z << std::endl;

	// Division (WORKS BUT TOO SLOW)
	//z = x / y;
	//std::cout << "DIV \n X / Y : " << z << std::endl;

	// Remainder (WORKS BUT TOO SLOW)
	//z = x % y;
	//std::cout << "POW \n X % Y : " << z << std::endl;

	// Pre/post increment/decrement
	z++;
	std::cout << "INC \n Z++: " << z << std::endl;
    
	// Bitwise and/or/xor
	z = x ^ y;
	std::cout << "BIT \n X xor Y: " << z << std::endl;

	// Comparisons
    if (x < y) { // all comparisons: <,>,<=,>=,==,!=
		std::cout << "COMP \n X lower than Y" << std::endl;
    }

	// sign
    z = -z;
	std::cout << "SIGN \n - Z:" << z << std::endl;

	// shift ops
    z <<= BigInt(5);
	std::cout << "SHIFT \n Z << 5:" << z << std::endl;

	// BigInt Power! (WORKS BUT TOO SLOW)
    // z = x.pow(y);
	// std::cout << "BIGPOW \n X ^ Y : " << z << std::endl;

    
}

