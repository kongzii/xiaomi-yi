# xiaomi-yi

Library to control your Xiaomi Yi from C++.

## Building

The first step is to make sure you have all dependencies installed:

```
sudo apt-get install g++ build-essential
```

Then compile.

```
g++ example.cpp -o yi.out -std=c++14
```
This will produce yi.out executable which you can run. 
## Sample usage

```
#include <iostream>
#include "xiaomi-yi.h"

int main() {
	using std::cout; using std::cerr; using std::endl; using std::boolalpha;
    using kongzii::xiaomi_yi::XiaomiYi;
    
    XiaomiYi yi; // Create XiaomiYi object with default values

    if(!(yi.start())) { // Will return false if connection to Yi fails
    	cerr << "Something went wrong." << endl;
    }

    yi.shoot(); // Take photo

    yi.stop(); // Disconnect from Yi

    return 0;
}
```

## Requirements

The only explicit requirement is a C++11 compatible compiler like gcc. Developed and tested on Ubuntu 16.04 with G++.  

## Contributing

Every contributor is welcomed! :)
