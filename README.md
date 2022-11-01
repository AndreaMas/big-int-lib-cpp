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


## Issues

- Efficient division doesn't work. Division is temporarely implemented as a repeated subtraction, which takes an unbearable amount of time.
- Given the above, there is no way to show a BigInt in decimal, only in binary.


## Features

### Declaring and Intializing Variables.

```c++
BigInt a("-35454657578383683445743788348");     // big integer initialized with a string
BigInt b(118009093);                            // big integer initialized with a 64bit integer
BigInt c(a);                                    // copy constructor
```

### Addition

```c++
c = a + b;             // Addition
c += b;                // Shorthand
```

### Subtraction 

```c++
c = a - b;             // Subtraction
c -= b;                // Shorthand
```

### Multiplication

```c++
c = a * b;             // Multiplication
c *= b;                // Shorthand
```

### Division

Division is temporarily obtained through iterative subtraction, it is therefore very slow.

```c++
c = a / b;                  // Slow Divison
c /= b;                     // Shorthand

// Alternative

BigInt mod;                 
c = a.Divide(b, mod);       // Fast Division (not working)

```

### Modulo

Mod is temporarily obtained through iterative subtraction, it is therefore very slow.

```c++
c = a % b;                  // Slow Modulo
c %= b;                     // Shorthand

// Alternative

BigInt mod;                 
c = a.Divide(b, mod);       // Fast Modulo (not working)

```

### Conditional Statements

Supports :  **>** , **<** ,  **>=** , **<=** , **==** , **!=**

```c++
if (a > b) cout << "a greater than b \n";

```

### Power

```c++
c = a.pow(b);               // a to the power of b
```


### Increment and Decrement

```c++
c++                        // increment c by one (post-increment)
++c                        // increment c by one (pre-increment)
c--                        // decrement
--c                        // decrement            
```



