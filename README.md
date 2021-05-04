# LLVM Kaleidoscope JIT

A JIT compiler using a llvm backend for a simple non trivial language inspired by [This guide](https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/index.html)

### Example

Example can be found in tests/testJit.cpp

### Features

Kaleidoscope language support: 

```
# Simple binary operation
1 + 2 - 3 * 4 / 5

# Function definitions
def foo(x)
    x + 1
    
# If then else
if (x>1) then
   1
else
    2

# For loop creation
for i=0, i<10, 1 in
    i*2
    
# Variable creation
var a = 0, b, c=1 in
    a

# Custom operator definition
def unary ! (v)
    if v then 
        0
    else
        1;
       
def binary : & 10 (a b)
    if a*b then
        return 1
    else
        return 0
    
# Calling external std lib function
extern cos(x)
cos(1)
```

All types are considered to be doubles.
