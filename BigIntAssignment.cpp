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
void CheckSum() {
    std::string sA =  "12233344445555566666678899900000000000000000000000000000000000000000000000000000000000";
    std::string sB =  "91111111111111111111111100011110000000000000000000000000000000000000000000000000000000";
    std::string sR = "103344455556666677777789999911110000000000000000000000000000000000000000000000000000000";
    BigInt a;
    BigInt b;
    a.StringToBigint(sA);
    b.StringToBigint(sB);
    BigInt c;
    c = a + b;
    std::string sC = c.BigintToString();
    assert(sC == sR);

}
void CheckBigIntToStringAndViceversa() {
    std::string sA = "-1223334444555556666667889990000";
    BigInt a;
    a.StringToBigint(sA);
    std::string sB = a.BigintToString();
    int i = 0;
    for (int i = 0; i < 1000; i++) std::cout << "";
    a.StringToBigint(sB);
    assert(sA == sB);
}
#pragma endregion


int main()
{
    
    {
        std::cout << "Runtime checks!\n";
        CheckSum();
        CheckBigIntToStringAndViceversa();
    }
    

    BigInt a(-9000000000000000);
    BigInt b(-9000000000000000);
    b = a + b;

    std::cout << "A: " << std::endl << a << std::endl;
    std::cout << "B: " << std::endl << b << std::endl;

	for (int i = 0; i < 1000; i++) {
		std::cout << "A: " << std::endl << a << std::endl;
		a = a + b;
	}

	std::cout << "A: " << std::endl << a << std::endl;

    std::string s = "1223334444555556666667889990000";
    b.StringToBigint(s);

	std::cout << "B: " << std::endl << b << std::endl;

    s = b.BigintToString();
    b.StringToBigint(s);

    std::cout << "B: " << std::endl << b << std::endl;






    // Yet to do:
    std::cout << std::endl;
	//BigInt x(1999999999);
    //BigInt y(1999999999);
    //BigInt z = x + y;
    //BigInt w("987654356789"); 
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

