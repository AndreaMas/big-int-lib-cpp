# C++ Big-Integer Library


## Description


As opposed to higher-level languages, C++ does not implement infinitely long integers by default. This library addresses this limitation.

The library implements a BigInt class, which possesses two attributes: a bool to store the sign, a deque to store as many 32bit cells as needed. These cells will store as many bits as we need to represent our big integer number.

The library follows the guidelines found in the paper: Weiguang et. al. "Implementation of Unlimited Integer" ICCSE 2009.


## How to use


Download **BigInt.h** and **BigInt.cpp** files and drop them in your project folder. Alternatively, make a static/dinamic library out of the files.

The include preprecessor directive is:
```c++
#include "BigInt.h"
```

## Features

Division is temporarily obtained through iterative subtraction, it is therefore unbearably slow.

```c++

BigInt a("-35454657578383683445743788348");     // big integer initialized with a string
BigInt b(118009093);                            // big integer initialized with a 64bit integer
BigInt c = a;                                   // copy constructor

c = a + b;                                      // Addition    
c = a - b;                                      // Subtraction       
c = a * b;                                      // Multiplication               
c = a / b;                                      // Divison (slow)
c = a % b;                                      // Mod (slow)
c += b;                                         // In-place operations: += , -=, *=, /=, %=
c++;                                            // Pre/post increment/decrement by one
c = a.pow(b);                                   // Power (a to the power of b)
c = -c;                                         // Change sign

BigInt mod;
c = a.Divide(b, mod);                           // Fast division & mod (not working)

if (a > b) cout << "a greater than b \n";       // Conditional statements: < , > , >= , <= , ==, !=

c = a & b                                       // bitwise AND
c = a | b                                       // bitwise OR
c = a ^ b                                       // bitwise XOR
c &= b                                          // in-place AND

c = a << b                                      // Bitshift (c is a shifted left by b bits)
c =>> b

BigInt c("50000000000");                        // Print
std::cout << c ;                                // Output:   00000000000000000000000000001011.10100100001110110111010000000000
                                                // The above shows a biginteger whose bits are stored into two 32bit cells
                                                // The left cell contains the most significant bits.

```

