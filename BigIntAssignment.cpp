// BigIntAssignment.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <assert.h>
#include "BigInt.h"

/*
* **********************************************
* Asserts
* ********************************************** 
*/
#pragma region asserts
void CheckLongLongConstructor() {
	BigInt a(UINT32_MAX);
	BigInt b(int64_t(UINT32_MAX) + 1);
	BigInt b2(int64_t(UINT32_MAX) + 2);
	BigInt b3(int64_t(UINT32_MAX) + 3);
	BigInt c(INT64_MAX);
	BigInt d(-INT64_MAX);
	//std::cout << "A : \n" << a << "\n";
	//std::cout << "B : \n" << b << "\n";
	//std::cout << "B2: \n" << b2 << "\n";
	//std::cout << "B3: \n" << b3 << "\n";
	//std::cout << "C : \n" << c << "\n";
	//std::cout << "D : \n" << d << "\n";
}
void CheckStringConstructor() {
    BigInt a("-11111111111111111111111111111111111");
    std::cout << "A : \n" << a << "\n";
}
void CheckBigIntToStringAndViceversa() {
    // string -> bigint
    std::string sA = "-1223334444555556666667889990000";
    BigInt a;
    a.StringToBigint(sA);
    // bigint -> string
    std::string sB = a.BigIntToString();
    a.StringToBigint(sB);
    assert(sA == sB);
}
void CheckSum() {
    BigInt        a( "122333444455555666666788999000000000000");
    BigInt        b( "911111111111111111111111000111100000000");
    BigInt expected("1033444555566666777777899999111100000000");
	BigInt res;
	res = a + b;
	assert(res == expected);
    a += b;
    assert(a == expected);
}
void CheckSub() {
	BigInt        a( "122333444455555666666788999000000000000");
	BigInt        b( "111111111111111111111111111000000000000");
	BigInt expected(  "11222333344444555555677888000000000000");
    BigInt res;
	res = a - b;
	assert(res == expected);
	//a -= b;
	//assert(a == expected);

}
void CheckSum2() {
	BigInt        a(uint64_t(UINT32_MAX) - 100);
	BigInt        b(200);
	BigInt expected(uint64_t(UINT32_MAX) + 100);
	BigInt res;
	res = a + b;
	assert(res == expected);
	a += b;
	assert(a == expected);
}
void CheckSub2() {
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
void CheckDiv() {
	BigInt a(500);
	BigInt b(5);
	BigInt expected(100);
	BigInt res;
	res = a / b;
    std::cout <<
        "\nA\n" << a <<
        "\nB\n" << b <<
        "\nEXP\n" << expected <<
        "\nRES\n" << res << std::endl;
	assert(res == expected);
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
        //CheckStringConstructor();
        //CheckBigIntToStringAndViceversa();
        //CheckSum2();
        //CheckSub2();
        //CheckMul();
        //CheckDiv();
        //CheckBitOps();
        //CheckBitShifts();
    }
    

    BigInt a("111111111111111111111111111111111111111111111111111111");
    BigInt b("-111111111111111111111111111111111111111111111111111111");
    BigInt expected("0");
    BigInt result = a + b;

	std::cout << "A: " << a << std::endl;
	std::cout << "B: " << b << std::endl;
	std::cout << "Expected: " << expected << std::endl;
	std::cout << "Result: " << result << std::endl;

	assert(result == expected);


    // Yet to do:
	//BigInt x(1999999999999999);
 //   BigInt y(199999999999999);
	//std::cout << "X : " << x;
	//std::cout << "Y : " << y;
	//BigInt z;
	//BigInt remainder;
	//z = x.Divide(y,remainder);



 //   BigInt z = x + y;
	//std::cout << "Z = X + Y : " << z;
 //   //BigInt w("987654356789"); 
 //   y = x.pow(10);
	//std::cout << "Y = X ^ 10 : ";
 //   BigInt e = x; // copy constructor
 //   z = x; // operator =
 //   z += y; // operator +, +=, -, -=, *, *=, /, /=, %, %=
 //   z %= y;
 //   //z %= 5; // same operators of above, provide overloads for int or just convert int to BigInt?
	//BigInt w = x;
 //   //w++; // pre/post increment/decrement
 //   ++w;
 //   if (w <= y) { // all comparisons: <,>,<=,>=,==,!=
 //       z &= x; // bitwise and, or, xor ...
 //   }
 //   std::cout << y; // stream operators
 //   x = -y; // signed/unsigned unary operator -
 //   x <<= 5; // shift operators
 //   w = w.pow(y);

    
}

