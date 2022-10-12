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
void CheckBigIntToStringAndViceversa() {
    // string -> bigint
    std::string sA = "-1223334444555556666667889990000";
    BigInt a;
    a.StringToBigint(sA);
    // bigint -> string
    std::string sB = a.BigintToString();
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
#pragma endregion


int main()
{
    
    {
        std::cout << "Runtime checks!\n";
        //CheckBigIntToStringAndViceversa();
        //CheckSum();
        //CheckSub();
    }
    

    //BigInt a("-111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");
    //BigInt b("-111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");

 //   BigInt a(UINT32_MAX); 
 //   std::cout << a << "\n";
 //   BigInt b(int64_t(UINT32_MAX) + 1);
 //   std::cout << "B: \n" << b << "\n";
	//BigInt b2(int64_t(UINT32_MAX) + 2);
	//std::cout << "B2: \n" << b2 << "\n";
	//BigInt b3(int64_t(UINT32_MAX) + 3);
	//std::cout << "B3: \n" << b3 << "\n";
    BigInt c(INT64_MAX);
    std::cout << "C: \n" << c << "\n";
    BigInt d(-INT64_MAX);
    std::cout << "D: \n" << d << "\n";

 //   a += b;
 //   b += a;

	//std::cout << "A: " << std::endl << a << std::endl;
	//std::cout << "B: " << std::endl << b << std::endl;

	//for (int i = 0; i < 10; i++) {
	//	std::cout << "A: " << std::endl << a << std::endl;
	//	a = a + b;
 //       b = b + a;
	//}




    // Yet to do:
	BigInt x(1999999999);
    BigInt y(1999999999);
    BigInt z = x + y;
    BigInt w("987654356789"); 
    //y = pow(x, 10);
    //BigInt z = x; // copy constructor
    //z = x; // operator =
    //z += y; // operator +, +=, -, -=, *, *=, /, /=, %, %=
    //z %= y;
    //z %= 5; // same operators of above, provide overloads for int or just convert int to BigInt?
    //w++; // pre/post increment/decrement
    //if (w <= y) { // all comparisons: <,>,<=,>=,==,!=
    //    z &= x; // bitwise and, or, xor ...
    //}
    //std::cout << y; // stream operators
    //x = -y; // signed/unsigned unary operator -
    //x <<= 5; // shift operators
    //w = pow(w,y);

    
}

